#ifndef AstLocalClassStatement_INCLUDED
#define AstLocalClassStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // Represents a local class declaration statement.
				  //
	class AstLocalClassStatement : public ReachComleteAbleStatement
	{
	public:
		static AstLocalClassStatement* LocalClassStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<AstLocalClassStatement*>
				(node->kind == LOCAL_CLASS ? node : NULL);
		}
		TypeDeclaration* declaration; // ClassTypeDeclaration, EnumTypeDeclaration

		  AstLocalClassStatement(ClassTypeDeclaration* decl);

		  AstLocalClassStatement(EnumTypeDeclaration* decl);

		~AstLocalClassStatement() {}

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



} // Close namespace Jikes block


#endif // AstLocalClassStatement_INCLUDED
