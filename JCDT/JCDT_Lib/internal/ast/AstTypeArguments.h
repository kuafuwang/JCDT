#ifndef AstTypeArguments_INCLUDED
#define AstTypeArguments_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AstNode.h"
#include "AstPrimitiveType.h"

namespace Jikes { // Open namespace Jikes block

	// Represents the type arguments associated with a TypeName, as well as the
	// explicit type arguments of ThisCall, SuperCall,MethodInvocation, and
	// ClassCreationExpression.  The grammar always allows wildcards, so the
	// semantic engine must reject them when they are illegal.
	
	class AstTypeArguments : public Ast
	{
		AstStoragePool* pool;
		// AstTypeName, AstArrayType, AstWildcard
		AstArray<AstType*>* type_arguments;

	public:
		Token* left_angle_token;
		Token* right_angle_token;

		static AstTypeArguments* TypeArgumentsCast(Ast* node)
		{
			return DYNAMIC_CAST<AstTypeArguments*>
				(node->kind == TYPE_ARGUMENTS ? node : NULL);
		}

		  AstTypeArguments(AstStoragePool* p, Token* l, Token* r)
			: Ast(TYPE_ARGUMENTS)
			, pool(p), type_arguments(nullptr)
			  , left_angle_token(l)
			, right_angle_token(r)
		{}
		~AstTypeArguments() {}

		  AstType*& TypeArgument(unsigned i);

		  unsigned NumTypeArguments();
		  void AllocateTypeArguments(unsigned estimate = 1);
		  void AddTypeArgument(AstType*);
		wstring getTypeArgumentNames();
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




} // Close namespace Jikes block


#endif // AstTypeArguments_INCLUDED
