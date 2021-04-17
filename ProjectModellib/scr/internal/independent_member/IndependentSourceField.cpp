#include "stdafx.h"


#include <JavaModelLib/internal/core/BaseJavaType.h>
#include <JavaModelLib/internal/independent_member/IndependentSourceField.h>
#include <JavaModelLib/internal/independent_member/IndependentMoses.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	IField* IndependentSourceField::ShallowCopy(IJavaElement* parent){
		auto  clone =   new IndependentSourceField(*this);
		clone->fParent = parent;
		return clone;
	}

	
	IndependentSourceField::IndependentSourceField(IJavaElement* parent,
		const wstring& name, IJavaElementType _type)
		: IField(parent, name, _type)
	{
	}

	IndependentSourceField::IndependentSourceField(IndependentSourceField& o): IField(o)
	{
	}


	MemberElementInfo* IndependentSourceField::getElementInfo()
	{
		if (!info)
		{
			info = new SourceFieldElementInfo();
			IndependentMoses::copyFieldElementInfoWithMosesPointHelp(this,moses, info, false);
			return info;
		}
		return info;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




