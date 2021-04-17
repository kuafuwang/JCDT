#include "stdafx.h"
#include "SimilarElement.h"
namespace Jikes { // Open namespace Jikes block


	namespace Correction {
		SimilarElement::SimilarElement(int kind, std::wstring& _name, 
			int relevance, int _modifiers): fKind(kind),
                                          fName(_name),
                                          fRelevance(
                                              relevance),
                                          modifiers(
                                              _modifiers)
		{
		}

		bool SimilarElement::eaqual(SimilarElement* o)
		{
			if (!o)
				return false;

			if (o == this)
				return true;

			if (o->fKind != fKind)
				return false;

			if (o->modifiers != modifiers)
				return false;

			if (o->fRelevance != fRelevance)
				return false;

			if (o->fName != fName)
				return false;

			return true;
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

