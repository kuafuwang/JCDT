#ifndef AstTypeName_INCLUDED
#define AstTypeName_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstType.h"
namespace Jikes { // Open namespace Jikes block
	// Represents a type. Occurs in several contexts - imports; supertypes;
	// throws clauses; parameter, field, and method return types; qualified this
	// and super; class literals; casts. Some of these uses can be parameterized.
	//
	class AstTypeName : public AstType
	{
	public:
		AstTypeName* base_opt;
		AstName* name;
		AstTypeArguments* type_arguments_opt;
		static AstTypeName* TypeNameCast(Ast* node)
		{
			return DYNAMIC_CAST<AstTypeName*>(node->kind == TYPE ? node : NULL);
		}

		AstTypeName(AstName* n);

		~AstTypeName() {}
		bool IsQualifiedTypeReference() override
		{
			return base_opt != nullptr;
		}
		  AstType*& TypeArgument(unsigned i);

		  unsigned NumTypeArguments();

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG
		virtual vector<wstring> getTypeName() override;
		virtual vector<wstring> getParameterizedTypeName() override;
		virtual Ast* Clone(AstStoragePool*);
		
		

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken();

		virtual Token* RightToken();

		virtual Token* IdentifierToken();
	};





} // Close namespace Jikes block


#endif // AstTypeName_INCLUDED
