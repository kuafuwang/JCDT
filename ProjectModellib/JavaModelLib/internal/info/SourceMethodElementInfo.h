
#ifndef SourceMethodElementInfo_java_model_INCLUDED_INCLUDED
#define SourceMethodElementInfo_java_model_INCLUDED_INCLUDED
#include "MemberElementInfo.h"
#include <map>
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class Member;
	class IField;
	class BaseMethod;

	class SourceMethodElementInfo :public MemberElementInfo {

	public:
		wstring return_type;
		vector<wstring>  paramenterNames;
		vector<wstring> m_throws;
		std::map<wstring, Member* > children;
		IField* getLocalField(const wstring& name, const wstring& declareType);
		void HelpToDeepCopy(SourceMethodElementInfo& o,
			BaseMethod* parent);
		~SourceMethodElementInfo();
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

