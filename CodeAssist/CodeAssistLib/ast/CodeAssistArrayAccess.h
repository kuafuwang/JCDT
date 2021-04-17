#ifndef Jikes_CodeAssist_AssistArrayReference_INCLUDED
#define Jikes_CodeAssist_AssistArrayReference_INCLUDED
#include "AssistExpression.h"
namespace Jikes { // Open namespace Jikes block

   namespace CodeAssist{

	//
	// ArrayAccess --> <ARRAY_ACCESS, Base, [_token, AssistExpression, ]_token>
	//
	class AssistArrayReference : public AssistExpression
	{
	public:
		
		AssistExpression* base ;
	
		AssistExpression* expression;
	
		  AssistArrayReference(AssistExpression* _base, AssistExpression* _expression)
			: AssistExpression(ARRAY_ACCESS),base(_base), expression(_expression)
		{}
		~AssistArrayReference() {}

	};

   }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistArrayReference_INCLUDED
