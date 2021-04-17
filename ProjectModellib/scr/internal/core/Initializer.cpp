#include "stdafx.h"
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/Initializer.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	IJavaElement* Initializer::DeepCopy(IJavaElement* parent)
	{
		auto clone = reinterpret_cast<Initializer*>(ShallowCopy(parent));
		if(info && clone){
			clone->info = new InitializerElementInfo();
			clone->info->HelpToDeepCopy(*info, this);
		
		}
		return clone;
	}

	IJavaElement* Initializer::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new Initializer(*this);

		if (!clone)
			return nullptr;

		clone->fParent = parent;
		parent->addChild(clone);
		return clone;
	}

	Initializer::Initializer(IType* parent, int occurrenceCount) : 
		IInitializer(parent, L""), start_pos(occurrenceCount), info(nullptr)
	{

	}

	Initializer::Initializer(Initializer& o):
		IInitializer(o), start_pos(o.start_pos), info(nullptr)
	{
			
	}

	void Initializer::addChild(IJavaElement* child)
	{
		assert(child);
		if (!child)
			return;
		switch (child->fLEType)
		{
		case TYPE:
		case FIELD:
		case LOCAL_FIELD:
		{
			auto full_name = child->getFullName();
			
			auto _info = reinterpret_cast<InitializerElementInfo*>(getElementInfo());
			_info->children.insert(std::make_pair(full_name, reinterpret_cast<Member*>(child)));
		}
		break;
		default:
			break;
		}
	}

	IField* Initializer::getLocalField(const wstring& name, const wstring& declareType){
		getElementInfo();
		if (!info)
			return nullptr;

		return info->getLocalField(name, declareType);
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




