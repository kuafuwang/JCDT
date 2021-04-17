#ifndef AstCharacterLiteral_def_INCLUDED
#define AstCharacterLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstCharacterLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void CharLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*CharLiteral:#" << id << "*/";
    bool old_expand = os.ExpandWchar();
    os.SetExpandWchar(true);
    os << lex_stream -> NameString(character_literal_token->index);
    os.SetExpandWchar(old_expand);
    if (debug_unparse)
        os << "/*:CharLiteral#" << id << "*/";
}

Ast* CharLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenCharacterLiteral(character_literal_token);
}

	CharLiteral::CharLiteral(Token* token): Expression(CHARACTER_LITERAL)
	                                        , character_literal_token(token)
	{}

	void CharLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (CharacterLiteral):  "
            << lex_stream.NameString(character_literal_token->index)
            << endl;
}
void  CharLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}

} // Close namespace Jikes block


#endif // AstCharacterLiteral_def_INCLUDED
