#ifndef Jikes_CodeAssist_AssistAstArrayCreationExpression_INCLUDED
#define Jikes_CodeAssist_AssistAstArrayCreationExpression_INCLUDED
#include "AssistExpression.h"

namespace Jikes { // Open namespace Jikes block

	// ArrayCreationExpression --> <ARRAY_CREATION, new_token, Type, DimExprs,
	// Brackets>
       namespace CodeAssist{
	class AssistAstArrayCreationExpression : public AssistExpression
	{
		
		//AstArray<AstDimExpr*>* dim_exprs;

	public:
		
		
		//AstType* array_type;
		//AstBrackets* brackets_opt;
		//AstArrayInitializer* array_initializer_opt;

		  AssistAstArrayCreationExpression()
			: AssistExpression(ARRAY_CREATION)
			
		{}
		~AssistAstArrayCreationExpression() {}

		//  AstDimExpr*& DimExpr(unsigned i);

		  unsigned NumDimExprs();
		 
		//  void AddDimExpr(AstDimExpr*);

		  unsigned NumBrackets();


	};
	
       }


} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstArrayCreationExpression_INCLUDED
