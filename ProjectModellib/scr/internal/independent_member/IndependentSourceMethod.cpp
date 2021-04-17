#include "stdafx.h"
#include <JavaModelLib/internal/independent_member/IndependentSourceMethod.h>
#include <JavaModelLib/internal/independent_member/IndependentMoses.h>
#include <JavaModelLib/internal/core/BaseJavaType.h>

namespace Jikes { // Open namespace Jikes block
namespace JavaModel{

	IJavaElement* IndependentSourceMethod::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new IndependentSourceMethod(*this);
		clone->fParent = parent;
		return clone;
	}

	IndependentSourceMethod::IndependentSourceMethod(IJavaElement* parent, 
		const wstring& name_symbol_ ,const vector<wstring>& parameterTypes)
		: BaseMethod(parent,name_symbol_, parameterTypes)
		
		
	{

	}

	IndependentSourceMethod::IndependentSourceMethod(IndependentSourceMethod&o):  BaseMethod(o)
	{

	}


	MemberElementInfo* IndependentSourceMethod::getElementInfo()
	{
		if (!info)
		{
			info = new SourceMethodElementInfo();
			IndependentMoses::copyMethodElementInfoWithMosesPointHelp(this,moses, info, false,formal_parameterTypes);
			return info;
		}
		return info;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




