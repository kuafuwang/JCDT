#include "stdafx.h"
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JavaModelLib/internal/independent_member/IndependentBinaryField.h>

#include <JavaModelLib/internal/core/BaseJavaType.h>
#include <JavaModelLib/internal/independent_member/IndependentMoses.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	IField* IndependentBinaryField::ShallowCopy(IJavaElement* parent)
	{
		 auto clone = new IndependentBinaryField(*this);
		 clone->fParent = parent;
		 return clone;
	}

	IndependentBinaryField::IndependentBinaryField(IType* parent, const wstring& name_symbol_, IJavaElementType _type)
		: IField(parent, name_symbol_, _type)
		
	{

	}

	IndependentBinaryField::IndependentBinaryField(IndependentBinaryField&o): IField(o)
	{
		
	}

	MemberElementInfo* IndependentBinaryField::getElementInfo()
	{
		if (!info)
		{
			info = new SourceFieldElementInfo();
			IndependentMoses::copyFieldElementInfoWithMosesPointHelp(this,moses, info, true);
			return info;
		}
		return info;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




