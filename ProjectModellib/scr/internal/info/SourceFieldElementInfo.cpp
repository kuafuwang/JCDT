#include "stdafx.h"
#include <JavaModelLib/internal/info/SourceFieldElementInfo.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {

		SourceFieldElementInfo::SourceFieldElementInfo() : initial_value(nullptr), local_fild_father_rng(nullptr)
		{
		}

		SourceFieldElementInfo::~SourceFieldElementInfo()
		{
			if (initial_value)
				delete initial_value;
			if (local_fild_father_rng)
				delete local_fild_father_rng;
		}

		void SourceFieldElementInfo::HelpToDeepCopy(SourceFieldElementInfo& o, IField* parent)
		{
			if (o.initial_value){
				delete initial_value;
				initial_value = o.initial_value->Clone();
			}
			_type = o._type;
			MemberElementInfo::HelpToDeepCopy(o);
			if(o.local_fild_father_rng){
				delete local_fild_father_rng;
				local_fild_father_rng = o.local_fild_father_rng;
			}
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

