#include "stdafx.h"
#include "JavaModelLib/internal/core/BaseJavaType.h"
#include <JavaModelLib/internal/core/PackageFragment.h>
#include <JavaModelLib/internal/core/BinaryField.h>
#include <JavaModelLib/internal/core/BinaryMethod.h>
#include  <JavaModelLib/internal/core/Initializer.h>
#include  <map>
#include <boost/algorithm/string.hpp>
#include <PathModelLib/MosesPath/MosesPath.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		IType* BaseJavaType::DeepCopyButNoChildren(IJavaElement* parent)
		{
			auto clone = reinterpret_cast<BaseJavaType*>(ShallowCopy(parent));
			if(clone)
				clone->HelpToDeepCopy(*this);
			return clone;
		}

		PathModel::MosesPath* BaseJavaType::getMosesPath()
		{
			if (moses)
				return moses;
			if (!fParent)
				return nullptr;

			if (fParent->fLEType == TYPE)
			{
				auto parent_moses = reinterpret_cast<MemberInFileMoses*>(fParent->getMosesPath());
				if (!parent_moses)
					return nullptr;
				vector<wstring> enclosing_types(parent_moses->enclosing_types);
				enclosing_types.push_back(parent_moses->name);
				moses = new MemberInFileMoses(
					parent_moses->proj,
					parent_moses->root,
					parent_moses->pkg,
					parent_moses->file,
					enclosing_types,
					fName,
					MosesPath::TYPE,isBinary());

			}

			else
			{
				auto parent_moses = reinterpret_cast<FileMoses*> (fParent->getMosesPath());
				if (!parent_moses)
					return nullptr;

				moses = new MemberInFileMoses(parent_moses->proj,
					parent_moses->root,
					parent_moses->pkg,
					parent_moses->file,
					{},
					fName,
					MosesPath::TYPE, isBinary());
			}
			return  moses;

		}
		void BaseJavaType::addChild(IJavaElement* child)
		{

		}

		const vector<IField*> *BaseJavaType::getFields(){
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());
			if (!_info)
				return nullptr;
			return _info->getFields();
		}

		std::vector<IJavaElement*>* BaseJavaType::getChildrenOfType(
			IJavaElementType _type)
		{
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());
			if (!_info)
				return nullptr;
			return _info->getChildrenOfType(_type);
		};

		BaseJavaType::BaseJavaType(IJavaElement* parent, const wstring& name) :
			IType(parent, name),
			status(0),
			file_symbol(nullptr), info(nullptr)
		{
		}

		BaseJavaType::BaseJavaType(BaseJavaType& o) : IType(o),	
			status(o.status),
			file_symbol(nullptr), info(nullptr)

		{


		}


		IJavaElement* BaseJavaType::DeepCopy(IJavaElement* parent)
		{
			auto clone = reinterpret_cast<BaseJavaType*>(ShallowCopy(parent));
			if(clone)
				clone->HelpToDeepCopy(*this);
			return clone;
		}

		wstring BaseJavaType::getFullName()
		{
			wstring _strType;
			if (IsInterface())
			{
				_strType = L"interface";
			}
			else if (IsEnum())
			{
				_strType = L"enum";
			}
			else if (isClass())
			{
				_strType = L"class";
			}
			else if (IsPrimitiveType())
			{

			}
			else if (IsArrayType())
			{

			}
			wstring ret;
			auto modifiStr = getModifersString();
			if (!modifiStr.empty())
				ret = +L" ";

			ret+= _strType + L" " + fName + L" in ";

			getElementInfo();
			ret += boost::join(info->qualifiedTypeNames, wstring(1, '.'));
			return ret;
		}

		bool BaseJavaType::hasChildren(){
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());
			if (!_info)
				return false;
			return  info->all_children.size();
		}

		std::vector<IJavaElement*>* BaseJavaType::getChildren()
		{
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());
			if (!_info)
				return nullptr;

			return _info->getChildren();
		}



		BaseJavaType::~BaseJavaType()
		{
			if (info){
				delete info;
			}
		}

		void  BaseJavaType::SortOutLineNameIdentity()
		{
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());
			if (_info)	
				_info->SortOutLineNameIdentity();
		}

		void BaseJavaType::GetIndendentInfoAndSepareteFromParent() 
		{
			Member::GetIndendentInfoAndSepareteFromParent();
			file_symbol = false;
		}
		void BaseJavaType::HelpToDeepCopy(BaseJavaType& o)
		{
		    if(o.info) {
				delete info;
				info = new SourceTypeElementInfo();	
				info->HelpToDeepCopy(*(o.info), this);
		    }
		}

		void BaseJavaType::HelpToDeepCopyButNoChildren(BaseJavaType& o)
		{
			if (o.info) {
				delete info;
				info = new SourceTypeElementInfo();
				info->HelpToDeepCopyButNoChildren(*(o.info), this);
			}
		}

		IInitializer* BaseJavaType::getInitializer(int start_pos){
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());

			if (!_info)
				return nullptr;

			return _info->getInitializer(start_pos);
		}

		std::wstring BaseJavaType::getJavaCardDocUrlFragment(){
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());
			if (!_info)
				return{};
			wstring ret;
			ret = boost::join(_info->qualifiedTypeNames, wstring(1, '/')) + L"/" + fName + L".html";
			return ret;
		}

		const std::vector<IType*>* BaseJavaType::getTypes() {
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());
			if (!_info)
				return nullptr;
			return _info->getTypes();
		}


		IMethod* BaseJavaType::getMethod(const wstring& name,
			const vector<wstring>& parameterTypes)
		{
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());

			if (!_info)
				return nullptr;

			return _info->getMethod(name, parameterTypes);
		
		}

		IField* BaseJavaType::getField(const wstring& name)
		{
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());

			if (!_info)
				return nullptr;

			return _info->getField(name);
		}


		const vector<IMethod*>* BaseJavaType::getMethods() {

			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());

			if (!_info)
				return nullptr;

			return _info->getMethods();
		}

		IType* BaseJavaType::getType(const wstring& name)
		{
			auto _info = reinterpret_cast<SourceTypeElementInfo*>(getElementInfo());

			if (!_info)
				return nullptr;

			return _info->getType(name);
			
		};


	} // Close namespace JavaModel block

} // Close namespace Jikes block

