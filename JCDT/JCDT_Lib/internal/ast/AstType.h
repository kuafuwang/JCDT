#ifndef AstType_INCLUDED
#define AstType_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include <vector>
#include "JCDT_Lib/internal/ast/AstNode.h"

namespace Jikes { // Open namespace Jikes block
	// This is the superclass of constructs which represent a type:
	// AstPrimitiveType, AstArrayType, AstWildcard, and AstTypeName. 
	
	class AstType : public Ast
	{
	public:
		TypeSymbol* symbol;
		virtual int dimensions() {
			return 0;
		}
		bool isArrayTypeReference()
		{
			return dimensions() > 0;
		}
		virtual bool IsQualifiedTypeReference() 
		{
			return false;
		}
		  AstType(AstKind k, AstTag t = NO_TAG)
			: Ast(k, t), symbol(nullptr)
		{}
		~AstType() {}

		virtual Token* IdentifierToken() = 0;
		virtual vector<wstring> getTypeName() = 0;
		virtual vector<wstring> getParameterizedTypeName() {
			return getTypeName();
		}
	};




} // Close namespace Jikes block


#endif // AstType_INCLUDED
