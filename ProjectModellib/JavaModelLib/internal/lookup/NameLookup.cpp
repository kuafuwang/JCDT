#include "stdafx.h"
#include <JavaModelLib/internal/core/IType.h>
#include "NameLookup.h"
#include <JavaModelLib/internal/core/BaseProject.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include "IJavaElementRequestor.hpp"
#include <boost/algorithm/string.hpp>
#include <JavaModelLib/internal/core/IClassFile.h>
#include "SingleTypeRequestor.h"
#include <JavaModelLib/internal/core/ICompilationUnit.h>
#include "JavaElementRequestor.h"
#include <JavaModelLib/internal/core/JarPackageFragment.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/CloneJavaProject.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/PackageFragmentRoot.h>
#include <JavaModelLib/internal/core/BinaryType.h>
#include <JavaModelLib/compiler/BuildMsgInfo.h>
#include <JavaModelLib/internal/core/BinaryField.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		bool CaculateToDecideUpdateNameLookup(map<BaseProject::ModifyLevel, set<ResourceOptKind> >& record_level)
		{
			if(record_level.find(BaseProject::PROJECT) != record_level.end()){
				return true;
			}
			auto root_find_it = record_level.find(BaseProject::PKG_ROOT);
			if(root_find_it != record_level.end()){
				if(!root_find_it->second.empty()){
					return true;
				}
			}
			auto pkg_find_it = record_level.find(BaseProject::PKG);
			if (root_find_it != record_level.end()) {
				if (root_find_it->second.empty()) {
					return false;
				}
				for(auto opt_kind : root_find_it->second){
					if(ELEMENT_CHANGE != opt_kind)
						return true;	
				}
			}
			return false;
		}


		 const wchar_t* NameLookup::fireware_root = L"";
		 const wchar_t* NameLookup::fireware_pkg = L"";
		 const wchar_t* NameLookup::fireware_file = L"";

		void NameLookup::configureFromProject()
		{	
			bool need_to_update_namelookup = false;
            auto   pJM	= owner->getJavaModel();

			if (!pJM)
				return;

			map<BaseProject::ModifyLevel, set<ResourceOptKind> > record_level;
			if( new_version != cur_version)
			{
				cur_version = new_version;

				set<wstring> projects;

				pJM->GetProjectsInClassPaths(projects,owner->fName);

				// 检查哪些工程需要被移除
				// 1.不在依赖链上的工程
				for (auto it = clone_projects.begin(); it != clone_projects.end();) {
					const wstring& project_name = it->first;
					auto find_it = projects.find(project_name);
					if (find_it == projects.end()) {
						it->second = nullptr;// 释放引用，让其自己释放
						it = clone_projects.erase(it);
						need_to_update_namelookup = true;
					}
					else {
						++it;
					}
					projects.erase(project_name);
				}
				
				for (auto it :clone_projects) {
					auto original = pJM->getJavaProject(it.first);
					auto clone = it.second;
					record_level.clear();
					if (!clone)
						continue;
					clone->SynchronousClone(original, record_level);//更新现有的project
					if(!need_to_update_namelookup)
						need_to_update_namelookup = CaculateToDecideUpdateNameLookup(record_level);
				}

				// 添加原来没有的project拷贝
				for (auto it_new_proj:projects){
					auto original = pJM->getJavaProject(it_new_proj);
					auto clone = original->Clone();
					clone_projects.insert( make_pair(it_new_proj,  clone) );
					need_to_update_namelookup = true;
				}
			}
			else{
			
				for (auto it : clone_projects ) {
					auto original = pJM->getJavaProject(it.first);
					auto clone = it.second;
					record_level.clear();
					if (!clone)
						continue;
					clone->SynchronousClone(original, record_level);//更新现有的project
					if (!need_to_update_namelookup)
						need_to_update_namelookup = CaculateToDecideUpdateNameLookup(record_level);
				}
			}
			if (!need_to_update_namelookup)
				return;
			fPackageFragmentRoots.clear();
			fPackageFragments.clear();
			
			for (auto it : clone_projects) {
				auto clone_project = it.second;
				auto  _children = clone_project->getInternalChildren();
				for (auto root_it : _children) {
				
					if(fPackageFragmentRoots.find(root_it.first) != fPackageFragmentRoots.end()){
						continue;
					}

					auto pkg_root = dynamic_cast<PackageFragmentRoot*>(root_it.second);

					if (!pkg_root)
						continue;

					fPackageFragmentRoots.insert(root_it);
					auto  frags = pkg_root->getInternalChildren();
				
					for (auto pkg_it: frags) {
						IPackageFragment* fragment = pkg_it.second;

						if (!fragment)
							continue;

						vector<IPackageFragment*>& entry = fPackageFragments[fragment->getElementName()];
						entry.push_back(fragment);
					}
				}
			}	
		}

		IType* NameLookup::findPrimitiveType(const wstring& typeName)
		{
			if (typeName == L"byte")
			{
				return owner->byte_type;
			}
			else if (typeName == L"char")
			{
				return owner->char_type;
			}
			else if (typeName == L"short")
			{
				return owner->short_type;
			}
			else if (typeName == L"int")
			{
				return owner->int_type;
			}
			else if (typeName == L"long")
			{
				return owner->long_type;
			}
			else if (typeName == L"float")
			{
				return owner->float_type;
			}
			else 
			{
				return owner->double_type;
			}
			
		}

		IType* NameLookup::findArrayType(IType* baseType, unsigned dimension)
		{
			if (!baseType)
				return nullptr;
			if (0 ==dimension)
				return baseType;

			auto typeName = baseType->getFullyQualifiedName('.');
			wstring strDimension;
			for (unsigned i = 0; i < dimension; ++i){
				strDimension.append(L"[]");
			}
			typeName += strDimension;
			auto find_type = owner->firmware_file->getTypeByElementName(typeName);
			if (find_type)
				return find_type;

			auto type = new BinaryType(owner->firmware_file, typeName);
			owner->firmware_file->addChild(type);

			auto field = new BinaryField(type,L"length",FIELD);
			AccessFlags& access = field->getAccessFlags();
			access.ResetACC_PUBLIC();
			auto info = reinterpret_cast<SourceTypeElementInfo*>( type->getElementInfo());
			if(info)
				info->InsertField(field);
			type->MarkPrimitiveType();
			return type;


		}

		ICompilationUnit* NameLookup::GetCodeResoveUnit(LayerPosInJavaModel * _pos)
		{
			if(!_pos)
				return nullptr;

			auto find_it = clone_projects.find(_pos->proj);
			if(find_it != clone_projects.end())
			{
				auto proj = find_it->second;
				auto   pkg =   proj->findPackageFragmentByName(_pos->projectPos.root, _pos->projectPos.pkg);
				if (!pkg)
					return nullptr;

				return pkg->getCompilationUnit(_pos->projectPos.name);
			}
			return nullptr;
		}

		NameLookup::NameLookup(OriginalJavaProject* project):owner(project){
			new_version = 1;
			cur_version = 0;
			usage_reference = new CRITICAL_SECTION();
			InitializeCriticalSectionAndSpinCount(usage_reference, 2000);
			configureFromProject();
		}
		NameLookup::~NameLookup()
		{
			DeleteCriticalSection(usage_reference);
			delete usage_reference;
		}
		/**
		* Configures this <code>NameLookup</code> based on the
		* info of the given <code>IJavaProject</code>.
		*
		* @throws JavaModelException if the <code>IJavaProject</code> has no classpath.
		*/
		
		 bool NameLookup::acceptType(IType* type, int acceptFlags) {
			if (acceptFlags == 0)
				return true; // no flags, always accepted
		
			if (type && type->isClass()) {
				return (acceptFlags & ACCEPT_CLASSES) != 0;
			}
			else {
				return (acceptFlags & ACCEPT_INTERFACES) != 0;
			}
		}
		 /**
		 * Finds every type in the project whose simple name matches
		 * the prefix, informing the requestor of each hit. The requestor
		 * is polled for cancellation at regular intervals.
		 *
		 * <p>The <code>partialMatch</code> argument indicates partial matches
		 * should be considered.
		 */
		  void  NameLookup::findAllTypes(const wstring& prefix, bool partialMatch, int acceptFlags, 
			  IJavaElementRequestor* requestor) {

			  if (!requestor)
				  return;

			 for (auto it : fPackageFragmentRoots) {
				 if (requestor->isCanceled())
					 return;
				
				 auto root = reinterpret_cast<PackageFragmentRoot*>(it.second);

				 if (!root)
					 continue;
				 auto	packages = root->getInternalChildren();
				 				
				 for (auto itPkg : packages) {
					 if (requestor->isCanceled())
						 return;
					 seekTypes(prefix, itPkg.second,partialMatch, acceptFlags, requestor);
				 }
			 }
		 }

		  /**
		  * Notifies the given requestor of all types (classes and interfaces) in the
		  * given package fragment with the given (unqualified) name.
		  * Checks the requestor at regular intervals to see if the requestor
		  * has canceled. If the given package fragment is <code>null</code>, all types in the
		  * project whose simple name matches the given name are found.
		  *
		  * @param name The name to search
		  * @param pkg The corresponding package fragment
		  * @param partialMatch partial name matches qualify when <code>true</code>;
		  *	only exact name matches qualify when <code>false</code>
		  * @param acceptFlags a bit mask describing if classes, interfaces or both classes and interfaces
		  * 	are desired results. If no flags are specified, all types are returned.
		  * @param requestor The requestor that collects the result
		  *
		  * @see #ACCEPT_CLASSES
		  * @see #ACCEPT_INTERFACES
		  */
		   void NameLookup::seekTypes(const wstring& name, IPackageFragment* pkg,
			   bool partialMatch, int acceptFlags,
			   IJavaElementRequestor* requestor) {

			   wstring matchName = partialMatch ?  boost::to_lower_copy( name): name;
			  if (matchName.find('.') != wstring::npos) { //looks for member type A.B
				  matchName = boost::replace_all_copy(matchName, L".", L"$");
			  }
			  if (pkg == nullptr) {
				  findAllTypes(matchName, partialMatch, acceptFlags, requestor);
				  return;
			  }
			
				  int packageFlavor = pkg->getKind();
				  switch (packageFlavor) {
				  case IPackageFragmentRoot::K_BINARY:
					  seekTypesInBinaryPackage(matchName, pkg, partialMatch, acceptFlags, requestor);
					  break;
				  case IPackageFragmentRoot::K_SOURCE:
					  seekTypesInSourcePackage(matchName, pkg, partialMatch, acceptFlags, requestor);
					  break;
				  default:
					  return;
				  } 
		  }

	
		   /**
		   * Performs type search in a binary package.
		   */
		   void NameLookup::seekTypesInBinaryPackage(const wstring& name, IPackageFragment* pkg,
			   bool partialMatch, int acceptFlags, IJavaElementRequestor* requestor){
			   vector<IClassFile*>  classFiles ;
			   if (!pkg || !requestor)
				   return;
			    classFiles = pkg->getClassFiles();
			  

			   int length = classFiles.size();

			   wstring unqualifiedName = name;
			   int index = name.rfind('$');
			   if (index != wstring::npos) {
				   //the type name of the inner type
				   unqualifiedName = name.substr(index + 1, name.length());
				   // unqualifiedName is empty if the name ends with a '$' sign
				   if ((unqualifiedName.length() > 0 
					   && isdigit(unqualifiedName[0]))
					   || unqualifiedName.length() == 0) {
					   unqualifiedName = name;
				   }
			   }
			   wstring matchName = partialMatch ? boost::to_lower_copy(name) : name;
			   for (int i = 0; i < length; i++) {
				   if (requestor->isCanceled())
					   return;
				   IClassFile* classFile = classFiles[i];

				   if (!classFile)
					   continue;

				   wstring elementName = classFile->getElementName();
				   if (partialMatch)
					   boost::to_lower(elementName);
				   /**
				    * Must use startWith because matchName will never have the
				    * extension ".class" and the elementName always will.
				   */
				   if (elementName.find(matchName) != wstring::npos) {		 
					   IType*  type = classFile->getType();
						if (!type)
							continue;

					   if (partialMatch) { //not an anonymous type
						   auto typeName = type->getElementName();
						   if(typeName.length() > 0 && !isdigit(typeName[0])){
							   if (nameMatches(unqualifiedName, type, partialMatch)
								   && acceptType(type, acceptFlags))
								   requestor->acceptType(type);
						   }
					   }
					   else{
						   if (nameMatches(unqualifiedName, type, partialMatch)
							   && acceptType(type, acceptFlags))
							   requestor->acceptType(type);
					   }
				   }
			   }
		   }
		   /**
		   * Performs type search in a source package.
		   */
		   void NameLookup::seekTypesInSourcePackage(
			   const wstring& name, IPackageFragment* pkg,
			   bool partialMatch, int acceptFlags,
			   IJavaElementRequestor* requestor){
			  
			   if (!pkg || !requestor)
				   return;
			 auto   compilationUnits = pkg->getCompilationUnits();
			   
			  
			   int length = compilationUnits.size();
			   wstring matchName = name;
			   int index = name.find('$');
			   bool potentialMemberType = false;
			   wstring potentialMatchName;
			   if (index != wstring::npos) {
				   //the compilation unit name of the inner type
				   potentialMatchName = name.substr(0, index);
				   potentialMemberType = true;
			   }

			   /**
			   * In the following, matchName will never have the extension ".java" and
			   * the compilationUnits always will. So add it if we're looking for
			   * an exact match.
			   */
			   auto unitName = partialMatch ? boost::to_lower_copy( matchName) : matchName /*+ L".java"*/; //$NON-NLS-1$
			   wstring potentialUnitName ;
			   if (potentialMemberType) {
				   potentialUnitName = partialMatch ? boost::to_lower_copy(potentialMatchName): potentialMatchName/* + L".java"*/; //$NON-NLS-1$
			   }

			   for (int i = 0; i < length; i++) {
				   if (requestor->isCanceled())
					   return;
				   ICompilationUnit* compilationUnit = compilationUnits[i];

				   if (!compilationUnit)
					   continue;

				   if (!potentialMemberType 
					   || nameMatches(unitName, compilationUnit, partialMatch)) {
					   
					  
					  auto types = compilationUnit->getTypes();
					  if (types) {
						  int typeLength = types->size();
						  for (int j = 0; j < typeLength; j++) {
							  if (requestor->isCanceled())
								  return;
							  IType* type = types->operator[](j);
							  if (nameMatches(matchName, type, partialMatch)) {
								  if (acceptType(type, acceptFlags))
									  requestor->acceptType(type);
							  }
						  }
					  }
				   }
				   else if (potentialMemberType && nameMatches(potentialUnitName, compilationUnit, partialMatch)) {
					   
					   auto types = compilationUnit->getTypes();
					   if (types) {
						   int typeLength = types->size();
						   for (int j = 0; j < typeLength; j++) {
							   if (requestor->isCanceled())
								   return;
							   IType* type = types->operator[](j);
							   if (nameMatches(potentialMatchName, type, partialMatch)) {
								   seekQualifiedMemberTypes(name.substr(index + 1, name.length()),
									   type, partialMatch, requestor, acceptFlags);
							   }
						   }
					   }
				   }
				 
			   }
		   }
	

		
			/**
			* Notifies the given requestor of all types (classes and interfaces) in the
			* given type with the given (possibly qualified) name. Checks
			* the requestor at regular intervals to see if the requestor
			* has canceled.
			*
			* @param partialMatch partial name matches qualify when <code>true</code>,
			*  only exact name matches qualify when <code>false</code>
			*/
			void NameLookup::seekQualifiedMemberTypes(const wstring& qualifiedName, IType* type,
				bool partialMatch, IJavaElementRequestor* requestor, int acceptFlags) {
				if (!type  || !requestor || !type)
					return;
				
				 auto 	types = type->getTypes();
				if(!types)
					return;

				auto matchName = qualifiedName;
				auto index = qualifiedName.find('$');
				bool nested = false;
				if (index != wstring::npos) {
					matchName = qualifiedName.substr(0, index);
					nested = true;
				}
				int length = types->size();
				for (int i = 0; i < length; i++) {
					if (requestor->isCanceled()) {
						return;
					}
					IType* memberType = types->operator[](i);
					if (nameMatches(matchName, memberType, partialMatch))
						if (nested) {
							seekQualifiedMemberTypes(
								qualifiedName.substr(index + 1, qualifiedName.length()),
								memberType,
								partialMatch,
								requestor, 
								acceptFlags);
						}
						else {
							if (acceptType(memberType, acceptFlags))
								requestor->acceptMemberType(memberType);
						}
				}
			
			}

			/**
			* Notifies the given requestor of all package fragments with the
			* given name. Checks the requestor at regular intervals to see if the
			* requestor has canceled. The domain of
			* the search is bounded by the <code>IJavaProject</code>
			* this <code>NameLookup</code> was obtained from.
			*
			* @param partialMatch partial name matches qualify when <code>true</code>;
			*	only exact name matches qualify when <code>false</code>
			*/
			void NameLookup::seekPackageFragments(const wstring& name, bool partialMatch,
				IJavaElementRequestor* requestor)
		    {
				if (!requestor)
					return;

				int count = fPackageFragmentRoots.size();
				auto matchName = partialMatch ? boost::to_lower_copy( name ): name;
				for (auto it : fPackageFragmentRoots) {
					if (requestor->isCanceled())
						return;
					auto root =   reinterpret_cast<PackageFragmentRoot*>( it.second );

					if (!root)
						continue;

					auto list = root->getInternalChildren();
					if (list.empty())
						continue;
					auto end_list = list.end();
					for (auto it_list = list.begin(); it_list != end_list; ++it_list) {
						if (requestor->isCanceled())
							return;
						auto packageFragment = it_list->second;
						if (nameMatches(matchName, packageFragment, partialMatch))
							requestor->acceptPackageFragment(packageFragment);
					}
				}
			}
			/**
			* Returns true if the given element's name matches the
			* specified <code>searchName</code>, otherwise false.
			*
			* <p>The <code>partialMatch</code> argument indicates partial matches
			* should be considered.
			* NOTE: in partialMatch mode, the case will be ignored, and the searchName must already have
			*          been lowercased.
			*/
			bool NameLookup::nameMatches(const wstring& searchName, IJavaElement* element, bool partialMatch)
			{
				assert(element);

				if (!element)
					return false;

				auto ele_name = element->getElementName();
				if (partialMatch) {
					// partial matches are used in completion mode, thus case insensitive mode
					if(CharOperation::prefixEquals(searchName, ele_name,false))
						return true;
					else
						return false;
				}
				else 
					return ele_name == searchName;
			}
			/**
			* Returns the first type in the given package whose name
			* matches the given (unqualified) name, or <code>null</code> if none
			* exist. Specifying a <code>null</code> package will result in no matches.
			* The domain of the search is bounded by the Java project from which
			* this name lookup was obtained.
			*
			* @param name the name of the type to find
			* @param pkg the package to search
			* @param partialMatch partial name matches qualify when <code>true</code>,
			*	only exact name matches qualify when <code>false</code>
			* @param acceptFlags a bit mask describing if classes, interfaces or both classes and interfaces
			* 	are desired results. If no flags are specified, all types are returned.
			*
			* @see #ACCEPT_CLASSES
			* @see #ACCEPT_INTERFACES
			*/
			IType* NameLookup::findType(const wstring& name, IPackageFragment* pkg,
				bool partialMatch, int acceptFlags) {
				if (!pkg) {
					return nullptr;
				}
				SingleTypeRequestor* typeRequestor = new SingleTypeRequestor();
				seekTypes(name, pkg, partialMatch, acceptFlags, typeRequestor);
				IType* type = typeRequestor->getType();
				delete typeRequestor;
				return type;
			
			}

			/**
			* Returns the type specified by the qualified name, or <code>null</code>
			* if none exist. The domain of
			* the search is bounded by the Java project from which this name lookup was obtained.
			*
			* @param name the name of the type to find
			* @param partialMatch partial name matches qualify when <code>true</code>,
			*	only exact name matches qualify when <code>false</code>
			* @param acceptFlags a bit mask describing if classes, interfaces or both classes and interfaces
			* 	are desired results. If no flags are specified, all types are returned.
			*
			* @see #ACCEPT_CLASSES
			* @see #ACCEPT_INTERFACES
			*/
			IType* NameLookup::findType(const wstring& name, bool partialMatch, int acceptFlags) {
				int index = name.rfind('.');
				wstring className , packageName ;

				if (index == wstring::npos) {
					packageName = IPackageFragment::DEFAULT_PACKAGE_NAME;
					className = name;
				}
				else {
					packageName = name.substr(0, index);
					className = name.substr(index + 1);
				}
				return findType(className, packageName, partialMatch, acceptFlags);
			}

			/**
			* Returns all the package fragments found in the specified
			* package fragment roots. Make sure the returned fragments have the given
			* project as great parent. This ensures the name lookup will not refer to another
			* project (through jar package fragment roots)
			*/
			vector<IPackageFragment*> NameLookup::getPackageFragmentsInRoots(
				vector<IPackageFragmentRoot*>& roots, IJavaProject* project)
			{

				// The following code assumes that all the roots have the given project as their parent
				vector<IPackageFragment*> frags ;
				auto roots_data = roots.data();
				for (int i = 0; i < roots.size(); i++) {
					auto root = dynamic_cast<PackageFragmentRoot*>(roots_data[i]);
					if (!root)
						continue;

					auto children = root->getInternalChildren();

						/* 2 jar package fragment roots can be equals but not belonging
						to the same project. As a result, they share the same element info.
						So this jar package fragment root could get the children of
						another jar package fragment root.
						The following code ensures that the children of this jar package
						fragment root have the given project as a great parent.
						*/
						
						if (children.empty()) 
							continue;

						
						if (root->getJavaProject() == (project) ) {
							// the children have the right parent, simply add them to the list
							auto end_children = children.end();
							for (auto it = children.begin(); end_children != it; ++ it) {
									frags.push_back(it->second);
							}
						}
						else {
							// create a new handle with the root as the parent
							auto end_children = children.end();
							for (auto it = children.begin(); end_children != it; ++it) {
								auto temp = root->getPackageFragment(it->first);
								frags.push_back(temp);
							}
						}
					
				}
			
				return frags;
			}

			/**
			*
			*/
			IType* NameLookup::findType(const wstring& typeName,  wstring packageName, bool partialMatch, int acceptFlags) {
				if (packageName.empty()) {
					packageName = IPackageFragment::DEFAULT_PACKAGE_NAME;
				}
				JavaElementRequestor* elementRequestor = new JavaElementRequestor();
				seekPackageFragments(packageName, false, elementRequestor);
				auto packages = elementRequestor->getPackageFragments();

				for (int i = 0, length = packages.size(); i < length; i++) {
					IType* type = findType(typeName, packages[i], partialMatch, acceptFlags);
					if (type != nullptr)
					{
						delete elementRequestor;
						return type;
					}
				}
				delete elementRequestor;
				return nullptr;
			}

			/**
			* Returns the package fragments whose name matches the given
			* (qualified) name, or <code>null</code> if none exist.
			*
			* The name can be:
			*	- empty: ""
			*	- qualified: "pack.pack1.pack2"
			* @param partialMatch partial name matches qualify when <code>true</code>,
			*	only exact name matches qualify when <code>false</code>
			*/
			vector<IPackageFragment*> NameLookup::findPackageFragments( wstring name, bool partialMatch)
			{
			
				if (partialMatch) {
					boost::to_lower( name );
					vector<IPackageFragment*> result;
					for (auto itRoot : fPackageFragmentRoots) {
						auto root = reinterpret_cast<PackageFragmentRoot*>(itRoot.second);
						if (!root)
							continue;

						auto	list = root->getInternalChildren();
						int elementCount = list.size();
						result.reserve(result.size() + elementCount);
						auto end_list = list.end();
						for (auto it = list.begin(); it != end_list; ++it) {
							auto packageFragment = it->second;
							if (nameMatches(name, packageFragment, true)) {
								result.push_back(packageFragment);
							}
						}
					}
					return result;
				}
				else {
					auto it =  fPackageFragments.find(name);
					if(it != fPackageFragments.end())
						return it->second;
					else
						return vector<IPackageFragment*>();
				}
			}

			/**
			* Returns the <code>ICompilationUnit</code> which defines the type
			* named <code>qualifiedTypeName</code>, or <code>null</code> if
			* none exists. The domain of the search is bounded by the classpath
			* of the <code>IJavaProject</code> this <code>NameLookup</code> was
			* obtained from.
			* <p>
			* The name must be fully qualified (eg "java.lang.Object", "java.util.Hashtable$Entry")
			*/
			ICompilationUnit* NameLookup::findCompilationUnit(const wstring& qualifiedTypeName)
			{
				auto pkgName = IPackageFragment::DEFAULT_PACKAGE_NAME;
				auto cuName = qualifiedTypeName;

				auto index = qualifiedTypeName.rfind('.');
				if (index != wstring::npos) {
					pkgName = qualifiedTypeName.substr(0, index);
					cuName = qualifiedTypeName.substr(index + 1);
				}
				index = cuName.find('$');
				if (index != wstring::npos) {
					cuName = cuName.substr(0, index);
				}
				cuName += L".java"; //$NON-NLS-1$
			    auto it   =  fPackageFragments.find(pkgName);
				if (it != fPackageFragments.end()) {
					auto frags = it->second;
					for (int i = 0; i < frags.size(); i++) {
						auto frag = frags[i];
						if (!( dynamic_cast<JarPackageFragment*>(frag))) {
							ICompilationUnit* cu = frag->getCompilationUnit(cuName);
							if (cu) {
								return cu;
							}
						}
					}
				}
				return nullptr;
			}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

