#ifndef AstPrimitiveType_INCLUDED
#define AstPrimitiveType_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstType.h"

namespace Jikes { // Open namespace Jikes block		  
	// Type --> PrimitiveType
	//        | ReferenceType
	//
	// PrimitiveType --> <PrimitiveKind, PrimitiveName>
	//
	// PrimitiveKind --> BYTE | SHORT | INT | LONG | CHAR | FLOAT | DOUBLE |
	//                   BOOLEAN | VOID
	//
	// PrimitiveName --> byte_token | short_token | int_token | long_token |
	//                   char_token | float_token | double_token | boolean_token |
	//                   void_token
				  
	class AstPrimitiveType : public AstType
	{
	public:
		Token* primitive_kind_token;

		static AstPrimitiveType* PrimitiveTypeCast(Ast *node)
		{
			return DYNAMIC_CAST<AstPrimitiveType*>
				(node->class_tag == PRIMITIVE_TYPE ? node : NULL);
		}

		  AstPrimitiveType(AstKind k, Token* token)
			: AstType(k, PRIMITIVE_TYPE)
			, primitive_kind_token(token)
		{}
		~AstPrimitiveType() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 virtual vector<wstring> getTypeName() override;
		virtual Token* LeftToken() { return primitive_kind_token; }
		virtual Token* RightToken() { return primitive_kind_token; }
		virtual Token* IdentifierToken() { return primitive_kind_token; }
	};



} // Close namespace Jikes block


#endif // AstPrimitiveType_INCLUDED
