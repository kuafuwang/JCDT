
#ifndef  IJavaSearchScope_JIKES_SERACH_INCLUDE
#define IJavaSearchScope_JIKES_SERACH_INCLUDE


#include <vector>
#include <map>

namespace Jikes { // Open namespace Jikes block
	namespace JavaModel
	{
		class IJavaElement;
	}
	
}
using std::wstring;
namespace Jikes { // Open namespace Jikes block
	

	namespace Search {

	
		class GroupInfo;
	
		

		class  IJavaSearchScope {
		public:
			virtual ~IJavaSearchScope(){};

			virtual bool IsLimitTo(int limit) = 0;
			virtual void setLimitTo(std::map<int, bool>&) = 0;
			virtual void setLimitTo(int, bool) = 0;
			virtual std::map<int, bool>& getLimits() = 0;
			virtual bool encloses(const wstring& resourcePath) = 0;
			virtual const std::vector< GroupInfo* >& getPaths() = 0;


		};
}// Close namespace Search block


} // Close namespace Jikes block

#endif