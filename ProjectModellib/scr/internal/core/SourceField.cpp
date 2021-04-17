#include "stdafx.h"
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JavaModelLib/internal/core/SourceField.h>
#include <JavaModelLib/internal/core/IType.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	IField* SourceField::ShallowCopy(IJavaElement* parent){
		auto  clone =   new SourceField(*this);
		clone->fParent = parent;
		return clone;
	}

	
	SourceField::SourceField(IJavaElement* parent,
		const wstring& name, IJavaElementType _type)
		: IField(parent, name, _type)
	{
	}

	SourceField::SourceField(SourceField& o): IField(o)
	{
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




