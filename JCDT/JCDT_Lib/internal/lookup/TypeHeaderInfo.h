
#ifndef TypeHeaderInfo_Jikes_INCLUDED
#define TypeHeaderInfo_Jikes_INCLUDED

#include <string>
#include <vector>
using std::wstring;
using std::vector;
namespace Jikes { // Open namespace Jikes block

	class TypeHeaderInfo
	{
	public:
		~TypeHeaderInfo()
		{
			delete superName;
			delete interfaceNames;
		}

		
		TypeHeaderInfo(const wstring& pkg, const wstring& typeName);
		std::pair<wstring, wstring>* superName;
		std::vector<std::pair<wstring, wstring> >* interfaceNames;
		wstring qualifiedPackage;
		wstring typeName;
	};

	inline TypeHeaderInfo::TypeHeaderInfo(const wstring& pkg,
		const wstring& _typeName):
		superName(nullptr), 
		interfaceNames(nullptr),
		qualifiedPackage(pkg),
		typeName(_typeName)
	{
	}
} // Close namespace Jikes block


#endif // _INCLUDED

