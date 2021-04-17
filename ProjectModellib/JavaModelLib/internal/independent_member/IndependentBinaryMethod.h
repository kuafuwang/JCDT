
#ifndef IndependentBinaryMethod_java_model_INCLUDED_INCLUDED
#define IndependentBinaryMethod_java_model_INCLUDED_INCLUDED

#include <JavaModelLib/internal/core/BaseMethod.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class BinaryType;

	class IndependentBinaryMethod :
		public BaseMethod
	{
	public:
		bool isBinary() override
		{
			return true;
		}
		IJavaElement* ShallowCopy(IJavaElement*) override;

		IndependentBinaryMethod(IType* parent, const wstring& name_symbol_, const vector<wstring>& parameterTypes);
		IndependentBinaryMethod(IndependentBinaryMethod&);
		virtual ~IndependentBinaryMethod();
	

		MemberElementInfo* getElementInfo() override;;
	};





	

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

