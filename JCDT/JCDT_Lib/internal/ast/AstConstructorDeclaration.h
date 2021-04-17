#ifndef AstConstructorDeclaration_INCLUDED
#define AstConstructorDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AbstractMethodDeclaration.h"
#include <JCDT_Lib/internal/util/depend.h>
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block

	//
	// ConstructorDeclaration --> <CONSTRUCTOR, ConstructorModifiers,
	//     MethodDeclarator, Throws, ConstructorBody>
	//
	// ConstructorBody --> <METHOD_BODY, {_token,
	//     ExplicitConstructorInvocation, BlockStatements, }_token>
	//
	// ConstructorModifier --> Modifier (PUBLIC, PROTECTED or PRIVATE)
	//
	// ExplicitConstructorInvocation --> ThisCall
	//                                 | SuperCall
	//
	// NOTE: We do not actually build ConstructorBodies. Instead, we have
	// overloaded MethodBody to store the necessary information. This is
	// because this() and super() are treated as Statements in the grammar;
	// and in the bytecode, constructors are just methods with a special
	// name.
	//
	class ConstructorDeclaration : public AbstractMethodDeclaration
	{
		AstStoragePool* pool;
		AstArray<AstTypeName*>* throws;

	public:

		static ConstructorDeclaration* ConstructorDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<ConstructorDeclaration*>
				(node->kind == CONSTRUCTOR ? node : NULL);
		}

		unsigned int BodyStart() override;

		unsigned int BodyEnd() override;

		int index; // Used in depend.cpp to detect cycles.

	
		AstMethodBody* constructor_body;

		ConstructorDeclaration(AstStoragePool* p);

		~ConstructorDeclaration() {}

		bool IsValid() { return method_symbol != NULL; }

		  AstTypeName*& Throw(unsigned i) { return (*throws)[i]; }
		  unsigned NumThrows() { return throws ? throws->Length() : 0; }
		  void AllocateThrows(unsigned estimate = 1);
		  void AddThrow(AstTypeName*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*) ;

		  void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		
		 Token* LeftToken() override;

		 Token* RightToken() override;
	
		
	};


	
	inline void ConstructorDeclaration::AddThrow(AstTypeName* exception)
	{
		assert(throws);
		throws->Next() = exception;
	}
	

} // Close namespace Jikes block


#endif // AstConstructorDeclaration_INCLUDED
