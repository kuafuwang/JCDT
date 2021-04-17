#include "stdafx.h"

#include <JavaModelLib/internal/independent_member/IndependentBinaryMethod.h>
#include <JavaModelLib/internal/core/BaseJavaType.h>
#include <JavaModelLib/internal/independent_member/IndependentMoses.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	

	
	IJavaElement* IndependentBinaryMethod::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new IndependentBinaryMethod(*this);
		clone->fParent = parent;
		return clone;
	}

	IndependentBinaryMethod::IndependentBinaryMethod(IType* parent, 
		const wstring& name_symbol_,
		const vector<wstring>& parameterTypes)
		: BaseMethod(parent, name_symbol_, parameterTypes)

	{

	}

	

	IndependentBinaryMethod::IndependentBinaryMethod(IndependentBinaryMethod&o): BaseMethod(o)
	{
	}


	

	
	IndependentBinaryMethod::~IndependentBinaryMethod()
	{
	
	}



	MemberElementInfo* IndependentBinaryMethod::getElementInfo()
	{
		if (!info){
			info = new SourceMethodElementInfo();	
			IndependentMoses::copyMethodElementInfoWithMosesPointHelp(this, moses, info, true, formal_parameterTypes);
		}
		return info;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




