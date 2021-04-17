#include "stdafx.h"
#include "MatchLocator.h"
#include <JavaModelLib/internal/core/ClassFile.h>
#include <SearchLib/internal/core/IJavaSearchScope.h>
#include <ProjectModelLib/ProjectModel/ProjectSolution/SolutionModel.h>
#include <JavaModelLib/internal/core/JavaModel.h>

#include <JavaModelLib/internal/core/CloneJavaProject.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/JarPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/JarPackageFragment.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>


#include <vector>
#include <set>
#include <JCDT_Lib/internal/parser/parser.h>
#include <JCDT_Lib/internal/lex/scanner.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <SearchLib/internal/pattern/SearchPattern.h>
#include "MatchingOpenable.h"
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include "LocatorCompiler.h"
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <SearchLib/internal/core/JavaSearchScopeData.h>
#include <SearchLib/internal/core/IJavaSearchResultCollector.h>
#include "MatchSet.h"
#include <PathModelLib/MosesPath/MosesPath.h>
#include "SearchResultNode.h"
#include <JCDT_Lib/internal/lookup/UnitFileSymbol.h>


using namespace Jikes::JavaModel;

using namespace std;

namespace Jikes { // Open namespace Jikes block


	
namespace Search{


	MatchLocator::MatchLocator(SearchPattern* _pattern, int _detailLevel, IJavaSearchResultCollector* _collector,
		IJavaSearchScope* _scope):pattern(_pattern), detailLevel(_detailLevel), collector(_collector), scope(_scope)
	{

	
	}

	/**
	* Locate declaration in the current class file. This class file is always in a jar.
	*/
	 void MatchLocator::locateMatchesInClassFile(Jikes::JavaModel::ClassFile* classFile)
	 {
		
	
	}

	 void MatchLocator::locateMatches(JavaModel::JavaModel* workspace,
		 IProgressMonitor* progressMonitor) {

		 if (!workspace){
			 assert(false);
			return;
		 }
		// 第一步初步过滤
		auto& groupInfos = 	scope->getPaths();
		auto jm = workspace;
	    for(auto it : groupInfos)
	    {
			auto original_project = jm->getJavaProject(it->groupName);
			if(!original_project){
				continue;
			}

			Option* option = new Option();
			option->bytecode = false;
			JikesClassPaths jikes_class_paths;
			jm->getExpandClasspath(jikes_class_paths, it->groupName);
			INameEnvironment* nameEnv = new INameEnvironment(option, jikes_class_paths);
			LocatorCompiler* compiler = new LocatorCompiler(nameEnv, option, original_project);
			vector< IPackageFragmentRoot* > binaryRoots;
			vector< IPackageFragmentRoot* > sourceRoots;
		
			auto& items = it->items;
			vector<LayerPosInJavaModel* > files_pos;

			original_project->beginToOccupy();
			for (auto info : items) {
				if (!info->with_sub_dir) {

					files_pos.push_back(original_project->getLayerPos(info->path));
				}
			}
			auto clone_project = original_project->Clone();
			original_project->endToOccupy();

			bool copy_children = collector->IsCopyChildren();

			auto roots = clone_project->getInternalChildren();
			auto _end = roots.end();

			for(auto info : items){
				if(info->with_sub_dir){
					auto find_it = roots.find(info->name);
					if(find_it == _end || !find_it->second){
						continue;
					}
					if(find_it->second->getKind() == IPackageFragmentRoot::K_BINARY){
						binaryRoots.push_back(find_it->second);
					}
					else{
						sourceRoots.push_back(find_it->second);
					}
				}
			}

			vector<CompilationUnit*> source_files;
			vector<JavaModel::ClassFile*> class_files;

			for(size_t i = 0 , size = files_pos.size(); i < size; ++i){
				auto _pos = files_pos[i];

				if (!_pos)
					continue;

				auto _pkg = clone_project->getPackageFrament(_pos->projectPos.root,_pos->projectPos.pkg);
					
				if (!_pkg)continue;
				if(_pos->projectPos._type == FileTypeInJavaProject::_A_EDIT_FILE){
				  auto 	_file = reinterpret_cast<CompilationUnit*>(_pkg->getCompilationUnit(_pos->projectPos.name));
				  source_files.push_back(_file);
				}
				else if(_pos->projectPos._type == FileTypeInJavaProject::_BELONG_TO_CLASS_FILE)
				{
					auto _file = reinterpret_cast<JavaModel::ClassFile*>(
						_pkg->getClassFile(_pos->projectPos.name));
					class_files.push_back(_file);
				}	
				delete _pos;
			}

			for(auto  it_soure_root : sourceRoots){
				auto real_root = (PackageFragmentRoot*)(it_soure_root);

				if (!real_root)
					continue;

				auto pkg_children = real_root->getInternalChildren();
				auto pkg_it = pkg_children.begin();
				auto pkg_end = pkg_children.end();
				for (; pkg_it != pkg_end; ++pkg_it){
					auto real_pkg = (PackageFragment*)(pkg_it->second);

					if (!real_pkg)
						continue;

					auto compiliatonunits = real_pkg->getInternalCompilationUnits();
					if (!compiliatonunits)
						continue;
					for (auto unit_pair : *compiliatonunits) {
						source_files.push_back((CompilationUnit*) unit_pair.second );
					}
				}
			}
			
				
			NameSymbol* name_symbol;
			
			vector<MatchingCompilationOpenable*> compilation_openables;
			set<FileSymbol*> pre_match_file_set;
			for(auto unit : source_files)
			{
				auto _path = unit->getPath();
			     name_symbol = nameEnv->FindOrInsertName(_path.c_str(), _path.size());
				 string utf_8_name = name_symbol->Utf8Name();
				FileSymbol* symbol = new UnitFileSymbol(name_symbol, utf_8_name);
				compiler->scanner->Scan(symbol);
				if (symbol->lex_stream && pattern->IsMayInTheFile(symbol->lex_stream)) {
					compiler->accept(symbol);
					auto openable = new MatchingCompilationOpenable(
						symbol,
						*compiler,
						unit, pattern);
					openable->matchSet->setLex(symbol->lex_stream);
					compilation_openables.push_back(openable);
					openable->locateMatchesInCompilationUnitBeforResolve();
				}
				else{
					delete symbol;
				}
			}
		
			auto data_openables = compilation_openables.data();
			for(size_t i =0, size = compilation_openables.size(); i < size ; ++i){
				auto _file = data_openables[i];

				if (!_file)
					continue;

				if(_file->ShouldResovle()){
					pre_match_file_set.insert(_file->TransferFileSymbol());
				}
				else
				{
					if (_file->matchSet->result.size()) {
						FileResultElement* item = new FileResultElement();
						item->resouce = _file->openable->getMosesPath()->Clone();
						_file->matchSet->getResult(item, copy_children);
						collector->accept(item);
					}
				}
			}

			if(pre_match_file_set.size()){	
				//jm->getExpandClasspath(jikes_class_paths, it->groupName);
				//nameEnv->AddClassPaths(jikes_class_paths);
				//nameEnv->RereadDirectories();

				compiler->compiler(pre_match_file_set);
				for (size_t i = 0, size = compilation_openables.size(); i < size; ++i){
					auto openable = data_openables[i];
					if (openable && openable->ShouldResovle()) {
						openable->locateMatchesInCompilationUnitAfterResolve();
						if (openable->matchSet->result.size()) {
							FileResultElement* item = new FileResultElement();
							item->resouce = openable->openable->getMosesPath()->Clone();
							openable->matchSet->getResult(item,copy_children);
							collector->accept(item);
						}
					}
				}
			}

			vector<MatchingClassOpenable*> class_openables;
			for(auto _Root : binaryRoots){
				auto binaryRoot = reinterpret_cast<JarPackageFragmentRoot*>(_Root);

				if (!binaryRoot)
					continue;

				auto pkg_children = binaryRoot->getInternalChildren();
				for(auto pkg_pair : pkg_children){
					auto pkg = reinterpret_cast<JarPackageFragment*>(pkg_pair.second);
					if(!pkg ||  !pattern->matchesBinaryPackge(pkg->getElementName())){
						continue;
					}
					auto class_children = pkg->getInternalClassFiles();
					if(!class_children)
						continue;

					for(auto class_pair : *class_children){
						auto class_file = reinterpret_cast<JavaModel::ClassFile*>(class_pair.second);
						if (!class_file || !pattern->matchesBinaryClass( class_file->getElementName() )) {
							continue;
						}

						auto openable = new MatchingClassOpenable(class_file, *compiler, pattern);
						openable->locateMatchesInClassFiles();
						class_openables.push_back(openable);
						if (openable->matchSet->result.size()) {
							FileResultElement* item = new FileResultElement();
							item->resouce = openable->openable->getMosesPath()->Clone();
							openable->matchSet->getResult(item, copy_children);
							collector->accept(item);
						}
					}	
				}
			}
			for(auto it_class_file : class_files)
			{
				auto openable = new MatchingClassOpenable(it_class_file, *compiler, pattern);
				openable->locateMatchesInClassFiles();
				class_openables.push_back(openable);
				if (openable->matchSet->result.size()) {
					FileResultElement* item = new FileResultElement();
					item->resouce = openable->openable->getMosesPath()->Clone();
					openable->matchSet->getResult(item, copy_children);
					collector->accept(item);
				}
			}
			for(size_t i = 0 , size = class_openables.size(); i < size; ++i){
				auto openable = class_openables[i];
				delete openable;
			}

			for (size_t i = 0, size = compilation_openables.size(); i < size; ++i) {
				auto openable = compilation_openables[i];
				delete openable;
			}
			delete compiler;

	    }
	}

	MatchLocator::~MatchLocator()
	{
	    delete  pattern;
		delete scope;
	}
}


} // Close namespace Jikes block


