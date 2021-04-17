#ifndef AstSwitchLabel_INCLUDED
#define AstSwitchLabel_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AstNode.h"
namespace Jikes { // Open namespace Jikes block



				  //
				  // Represents "case <constant> :" and "default :".
				  //
	class AstSwitchLabel : public Ast
	{
	public:
		static AstSwitchLabel* SwitchLabelCast(Ast* node)
		{
			return DYNAMIC_CAST<AstSwitchLabel*>(node->kind == SWITCH_LABEL ? node : NULL);
		}
		Token* case_token;
		Expression* expression_opt;
		Token* colon_token;

		//
		// The sorted index of this label in the overall switch. Default cases
		// are set to NumCases().
		//
		unsigned map_index;

		AstSwitchLabel();

		~AstSwitchLabel() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return case_token; }
		virtual Token* RightToken() { return colon_token; }
	};



} // Close namespace Jikes block


#endif // AstSwitchLabel_INCLUDED
