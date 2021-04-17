#ifndef Jikes_CodeAssist_AssistAstClassCreationExpression_INCLUDED
#define Jikes_CodeAssist_AssistAstClassCreationExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block


	//
	// ClassCreationExpression represents a class instance creation (keyword new,
	// including anonymous classes). Also see ArrayCreationExpression. Sometimes,
	// during semantic analysis an artificial base_opt expression is constructed.
	// In such a case, the user can determine this condition by testing
	// base_opt -> generated.
	//
     namespace CodeAssist{
	class AssistAstClassCreationExpression : public AssistExpression
	{
	public:
		
		AssistExpression* base_opt;
		
		
		 AssistAstClassCreationExpression()
			: AssistExpression(CLASS_CREATION)
		{}
		~AssistAstClassCreationExpression() {}


	};

     }


} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstClassCreationExpression_INCLUDED
