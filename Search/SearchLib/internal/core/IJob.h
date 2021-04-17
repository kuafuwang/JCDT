#ifndef  IJob_JIKES_SERACH_INCLUDE
#define  IJob_JIKES_SERACH_INCLUDE
#include <string>

namespace Jikes{
	class IProgressMonitor;
}

namespace Jikes { // Open namespace Jikes block

	namespace Search {

		class  IJob {
		public:
			virtual ~IJob()
			{
			}

			/* Waiting policies */
			static const 	int ForceImmediate = 1;
			static const 	int CancelIfNotReady = 2;
			static const int WaitUntilReady = 3;

			/* Job's result */
			static const bool FAILED = false;
			static const bool COMPLETE = true;

			/**
			 * Answer true if the job belongs to a given family (tag)
			 */
			virtual bool belongsTo(const std::wstring& jobFamily) = 0;
			/**
			 * Asks this job to cancel its execution. The cancellation
			 * can take an undertermined amount of time.
			 */
			virtual void cancel() = 0;

			/**
			 * Execute the current job, answering:
			 *      RESCHEDULE if the job should be rescheduled later on
			 *      COMPLETE if the job is over
			 */
			virtual bool  execute(IProgressMonitor* progress) = 0;
		};
	}// Close namespace Search block


} // Close namespace Jikes block
#endif