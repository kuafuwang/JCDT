#ifndef AstThisCall_INCLUDED
#define AstThisCall_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
#include "JCDT_Lib/internal/ast/ExplicitConstructorCall.h"
namespace Jikes { // Open namespace Jikes block
	// Represents an explicit call to another constructor in this class.
	class AstThisCall : public ReachComleteAbleStatement,public ExplicitConstructorCall
	{
	public:
		MethodSymbol* symbol;

		AstTypeArguments* type_arguments_opt;
		Token* this_token;
		AstArguments* arguments;
		Token* semicolon_token;
		static AstThisCall* ThisCallCast(Ast* node)
		{
			return DYNAMIC_CAST<AstThisCall*>(node->kind == THIS_CALL ? node : NULL);
		}

		AstThisCall();

		~AstThisCall() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken();

		virtual Token* RightToken() { return semicolon_token; }

		bool isSuperAccess() override;
	
	};




} // Close namespace Jikes block


#endif // AstThisCall_INCLUDED
