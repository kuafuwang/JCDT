
#ifndef Signature_Jikes_INCLUDED
#define Signature_Jikes_INCLUDED
#include <string>
#include <vector>

using std::wstring;

namespace Jikes { // Open namespace Jikes block

	class IllegalArgumentException :public std::exception
	{

	};
	class Signature {
	public:
	  
	   static std::wstring checkClassName(std::wstring classname);
	  
	   static  std::pair<std::vector<wstring>,wstring> parseSignature(const std::string& signature);
	  
	   static  std::wstring parseType(const std::string& signature, int & i,  int num_dimensions = 0);
		static std::wstring parseType(const char* signature, int& i, int num_dimensions = 0);
	};

} // Close namespace Jikes block


#endif // _INCLUDED

