#include <JCDT_Lib/internal/ast/AstExpression.h>
#ifndef AstSwitchLabel_def_INCLUDED
#define AstSwitchLabel_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstSwitchLabel.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block




void AstSwitchLabel::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstSwitchLabel:#" << id << "*/";
    os << lex_stream -> NameString(case_token->index) << ' ';
    if (expression_opt)
        expression_opt -> Unparse(os, lex_stream);
    os << ':' << endl;
    if (debug_unparse)
        os << "/*:AstSwitchLabel#" << id << "*/";
}


Ast* AstSwitchLabel::Clone(AstStoragePool* ast_pool)
{
    AstSwitchLabel* clone = ast_pool -> GenSwitchLabel();
    clone -> case_token = case_token;
    if (expression_opt)
        clone -> expression_opt =
            (Expression*) expression_opt -> Clone(ast_pool);
    clone -> colon_token = colon_token;
    clone -> map_index = map_index;
    return clone;
}


void   AstSwitchLabel::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		if (expression_opt)
			expression_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstSwitchLabel::AstSwitchLabel(): Ast(SWITCH_LABEL), case_token(nullptr), 
		expression_opt(nullptr), colon_token(nullptr), map_index(0)
	{}

	void AstSwitchLabel::Print(Ostream& os,LexStream& lex_stream)
{
	os << '#' << id << " (SwitchLabel, map_index " << map_index << "):  "
		<< lex_stream.NameString(case_token->index) << '#'
		<< (expression_opt ? expression_opt->id : 0) << ':' << endl;
	if (expression_opt)
		expression_opt->Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstSwitchLabel_def_INCLUDED
