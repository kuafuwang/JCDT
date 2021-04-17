#include "stdafx.h"
#include <JavaModelLib/internal/independent_member/IndependentBinaryType.h>
#include <JavaModelLib/internal/independent_member/IndependentMoses.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	


	IJavaElement* IndependentBinaryType::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new IndependentBinaryType(*this);
		return clone;
	}



	IndependentBinaryType::IndependentBinaryType(IJavaElement* parent, const wstring& name)
		: BinaryType(parent, name)
	{

	}

	IndependentBinaryType::IndependentBinaryType(IndependentBinaryType&o)
		: BinaryType(o)
	{
		HelpToDeepCopy(o);
	}

	IndependentBinaryType::~IndependentBinaryType()
	{
	}


	MemberElementInfo* IndependentBinaryType::getElementInfo()
	{
		if (!info)
		{
			info = new SourceTypeElementInfo();
			IndependentMoses::copyTypeElementInfoWithMosesPointHelp(this,moses, info, true);
		}
		return info;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




