#ifndef AstArrayCreationExpression_INCLUDED
#define AstArrayCreationExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/astExpression.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block

	// ArrayCreationExpression --> <ARRAY_CREATION, new_token, Type, DimExprs,
	// Brackets>
	class AstArrayCreationExpression : public Expression
	{
		AstStoragePool* pool;
		AstArray<AstDimExpr*>* dim_exprs;

	public:
		static AstArrayCreationExpression* ArrayCreationExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<AstArrayCreationExpression*>
				(node->kind == ARRAY_CREATION ? node : NULL);
		}
		Token* new_token;
		AstType* array_type;
		AstBrackets* brackets_opt;
		AstArrayInitializer* array_initializer_opt;

		AstArrayCreationExpression(AstStoragePool* p);

		~AstArrayCreationExpression() {}

		  AstDimExpr*& DimExpr(unsigned i);

		  unsigned NumDimExprs();
		  void AllocateDimExprs(unsigned estimate = 1);
		  void AddDimExpr(AstDimExpr*);

		  unsigned NumBrackets();

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return new_token; }

		virtual Token* RightToken();
	};
	inline void AstArrayCreationExpression::AllocateDimExprs(unsigned estimate)
	{
		assert(!dim_exprs);
		dim_exprs = new (pool) AstArray<AstDimExpr*>(pool, estimate);
	}

	inline void AstArrayCreationExpression::AddDimExpr(AstDimExpr* dim_expr)
	{
		assert(dim_exprs);
		dim_exprs->Next() = dim_expr;
	}

	


} // Close namespace Jikes block


#endif // AstArrayCreationExpression_INCLUDED
