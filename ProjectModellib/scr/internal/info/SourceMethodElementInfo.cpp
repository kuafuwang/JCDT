#include "stdafx.h"
#include "JavaModelLib/internal/info/SourceMethodElementInfo.h"
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/core/BaseMethod.h>
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		void SourceMethodElementInfo::HelpToDeepCopy(SourceMethodElementInfo& o, 
			BaseMethod* parent)
		{

			for (auto it : o.children) {

				if (!it.second)
					continue;

				auto clone_child = reinterpret_cast<Member*>(it.second->ShallowCopy(parent));
				children.insert(make_pair(it.first, clone_child));
			}
			return_type = o.return_type;
			paramenterNames = o.paramenterNames;
			MemberElementInfo::HelpToDeepCopy(o);
		}

		SourceMethodElementInfo::~SourceMethodElementInfo()
		{
			for (auto it : children) {
				delete it.second;
			}
			children.clear();
		}

		IField* SourceMethodElementInfo::getLocalField(const wstring& name, const wstring& declareType)
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

