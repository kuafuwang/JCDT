#ifndef AstMethodBody_INCLUDED
#define AstMethodBody_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstBlock.h"
namespace Jikes { // Open namespace Jikes block


	//
	// This class represents a method body, for methods, constructors, and
	// initializers. It is basically a block, with the addition of an explicit
	// constructor invocation (used only in the context of constructors, NULL
	// otherwise).
	//
	class AstMethodBody : public AstBlock
	{
	public:
		bool HasLocalTypeMASK;
		ReachComleteAbleStatement* explicit_constructor_opt;
		static AstMethodBody* MethodBodyCast(Ast* node)
		{
			return DYNAMIC_CAST<AstMethodBody*>(node->kind == METHOD_BODY ? node : NULL);
		}
		  AstMethodBody(AstStoragePool* p)
			: AstBlock(p, METHOD_BODY, true), HasLocalTypeMASK(false)
			  , explicit_constructor_opt(nullptr)
		{
			no_braces = true;
			SetTag(AstBlock::METHOD_BODY_BLOCK);
		}
		~AstMethodBody() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;
		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		// Inherited LeftToken(), RightToken() are adequate.
		
	};




} // Close namespace Jikes block


#endif // AstMethodBody_INCLUDED
