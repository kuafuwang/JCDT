#ifndef AstModifierKeyword_def_INCLUDED
#define AstModifierKeyword_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstModifierKeyword.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



    void AstModifierKeyword::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstModifierKeyword:#" << id << "*/";
        os << lex_stream -> NameString(modifier_token->index);
        if (debug_unparse)
        os << "/*:AstModifierKeyword#" << id << "*/";
    }
    
	Ast* AstModifierKeyword::Clone(AstStoragePool* ast_pool)
	{
		return ast_pool->GenModifierKeyword(modifier_token);
	}


    void   AstModifierKeyword::traverse(ASTVisitor* visitor,AstNodeScope* scope)
    {
        if (visitor->visit(this,scope)) {

        }
        visitor->endVisit(this,scope);
    }
    void AstModifierKeyword::Print(Ostream& os,LexStream& lex_stream)
    {
        os << '#' << id << " (ModifierKeyword):  "
                << lex_stream.NameString(modifier_token->index) << endl;
    }


} // Close namespace Jikes block


#endif // AstModifierKeyword_def_INCLUDED
