#ifndef AstArrayInitializer_INCLUDED
#define AstArrayInitializer_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstMemberValue.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block
	// Covers all array initializer expressions, including those added by JSR 175.
	class AstArrayInitializer : public AstMemberValue
	{
		AstStoragePool* pool;
		AstArray<AstMemberValue*>* variable_initializers;

	public:
		Token* left_brace_token;
		Token* right_brace_token;
		static AstArrayInitializer* ArrayInitializerCast(Ast* node)
		{
			return DYNAMIC_CAST<AstArrayInitializer*>
				(node->kind == ARRAY_INITIALIZER ? node : NULL);
		}

		AstArrayInitializer(AstStoragePool* p);

		~AstArrayInitializer() {}

		  AstMemberValue*& VariableInitializer(unsigned i);

		  unsigned NumVariableInitializers();
		  void AllocateVariableInitializers(unsigned estimate = 1);
		  void AddVariableInitializer(AstMemberValue*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return left_brace_token; }
		virtual Token* RightToken() { return right_brace_token; }
	};


	inline void AstArrayInitializer::AllocateVariableInitializers(unsigned estimate)
	{
		assert(!variable_initializers);
		variable_initializers =
			new (pool) AstArray<AstMemberValue*>(pool, estimate);
	}

	inline void AstArrayInitializer::AddVariableInitializer(AstMemberValue* initializer)
	{
		assert(variable_initializers);
		variable_initializers->Next() = initializer;
	}

	inline	AstMemberValue*& AstArrayInitializer::VariableInitializer(unsigned i)
	{
		return (*variable_initializers)[i];
	}

	inline	unsigned AstArrayInitializer::NumVariableInitializers()
	{
		return variable_initializers ? variable_initializers->Length() : 0;
	}


} // Close namespace Jikes block


#endif // AstArrayInitializer_INCLUDED
