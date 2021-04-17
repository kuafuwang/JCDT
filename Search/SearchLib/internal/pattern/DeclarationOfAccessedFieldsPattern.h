#ifndef  DeclarationOfAccessedFieldsPattern_JIKES_SERACH_INCLUDE
#define DeclarationOfAccessedFieldsPattern_JIKES_SERACH_INCLUDE
#include "FieldReferencePattern.h"
#include <boost/unordered/unordered_set.hpp>
namespace Jikes { // Open namespace Jikes block
	namespace JavaModel
	{
		class IJavaElement;
	}
namespace Search{
	class MatchSet;
	class DeclarationOfAccessedFieldsPattern:public FieldReferencePattern
	{
	public :
		JavaModel::IJavaElement* enclosingElement;
		boost::unordered_set<JavaModel::IJavaElement*> knownFields;
		DeclarationOfAccessedFieldsPattern(JavaModel::IJavaElement *_enclosingElement);

		/**
		* @see SearchPattern#matchReportReference
		*/
		void matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
		                          int accuracy, MatchSet* locator);

		void reportDeclaration(VariableSymbol* fieldBinding, MatchSet* locator);
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif