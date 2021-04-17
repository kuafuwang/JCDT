#include "stdafx.h"
#include <JavaModelLib/internal/core/JavaElement.h>
#include <PathModelLib/MosesPath/MosesPath.h>
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	

	JavaElement::JavaElement(IJavaElement* parent, 
		 IJavaElementType _type, const wstring& name)
		: IJavaElement(_type), fName(name),fParent(parent), moses(nullptr)
	{

	}

	JavaElement::JavaElement(JavaElement& o)
		: IJavaElement(o),fOccurrenceCount(o.fOccurrenceCount),
		fName(o.fName), fParent(o.fParent), moses(nullptr)
	{
		if(o.moses){
			moses = o.moses->Clone();
		}
	}

	bool JavaElement::equals(IJavaElement* o)
	{
		if (this == o) return true;
		if (!o) return false;
	
		if (!fParent)
			return false;

		if (fLEType != o->fLEType)
			return false;
		JavaElement* other = reinterpret_cast<JavaElement*>(o);
		return fName ==(other->fName) &&
			fParent->equals(other->fParent) &&
			fOccurrenceCount == other->fOccurrenceCount;

		return false;
	}

	JavaElement::~JavaElement(){
	
			delete moses;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




