#ifndef AstTypeParameters_INCLUDED
#define AstTypeParameters_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block

	// Represents type parameter declarations, used by ClassTypeDeclaration,
	// InterfaceTypeDeclaration, MethodDeclaration, ConstructorDeclaration.
	//
	class AstTypeParameters : public Ast
	{
		AstStoragePool* pool;
		AstArray<AstTypeParameter*>* parameters;

	public:
		Token* left_angle_token;
		Token* right_angle_token;
		static AstTypeParameters* TypeParametersCast(Ast* node)
		{
			return DYNAMIC_CAST<AstTypeParameters*>(node->kind == PARAM_LIST ? node : NULL);
		}

		AstTypeParameters(AstStoragePool* p);

		~AstTypeParameters() {}

		  AstTypeParameter*& TypeParameter(unsigned i)
		{
			return (*parameters)[i];
		}
		  unsigned NumTypeParameters()
		{
			return parameters ? parameters->Length() : 0;
		}
		  void AllocateTypeParameters(unsigned estimate = 1);
		  void AddTypeParameter(AstTypeParameter*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return left_angle_token; }
		virtual Token* RightToken() { return right_angle_token; }
	};



	inline    void AstTypeParameters::AllocateTypeParameters(unsigned estimate)
	{
		assert(!parameters);
		parameters = new (pool) AstArray<AstTypeParameter*>(pool, estimate);
	}

	inline    void AstTypeParameters::AddTypeParameter(AstTypeParameter* type)
	{
		assert(parameters);
		parameters->Next() = type;
	}


} // Close namespace Jikes block


#endif // AstTypeParameters_INCLUDED
