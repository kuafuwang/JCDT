#ifndef AstEnumConstant_INCLUDED
#define AstEnumConstant_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclared.h"
namespace Jikes { // Open namespace Jikes block

	//
	// Represents an enum constant, added by JSR 201.
	//
	class AstEnumConstant : public AstDeclared
	{
	public:
		static AstEnumConstant* EnumConstantCast(Ast* node)
		{
			return DYNAMIC_CAST<AstEnumConstant*>(node->kind == ENUM_CONSTANT ? node : NULL);
		}
		Token* identifier_token;
		AstArguments* arguments_opt;
		AstClassBody* class_body_opt;

		u4 ordinal; // the sequential position of the constant
		VariableSymbol* field_symbol; // the field the constant lives in
		MethodSymbol* ctor_symbol; // the constructor that builds the constant

		AstEnumConstant(Token* t);

		~AstEnumConstant() {}

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


#endif // AstEnumConstant_INCLUDED
