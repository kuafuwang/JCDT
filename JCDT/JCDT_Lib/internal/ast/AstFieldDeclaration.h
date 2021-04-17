#ifndef AstFieldDeclaration_INCLUDED
#define AstFieldDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclared.h"
#include "AbstractVariableDeclaration.h"
#include "AstArray.h"
#include "AstVariableDeclarator.h"
namespace Jikes { // Open namespace Jikes block


	//
	// FieldDeclaration --> <FIELD, VariableModifiers, Type, VariableDeclarators,
	// ;_token>
	//
	// FieldModifier --> Modifier (PUBLIC, PROTECTED, PRIVATE, FINAL, STATIC,
	// TRANSIENT or VOLATILE)
	//
	class FieldDeclaration : public AstDeclared,public AbstractVariableDeclaration
	{
		AstStoragePool* pool;
		AstArray<AstVariableDeclarator*>* variable_declarators;

	public:
		static FieldDeclaration* FieldDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<FieldDeclaration*>(node->kind == FIELD ? node : NULL);
		}
		static FieldDeclaration* StaticFieldCast(Ast* node)
		{
			return DYNAMIC_CAST<FieldDeclaration*>
				(node->kind == FIELD &&
					node->other_tag == FieldDeclaration::STATIC ? node : NULL);
		}

		enum FieldDeclarationTag
		{
			NONE,
			STATIC
		};

		AstType* type;
		Token* semicolon_token;

		FieldDeclaration(AstStoragePool* p);

		~FieldDeclaration() {}

		  void MarkStatic() { other_tag = STATIC; }

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

		virtual Token* RightToken() { return semicolon_token; }
	};

	inline void FieldDeclaration::AllocateVariableDeclarators(unsigned estimate)
	{
		assert(!variable_declarators);
		variable_declarators =
			new (pool) AstArray<AstVariableDeclarator*>(pool, estimate);
	}

	inline void FieldDeclaration::AddVariableDeclarator(AstVariableDeclarator* variable_declarator)
	{
		assert(variable_declarators);
		variable_declarator->owner = this;
		variable_declarators->Next() = variable_declarator;
	}


} // Close namespace Jikes block


#endif // AstFieldDeclaration_INCLUDED
