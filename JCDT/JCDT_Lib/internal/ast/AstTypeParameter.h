#ifndef AstTypeParameter_INCLUDED
#define AstTypeParameter_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block

	// Represents a type parameter, used by AstTypeParameters.
	class AstTypeParameter : public Ast
	{
		AstStoragePool* pool;
		AstArray<AstTypeName*>* bounds;

	public:
		Token* identifier_token;

		TypeSymbol* symbol;
		static AstTypeParameter* TypeParameterCast(Ast* node)
		{
			return DYNAMIC_CAST<AstTypeParameter*>(node->kind == TYPE_PARAM ? node : NULL);
		}

		AstTypeParameter(AstStoragePool* p, Token* token);

		~AstTypeParameter() {}

		  AstTypeName*& Bound(unsigned i) { return (*bounds)[i]; }
		  unsigned NumBounds() { return bounds ? bounds->Length() : 0; }
		  void AllocateBounds(unsigned estimate = 1);
		  void AddBound(AstTypeName*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken() { return identifier_token; }

		virtual Token* RightToken();
	};




	inline    void AstTypeParameter::AddBound(AstTypeName* bound)
	{
		assert(bounds);
		bounds->Next() = bound;
	}


} // Close namespace Jikes block


#endif // AstTypeParameter_INCLUDED
