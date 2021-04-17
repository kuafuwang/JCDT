
#ifndef InitializerElementInfo_java_model_INCLUDED_INCLUDED
#define InitializerElementInfo_java_model_INCLUDED_INCLUDED
#include "MemberElementInfo.h"
#include <JavaModelLib/internal/core/BaseMethod.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IInitializer;

	class InitializerElementInfo:public MemberElementInfo
	{
	public:
		std::map<wstring, Member* > children;
		void HelpToDeepCopy(InitializerElementInfo& o, IInitializer* parent);
		IField* getLocalField(const wstring& name, const wstring& declareType) ;

	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

