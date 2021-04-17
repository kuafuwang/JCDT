
#ifndef CloneJavaProject_java_model_INCLUDED_INCLUDED
#define CloneJavaProject_java_model_INCLUDED_INCLUDED
#include "BaseProject.h"
#include <PathModelLib/DeltaPath/ResouceOptStack.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class OriginalJavaProject;
	class IJavaModel;
	class CloneJavaProject :public BaseProject
	{
	public:
		IJavaElement* DeepCopy(IJavaElement*) override;
		IJavaElement* ShallowCopy(IJavaElement* parent) override;

		CloneJavaProject(IJavaModel* p, const wstring& projectPath,
		                 const wstring& name);

		CloneJavaProject(OriginalJavaProject&);
		~CloneJavaProject();
		void SynchronousClone(boost::shared_ptr<OriginalJavaProject>&,
			map<ModifyLevel, set<PathModel::ResourceOptKind> >& );

		// 对应clone的工程中stack的更新在2处地方
		// 1.original 在改变自己的时候，通过消息来告诉clone 它的改动
		// 2.clone 向original 复制的时候
		// 因为original 改变的时候，clone是不允许获得original的数据的，所有 1和2 不能同时发生
		
		void CloneAcceptNotify(vector<PathModel::NameLookUpNotify*>&);

	private:

		void ProcessRemoveRoot(PathModel::NotifyResult::RootT&, set<PathModel::ResourceOptKind>&);

		void ProcessRemovePkg(PathModel::NotifyResult::PkgT&, set<PathModel::ResourceOptKind>& );

		void ProcessRemoveFiles(PathModel::NotifyResult::FileT&);


		//virtual ~CloneJavaProject();

		void SynchronousProcessRoot(OriginalJavaProject* , PathModel::NotifyResult::RootT&, set<PathModel::ResourceOptKind>& );

		void SynchronousProcessPkg(OriginalJavaProject* , PathModel::NotifyResult::PkgT&,set<PathModel::ResourceOptKind>& );

		void SynchronousProcessFiles(OriginalJavaProject* , PathModel::NotifyResult::FileT&);
	protected:
		bool generateInfos(IProgressMonitor* pm) override;
		void removeInfo() override;
		
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

