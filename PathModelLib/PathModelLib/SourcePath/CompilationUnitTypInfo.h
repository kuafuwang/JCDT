
#ifndef CompilationUnitTypInfo_jikes_PathModel_INCLUDED_INCLUDED
#define CompilationUnitTypInfo_jikes_PathModel_INCLUDED_INCLUDED
#include <string>
#include <vector>
#include <JCDT_Lib/internal/lookup/TypeHeaderInfo.h>
using std::wstring;
using std::vector;
namespace Jikes { // Open namespace Jikes block

namespace PathModel{

	class CompilationUnitTypInfo
	{
	public:
		
	
		vector<TypeHeaderInfo*> typesInfo;

		~CompilationUnitTypInfo()
		{
			for(auto it : typesInfo)
			{
				delete  it;
			}
		}
	};

}// Close namespace PathModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

