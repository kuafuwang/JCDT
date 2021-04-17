#include "stdafx.h"
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JavaModelLib/internal/core/BinaryField.h>
#include <JCDT_Lib/internal/codegen/Signature.h>
#include <JavaModelLib/internal/core/IType.h>
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	IField* BinaryField::ShallowCopy(IJavaElement* parent)
	{
		 auto clone = new BinaryField(*this);
		 clone->fParent = parent;
		 return clone;
	}

	BinaryField::BinaryField(IType* parent, const wstring& name_symbol_, IJavaElementType _type)
		: IField(parent, name_symbol_, _type)
		
	{

	}

	BinaryField::BinaryField(BinaryField&o): IField(o)
	{
		
	}


}// Close namespace JavaModel block
	

} // Close namespace Jikes block




