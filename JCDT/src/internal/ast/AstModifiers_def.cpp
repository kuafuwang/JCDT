#include <JCDT_Lib/internal/ast/AstModifierKeyword.h>
#ifndef AstModifiers_def_INCLUDED
#define AstModifiers_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstModifiers.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

#include <JCDT_Lib/internal/ast/AstAnnotation.h>
namespace Jikes { // Open namespace Jikes block


    void AstModifiers::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstModifiers:#" << id << "*/";
        for (unsigned i = 0; i < NumModifiers(); i++)
        {
            Modifier(i) -> Unparse(os, lex_stream);
            if (AstModifierKeyword::ModifierKeywordCast(Modifier(i)))
                os << ' ';
        }
        if (debug_unparse)
        os << "/*:AstModifiers#" << id << "*/";
    }
    
	Ast* AstModifiers::Clone(AstStoragePool* ast_pool)
	{
		AstModifiers* clone = ast_pool->GenModifiers();
		clone->AllocateModifiers(NumModifiers());
		for (unsigned i = 0; i < NumModifiers(); i++)
		{
			if (AstModifierKeyword::ModifierKeywordCast(Modifier(i)))
				clone->AddModifier((AstModifierKeyword*)
					Modifier(i)->Clone(ast_pool));
			else clone->AddModifier((AstAnnotation*)
				Modifier(i)->Clone(ast_pool));
		}
		clone->static_token_opt = static_token_opt;
		return clone;
	}


void   AstModifiers::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		auto sz = NumModifiers();
		for (i = 0; i < sz; i++)
			Modifier(i)->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void AstModifiers::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (Modifiers): ";
    for (i = 0; i < NumModifiers(); i++)
        os << " #" << Modifier(i) -> id;
    os << endl;
    for (i = 0; i < NumModifiers(); i++)
        Modifier(i) -> Print(os, lex_stream);
}

	AstModifiers::AstModifiers(AstStoragePool* p): Ast(MODIFIERS)
	                                               , pool(p), modifiers(nullptr), static_token_opt(nullptr)
	{}

	void AstModifiers::AddModifier(AstAnnotation* annotation)
{
	assert(modifiers);
	modifiers->Next() = annotation;
}

 void AstModifiers::AddModifier(AstModifierKeyword* keyword)
{
	assert(modifiers);
	modifiers->Next() = keyword;
}
} // Close namespace Jikes block


#endif // AstModifiers_def_INCLUDED
