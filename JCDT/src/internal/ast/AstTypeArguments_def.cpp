#ifndef AstTypeArguments_def_INCLUDED
#define AstTypeArguments_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstTypeArguments.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block

void AstTypeArguments::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstTypeArguments:#" << id << "*/";
    os << lex_stream -> NameString(left_angle_token->index);
    if (NumTypeArguments())
        TypeArgument(0) -> Unparse(os, lex_stream);
    for (unsigned i = 1; i < NumTypeArguments(); i++)
    {
        os << ", ";
        TypeArgument(i) -> Unparse(os, lex_stream);
    }
    os << lex_stream -> NameString(right_angle_token->index);
    if (debug_unparse)
        os << "/*:AstTypeArguments#" << id << "*/";
}
	Ast* AstTypeArguments::Clone(AstStoragePool* ast_pool)
	{
		AstTypeArguments* clone = ast_pool->GenTypeArguments(left_angle_token,
			right_angle_token);
		clone->AllocateTypeArguments(NumTypeArguments());
		for (unsigned i = 0; i < NumTypeArguments(); i++)
			clone->AddTypeArgument((AstType*)TypeArgument(i) ->
				Clone(ast_pool));
		return clone;
	}

	
void   AstTypeArguments::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		unsigned num = NumTypeArguments();
		for (i = 0; i < num; i++)
			(*type_arguments)[i]->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstType*& AstTypeArguments::TypeArgument(unsigned i)
	{ return (*type_arguments)[i]; }

	unsigned AstTypeArguments::NumTypeArguments()
	{
		assert(type_arguments);
		return type_arguments->Length();
	}

	wstring AstTypeArguments::getTypeArgumentNames()
{
	unsigned i;
	wstring _arg_name;
	_arg_name.append(L"<");
	auto  num_type_arguments =  NumTypeArguments();
	for (i = 0; i < num_type_arguments; ++i) {
		auto some_names = TypeArgument(i)->getParameterizedTypeName();
		for (size_t k = 0; k < some_names.size(); ++k)
		{
			_arg_name += some_names[i];
		}
		if(num_type_arguments < (i + 1))
			_arg_name.append(L",");

	}
	_arg_name.append(L">");
	return _arg_name;
}
void AstTypeArguments::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (TypeArguments):  <";
    for (i = 0; i < NumTypeArguments(); i++)
        os << " #" << TypeArgument(i) -> id;
    os << '>' << endl;
    for (i = 0; i < NumTypeArguments(); i++)
        TypeArgument(i) -> Print(os, lex_stream);
}

 void AstTypeArguments::AllocateTypeArguments(unsigned estimate)
{
	assert(!type_arguments && estimate);
	type_arguments = new (pool) AstArray<AstType*>(pool, estimate);
}

 void AstTypeArguments::AddTypeArgument(AstType* argument)
{
	assert(!AstPrimitiveType::PrimitiveTypeCast(argument));
	assert(type_arguments);
	type_arguments->Next() = argument;
}

} // Close namespace Jikes block


#endif // AstTypeArguments_def_INCLUDED
