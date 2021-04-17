
#ifndef BuilderMgr_java_model_INCLUDED_INCLUDED
#define BuilderMgr_java_model_INCLUDED_INCLUDED
#include <boost/smart_ptr/shared_ptr.hpp>
#include <vector>
namespace Jikes
{
	class IProgressMonitor;
}

class SolutionModel;
#include <map>
using std::map;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class OriginalJavaProject;
	class JavaModel;
	class JDTCompiler;
	
	class BuilderMgr
	{
	public:

		explicit BuilderMgr(IProgressMonitor* p, JavaModel* javamodel, SolutionModel* ps);
		virtual ~BuilderMgr();
		virtual void HookBuildProc();
		void BuildProc();
		
		void GetBuildProject(map<std::wstring, boost::shared_ptr<OriginalJavaProject>>& projects,
			std::vector<OriginalJavaProject*>& sorted_project,
			std::vector<OriginalJavaProject*>& unorder_projects);
	
		void BuildOnOneCore(map<std::wstring, boost::shared_ptr<OriginalJavaProject>>& projects);
		void BuildMutipleCore(map<std::wstring, boost::shared_ptr<OriginalJavaProject>>& projects);
		
		IProgressMonitor* pm;
		JavaModel* pJM;
		SolutionModel*  pSolution;
		unsigned  _build_stamp;
		JDTCompiler* m_Compiler;
	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

