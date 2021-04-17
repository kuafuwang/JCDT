#ifndef Jikes_CodeAssist_AssistInstanceofExpression_INCLUDED
#define Jikes_CodeAssist_AssistInstanceofExpression_INCLUDED

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

 namespace CodeAssist{

				  //
				  // Represents instanceof expressions.
				  //
	class AssistInstanceofExpression : public AssistExpression
	{
	public:

		
		AssistExpression* expression;
		
		//AstType* type; // AstArrayType, AstTypeName

		  AssistInstanceofExpression()
			: AssistExpression(INSTANCEOF)
		{}
		~AssistInstanceofExpression() {}


	};
 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistInstanceofExpression_INCLUDED
