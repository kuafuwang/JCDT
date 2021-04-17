#ifndef  DeclarationOfReferencedMethodsPattern_JIKES_SERACH_INCLUDE
#define DeclarationOfReferencedMethodsPattern_JIKES_SERACH_INCLUDE
#include "MethodReferencePattern.h"
#include <boost/unordered/unordered_set.hpp>

namespace Jikes { // Open namespace Jikes block
	namespace JavaModel
	{
		class IJavaElement;
		class IMethod;
	}
namespace Search{
	class DeclarationOfReferencedMethodsPattern: public MethodReferencePattern
	{
	public :
		boost::unordered_set<JavaModel::IMethod*>knownMethods;
		JavaModel::IJavaElement* enclosingElement;
		DeclarationOfReferencedMethodsPattern(JavaModel::IJavaElement* _enclosingElement)
			:MethodReferencePattern({}, PATTERN_MATCH, false, {}, {}, {}, {}, {}, {}, {}) ,enclosingElement(_enclosingElement)
		{
		
		}


		/**
		* @see SearchPattern#matchReportReference
		*/
		void matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
		                          int accuracy, MatchSet* locator);
		void reportDeclaration(MethodSymbol* methodBinding, MatchSet* locator);
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif