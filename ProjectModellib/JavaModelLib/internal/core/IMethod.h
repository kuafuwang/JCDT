
#ifndef IMethod_java_model_INCLUDED_INCLUDED
#define IMethod_java_model_INCLUDED_INCLUDED

#include "Member.h"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IType;
	class IField;

	class IMethod
		: public Member
	{

	public:
		virtual IField* getLocalField(const wstring& name, const wstring& declareType) = 0;

		IMethod(IJavaElement* parent, const wstring& name, const vector<wstring>& parameterTypes)
			: Member(parent, IJavaElementType::METHOD, name)
		{
		}

		 IMethod(IMethod& o)
			: Member(o)
		{
		}
	
		 wstring getOutLineIdentity()  override {
			 wchar_t temp[8];
			 int idx = wsprintfW(temp, L"%d", fOccurrenceCount);
			 temp[idx] = 0x00;
			 return getMethodNameWithParameterNames() + temp;
		 }
		 wstring getOutLineName() override {
			 return getMethodNameWithParameterNames() ;
		 }
		 virtual void MarkConstructor(bool) = 0;
		 virtual bool isConstructor() = 0;
		 virtual  const wstring getMethodNameWithParameterNames() = 0;
		 virtual  const std::vector<wstring>& getParameterNames() = 0;
		virtual  const std::vector<wstring>& getParameterTypes() = 0;
		virtual  unsigned getNumParameterTypes() = 0;

		virtual const wstring& getReturnType() = 0;
		

	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

