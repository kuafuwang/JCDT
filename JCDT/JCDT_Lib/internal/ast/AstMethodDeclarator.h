#ifndef AstMethodDeclarator_INCLUDED
#define AstMethodDeclarator_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AstNode.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block
	//
	// MethodDeclarator --> <METHOD_DECLARATOR, identifier_token, (_token,
	// FormalParameters, )_token, Brackets>
	//
	class AstMethodDeclarator : public Ast
	{
	
	public:
		AstStoragePool* pool;
		AstArray<AstFormalParameter*>* formal_parameters;

		Token* identifier_token;
		Token* left_parenthesis_token;
		Token* right_parenthesis_token;
		AstBrackets* brackets_opt;
		static AstMethodDeclarator* MethodDeclaratorCast(Ast* node)
		{
			return DYNAMIC_CAST<AstMethodDeclarator*>
				(node->kind == METHOD_DECLARATOR ? node : NULL);
		}

		AstMethodDeclarator(AstStoragePool* p);

		~AstMethodDeclarator() {}

		  AstFormalParameter*& FormalParameter(unsigned i);

		  unsigned NumFormalParameters();
		  void AllocateFormalParameters(unsigned estimate = 1);
		  void AddFormalParameter(AstFormalParameter*);

		  unsigned NumBrackets();

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

	

	inline void AstMethodDeclarator::AllocateFormalParameters(unsigned estimate)
	{
		assert(!formal_parameters);
		formal_parameters =
			new (pool) AstArray<AstFormalParameter*>(pool, estimate);
	}

	inline void AstMethodDeclarator::AddFormalParameter(AstFormalParameter* formal_parameter)
	{
		assert(formal_parameters);
		formal_parameters->Next() = formal_parameter;
	}
	inline AstFormalParameter*& AstMethodDeclarator::FormalParameter(unsigned i)
	{
		return (*formal_parameters)[i];
	}

	inline unsigned AstMethodDeclarator::NumFormalParameters()
	{
		return formal_parameters ? formal_parameters->Length() : 0;
	}

	
} // Close namespace Jikes block


#endif // AstMethodDeclarator_INCLUDED
