
#ifndef SourceFieldElementInfo_java_model_INCLUDED_INCLUDED
#define SourceFieldElementInfo_java_model_INCLUDED_INCLUDED
#include "MemberElementInfo.h"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IField;

	class SourceFieldElementInfo:public MemberElementInfo
	{
	public:
		LiteralValue*  initial_value;
		wstring _type;
		SourceFieldElementInfo();
		~SourceFieldElementInfo();
		void HelpToDeepCopy(SourceFieldElementInfo& o, IField* parent);
		ISourceRange*  local_fild_father_rng;
	};


}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

