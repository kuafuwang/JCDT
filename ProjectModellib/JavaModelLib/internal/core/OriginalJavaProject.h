
#ifndef OriginalJavaProject_java_model_INCLUDED_INCLUDED
#define OriginalJavaProject_java_model_INCLUDED_INCLUDED
#include "BaseProject.h"
#include <boost/signals2/signal.hpp>
#include <JavaModelLib/internal/lookup/SelectionRequestor.h>
#include "ClassFile.h"
#include <JavaModelLib/compiler/BuildMsgInfo.h>
#include <boost/unordered/unordered_map.hpp>

using std::pair;
using std::string;
namespace Jikes { // Open namespace Jikes block
	class FileSymbol;
	class JikesError;
	class Semantic;

	namespace CodeAssist
	{
		class SelectionEngine;
	}

	namespace PathModel
	{
		class UnitManager;
		class CompilationUnitTypInfo;
	}
namespace JavaModel{
	class LayerPosInJavaModel;
	
	class NameLookup;
	class BraveCompiler;
	class CloneJavaProject;
	class CompilationUnit;
	class LayerPosInProject;
	class BuildMsgInfo;
	class IJavaModel;
	class PackageFragmentRoot;
	class BinaryType;
	
	class OriginalJavaProject :public BaseProject
	{
	public:
		IJavaElement* DeepCopy(IJavaElement*) override;


		IJavaElement* ShallowCopy(IJavaElement* parent) override;
	protected:
		bool generateInfos(IProgressMonitor* pm) override;
		void removeInfo() override;

	public:

	
	
		void SetOutputDir(const wstring& _path);
		OriginalJavaProject(
			IJavaModel* p,
			const wstring& name,
			const wstring& projectPath, 
			PathModel::JarPathManager&  jar_path_mgr
		);
		virtual ~OriginalJavaProject();
		boost::shared_ptr<CloneJavaProject> Clone();


		BaseJavaFile* getJavaFileUnit(LayerPosInJavaModel* _pos){
			return getJavaFileUnit(&(_pos->projectPos));
		}
		BaseJavaFile* getJavaFileUnit(LayerPosInProject* _pos);

		void PushResourceOpt(vector<NameLookUpNotify*>&  _notisfys);
		void PushResourceOpt(NameLookUpNotify*  _notisfy) ;
		void IncrementUpdate(NotifyResult*);
		void CalculateOptStack();
	public:
		void IncrementProcessRoot(NotifyResult::RootT& _roots);
		void IncrementUpdateProcessPkg(NotifyResult::PkgT& _pkgs);
		void IncrementUpdateProcessFiles(NotifyResult::FileT& _files);
		
		void HookChangeSourceFiles(NotifyResult::FileT& _files, NotifyResult::FileT& hook);
		void MarkBraveCompilerRebuild();

		 NameLookup* getNameLookup();
		 NameLookup* getNameLookupWhenNoLookupDonotGenerateIt() const;
		
		
		bool IsNeedBuild() const;
		LayerPosInJavaModel* getLayerPos(const wstring& _path);
		ComilerMsgInfo* UpdateCompilationUnitInSourceEntry(const wstring& errorInfo);
		
		wstring window_style_output_dir;
	

		boost::shared_ptr<vector<JikesError*>> symstem_problems;
		
		map< wstring, set<wstring> > last_compiled_class_path;
		void GetErrorFilse(set<wstring >&);
		void getAllSourceFiles(set<wstring>&);
		PathModel::JarPathManager&  jar_path_manager_;
		PackageFragmentRoot* firmware;
		ClassFile* firmware_file;
		BinaryType* byte_type;
		BinaryType* char_type;
		BinaryType* short_type;
		BinaryType* int_type;
		BinaryType* long_type;
		BinaryType* float_type;
		BinaryType* double_type;
		std::map<wstring, boost::shared_ptr< BuildProblem > > problems;
	
	private:

		typedef boost::signals2::signal< void(vector<NameLookUpNotify*>&) > SignalType;

		SignalType* mSignal;

		NameLookup* namel_lookup_;
		unsigned  old_project_stamp;
		volatile unsigned  cur_project_stamp;
		
		bool rebuild;
	
		set<wstring> has_problem_files;

		void addPackageFramentRoot(const wstring&, const wstring&, const wstring&, bool);

		
	};

	inline  NameLookup* OriginalJavaProject::getNameLookupWhenNoLookupDonotGenerateIt() const
	{
		return namel_lookup_;
	}
	
	inline bool OriginalJavaProject::IsNeedBuild() const {
		return  old_project_stamp != cur_project_stamp;
	};
	inline void  OriginalJavaProject::MarkBraveCompilerRebuild()
	{
		EnterCriticalSection(opt_cs);
		rebuild = true;
		::InterlockedIncrement(&cur_project_stamp);
		LeaveCriticalSection(opt_cs);
	}

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

