#ifndef AstLocalVariableStatement_INCLUDED
#define AstLocalVariableStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
#include "JCDT_Lib/internal/ast/AbstractVariableDeclaration.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // Represents a local variable declaration statement.
				  //
	class LocalDeclaration : public ReachComleteAbleStatement,public AbstractVariableDeclaration
	{
		AstStoragePool* pool;
		AstArray<AstVariableDeclarator*>* variable_declarators;

	public:
		static LocalDeclaration* LocalVariableStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<LocalDeclaration*>
				(node->kind == LOCAL_VARIABLE_DECLARATION ? node : NULL);
		}
		AstModifiers* modifiers_opt;
		AstType* type;
		Token* semicolon_token_opt;

		LocalDeclaration(AstStoragePool* p);

		~LocalDeclaration() {}

		  AstVariableDeclarator*& VariableDeclarator(unsigned i);

		  unsigned NumVariableDeclarators();
		  void AllocateVariableDeclarators(unsigned estimate = 1);
		  void AddVariableDeclarator(AstVariableDeclarator*);

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


	inline LocalDeclaration::LocalDeclaration(AstStoragePool* p): ReachComleteAbleStatement(LOCAL_VARIABLE_DECLARATION)
	                                                              , pool(p), variable_declarators(nullptr), modifiers_opt(nullptr), type(nullptr), semicolon_token_opt(nullptr)
	{}

	inline void LocalDeclaration::AllocateVariableDeclarators(unsigned estimate)
	{
		assert(!variable_declarators);
		variable_declarators =
			new (pool) AstArray<AstVariableDeclarator*>(pool, estimate);
	}

	inline void LocalDeclaration::AddVariableDeclarator(AstVariableDeclarator* variable_declarator)
	{
		assert(variable_declarators);
		variable_declarator->owner = this;
		variable_declarators->Next() = variable_declarator;
	}


} // Close namespace Jikes block


#endif // AstLocalVariableStatement_INCLUDED
