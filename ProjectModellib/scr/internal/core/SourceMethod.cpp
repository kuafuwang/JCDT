#include "stdafx.h"
#include <JavaModelLib/internal/core/SourceMethod.h>


namespace Jikes { // Open namespace Jikes block
namespace JavaModel{

	IJavaElement* SourceMethod::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new SourceMethod(*this);
		clone->fParent = parent;
		return clone;
	}

	SourceMethod::SourceMethod(IJavaElement* parent, 
		const wstring& name_symbol_, const vector<wstring>& parameterTypes)
		: BaseMethod(parent,name_symbol_, parameterTypes)
		
		
	{

	}

	SourceMethod::SourceMethod(SourceMethod&o):  BaseMethod(o)
	{

	}
	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




