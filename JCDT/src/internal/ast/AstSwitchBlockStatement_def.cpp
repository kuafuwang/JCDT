#include <JCDT_Lib/internal/ast/AstSwitchLabel.h>
#ifndef AstSwitchBlockStatement_def_INCLUDED
#define AstSwitchBlockStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstSwitchBlockStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void AstSwitchBlockStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstSwitchBlockStatement:#" << id << "*/";
    for (unsigned i = 0; i < NumSwitchLabels(); i++)
        SwitchLabel(i) -> Unparse(os, lex_stream);
    AstBlock::Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstSwitchBlockStatement#" << id << "*/";
}


Ast* AstSwitchBlockStatement::Clone(AstStoragePool* ast_pool)
{
    AstSwitchBlockStatement* clone = ast_pool -> GenSwitchBlockStatement();
    clone -> CloneBlock(ast_pool, this);
    clone -> AllocateSwitchLabels(NumSwitchLabels());
    for (unsigned i = 0; i < NumSwitchLabels(); i++)
        clone -> AddSwitchLabel((AstSwitchLabel*) SwitchLabel(i) ->
                                Clone(ast_pool));
    return clone;
}




void   AstSwitchBlockStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		for (i = 0; i < NumSwitchLabels(); i++)
			SwitchLabel(i)->traverse(visitor, scope);
		AstBlock::traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	Token* AstSwitchBlockStatement::LeftToken()
	{ return SwitchLabel(0)->case_token; }

	unsigned AstSwitchBlockStatement::NumSwitchLabels()
	{
		return switch_labels ? switch_labels->Length() : 0;
	}

	void AstSwitchBlockStatement::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (SwitchBlockStatement): ";
    for (i = 0; i < NumSwitchLabels(); i++)
    {
        if (i % 10 == 0)
            os << endl << "        ";
        os << " #" << SwitchLabel(i) -> id << ':';
    }
    os << endl;
    for (i = 0; i < NumSwitchLabels(); i++)
        SwitchLabel(i) -> Print(os, lex_stream);
    AstBlock::Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstSwitchBlockStatement_def_INCLUDED
