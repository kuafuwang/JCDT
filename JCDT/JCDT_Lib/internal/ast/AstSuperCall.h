#ifndef AstSuperCall_INCLUDED
#define AstSuperCall_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
#include "JCDT_Lib/internal/ast/ExplicitConstructorCall.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // Represents an explicit call to a superconstructor.
				  //
	class AstSuperCall : public ReachComleteAbleStatement,public ExplicitConstructorCall
	{
	public:
		MethodSymbol* symbol;

		Expression* base_opt;
		AstTypeArguments* type_arguments_opt;
		Token* super_token;
		AstArguments* arguments;
		Token* semicolon_token;
		static AstSuperCall* SuperCallCast(Ast* node)
		{
			return DYNAMIC_CAST<AstSuperCall*>(node->kind == SUPER_CALL ? node : NULL);
		}

		AstSuperCall();

		~AstSuperCall() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken();

		virtual Token* RightToken() { return semicolon_token; }

		bool isSuperAccess() override;
		
	};



} // Close namespace Jikes block


#endif // AstSuperCall_INCLUDED
