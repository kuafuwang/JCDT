
#ifndef IInitializer_java_model_INCLUDED_INCLUDED
#define IInitializer_java_model_INCLUDED_INCLUDED
#include "Member.h"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IField;
	class IInitializer
		: public Member
	{
	public:

		virtual IField* getLocalField(const wstring& name, const wstring& declareType) = 0;

		wstring getFullName() override {
			return L"{ .. }";
		}

		IInitializer(IJavaElement* parent, const wstring& name)
			: Member(parent, IJavaElementType::INITIALIZER, name)
		{
		}

		 IInitializer(IInitializer& o)
			: Member(o)
		{
		}
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

