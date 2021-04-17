
#ifndef BinaryMethod_java_model_INCLUDED_INCLUDED
#define BinaryMethod_java_model_INCLUDED_INCLUDED

#include <JCDT_Lib/internal/util/tuple.h>
#include "BaseMethod.h"


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class BinaryType;

	class BinaryMethod :
		public BaseMethod
	{
	public:
		bool isBinary() override
		{
			return true;
		}
		IJavaElement* ShallowCopy(IJavaElement*) override;

		BinaryMethod(IType* parent, const wstring& name_symbol_, const vector<wstring>& parameterTypes);

		void SetParamenterNamesAndNameRange(vector<wstring>& names, ISourceRange* rng);
		BinaryMethod(BinaryMethod&);
		virtual ~BinaryMethod();

		
		
		


		
	};





	

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

