#include "stdafx.h"
#include <assert.h>
#include "JavaModelLib/internal/core/BaseMethod.h"
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/info/SourceTypeElementInfo.h>
#include <boost/algorithm/string.hpp>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/BaseJavaType.h>
#include <PathModelLib/MosesPath/MosesPath.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		wstring EMPTY_STRING;
		wstring& BaseMethod::getReturnType()
		{
			// Make sure that the method signature associated with this method is
			// processed prior to invoking this function.
			//  ( "this -> ProcessMethodSignature(sem, tok);" )
			auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());

			if (!_info)
				return EMPTY_STRING;

			return _info->return_type;
		}

		bool BaseMethod::equals(IJavaElement* o)
		{
			if (!o)
				return false;

			if(JavaElement::equals(o)){
				
				if (formal_parameterTypes == ((BaseMethod*)o)->getParameterTypes())
					return true;
			}
			return false;
			
			
		}

		void BaseMethod::MarkConstructor(bool _v)
		{
			_isConstructor = _v;
		}

		bool BaseMethod::isConstructor()
		{
			return _isConstructor;
		}
		PathModel::MosesPath* BaseMethod::getMosesPath()
		{
			if (moses)
				return moses;
			if (!fParent || fParent->fLEType != TYPE)
				return nullptr;

			auto parent_moses = reinterpret_cast<PathModel::MemberInFileMoses*>(fParent->getMosesPath());
			if (!parent_moses)
				return nullptr;
			vector<wstring> enclosing_types(parent_moses->enclosing_types);
			enclosing_types.push_back(parent_moses->name);
			return 	moses = new PathModel::MemberInFileMoses(
				parent_moses->proj,
				parent_moses->root,
				parent_moses->pkg,
				parent_moses->file,
				enclosing_types,
				fName,
				PathModel::MosesPath::METHOD ,isBinary());
		}
		const wstring BaseMethod::getMethodNameWithParameterNames()
		{
			//fName  
			wstring _ret;
			_ret = fName;
			_ret.push_back('(');
			auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());
			if (!_info)
				return{};
			int  size_paramenterNames = _info->paramenterNames.size();
			if(size_paramenterNames)
			{
				size_paramenterNames = size_paramenterNames - 1;
			}
			else
			{
				size_paramenterNames = -1;
			}
			auto size_formal_parameters = formal_parameterTypes.size();
			for(int i = 0 , size = size_formal_parameters; i < size ; ++i)
			{
				_ret.append(formal_parameterTypes[i]);
				
				if (i <= size_paramenterNames) {
					_ret.append(L" ").append(_info->paramenterNames[i]);
				}
				
				if( i != size - 1){
					_ret.push_back(',');
				}
			}
			_ret.append(L" )");
			_ret.append(L" : ").append(_info->return_type);
			return _ret;
		}

		void BaseMethod::HelpToDeepCopy(BaseMethod& o)
		{


			if(o.info)
			{
				delete info;
				info = new SourceMethodElementInfo();
				info->HelpToDeepCopy(* (o.info), this);
			}
		}


		IJavaElement* BaseMethod::DeepCopy(IJavaElement* parent)
		{
			auto clone = reinterpret_cast<BaseMethod*>(ShallowCopy(parent));
			if(clone)
				clone->HelpToDeepCopy(*this);
			return clone;
		}

		unsigned BaseMethod::getNumParameterTypes()
		{	
			return formal_parameterTypes.size();
		}

		IField* BaseMethod::getLocalField(const wstring& name,
			const wstring& declareType){
			auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());
			if (!_info)
				return nullptr;

			return _info->getLocalField(name, declareType);
		}

		wstring BaseMethod::getFullName()
		{
			getNameRange();
			wstring ret;
			auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());

			if (!_info)
				return ret;

			ret = getModifersString() + L" " + _info->return_type + L" " + fName + L"(";
			
			int  size_paramenterNames = _info->paramenterNames.size();
			if (size_paramenterNames)
			{
				size_paramenterNames = size_paramenterNames - 1;
			}
			else
			{
				size_paramenterNames = -1;
			}
			auto size_formal_parameters = formal_parameterTypes.size();
			for (int i = 0, size = size_formal_parameters; i < size; ++i)
			{
				ret.append(formal_parameterTypes[i]);

				if (i <= size_paramenterNames) {
					ret.append(L" ").append(_info->paramenterNames[i]);
				}

				if (i != size - 1) {
					ret.push_back(',');
				}
			}
		

			ret += L"); in ";

			ret += boost::join(_info->qualifiedTypeNames, wstring(1, '.'));

			return ret;
		}

		void BaseMethod::addChild(IJavaElement* child)
		{
			assert(child);
			if(!child)
			{
				return;
			}
			switch(child->fLEType)
			{
			case TYPE:
			case FIELD:
			case LOCAL_FIELD:
			{
				auto full_name = child->getFullName();
				auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());
				_info->children.insert(std::make_pair(full_name, reinterpret_cast<Member*>(child)));
			}
			break;
			default:
				break;
			}
			
		}

		BaseMethod::~BaseMethod()
		{
			if (info) {
				delete info;
			}
		}

		BaseMethod::BaseMethod(IJavaElement* p, const wstring& name, const vector<wstring>& parameterTypes)
			: IMethod(p, name, parameterTypes),
			formal_parameterTypes(parameterTypes),
			status(0), _isConstructor(false),info(nullptr)
		{
			
		}

		BaseMethod::BaseMethod(BaseMethod& o)
			:
			IMethod(o),
			formal_parameterTypes(o.formal_parameterTypes), status(o.status),
			_isConstructor(o._isConstructor),info(nullptr)
		{
		
		}

	
		

		const vector<wstring>& BaseMethod::getParameterTypes()
		{
			return formal_parameterTypes;
		}

		const vector<wstring>& BaseMethod::getParameterNames()
		{
			auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());
			assert(_info);
			return  _info->paramenterNames;
		}

		std::wstring BaseMethod::getJavaCardDocUrlFragment()
		{
			auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());
			assert(_info);
			wstring ret;
			ret = boost::join(_info->qualifiedTypeNames, wstring(1, '/'));
			ret += L".html#" + fName + L"(";

			//<a href = "../../javacard/framework/Applet.html#getShareableInterfaceObject(javacard.framework.AID, byte)">getShareableInterfaceObject< / a>
			
			auto size_formal_parameters = formal_parameterTypes.size();
			for (int i = 0, size = size_formal_parameters; i < size; ++i){
				ret.append(formal_parameterTypes[i]);
				if (i != size - 1) {
					ret.append(L", ");
				}
			}
			ret += L")";
			return ret;
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

