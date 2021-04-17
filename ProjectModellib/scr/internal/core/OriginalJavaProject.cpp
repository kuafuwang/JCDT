#include "stdafx.h"
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/IPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include <JavaModelLib/internal/core/CloneJavaProject.h>
#include "JCDT_Lib/internal/impl/option.h"
#include <boost/algorithm/string.hpp>
#include <JavaModelLib/internal/core/JarPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <JavaModelLib/compiler/BraveCompiler.h>
#include <JavaModelLib/internal/core/PackageFragment.h>
#include <boost/smart_ptr/make_shared.hpp>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>
#include <JCDT_Lib/internal/impl/FileLocation.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>

#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JavaModelLib/internal/core/BinaryType.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <PathModelLib/SourcePath/UnitManager.h>
#include <PathModelLib/SourcePath/CompilationUnitTypInfo.h>

#include "JavaModelLib/compiler/DefaultProblem.h"
using namespace std;
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		void OriginalJavaProject::addPackageFramentRoot(const wstring& _name,
			const wstring& _path, const wstring&  attach_path ,bool _source)
		{	
			if (wstring::npos != _path.find(L".jar") || wstring::npos != _path.find(L".zip")) {
				auto pkg_root = new JarPackageFragmentRoot(_path, this, _name, attach_path);
				all_children.insert(make_pair(_name, pkg_root));
			}
			else
			{	
				int root_type;
				if (_source) {
					root_type = IPackageFragmentRoot::K_SOURCE;
				}
				else {
					root_type = IPackageFragmentRoot::K_BINARY;
				}
				auto pkg_root = new PackageFragmentRoot(_path, root_type, this, _name, attach_path);
				all_children.insert(make_pair(_name, pkg_root));
			}
		}

		IJavaElement* OriginalJavaProject::DeepCopy(IJavaElement* parent)
		{
			auto clone = new CloneJavaProject(*this);
			clone->MarkClone();
			clone->HelpToDeepCopy(*this);
			return clone;
		}

		IJavaElement* OriginalJavaProject::ShallowCopy(IJavaElement* parent)
		{
		   auto clone =	new CloneJavaProject(*this);
		   clone->MarkClone();
		   return clone;
		}

		bool OriginalJavaProject::generateInfos(IProgressMonitor* pm)
		{
			JikesClassPaths paths;
			getJavaModel()->getClasspathButNoPorjcetEntry(paths,fName);
			 auto it = paths._paths.begin();
			 auto end = paths._paths.end();
			 for (; it != end; ++it) {
				 auto _name = it->first;
				 auto element = it->second;
				 if (!element)
					 continue;
				 addPackageFramentRoot(_name, element->class_path, element->source_attach_path,element->source_only);
			 }
			 calculateSourceEntry();

			 for (auto _soure_it = source_children.begin(); _soure_it != source_children.end(); ++_soure_it)
			 {
				 if(_soure_it->second)
					 _soure_it->second->buildStructure(true,nullptr);
			 }
			 return true;
		}

		void OriginalJavaProject::removeInfo()
		{

			set<wstring> files;
			getAllSourceFiles(files);
			opt_stack->Clear();
			BaseProject::removeInfo();
			if (mSignal) {
				ProjectNotify* _n = new ProjectNotify(ELEMENT_CHANGE, fName);
				vector<NameLookUpNotify*> push_notisfy(1, _n);
				mSignal->operator()(push_notisfy);
				delete _n;
			}
	
		}

	

		void OriginalJavaProject::SetOutputDir(const wstring& _path)
		{
			window_style_output_dir = boost::replace_all_copy(_path, L"/", L"\\");
			
		}

		OriginalJavaProject::OriginalJavaProject(
			IJavaModel* p,
			const wstring& name,
			const wstring& projectPath,
			PathModel::JarPathManager& jar_path_mgr
			)
			: BaseProject(p, projectPath, name),
			jar_path_manager_(jar_path_mgr), 
			mSignal(nullptr),
			namel_lookup_(nullptr),
			old_project_stamp(0),
			cur_project_stamp(0),
			rebuild(false)
		
		{
			
			firmware = new PackageFragmentRoot(NameLookup::fireware_root,
				IPackageFragmentRoot::K_BINARY, this,
				NameLookup::fireware_root,L""
			);
			auto pkg = new PackageFragment(NameLookup::fireware_pkg, firmware, PACKAGE_FRAGMENT);
			pkg->setIsStructureKnown(true);
			firmware->addChild(NameLookup::fireware_pkg, pkg);
			firmware->setIsStructureKnown(true);

			firmware_file = new ClassFile(CLASS_FILE, pkg, NameLookup::fireware_file);
			pkg->addChild(firmware_file);
			firmware_file->setIsStructureKnown(true);

			byte_type = new BinaryType(firmware_file,L"byte");
			byte_type->MarkPrimitiveType();
			firmware_file->addChild(byte_type);


			char_type = new BinaryType(firmware_file, L"char");
			char_type->MarkPrimitiveType();
			firmware_file->addChild(char_type);

			short_type = new BinaryType(firmware_file, L"short");
			short_type->MarkPrimitiveType();
			firmware_file->addChild(short_type);




			int_type = new BinaryType(firmware_file, L"int");
			int_type->MarkPrimitiveType();
			firmware_file->addChild(int_type);


			long_type = new BinaryType(firmware_file, L"long");
			long_type->MarkPrimitiveType();
			firmware_file->addChild(long_type);
		

			float_type = new BinaryType(firmware_file, L"float");
			float_type->MarkPrimitiveType();
			firmware_file->addChild(float_type);


			double_type = new BinaryType(firmware_file, L"double");
			double_type->MarkPrimitiveType();
			firmware_file->addChild(double_type);


		}
		void OriginalJavaProject::PushResourceOpt(vector<NameLookUpNotify*>&  _notisfys)
		{
			EnterCriticalSection(opt_cs);
			for (auto i = 0; i < _notisfys.size(); ++i){
				opt_stack->PushNotify(_notisfys[i]);
			}
			::InterlockedIncrement(&cur_project_stamp);
			LeaveCriticalSection(opt_cs);
		}
		void OriginalJavaProject::PushResourceOpt(NameLookUpNotify* _notisfy) 
		{
			EnterCriticalSection(opt_cs);
			opt_stack->PushNotify(_notisfy);
			
			::InterlockedIncrement(&cur_project_stamp);
			LeaveCriticalSection(opt_cs);
		}

		void OriginalJavaProject::IncrementProcessRoot(NotifyResult::RootT& _roots)
		{
			auto remove_root_find = _roots.find(ELEMENT_REMOVE);
			if (remove_root_find != _roots.end()) {

				for (auto it : remove_root_find->second) {
					auto find_it = all_children.find(it.first);
					if (find_it != all_children.end())
					{
						delete find_it->second;
						all_children.erase(find_it);
					}	
				}
			}
			auto change_root_find = _roots.find(ELEMENT_CHANGE);
			if (change_root_find != _roots.end()) {
				for (auto it : change_root_find->second){
					auto find_it = all_children.find(it.first);
					if (find_it != all_children.end() && find_it->second) {
						PackageFragmentRoot* _root_temp = reinterpret_cast<PackageFragmentRoot*>(find_it->second);
						_root_temp->setIsStructureKnown(false);
						if(_root_temp->getKind() == IPackageFragmentRoot::K_BINARY){
							auto _notisfy = (PackageFragmentRootMsg*)(it.second);
							if(_notisfy)
								_root_temp->sourceAttachmentRootPath = _notisfy->attach_path;
						}		
					}
				}
			}

			// 这里复制处理add 和  change的拷贝
			auto add_root_find = _roots.find(ELEMENT_ADD);
			if (add_root_find != _roots.end()) {
				for (auto it : add_root_find->second){
					auto _name = it.first;
					auto find_it = all_children.find(_name);
					if (find_it != all_children.end() && find_it->second){
						
						find_it->second->setIsStructureKnown(false);
					}
					else{
						auto _notisfy = (PackageFragmentRootMsg*)(it.second);
						if(_notisfy)
							addPackageFramentRoot(_name, _notisfy->root_path, _notisfy->attach_path, _notisfy->source_only);	
					}
				}
			}
			calculateSourceEntry();
		}

		void OriginalJavaProject::IncrementUpdateProcessPkg(NotifyResult::PkgT& _pkgs)
		{
			for (auto it : _pkgs) {
				auto pkg_root_name = it.first;
				auto find_it_original = findPackageFragmentRootByName(pkg_root_name);
				if (find_it_original) {
					find_it_original->IncrementUpdateProcess(it.second);
				}
			}
		}

		void OriginalJavaProject::IncrementUpdateProcessFiles(NotifyResult::FileT& _files)
		{
			for (auto it : _files)
			{
				auto pkg_root_name = it.first;
				for (auto it_pkg : it.second){
					auto pkg_name = it_pkg.first;
					auto  find_it_original = findPackageFragmentByName(pkg_root_name, pkg_name);
					if (find_it_original)
						find_it_original->IncrementUpdateProcess(it_pkg.second);
				}
			}
		}
	
		
		void OriginalJavaProject::HookChangeSourceFiles(NotifyResult::FileT& _files, NotifyResult::FileT& hook)
		{
			for (auto it = _files.begin(); it != _files.end(); ++it)
			{
				auto pkg_root_name = it->first;
				bool in_source_entry = false;
				IPackageFragmentRoot* root = nullptr;
				auto find_it = source_children.find(pkg_root_name);
				if (find_it != source_children.end())
					root = find_it->second;
				else
					continue;

			
				for (auto it_pkg : it->second)
				{
					auto pkg_name = it_pkg.first;
					auto  find_it_original =  root->getPackageFragment(pkg_name);
					if (find_it_original){
						auto change_files = it_pkg.second.find(ELEMENT_CHANGE);
						if(change_files != it_pkg.second.end()){
							hook[pkg_root_name][pkg_name][ELEMENT_CHANGE] = change_files->second;
							change_files->second.clear();
						}	
					}
				}
			}
		}

		void OriginalJavaProject::IncrementUpdate(NotifyResult* _notifys){

			if (!_notifys)
				return;

			beginToOccupy();
			if (_notifys->act == REBUILD) {
				endToOccupy();
				makeConsistent(nullptr);
				return;
			}
			IncrementProcessRoot(_notifys->_roots);
			IncrementUpdateProcessPkg(_notifys->_pkgs);
			IncrementUpdateProcessFiles(_notifys->_files);
			endToOccupy();
		}

		

		void OriginalJavaProject::CalculateOptStack()
		{
			vector<NameLookUpNotify*> _stack;
			EnterCriticalSection(opt_cs);
			if (!opt_stack->empty()) {
				_stack.swap(opt_stack->stack);
			}
			old_project_stamp = cur_project_stamp;
			LeaveCriticalSection(opt_cs);


			NotifyResult_For_Origianal* _notifys;
			if (!_stack.empty()) {
				_notifys = (NotifyResult_For_Origianal*)PathModel::ResouceOptStack::GetClassifyNotify(_stack, true);
			}
			else
				_notifys = nullptr;



			if (_notifys) {
				IncrementUpdate(_notifys);
				if (mSignal && _notifys->all_notify) {
					mSignal->operator()(*(_notifys->all_notify));
				}
				if (!rebuild) {
					set<wstring> needTodeleteFile;
					if (!_notifys->remove_file_set.empty())
					{

						for (auto it : _notifys->remove_file_set) {
							auto& _path = it.second;
							problems.erase(_path);
							needTodeleteFile.insert(_path);
						}

					}

					for (auto it : _notifys->chang_file_set) {
						needTodeleteFile.insert(it.second);
					}
					auto end = last_compiled_class_path.end();
					for (auto it : needTodeleteFile) {
						auto temp_path = boost::replace_all_copy(it, L"\\", L"/");
						auto find_it = last_compiled_class_path.find(temp_path);
						if (find_it != end) {
							auto& _the_set = find_it->second;
							boost::system::error_code ec;
							for (auto delete_file : _the_set) {

								boost::filesystem::remove(delete_file, ec);
							}
							last_compiled_class_path.erase(find_it);
						}
					}
				}
			}

			if (rebuild) {
				problems.clear();
				boost::system::error_code ec;
				boost::filesystem::remove_all(window_style_output_dir, ec);
				last_compiled_class_path.clear();
			}
			
			rebuild = false;

			delete _notifys;

			for (auto it : _stack) {
				delete it;
			}

			
		}


		OriginalJavaProject::~OriginalJavaProject() {
			set<wstring> file;
			getAllSourceFiles(file);

			if (mSignal){
				delete mSignal;
			}
			if (namel_lookup_)
				delete namel_lookup_;
			delete firmware;
		}

		
		boost::shared_ptr<CloneJavaProject> OriginalJavaProject::Clone() {
			auto clone = new CloneJavaProject(*this);
			boost::shared_ptr<CloneJavaProject> spClone(clone);
			if (!mSignal) {
				mSignal = new SignalType();
			}
			mSignal->connect(
				SignalType::slot_type( 
					&CloneJavaProject::CloneAcceptNotify, spClone.get(), _1).track(spClone)
				);
				return spClone;
		}

		BaseJavaFile* OriginalJavaProject::getJavaFileUnit(LayerPosInProject* _pos)
		{
			assert(_pos);
			if(!_pos)
			{
				return nullptr;
			}
			beginToOccupy();
			
			LazyLoad();
			BaseJavaFile* _ret = nullptr;
		
			auto root_find_it = all_children.find(_pos->root);
			if (root_find_it == all_children.end())
			{
				endToOccupy();
				return nullptr;
			}
				
			auto real_root = (PackageFragmentRoot*)(root_find_it->second);
			if (!real_root)
			{
				endToOccupy();
				return nullptr;
			}



			auto pkgs = real_root->getInternalChildren();
			auto pkg_find_it = pkgs.find(_pos->pkg);
			if (pkg_find_it == pkgs.end())
			{
				endToOccupy();
				return nullptr;
			}


			auto real_pkg = (PackageFragment*)(pkg_find_it->second);
			if (!real_pkg)
			{
				endToOccupy();
				return nullptr;
			}


			BaseJavaFile* unit = nullptr;
			if(real_root->getKind() == PackageFragmentRoot::K_SOURCE)
			{
				auto files = real_pkg->getInternalCompilationUnits();
				if(files == nullptr)
				{			
					endToOccupy();
					return nullptr;
				}
				auto find_it = files->find(_pos->name);
				if (find_it != files->end())
				{
					unit = find_it->second;
				}
					
			}
			else
			{
				auto files = real_pkg->getInternalClassFiles();
				if(files == nullptr)
				{
					endToOccupy();
					return nullptr;
				}
				auto find_it= files->find(_pos->name);
				if (find_it != files->end())	
					unit=  find_it->second;
			}
			endToOccupy();
			return unit;
		}

		NameLookup* OriginalJavaProject::getNameLookup()
		{
			beginToOccupy();
			if (!namel_lookup_)
				namel_lookup_ = new NameLookup(this);
			
			endToOccupy();
			return namel_lookup_;
		}


		
	
		LayerPosInJavaModel* OriginalJavaProject::getLayerPos(const wstring& _path)
		{
			
			beginToOccupy();
			LazyLoad();
			map< wstring, FileTypeInJavaProject>  possiable_root;
			for(const auto& it : all_children)
			{
				auto root = it.second;

				if (!root)
					continue;

				FileTypeInJavaProject _type;
				wstring root_path;
				if (root->getKind() == IPackageFragmentRoot::K_SOURCE) {
					root_path = ((PackageFragmentRoot*)root)->resoucePath.wstring();
					_type = _A_EDIT_FILE;
				}
				else {
					root_path = root->getSourceAttachmentRootPath();
					_type = _BELONG_TO_CLASS_FILE;
				}
				if (root_path.empty())
					continue;

				auto idx = _path.find(root_path);
				if (idx != wstring::npos) 
				{
					possiable_root.insert({ it.first,_type });
				}
			}
			
			for(const auto& it : possiable_root)
			{
				auto root = all_children[it.first];
				FileTypeInJavaProject _type;
				wstring root_path;
				if (root->getKind() == IPackageFragmentRoot::K_SOURCE) {
					root_path = ((PackageFragmentRoot*)root)->resoucePath.wstring();
					_type = _A_EDIT_FILE;
				}
				else {
					root_path = root->getSourceAttachmentRootPath();
					_type = _BELONG_TO_CLASS_FILE;
				}
				if (root_path.empty())
					continue;

				auto idx = _path.find(root_path);
				if (idx != wstring::npos)
				{
					wstring pkg_and_file_name = _path.substr(idx + root_path.size());

					if (CharOperation::suffixEquals(L".java", pkg_and_file_name, false))
					{
						wstring fileName(boost::filesystem::path(_path).stem().wstring());
						pkg_and_file_name = pkg_and_file_name.substr(0, pkg_and_file_name.size() - 5);

						auto idx_pkg = pkg_and_file_name.rfind(fileName);
						if (idx_pkg != wstring::npos) 
						{
							wstring pkg = pkg_and_file_name.substr(1, idx_pkg - 2);//
							boost::replace_all(pkg, L"/", L".");

							auto pkgFrag = root->getPackageFragment(pkg);
							if (!pkgFrag)
								continue;

							const auto _ret = new LayerPosInJavaModel(fName, it.first, pkg, fileName, it.second);
							endToOccupy();
							return _ret;
						}
					}
				}
				
			}

			endToOccupy();

			return nullptr;
		}

		ComilerMsgInfo*  OriginalJavaProject::UpdateCompilationUnitInSourceEntry(const wstring& errorInfo)
		{

			ComilerMsgInfo* info = new ComilerMsgInfo(fName);
			
			beginToOccupy();
			std::vector< ECJSource*> files;
			ParserXmlData(errorInfo, files);
	
			auto _size = files.size();
			has_problem_files.clear();
			///last_compiled_class_path[_path].swap(paths_set);
			for (auto i = 0; i < _size; ++i)
			{
				auto filesymbol = files[i];
				const auto& _path = filesymbol->filePath;
				info->infos.insert(_path);
				auto find_problem = problems.find(_path);

				if (!filesymbol)
					continue;

				auto number = filesymbol->NumErrors();
				if(number)
				{
					has_problem_files.insert(_path);
				}
				if (find_problem != problems.end()) {
					if (!number) {
						find_problem->second = nullptr;
					}
					else {
						find_problem->second = boost::make_shared<BuildProblem>();
						filesymbol->TransferError(find_problem->second->data);
					}
				}
				else
				{
					if (!number) {
						problems.insert(make_pair(_path, nullptr));
					}
					else {
						BuildProblem* temp = new BuildProblem();
						problems.insert(make_pair(_path, boost::shared_ptr< BuildProblem >(temp)));
						filesymbol->TransferError(temp->data);
					}
				}
				
				last_compiled_class_path[_path].swap(filesymbol->class_file_paths);

			}
			

			endToOccupy();

		
			return  info;
		}
		void OriginalJavaProject::getAllSourceFiles(set<wstring>& files)
		{
			beginToOccupy();
			auto it = source_children.begin();
			auto end = source_children.end();
			for (; it != end; ++it){		
				auto real_root = (PackageFragmentRoot*)(it->second);
				if (!real_root)
					continue;
				auto pkg_children = real_root->getInternalChildren();
				auto pkg_it = pkg_children.begin();
				auto pkg_end = pkg_children.end();
				for (; pkg_it != pkg_end; ++pkg_it)
				{
					auto real_pkg =(PackageFragment*)(pkg_it->second);

					if (!real_pkg)
						continue;

					auto compiliatonunits = real_pkg->getInternalCompilationUnits();
					if (!compiliatonunits)
						continue;
					for (auto it_file : *compiliatonunits){
						if (!it_file.second)
							continue;
						files.insert(it_file.second->getPath());
					}
				}
			}
			endToOccupy();
	   }


		void OriginalJavaProject::GetErrorFilse(set<wstring>& out)
		{
			beginToOccupy();
			for(const auto& it :has_problem_files)
			{
				out.insert(it);
			}
			endToOccupy();
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

