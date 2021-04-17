#ifndef Jikes_CodeAssist_AssistFieldReference_INCLUDED
#define Jikes_CodeAssist_AssistFieldReference_INCLUDED
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block


	//
	// FieldAccess --> <DOT, Primary, ._token, Identifier>
	//
     namespace CodeAssist{
	class AssistFieldReference : public AssistExpression
	{
	public:
	
		AssistExpression* base; // Not AstName.
	
		  AssistFieldReference()
			: AssistExpression(DOT)
		{}
		~AssistFieldReference() {}


	};

     }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistFieldReference_INCLUDED
