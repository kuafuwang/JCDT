#include "stdafx.h"
#include <JavaModelLib/internal/independent_member/IndependentSourceType.h>
#include <JavaModelLib/internal/independent_member/IndependentMoses.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	


	IJavaElement* IndependentSourceType::DeepCopy(IJavaElement* parent)
	{
		auto clone = reinterpret_cast<IndependentSourceType*>(ShallowCopy(parent));
		clone->HelpToDeepCopy(*this);
		return clone;
	}
	

	IJavaElement* IndependentSourceType::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new IndependentSourceType(*this);
		return clone;
	}

	IndependentSourceType::IndependentSourceType(IJavaElement* parent, const wstring& name)
	: SourceType(parent, name)
		
	{
		HelpToDeepCopy(*this);
	}

	IndependentSourceType::IndependentSourceType(IndependentSourceType& o): 
		SourceType(o)
	{

	}

	MemberElementInfo* IndependentSourceType::getElementInfo()
	{
		if (info)
			return info;

		info = new SourceTypeElementInfo();
		IndependentMoses::copyTypeElementInfoWithMosesPointHelp(this,moses, info, true);
		return info;
	}

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




