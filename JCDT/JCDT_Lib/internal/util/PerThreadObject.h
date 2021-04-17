
#ifndef PerThreadObject_Jikes_INCLUDED
#define PerThreadObject_Jikes_INCLUDED
#include <map>
#include <mutex>
#include <windows.h>

namespace Jikes { // Open namespace Jikes block

	/**
	* Implementation of data structure remembering an Object value by thread. Its purpose is to ease
	* writing multi-threaded algorithms by providing a per thread data structure.
	*/
	 class PerThreadObject {

	 private:
		 std::map<int,void*> internalMap ; // synchronized map
		 std::mutex ready_me_mutex_;
		/**
		* Answer the current map for this thread
:		*/
	 public:
		 void* getCurrent() {
			 std::lock_guard<std::mutex> lg(ready_me_mutex_);
			return internalMap[GetCurrentThreadId()];
			
		}

		/**
		* Set the map for this current thread - setting to null is equivalent to removing it
		*/
		 void setCurrent(void* current) {
			 std::lock_guard<std::mutex> lg(ready_me_mutex_);
			if (current == nullptr) {
				internalMap.erase(GetCurrentThreadId());
			}
			else {
				internalMap[GetCurrentThreadId()] = current;
			}
		}
	 };
	

} // Close namespace Jikes block


#endif // _INCLUDED

