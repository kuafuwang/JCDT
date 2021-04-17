#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/ast/AstType.h>
#include <JCDT_Lib/internal/ast/AstArray.h>
#include <JCDT_Lib/internal/ast/AstBrackets.h>
#include <JCDT_Lib/internal/ast/AstDimExpr.h>
#include <JCDT_Lib/internal/ast/AstArrayInitializer.h>
#include <JCDT_Lib/internal/lex/Token.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#ifndef AstArrayCreationExpression_def_INCLUDED
#define AstArrayCreationExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstArrayCreationExpression.h"
namespace Jikes { // Open namespace Jikes block



void AstArrayCreationExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstArrayCreationExpression:#" << id << "*/";
    os << lex_stream -> NameString(new_token->index) << ' ';
    array_type -> Unparse(os, lex_stream);
    for (unsigned i = 0; i < NumDimExprs(); i++)
        DimExpr(i) -> Unparse(os, lex_stream);
    if (brackets_opt)
        brackets_opt -> Unparse(os, lex_stream);
    if (array_initializer_opt)
        array_initializer_opt -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstArrayCreationExpression#" << id << "*/";
}

Ast* AstArrayCreationExpression::Clone(AstStoragePool* ast_pool)
{
    AstArrayCreationExpression* clone =
        ast_pool -> GenArrayCreationExpression();
    clone -> new_token = new_token;
    clone -> array_type = (AstType*) array_type -> Clone(ast_pool);
    clone -> AllocateDimExprs(NumDimExprs());
    for (unsigned i = 0; i < NumDimExprs(); i++)
        clone -> AddDimExpr((AstDimExpr*) DimExpr(i) -> Clone(ast_pool));
    if (brackets_opt)
        clone -> brackets_opt = (AstBrackets*) brackets_opt -> Clone(ast_pool);
    if (array_initializer_opt)
        clone -> array_initializer_opt =
            (AstArrayInitializer*) array_initializer_opt -> Clone(ast_pool);
    return clone;
}

	AstArrayCreationExpression::AstArrayCreationExpression(AstStoragePool* p)
		: Expression(ARRAY_CREATION) , pool(p), dim_exprs(nullptr), new_token(nullptr), array_type(nullptr), brackets_opt(nullptr), array_initializer_opt(nullptr)
	{}

	AstDimExpr*& AstArrayCreationExpression::DimExpr(unsigned i)
	{ return (*dim_exprs)[i]; }

	unsigned AstArrayCreationExpression::NumDimExprs()
	{
		return dim_exprs ? dim_exprs->Length() : 0;
	}

	unsigned AstArrayCreationExpression::NumBrackets()
	{
		return brackets_opt ? brackets_opt->dims : 0;
	}

	void AstArrayCreationExpression::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (ArrayCreationExpression):  "
            << lex_stream.NameString(new_token->index)
            << " #" << array_type -> id << "dimexpr:( ";
    for (i = 0; i < NumDimExprs(); i++)
        os << " #" << DimExpr(i) -> id;
    os << ") brackets:#" << (brackets_opt ? brackets_opt -> id : 0)
            << " initializer:#"
            << (array_initializer_opt ? array_initializer_opt -> id : 0)
            << endl;
    array_type -> Print(os, lex_stream);
    for (i = 0; i < NumDimExprs(); i++)
        DimExpr(i) -> Print(os, lex_stream);
    if (brackets_opt)
        brackets_opt -> Print(os, lex_stream);
    if (array_initializer_opt)
        array_initializer_opt -> Print(os, lex_stream);
}
void  AstArrayCreationExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	unsigned i;
	if (visitor->visit(this,scope)) {
		array_type->traverse(visitor,scope);
		unsigned num = NumDimExprs();
		for (i = 0; i < num; i++)
			(*dim_exprs)[i]->traverse(visitor,scope);
		if (brackets_opt)
			brackets_opt->traverse(visitor,scope);
		if (array_initializer_opt)
			array_initializer_opt->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}

	Token* AstArrayCreationExpression::RightToken()
	{
		return array_initializer_opt
			       ? array_initializer_opt->right_brace_token
			       : brackets_opt ? brackets_opt->right_bracket_token
			       : DimExpr(NumDimExprs() - 1)->right_bracket_token;
	}

	

} // Close namespace Jikes block


#endif // AstArrayCreationExpression_def_INCLUDED
