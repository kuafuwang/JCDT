#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/ast/AstBrackets.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#ifndef AstArrayType_def_INCLUDED
#define AstArrayType_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstArrayType.h"
namespace Jikes { // Open namespace Jikes block

void AstArrayType::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstArrayType:#" << id << "*/";
    type -> Unparse(os, lex_stream);
    brackets -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstArrayType#" << id << "*/";
}

	Ast* AstArrayType::Clone(AstStoragePool* ast_pool)
	{
		return ast_pool->GenArrayType((AstType*)type->Clone(ast_pool),
			((AstBrackets*)brackets ->
				Clone(ast_pool)));
	}

	AstArrayType::AstArrayType(AstType* t, AstBrackets* b): AstType(ARRAY)
	                                                        , type(t)
	                                                        , brackets(b)
	{}
	 int AstArrayType::dimensions() {
		 return  brackets->dims;
	 }
	unsigned AstArrayType::NumBrackets()
	{ return brackets->dims; }

	void AstArrayType::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ArrayType):  "
            << '#' << type -> id << ' ' << brackets -> id << endl;
    type -> Print(os, lex_stream);
    brackets -> Print(os, lex_stream);
}
void  AstArrayType::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		type->traverse(visitor,scope);
		brackets->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}
 vector<wstring> AstArrayType::getTypeName()
{
	auto prefix_type_name  = type->getTypeName();
	return prefix_type_name;
}

	vector<wstring> AstArrayType::getParameterizedTypeName()
	{
		auto prefix_type_name = type->getParameterizedTypeName();
		int dimension = brackets->dims;
		if (dimension) {
			auto size = prefix_type_name.size() - 1;
			auto lastName = prefix_type_name[size];
			for (size_t i = 0; i < dimension; ++i)
			{
				lastName.append(L"[]");
			}
			prefix_type_name[size].swap(lastName);
		}

		return prefix_type_name;
	}

	Token* AstArrayType::RightToken()
	{ return brackets->right_bracket_token; }

	
} // Close namespace Jikes block


#endif // AstArrayType_def_INCLUDED
