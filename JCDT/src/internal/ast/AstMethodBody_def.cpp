#ifndef AstMethodBody_def_INCLUDED
#define AstMethodBody_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstMethodBody.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void AstMethodBody::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstMethodBody:#" << id << "*/";
    os << '{' << endl;
    if (explicit_constructor_opt)
        explicit_constructor_opt -> Unparse(os, lex_stream);
    AstBlock::Unparse(os, lex_stream);
    os << '}' << endl;
    if (debug_unparse)
        os << "/*:AstMethodBody#" << id << "*/";
}
	Ast* AstMethodBody::Clone(AstStoragePool* ast_pool)
	{
		AstMethodBody* clone = ast_pool->GenMethodBody();
		clone->HasLocalTypeMASK = HasLocalTypeMASK;
		
		clone->CloneBlock(ast_pool, this);
		if (explicit_constructor_opt)
			clone->explicit_constructor_opt =
			(ReachComleteAbleStatement*)explicit_constructor_opt->Clone(ast_pool);
		return clone;
	}
	

void   AstMethodBody::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

		if (explicit_constructor_opt)
			explicit_constructor_opt->traverse(visitor, scope);

		AstBlock::traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void AstMethodBody::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (MethodBody):  ";
    if (explicit_constructor_opt)
        os << " #" << explicit_constructor_opt -> id << endl;
    else os << " #0" << endl;
    AstBlock::Print(os, lex_stream);

    if (explicit_constructor_opt)
        explicit_constructor_opt -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstMethodBody_def_INCLUDED
