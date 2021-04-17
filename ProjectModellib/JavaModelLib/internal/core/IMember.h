
#ifndef IMember_java_model_INCLUDED_INCLUDED
#define IMember_java_model_INCLUDED_INCLUDED

#include <vector>
#include <string>
#include "ISourceReference.h"
#include <JavaModelLib/internal/info/MemberElementInfo.h>
#include <boost/algorithm/string.hpp>
#include <JavaModelLib/internal/core/IJavaCardDocType.h>
using std::wstring;

namespace Jikes { // Open namespace Jikes block
	class AccessFlags;
namespace JavaModel{

	class ISourceRange;
	class IType;
	class IMember :public ISourceReference
	{
	public:
		IMember();
		IMember(IMember& o);
		virtual ~IMember();
		virtual bool isBinary();
		virtual IType* getDeclaringType() = 0;
		virtual void  setAccessFlags(unsigned flag) = 0;
		virtual const std::vector< std::wstring>* const getModfiers() = 0;
		virtual wstring  getModifersString(wchar_t seperator = ' ') = 0;
		virtual AccessFlags getAccessFlags() = 0;
		
		virtual wstring getOutLineName() = 0;
		virtual wstring getOutLineIdentity() = 0;
		virtual MemberElementInfo* getElementInfo() = 0;

		

		
		inline wstring getFullyQualifiedName(wchar_t enclosingTypeSeparator){
			auto info = getElementInfo();

			auto type_part = boost::join(info->qualifiedTypeNames, wstring(1,enclosingTypeSeparator));
			if(!type_part.empty()){
				wstring temp(L".");
				temp += info->name;
				return type_part + temp;
			}
			return type_part;
		}
		
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

