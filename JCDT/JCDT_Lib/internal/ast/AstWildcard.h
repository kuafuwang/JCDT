#ifndef AstWildcard_INCLUDED
#define AstWildcard_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstType.h"
namespace Jikes { // Open namespace Jikes block
	// Represents a wildcard type. Only occurs in type arguments for naming a
	// generic type or method (but not in explicit type arguments for invoking
	// a method).
	class AstWildcard : public AstType
	{
	public:
		static AstWildcard* WildcardCast(Ast* node)
		{
			return DYNAMIC_CAST<AstWildcard*>(node->kind == WILDCARD ? node : NULL);
		}
		static const wchar_t  WILDCARD_NAME[];
		static const wchar_t WILDCARD_SUPER[];
		static const wchar_t WILDCARD_EXTENDS[];
		Token* question_token;
		// 0 or 1 of the next two fields, but never both
		Token* extends_token_opt;
		Token* super_token_opt;
		AstType* bounds_opt; // AstArrayType, AstTypeName
		bool IsQualifiedTypeReference() override
		{
			if (bounds_opt){
				return false;
			}
			return bounds_opt->IsQualifiedTypeReference();
		}
		AstWildcard(Token* t);

		~AstWildcard() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken() { return question_token; }
		virtual Token* RightToken()
		{
			return bounds_opt ? bounds_opt->RightToken() : question_token;
		}
		virtual vector<wstring> getTypeName()  override;
		virtual vector<wstring> getParameterizedTypeName()  override;
		
		virtual Token* IdentifierToken() { return question_token; }
	};




} // Close namespace Jikes block


#endif // AstWildcard_INCLUDED
