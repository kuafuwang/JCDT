
#ifndef Jikes_AstName_INCLUDED
#define Jikes_AstName_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
#include "InvocationSite.h"

namespace Jikes { // Open namespace Jikes block


//
// Simple and qualified names.
//



#define GetAstNameAllNames( _name, all_names ) \
	AstName* __name ## __cur_1 = (_name);  \
	 while(__name ## __cur_1){\
		(all_names).push_back(__name ## __cur_1->identifier_token->getName());\
	  __name ## __cur_1 =  __name ## __cur_1->base_opt;}std::reverse(all_names.begin(),all_names.end());\

	class  AstName : public Expression,public InvocationSite
	{
	public:
		AstName* base_opt;
		Token* identifier_token;

		//
		// When a name refers to a member in an enclosing scope, it is mapped
		// into an expression that creates a path to the member in question.
		//
		bool IsQualifiedName()
		{
			return base_opt != nullptr;
		}
		Expression* resolution_opt;
		static AstName* NameCast(Ast* node)
		{
			return DYNAMIC_CAST<AstName*>(node->kind == NAME ? node : NULL);
		}
		  AstName(Token* token)
			: Expression(NAME), base_opt(nullptr)
			  , identifier_token(token), resolution_opt(nullptr)
		{}
		~AstName() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		 vector<const wchar_t*> getTokenNames();
		 void getCompoundNames(wstring& _name);
		virtual Token* LeftToken()
		{
			return base_opt ? base_opt->LeftToken() : identifier_token;
		}
		virtual Token* RightToken() { return identifier_token; }

		bool isTypeReference() override;
		bool isSuperAccess() override {
			return false;
		};
		bool isTypeAccess() override;
	
		
	};




} // Close namespace Jikes block


#endif // AstName_INCLUDED
