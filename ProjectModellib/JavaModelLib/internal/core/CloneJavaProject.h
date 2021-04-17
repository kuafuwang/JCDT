
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

		// ��Ӧclone�Ĺ�����stack�ĸ�����2���ط�
		// 1.original �ڸı��Լ���ʱ��ͨ����Ϣ������clone ���ĸĶ�
		// 2.clone ��original ���Ƶ�ʱ��
		// ��Ϊoriginal �ı��ʱ��clone�ǲ�������original�����ݵģ����� 1��2 ����ͬʱ����
		
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

