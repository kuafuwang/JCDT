#ifndef AstArrayType_INCLUDED
#define AstArrayType_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstType.h"
namespace Jikes { // Open namespace Jikes block

// ReferenceType --> ClassType
//                 | ArrayType
//
// ClassType --> Name
//
// ArrayType --> <ARRAY, ArrayKind, [_token, ]_token>
//
// ArrayKind --> PrimitiveType
//             | Name
//             | ArrayType
//
	class AstArrayType : public AstType
	{
	public:
		AstType* type; // AstPrimitiveType, AstTypeName

		AstBrackets* brackets;
		static AstArrayType* ArrayTypeCast(Ast *node)
		{
			return DYNAMIC_CAST<AstArrayType*>(node->kind == ARRAY ? node : NULL);
		}
		 bool IsQualifiedTypeReference() override
		{
			return type->IsQualifiedTypeReference();
		}
		AstArrayType(AstType* t, AstBrackets* b);
		int dimensions() override;

		~AstArrayType() {}

		  unsigned NumBrackets();

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		 virtual vector<wstring> getTypeName() override;
		 virtual vector<wstring> getParameterizedTypeName();
		virtual Token* LeftToken() { return type->LeftToken(); }

		virtual Token* RightToken();

		virtual Token* IdentifierToken()
		{
			if (type)
				return type->IdentifierToken();
			else
				return nullptr;
		}

		AstType* leftComponetType()
		{
			return type;
		};
	};



} // Close namespace Jikes block


#endif // AstArrayType_INCLUDED
