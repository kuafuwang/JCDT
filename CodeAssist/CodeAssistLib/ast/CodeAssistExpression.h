#ifndef Jikes_CodeAssist_AssistArrayReference_INCLUDED
#define Jikes_CodeAssist_AssistArrayReference_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block



	//
	// ArrayAccess --> <ARRAY_ACCESS, Base, [_token, AssistExpression, ]_token>
	//
     namespace CodeAssist{
	class AssistArrayReference : public AssistExpression
	{
	public:
		
		AssistExpression* base;
		
		AssistExpression* expression;
		

		  AssistArrayReference()
			: AssistExpression(ARRAY_ACCESS)
		{}
		~AssistArrayReference() {}


	};

     }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistArrayReference_INCLUDED
