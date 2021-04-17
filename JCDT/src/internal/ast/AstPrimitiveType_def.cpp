#ifndef AstPrimitiveType_def_INCLUDED
#define AstPrimitiveType_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstPrimitiveType.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void AstPrimitiveType::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstPrimitiveType:#" << id << "*/";
    os << lex_stream -> NameString(primitive_kind_token->index);
    if (debug_unparse)
        os << "/*:AstPrimitiveType#" << id << "*/";
}

	Ast* AstPrimitiveType::Clone(AstStoragePool* ast_pool)
	{
		return ast_pool->GenPrimitiveType(kind, primitive_kind_token);
	}


void   AstPrimitiveType::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void AstPrimitiveType::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (PrimitiveType):  "
            << lex_stream.NameString(primitive_kind_token->index) << endl;
}

vector<wstring> AstPrimitiveType::getTypeName()
{
	wstring _type;
		switch(kind)
		{
		case BOOLEAN:
			_type = L"boolean";
			break;
		case Ast::BYTE:
			_type = L"byte";
			break;
		case Ast::SHORT:
			_type = L"short";
			break;

		case Ast::INT:
			_type = L"int";
			break;
		case Ast::LONG:
			_type = L"long";
			break;

		case Ast::CHAR:
			_type = L"char";
			break;

		case Ast::FLOAT:
			_type = L"float";
			break;

		case Ast::DOUBLE:
			_type = L"double";
			break;

		case Ast::VOID_TYPE:
			_type = L"void";
			break;

		default:
			break;
		}
		return  vector<wstring>(1, _type);
}

} // Close namespace Jikes block


#endif // AstPrimitiveType_def_INCLUDED
