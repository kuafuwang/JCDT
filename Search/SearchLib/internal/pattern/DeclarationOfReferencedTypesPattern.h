#ifndef  DeclarationOfReferencedTypesPattern_JIKES_SERACH_INCLUDE
#define DeclarationOfReferencedTypesPattern_JIKES_SERACH_INCLUDE
#include "TypeReferencePattern.h"
#include <boost/unordered/unordered_set.hpp>
namespace Jikes { // Open namespace Jikes block
	namespace JavaModel
	{
		class IJavaElement;
		class IType;
	}
namespace Search{
	class DeclarationOfReferencedTypesPattern:public TypeReferencePattern
	{
	public :
		JavaModel::IJavaElement* enclosingElement;
		boost::unordered_set<JavaModel::IType*> knownTypes;
		 DeclarationOfReferencedTypesPattern(JavaModel::IJavaElement* _enclosingElement);
	
		/**
		* @see SearchPattern#matchReportReference
		*/
		void matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
		                          int accuracy, MatchSet* locator);
		void reportDeclaration(TypeSymbol* typeBinding, int maxType, MatchSet* locator);
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif