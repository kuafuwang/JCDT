#ifndef  CallHierarcky_JIKES_SERACH_INCLUDE
#define CallHierarcky_JIKES_SERACH_INCLUDE
namespace Jikes { // Open namespace Jikes block
	namespace  CodeAssist
	{
		class AssistInfo;

	}
	namespace JavaModel
	{
		class  JavaModel;
	}
namespace Search{
	class CallHierarckyCollector;
	class CallHierarcky
	{
	public:

		CallHierarcky(JavaModel::JavaModel* _pJM, CodeAssist::AssistInfo* _cur_info);
		~CallHierarcky();

	   void OpenCallHierarcky(CallHierarckyCollector* collector_) const;

		CodeAssist::AssistInfo* cur_info;

		JavaModel::JavaModel* pJM;
		;
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif