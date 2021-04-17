#ifndef AstArguments_INCLUDED
#define AstArguments_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
#include "JCDT_Lib/internal/ast/AstArray.h"


namespace Jikes { // Open namespace Jikes block

	//
	// Represents the arguments of AstThisCall, AstSuperCall, AstMethodInvocation,
	// AstClassCreationExpression, and AstEnumConstant. For convenience, the need
	// to add null argument or pass shadow parameters is contained here, even
	// though not all the calling instances can use these features.
	//
	class AstArguments : public Ast
	{
		AstStoragePool* pool;
		AstArray<Expression*>* arguments;
		AstArray<AstName*>* shadow_arguments;

	public:
		Token* left_parenthesis_token;
		Token* right_parenthesis_token;
		static AstArguments* ArgumentsCast(Ast* node)
		{
			return DYNAMIC_CAST<AstArguments*>(node->kind == ARGUMENTS ? node : NULL);
		}

		AstArguments(AstStoragePool* p, Token* l, Token* r);

		~AstArguments() {}

		  Expression*& Argument(unsigned i) const;

		  unsigned NumArguments();
		  void AllocateArguments(unsigned estimate = 1);
		  void AddArgument(Expression*);

		  AstName*& LocalArgument(unsigned i);

		  unsigned NumLocalArguments();
		  void AllocateLocalArguments(unsigned estimate = 1);
		  void AddLocalArgument(AstName*);

		  void AddNullArgument() { other_tag = true; }
		  bool NeedsExtraNullArgument()
		  {
			  if (other_tag)
				  return true;
			  else
				  return false;
			  
		  }

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken() { return left_parenthesis_token; }
		virtual Token* RightToken() { return right_parenthesis_token; }
	};


	inline Expression*& AstArguments::Argument(unsigned i) const
	{
		return (*arguments)[i];
	}

	inline unsigned AstArguments::NumArguments()
	{
		return arguments ? arguments->Length() : 0;
	}

	inline AstName*& AstArguments::LocalArgument(unsigned i)
	{
		return (*shadow_arguments)[i];
	}

	inline	unsigned AstArguments::NumLocalArguments()
	{
		return shadow_arguments ? shadow_arguments->Length() : 0;
	}

	inline void AstArguments::AllocateArguments(unsigned estimate)
	{
		assert(!arguments);
		arguments = new (pool) AstArray<Expression*>(pool, estimate);
	}

	inline void AstArguments::AddArgument(Expression* argument)
	{
		assert(arguments);
		arguments->Next() = argument;
	}



	inline void AstArguments::AddLocalArgument(AstName* argument)
	{
		assert(shadow_arguments);
		shadow_arguments->Next() = argument;
	}

} // Close namespace Jikes block


#endif // AstArguments_INCLUDED
