#include "stdafx.h"
#include "SelectionRequestor.h"
#include "NameLookup.h"
#include <JavaModelLib/internal/core/IJavaElement.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/core/ICompilationUnit.h>
#include <boost/algorithm/string.hpp>
#include <JavaModelLib/internal/core/JavaElement.h>
#include <JCDT_Lib/internal/util/stringex.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {


		void SelectionRequestor::recordMatchSelectElement(IJavaElement* element)
		{
			if(element)
				fElements.push_back(element);
		}

		SelectionRequestor::~SelectionRequestor()
		{
			if(fNameLookup)
			{
				fNameLookup->endToOccupy();
			}
			if (pos)
				delete pos;
		}

		SelectionRequestor::SelectionRequestor(boost::shared_ptr<OriginalJavaProject>_project, LayerPosInJavaModel* _pos)
		{
			project = _project;
			fCodeResolve = nullptr;
			pos = _pos;
			fNameLookup = nullptr;
		}

	
	
		

	

		void SelectionRequestor::acceptClass(const wstring& packageName,
			const wstring& className, bool needQualification)
		{
			acceptType(packageName, className, NameLookup::ACCEPT_CLASSES, needQualification);
		}
		void SelectionRequestor::acceptField(const wstring& declaringTypePackageName,
			const wstring& declaringTypeName, const wstring& name)
		{
			IType *type = resolveType(declaringTypePackageName, declaringTypeName,
				NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES);
			if (type != nullptr) {
				IField* field = type->getField(name);
				if (field) {
					fElements.push_back(field);
				}
			}
		}

		void SelectionRequestor::acceptInterface(const wstring& packageName,
			const wstring& interfaceName, bool needQualification)
		{
			acceptType(packageName, interfaceName, NameLookup::ACCEPT_INTERFACES, needQualification);
		}

		void SelectionRequestor::acceptMethod(
			const wstring& declaringTypePackageName,
			const wstring& declaringTypeName,
			const wstring& selector,
			 vector<wstring>& parameterPackageNames,
			 vector<wstring>& parameterTypeNames,
			bool isConstructor)
		{
			IType* type = resolveType(declaringTypePackageName, declaringTypeName,
				NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES);
			// fix for 1FWFT6Q
			if (type)
				return;
			if (type->isBinary()) {
				if (isConstructor)
				{
					IType* declaringDeclaringType = dynamic_cast<IType*>(type->getDeclaringType());
					if(declaringDeclaringType != nullptr)
					{
						// need to add a paramater for constructor in binary type
						auto pkg_frag = declaringDeclaringType->getPackageFragment();
						if (!pkg_frag)
							return;
						parameterPackageNames.insert(parameterPackageNames.begin(),pkg_frag->getElementName());

						parameterTypeNames.insert(parameterTypeNames.begin(),
							declaringDeclaringType->getTypeQualifiedName('$'));
					}	
				}
				auto methods = resolveMethod(type, selector, parameterPackageNames, parameterTypeNames);
				if (methods.size()) {
					fElements.push_back(methods[0]);
				}
			}
			else {
				vector<IMethod*> matches = resolveMethod(type, selector, parameterPackageNames,
					parameterTypeNames);

				// if no matches, nothing to report
				if (matches.size() == 0) {
					// no match was actually found, but a method was originally given -> default constructor
					fElements.push_back(type);
				}
				else {
					for (auto it : matches) {
						fElements.push_back(it);
					}
				}
			}	
		}

		vector<IMethod*> SelectionRequestor::getMethod(const wstring& declaringTypePackageName,
			const wstring& declaringTypeName,
			const wstring& selector,
			vector<wstring>& parameterPackageNames,
			vector<wstring>& parameterTypeNames,
			bool isConstructor)
		{
			IType* type = resolveType(declaringTypePackageName, declaringTypeName,
				NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES);
			// fix for 1FWFT6Q
			if (!type)
				return vector<IMethod*>();
			if (type->isBinary()) {
				if (isConstructor) {
					IType* declaringDeclaringType = dynamic_cast<IType*>(type->getDeclaringType());
					if (declaringDeclaringType != nullptr)
					{
						// need to add a paramater for constructor in binary type
						int length = parameterPackageNames.size();
						auto pkg_frag = declaringDeclaringType->getPackageFragment();
						if (!pkg_frag)
							return {};

						parameterPackageNames.insert(parameterPackageNames.begin(),pkg_frag->getElementName());

						parameterTypeNames.insert(parameterTypeNames.begin(),
							declaringDeclaringType->getTypeQualifiedName('$'));
					}
				}	
			}
			return resolveMethod(type, selector, parameterPackageNames, parameterTypeNames);			
		}


		void SelectionRequestor::acceptPackage(const wstring& packageName)
		{
			getNameLookUp();

			if (!fNameLookup)
				return;

			auto pkgs = fNameLookup->findPackageFragments(packageName, false);
			if (pkgs.size() != 0) {
				for (int i = 0, length = pkgs.size(); i < length; i++) {
					fElements.push_back(pkgs[i]);
				}
			}
		}

		vector<IMethod*> SelectionRequestor::resolveMethod(IType* type, const wstring& selector,
			const vector<wstring>& parameterPackageNames, const vector<wstring>& parameterTypeNames)
		{
			vector<IMethod*> matches;
			if (!type)
				return matches;

			if(type->isBinary()){
				vector<wstring> parameterTypes;
				if (parameterTypeNames.size() != 0) {

					for (int i = 0, max = parameterTypeNames.size(); i < max; i++) {
						auto pkg = IPackageFragment::DEFAULT_PACKAGE_NAME;
						if (parameterPackageNames[i].empty() != true && parameterPackageNames[i].length() > 0) {
							pkg = parameterPackageNames[i] + L"."; //$NON-NLS-1$
						}
						auto typeName = parameterTypeNames[i];
						if (typeName.find('.') > 0)
							boost::replace_all(typeName, L".", L"$");
						parameterTypes.push_back(pkg + typeName);
					}
				}
				IMethod* method = type->getMethod(selector, parameterTypes);
				if(method)
					matches.push_back(method);
			}
			else{
				auto methods = type->getMethods();
				if (!methods)
					return vector<IMethod*>();
				for (auto it : *methods) {

					if (!it)
						continue;

					auto elementName = it->getElementName();
					if (elementName == selector){
						const std::vector<wstring>&  signatures = it->getParameterTypes();
						auto signature_size = signatures.size();
						auto parameter_size = parameterTypeNames.size();
						if (signature_size != parameter_size)
							continue;	

						bool match = true;
						for (auto p = 0; p < signature_size; p++) {
							auto signature_name = signatures[p];
							const wstring& parameterPackageName = parameterPackageNames[p];
						
							if (parameterPackageName.size()){
								const wstring& parameterTypeName = parameterTypeNames[p];
								Jikes::wstringex  compoundName(parameterPackageName);
								compoundName.push_back('.');
								compoundName.append(parameterTypeName);
								
								if (!compoundName.end_with(signature_name)) {
									match = false;
									break;
								}
							}
							else{
								Jikes::wstringex  parameterTypeName = parameterTypeNames[p];
								if (!parameterTypeName.end_with(signature_name)) {
									match = false;
									break;
								}
							}
						}
						if (match)
							matches.push_back(it);
					}
				}
				
			}
			return  matches;
		}


		void SelectionRequestor::acceptType(const wstring& packageName, const wstring& typeName, bool needQualification)
		{
			acceptType(packageName, typeName, NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES, needQualification);
		}

		IType* SelectionRequestor::getType(const wstring& packageName, 
			const wstring& typeName, int acceptFlags) 
		{
			return resolveType(packageName, typeName, acceptFlags);
		}

		void SelectionRequestor::acceptType(const wstring& packageName,
			const wstring& typeName,
			int acceptFlags, bool needQualification)
		{
			IType *type = resolveType(packageName, typeName, acceptFlags);
			
			if (type != nullptr) {
				fElements.push_back(type);	
			}
		}

		NameLookup* SelectionRequestor::getNameLookUp()
		{
			if(!fNameLookup){
				fNameLookup = project->getNameLookup();
				if(!fNameLookup)
				{
					assert(false);
					return nullptr;
				}
				fNameLookup->beginToOccupy();
				fCodeResolve = fNameLookup->GetCodeResoveUnit(pos);
			}
			return fNameLookup;
		}

		IType*  SelectionRequestor::resolveType(const wstring& packageName,
			const wstring& typeName, int acceptFlags) 
		{
			getNameLookUp();
			if (!fNameLookup)
				return nullptr;

			if(NameLookup::IsPrimitiveType(typeName)){
				return fNameLookup->findPrimitiveType(typeName);
			}
			else 
			{
				auto idx = typeName.find(L"[");
				if( idx != wstring::npos){
					auto baseTypeStr = typeName.substr(0, idx);
					int dimension = 0;
					auto size = typeName.size();
					for(int i = idx; i  < size; ++ i){
						if(typeName[i] == '['){
							dimension += 1;
						}
					}
					IType *base_type = resolveType(packageName, baseTypeStr, NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES);
					return fNameLookup->findArrayType(base_type, dimension);

				}
				
			}
			IType* type = nullptr;
			ICompilationUnit* wc = dynamic_cast<ICompilationUnit*>(fCodeResolve);
			if (wc) 
			{
				     auto decls = wc->getPackageDeclarations();
					 auto pkgNameLen = packageName.length();
					if (  (  (pkgNameLen == 0)   &&  decls  && decls->size() == 0 ) ||
						
						(!(pkgNameLen == 0)
							&& wc->getPackageDeclaration(packageName))) 
					{
						vector<wstring> compoundName;
						boost::split(compoundName, typeName, boost::is_any_of("."));
						
						if (compoundName.size() > 0) {
							type = wc->getType(compoundName[0]);
							if (type) {
								for (int i = 1, length = compoundName.size(); i < length; i++) {
									type = type->getType(compoundName[i]);
								}
							}
						}

				
					}
			}

			if (!type) {
				auto pkgs = fNameLookup->findPackageFragments(
					(packageName.length() == 0) ? IPackageFragment::DEFAULT_PACKAGE_NAME : (packageName),
					false);
				// iterate type lookup in each package fragment
				for (int i = 0, length = pkgs.size(); i < length; i++) {
					type = fNameLookup->findType(typeName, pkgs[i], false, acceptFlags);
					if (type)
						break;
				}
				if (!type) {
					wstring pName;
					if (packageName.empty() != true) 
						pName = packageName;	
					else
						pName = IPackageFragment::DEFAULT_PACKAGE_NAME;
		
					if (fCodeResolve) 
					{
						auto tmep =  fCodeResolve->getParent() ;
						if(tmep && tmep->getElementName() == (pName))
						{
							// look inside the type in which we are resolving in
							auto tName = (typeName);
							boost::replace_all(tName, L".", L"$");

							
							auto allTypes = fCodeResolve->getAllTypes();
							if(allTypes){
								for (auto it : *allTypes) {
									auto _type = dynamic_cast<IType*>(it);
									if (_type && _type->getTypeQualifiedName('$') == (tName)) {
										delete allTypes;
										return _type;
									}
								}
							}
						}	
					}
				}
			}
			return type;
		}

		vector<IMethod*> SelectionRequestor::getMethod(const wstring& declaringTypePackageName, const wstring& declaringTypeName, const wstring& selector, bool isConstructor)
		{
			IType* type = resolveType(declaringTypePackageName, declaringTypeName,
				NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES);
			// fix for 1FWFT6Q
			if (!type)
				return vector<IMethod*>();
			
			vector<IMethod*> matches;
			
			auto methods = type->getMethods();
			if (!methods)
				return matches;
			for (auto it : *methods) {

				if (!it)
					continue;

				auto elementName = it->getElementName();
				if (elementName == selector) {
					matches.push_back(it);
				}
			}
		
			return  matches;
		}
	} // Close namespace JavaModel block


} // Close namespace Jikes block

