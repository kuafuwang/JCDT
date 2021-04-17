
#ifndef CodeAssistMgr_CodeAssist_INCLUDED
#define CodeAssistMgr_CodeAssist_INCLUDED
#include <string>

namespace Jikes {
	class IProgressMonitor;
}

using std::wstring;
namespace Jikes {
	namespace JavaModel {
		class JavaModel;
	}
}
namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class AssistInfo;

	class CodeAssistMgr
	{
		
	public:
		static void DispatchToSelect(
			AssistInfo* cur_info,
			JavaModel::JavaModel* pJM,
			IProgressMonitor* pm);

		static void DispatchToCorrectionError(
			AssistInfo* cur_info,
			JavaModel::JavaModel* pJM,
			IProgressMonitor* pm);

		void DispatchToCompelet(AssistInfo* cur_info, JavaModel::JavaModel* pJM, IProgressMonitor* pm);
		explicit CodeAssistMgr(IProgressMonitor* p, JavaModel::JavaModel* javamodel);
		virtual ~CodeAssistMgr();
		virtual void  HookDispathProc();
		void DispathProc();

		void UpdateInfo(AssistInfo* _value);
		IProgressMonitor* pm;


		void DispatchEvent(AssistInfo*);
	private:

		AssistInfo* _info;
		
		JavaModel::JavaModel* pJM;
	};

	
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

