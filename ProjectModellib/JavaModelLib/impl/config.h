
#ifndef config_java_model_INCLUDED_INCLUDED
#define config_java_model_INCLUDED_INCLUDED
#include <boost/thread/win32/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>
typedef boost::shared_lock<boost::shared_mutex> readLock;
typedef boost::unique_lock<boost::shared_mutex> writeLock;

#endif // _INCLUDED

