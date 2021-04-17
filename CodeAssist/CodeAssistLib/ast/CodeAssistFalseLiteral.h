#ifndef Jikes_CodeAssist_AssistFalseLiteral_INCLUDED
#define Jikes_CodeAssist_AssistFalseLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes
{ // Open namespace Jikes block


	//
	// AssistFalseLiteral --> <FALSE_LITERAL, Literal, value>
	//
	namespace CodeAssist
	{
		class AssistFalseLiteral : public AssistExpression
		{
		public:


			
			AssistFalseLiteral()
				: AssistExpression(FALSE_LITERAL)
		
			{
			}
			static const bool value = false;
			~AssistFalseLiteral()
			{
			}
		};
	}
} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistFalseLiteral_INCLUDED


