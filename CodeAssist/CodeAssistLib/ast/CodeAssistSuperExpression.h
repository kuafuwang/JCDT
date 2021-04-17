#ifndef Jikes_CodeAssist_AssistAstSuperExpression_INCLUDED
#define Jikes_CodeAssist_AssistAstSuperExpression_INCLUDED
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

 namespace CodeAssist{

				  //
				  // Represents qualified and simple 'super'.
				  //
	 class AssistAstName;
	class AssistAstSuperExpression : public AssistExpression
	{
	public:
		AssistAstName* base_opt;
		AssistAstSuperExpression()
		: AssistExpression(SUPER_EXPRESSION), base_opt(nullptr)
		{
			
		}
		~AssistAstSuperExpression() {}


	};
 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstSuperExpression_INCLUDED
