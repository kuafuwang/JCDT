#include "stdafx.h"
#include "JavaModelLib/internal/info/InitializerElementInfo.h"
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/core/BaseMethod.h>
#include <JavaModelLib/internal/core/IInitializer.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		void InitializerElementInfo::HelpToDeepCopy(InitializerElementInfo& o,
			IInitializer* parent)
		{
			MemberElementInfo::HelpToDeepCopy(o);
			for (auto it : o.children) {
				if (!it.second)
					continue;

				auto clone_child = reinterpret_cast<Member*>(it.second->ShallowCopy(parent));
				children.insert(make_pair(it.first, clone_child));
			}
		}

		IField* InitializerElementInfo::getLocalField(const wstring& name, const wstring& declareType)
		{

			for (auto it : children)
			{
				auto element = it.second;

				if (element && element->fLEType == LOCAL_FIELD)
				{
					if (element->getElementName() == name)
					{
						auto field = reinterpret_cast<IField*>(element);
						auto field_info = reinterpret_cast<SourceFieldElementInfo*>( field->getElementInfo());
						if (field_info && field_info->_type == declareType)
						{
							return field;
						}
					}
				}

			}
			return nullptr;
		}


	} // Close namespace JavaModel block

} // Close namespace Jikes block

