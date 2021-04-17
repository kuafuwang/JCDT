#ifndef Jikes_CodeAssist_AssistTrueLiteral_INCLUDED
#define Jikes_CodeAssist_AssistTrueLiteral_INCLUDED
#include "AssistExpression.h"


namespace Jikes
{ // Open namespace Jikes block
	namespace CodeAssist
	{
		// AssistTrueLiteral --> <TRUE_LITERAL, Literal, value>			  
		class AssistTrueLiteral : public AssistExpression
		{
		public:
			static const bool value = true;
			AssistTrueLiteral(): AssistExpression(TRUE_LITERAL)
			{
			}

			~AssistTrueLiteral()
			{
			}
		};
	}
} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistTrueLiteral_INCLUDED


