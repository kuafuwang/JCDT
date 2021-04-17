
#ifndef BinaryField_java_model_INCLUDED_INCLUDED
#define BinaryField_java_model_INCLUDED_INCLUDED

#include "IField.h"

namespace Jikes { // Open namespace Jikes block
	class LiteralValue;

namespace JavaModel{
	class IType;

	class BinaryField
		:  public IField
	{
	public:
		 bool isBinary() override
		{
			return true;
		}
		IField* ShallowCopy(IJavaElement*) override;
		
		 BinaryField(IType* parent, const wstring& name_symbol_, IJavaElementType _type);
		 BinaryField(BinaryField&);
		virtual ~BinaryField() { }

	
		
	};

	

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

