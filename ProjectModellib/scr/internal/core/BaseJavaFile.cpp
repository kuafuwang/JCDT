#include "stdafx.h"
#include <JavaModelLib/internal/core/BaseJavaFile.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include <JavaModelLib/internal/core/PackageFragment.h>
#include <JavaModelLib/internal/core/JarPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/IJavaProject.h>
#include <JavaModelLib/internal/core/JarPackageFragment.h>
#include <JavaModelLib/internal/core/BaseProject.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/IClassFile.h>
#include <JavaModelLib/internal/core/ICompilationUnit.h>

#include <boost/algorithm/string.hpp>

#include <PathModelLib/MosesPath/MosesPath.h>
#include <JavaModelLib/internal/core/IImportDeclaration.h>

namespace Jikes { // Open namespace Jikes block

	namespace JavaModel {
		
		BaseJavaFile::~BaseJavaFile(){

			if (types){

				for (auto it : *types){
					delete it;
				}

				delete types;
				types = nullptr;
			}

			if (packageDecls) {

				for (auto it : *packageDecls)
					delete it;
				delete packageDecls;
				packageDecls = nullptr;

			}
			if(_pos)
			{
				delete _pos;
			}
			if(importDecls)
			{
				for (auto it : *importDecls)
					delete it;
				delete importDecls;
				importDecls = nullptr;
			}
		}

		BaseJavaFile::BaseJavaFile(IJavaElementType _type,
			IPackageFragment* p,const wstring& name):
			Openable(p,_type, name),m_iDeclsVersion(0), _pos(nullptr), types(nullptr), importDecls(nullptr), package(p)
		{
			
		
		}

		BaseJavaFile::BaseJavaFile(BaseJavaFile& o)
			: Openable(o), m_iDeclsVersion(o.m_iDeclsVersion),
			_pos(nullptr), types(nullptr), importDecls(nullptr),package(o.package)
		{
		
		
		}


		

		

		void BaseJavaFile::removeInfo()
		{
			if(packageDecls){
				for (auto it : *packageDecls)
					delete it;
				packageDecls->clear();
			}

			if (types) {
				for (auto it : *types)
					delete it;
				types->clear();
			}
			if(importDecls){
				for (auto it : *importDecls)
					delete it;
				importDecls->clear();
			}
		}


	

		std::vector<IJavaElement*>* BaseJavaFile::getChildren()
		{
			LazyLoad();
			return new std::vector<IJavaElement*>(types->begin(), types->end());
		}

		const vector<IType*>* BaseJavaFile::getAllTypes()
		{
				LazyLoad();
				return types ;
			
		}

		IType* BaseJavaFile::getTypeByElementName(wstring& elementName)
		{
			LazyLoad();
			if(types)
			{
				auto size = types->size();
				for (auto _t : *types)
				{
					if (!_t)
						continue;

					if(_t->getElementName() == elementName)
					{
						return _t;
					}
				}
			}
			return nullptr;
		}
		
		BaseJavaFile* BaseJavaFile::GetNoLazyLoadAndSeparateFromParentClone()
		{
			auto types = getAllTypes();
			if (types && types->size())
			{
				auto temp = types->operator[](0);
				if(temp)
					temp->getSourceRange();//针对.class文件，让其解析attach source 来匹配行号8
			}
		

			auto clone = reinterpret_cast<BaseJavaFile*>(DeepCopy(fParent));
			clone->CalCulatePos();
			clone->fParent = nullptr;
			return clone;
		}

		vector<wstring>& BaseJavaFile::getImportPackages()
		{
			LazyLoad();
			if(importDecls)
			{
				if(import_pakcages.empty()){
					for(const auto& it : *importDecls){
						if(it)
						 import_pakcages.push_back(it->getElementName());
					}
				}
			}
			return import_pakcages;
		}

		void BaseJavaFile::SortOutLineNameIdentity()
		{
			LazyLoad();
			if (types)
			{
				map<wstring, int> occurrents;
				auto end = occurrents.end();
				auto size = types->size();
				for (auto i = 0; i < size; ++i)
				{
					auto item = types->operator[](i);
					if(!item)
					{
						continue;
					}

					wstring& outlineName = item->fName;
					auto find_it = occurrents.find(outlineName);
					if (end != find_it)
					{
						find_it->second += 1;
						item->fOccurrenceCount = find_it->second;
					}
					else {
						occurrents.insert(make_pair(outlineName, 1));
					}
					item->SortOutLineNameIdentity();
				}
			}
		}

		void BaseJavaFile::CalCulatePos()
		{
			auto pkg = reinterpret_cast<IPackageFragment*>(fParent);
			if (!pkg)
				return;

			auto root = reinterpret_cast<IPackageFragmentRoot*>(pkg->getParent());
			if(!root){
				return;
			}

			auto project = reinterpret_cast<IJavaProject*>(root->getParent());
			if (!project)
				return;

			
			if (_pos){
				_pos->proj = project->getElementName();
				_pos->projectPos.root = root->getElementName();
				_pos->projectPos.pkg = pkg->getElementName();
				_pos->projectPos.name = fName;
			}
			else
			{
				auto _type = _BELONG_TO_CLASS_FILE;
				if (root->getKind() == IPackageFragmentRoot::K_SOURCE)
					_type = _BELONG_TO_CLASS_FILE;
					
				_pos = new LayerPosInJavaModel(project->getElementName(),
					root->getElementName(),
					pkg->getElementName(), fName, _type);
			}
				
			
		}


		wstring BaseJavaFile::getPath()
		{
			wstring rootPath;
			if (fParent)
			{
				rootPath =fParent->getPath();
			}
			
			if (rootPath.empty())
				return{};
			if (IsZip())
				return rootPath;

			rootPath.append(L"/").append(fName);
			if (fLEType != CLASS_FILE)
				rootPath.append(L".java");
			else
				rootPath.append(L".class");
			return rootPath;
		}

		IJavaElement* BaseJavaFile::DeepCopy(IJavaElement* parent)
		{
			auto clone = reinterpret_cast<BaseJavaFile*>(ShallowCopy(parent));
			clone->HelpToDeepCopy(*this);
			return clone;
		}

		wstring BaseJavaFile::getAttachSourcePath()
		{

			if(fParent){

				auto _root = reinterpret_cast<IPackageFragmentRoot*>(fParent->getParent());
				if (!_root)
					return wstring();
				wstring root_path;
				if(_root->getKind() == IPackageFragmentRoot::K_SOURCE)
				{
					 root_path = _root->getPath();
					
				}
				else
				{
					root_path = _root->getSourceAttachmentRootPath();
					if (root_path.empty())
						return{};
				}
				auto pkg_part = boost::replace_all_copy(fParent->getElementName(), L".", L"/");
			
				auto _path = root_path  +L"/" +
					pkg_part
					+ L"/" + fName + L".java";
				
				return _path;

			}
			
			return{};

		}

		std::vector<IJavaElement*>* BaseJavaFile::getChildrenOfType(IJavaElementType _type)
		{
			LazyLoad();
			if(_type == IJavaElementType::TYPE)
			{
				if (types)
					return new vector<IJavaElement*>(types->begin(), types->end());
				else
					return nullptr;
			}
			else if(_type == IJavaElementType::PACKAGE_DECLARATION)
			{
				if (packageDecls)
					return new vector<IJavaElement*>(packageDecls->begin(), packageDecls->end());
				else
					return nullptr;
			}
			return nullptr;
		}

		bool BaseJavaFile::hasChildren()
		{
			LazyLoad();
			if(types && types->empty() == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		PathModel::MosesPath* BaseJavaFile::getMosesPath()
		{
			if (moses)
				return moses;
			if (!fParent)
				return nullptr;

			auto parent_moses = reinterpret_cast<PkgMoses*>(fParent->getMosesPath());
			if (!parent_moses)
				return nullptr;
			bool binary;
			if(fLEType == COMPILATION_UNIT)
			{
				binary = false;
			}
			else
			{
				binary = true;
			}
			moses = new FileMoses(
				parent_moses->proj,
				parent_moses->root,
				parent_moses->pkg , fName, binary);

			return moses;
		}

	
		bool BaseJavaFile::IsZip() {
			return fLEType == JAR_CLASS_FILE || fLEType == JAR_COMPILATION_UNIT;
		}
		void BaseJavaFile::addChild(IJavaElement* child)
		{
			if (child)
			{
				if (child->getElementType() == IJavaElementType::TYPE)
				{
					if (!types) {
						types = new vector<IType*>();
					}
					types->push_back(dynamic_cast<IType*>(child));
				}

			}
		}


		void BaseJavaFile::HelpToDeepCopy(BaseJavaFile& o)
		{
			fIsStructureKnown = o.fIsStructureKnown;
			if (o._pos) {
				_pos = new LayerPosInJavaModel(
					o._pos->proj,
					o._pos->projectPos.root,
					o._pos->projectPos.pkg,
					o._pos->projectPos.name,
					o._pos->projectPos._type);
			}

			if (o.types)
			{

				auto orininal_types_data = o.types->data();
				size_t i = 0, size = o.types->size();
				if (!types)
					types = new vector<IType*>(size);
				else
					types->assign(size, nullptr);

				auto copy_types_data = types->data();
				for (; i < size; ++i) {
					auto temp = reinterpret_cast<IType*>(orininal_types_data[i]->DeepCopy(this));
					copy_types_data[i]= temp;
				}

			}
			if (o.packageDecls){

				auto  orininal_pkg_data = o.packageDecls->data();
				size_t i = 0, size = o.packageDecls->size();
				if(!packageDecls)
					packageDecls = new vector<IPackageDeclaration*>(size);
				else
					packageDecls->assign(size, nullptr);

				auto copy_packageDecls = packageDecls->data();
				for (; i < size ; ++i) {
					auto temp = reinterpret_cast<IPackageDeclaration*>( orininal_pkg_data[i]->DeepCopy(this) );
					copy_packageDecls[i] = (temp);
				}

			}

			if(o.importDecls){

				auto  orininal_import_data = o.importDecls->data();
				size_t i = 0, size = o.importDecls->size();
				if(!importDecls)
					importDecls = new vector<IImportDeclaration*>(size);
				else
					importDecls->assign(size, nullptr);

				auto copy_import_data = importDecls->data();
				for (; i < size; ++i) {
					auto temp = reinterpret_cast<IImportDeclaration*>(orininal_import_data[i]->DeepCopy(this));
					copy_import_data[i] = (temp);
				}

			}
			m_iDeclsVersion = o.m_iDeclsVersion;
		}

		bool BaseJavaFile::CopyChildFromOriginalWhenIsClone()
		{
			if (is_Clone && fParent)
			{

				auto pkg = (fParent);
				auto pkg_root = (pkg->getParent());
				if (!pkg_root)
					return true;

				auto proj = (pkg_root->getParent());
				if (!proj)
					return true;

				auto original_proj = getJavaModel()->getJavaProject(proj->getElementName());
				if (!original_proj)
					return true;

				original_proj->beginToOccupy();
				auto __original_pkg = original_proj->getPackageFrament(pkg_root->getElementName(), pkg->getElementName());
				
				BaseJavaFile* original_file = nullptr;
				if(__original_pkg)
				{
					if (fLEType == CLASS_FILE || fLEType == JAR_CLASS_FILE)
						original_file = reinterpret_cast<BaseJavaFile*>( __original_pkg->getClassFile(fName) );
					else
						original_file = reinterpret_cast<BaseJavaFile*>(__original_pkg->getCompilationUnit(fName));
				}
				if (original_file){
					if (!original_file->isStructureKnown())
						original_file->buildStructure(true, nullptr);

					HelpToDeepCopy(*original_file);
				}
				
				original_proj->endToOccupy();

				return true;
			}
			{
				return false;
			}
		}

	} // Close namespace JavaModel block

} // Close namespace Jikes block

