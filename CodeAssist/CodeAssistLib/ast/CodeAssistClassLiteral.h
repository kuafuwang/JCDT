#ifndef Jikes_CodeAssist_AssistClassLiteralAccess_INCLUDED
#define Jikes_CodeAssist_AssistClassLiteralAccess_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block


	//
	// Represents class literals.
	//
     namespace CodeAssist{
	class AssistClassLiteralAccess : public AssistExpression
	{
	public:


		//AstType* type;
		//
		// If this expression requires a caching variable and a call to class$(),
		// the resolution holds the needed class$xxx or array$xxx cache.
		//
		  AssistClassLiteralAccess()
			: AssistExpression(CLASS_LITERAL)
	
		{}
		~AssistClassLiteralAccess() {}


	};


     }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistClassLiteralAccess_INCLUDED
