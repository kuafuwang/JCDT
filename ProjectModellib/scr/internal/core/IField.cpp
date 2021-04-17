#include "stdafx.h"
#include <JavaModelLib/internal/core/IField.h>

#include <boost/algorithm/string.hpp>
#include <PathModelLib/MosesPath/MosesPath.h>


namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		PathModel::MosesPath* IField::getMosesPath()
		{
			if (moses)
				return moses;
			if (!fParent)
				return nullptr;

			auto parent_moses = reinterpret_cast<PathModel::MemberInFileMoses*>(fParent->getMosesPath());
			if (!parent_moses)
				return nullptr;

			vector<wstring> enclosing_types(parent_moses->enclosing_types);
			enclosing_types.push_back(parent_moses->name);
			return moses = new PathModel::MemberInFileMoses(
				parent_moses->proj,
				parent_moses->root,
				parent_moses->pkg,
				parent_moses->file,
				enclosing_types,
				fName,
				PathModel::MosesPath::FIELD ,isBinary());

		}
		void IField::GetIndendentInfoAndSepareteFromParent()
		{
			if (!fParent) {
				return;
			}

			if(isBinary()){
				getSourceRange();
			}
			getMosesPath();
			if(fLEType == LOCAL_FIELD){
				getElementInfo();
				if(info && !info->local_fild_father_rng){
					auto member_father = reinterpret_cast<Member*>(fParent);
					auto father_rng = member_father->getSourceRange();
					if(father_rng)	{
						info->local_fild_father_rng = father_rng->Clone();
					}
						
				}
			}
		}

		IJavaElement* IField::DeepCopy(IJavaElement* parent){

			auto clone = reinterpret_cast<IField*>( ShallowCopy(parent));

			if(clone)
				clone->HelpToDeepCopy(*this);

			return clone;
		}
		wstring IField::getOutLineName() 
		{
			auto _info = reinterpret_cast<SourceFieldElementInfo*>(getElementInfo());
			if (!_info)
				return{};
			return fName + L" : " + _info->_type;
		}
		wstring IField::getFullName(){
			auto _info = reinterpret_cast<SourceFieldElementInfo*>(getElementInfo());
			if (!_info)
				return{};

			std::wstring const_string;
			
			if (_info->initial_value){
				if(_info->initial_value->getKind() == LiteralValue::INT_KIND){
					auto _value = reinterpret_cast<IntLiteralValue*>(_info->initial_value)->value;
					if(_value && _info->_type == L"byte"){
						wchar_t temp[32];
						_value = _value & 0x000000ff;
						int idx = wsprintfW(temp,L"= %d [0x%X]", _value, _value);
						temp[idx] = 0x00;
						const_string = temp;
					}
					else if(_info->_type == L"short" || _info->_type == L"char"){
						wchar_t temp[32];
						_value = _value & 0x0000ffff;
						int idx = wsprintfW(temp, L"= %d [0x%X]",_value, _value);
						temp[idx] = 0x00;
						const_string = temp;
					}
					else
					{
						wchar_t temp[32];
						int idx = wsprintfW(temp, L"= %d [0x%X]", _value, _value);
						temp[idx] = 0x00;
						const_string = temp;
					}
				}
			}
			if(!IsLocal()){	
				auto ret = getModifersString() + L" " + _info->_type + L" " + fName + const_string + L" in ";
				ret += boost::join(_info->qualifiedTypeNames, wstring(1, '.'));
				return ret;
			}	
			wstring myself = getModifersString() + L" " + _info->_type + L" " + fName + const_string;
			wstring owner;
			if (fParent){
				owner = reinterpret_cast<Member*>(fParent)->getFullName();
			}
			return myself + L" " + L"in " + owner;
		}

		 IField::IField(IJavaElement* parent, const wstring& name, IJavaElementType _type) 
			 : Member(parent, _type, name), info(nullptr)
		 {

		}

		IField::~IField(){
			
				delete info;
				info = nullptr;
		}

		IField::IField(IField& o): Member(o), info(nullptr)
		{

		}

		void IField::HelpToDeepCopy(IField& o)
		{
			if (o.info)
			{
				delete info;
				info = new SourceFieldElementInfo();
				info->HelpToDeepCopy(*(o.info), this);
			}
		}

		std::wstring IField::getJavaCardDocUrlFragment()
		{
			auto _info = reinterpret_cast<SourceFieldElementInfo*>(getElementInfo());
			wstring ret;
			if(_info)
				ret = boost::join(_info->qualifiedTypeNames, wstring(1, '/'))  + L".html#"  + fName;
			return ret;
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

