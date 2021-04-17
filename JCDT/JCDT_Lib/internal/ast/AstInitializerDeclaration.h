#ifndef AstInitializerDeclaration_INCLUDED
#define AstInitializerDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclared.h"
namespace Jikes { // Open namespace Jikes block


	//
	// This class represents static and instance initializers. It also accepts
	// other modifiers, to give a nicer error message.
	//
	class Initializer : public AstDeclared
	{
	public:
		enum InitializerDeclarationTag
		{
			NONE,
			STATIC
		};

		AstMethodBody* block;
		static Initializer* InitializerDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<Initializer*>
				(node->kind == INITIALIZER ? node : NULL);
		}
		static  Initializer* StaticInitializerCast(Ast* node)
		{
			return DYNAMIC_CAST<Initializer*>
				(node->kind == INITIALIZER &&
					node->other_tag == Initializer::STATIC ? node : NULL);
		}

		Initializer();

		~Initializer() {}

		  void MarkStatic() { other_tag = STATIC; }

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;


		virtual Token* LeftToken();

		virtual Token* RightToken();
	};

	inline Initializer::Initializer(): AstDeclared(INITIALIZER), block(nullptr)
	{}
} // Close namespace Jikes block


#endif // AstInitializerDeclaration_INCLUDED
