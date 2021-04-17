#ifndef AstModifiers_INCLUDED
#define AstModifiers_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block

				  //
				  // Represents one or more modifier keywords, as well as annotations (added in
				  // JSR 175).
				  //
	class AstModifiers : public Ast
	{
		AstStoragePool* pool;
		AstArray<Ast*>* modifiers; // AstAnnotation, AstModifierKeyword

	public:
		// Allows sorting between static and non-static declarations.
		Token* static_token_opt;
		static AstModifiers* ModifiersCast(Ast* node)
		{
			return DYNAMIC_CAST<AstModifiers*>(node->kind == MODIFIERS ? node : NULL);
		}

		AstModifiers(AstStoragePool* p);

		~AstModifiers() {}

		  Ast*& Modifier(unsigned i)
		{
			return (*modifiers)[i];
		}
		  unsigned NumModifiers()
		{
			assert(modifiers);
			return modifiers->Length();
		}
		  void AllocateModifiers(unsigned estimate = 1);
		  void AddModifier(AstAnnotation*);
		  void AddModifier(AstModifierKeyword*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return Modifier(0)->LeftToken(); }
		virtual Token* RightToken()
		{
			return Modifier(NumModifiers() - 1)->RightToken();
		}
		bool HasModifier(int imodifierFlag)
		{
			return true;
		}
	};



	inline    void AstModifiers::AllocateModifiers(unsigned estimate)
	{
		assert(!modifiers && estimate);
		modifiers = new (pool) AstArray<Ast*>(pool, estimate);
	}




} // Close namespace Jikes block


#endif // AstModifiers_INCLUDED
