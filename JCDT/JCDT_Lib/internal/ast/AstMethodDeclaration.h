#ifndef AstMethodDeclaration_INCLUDED
#define AstMethodDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include  "AbstractMethodDeclaration.h"

namespace Jikes { // Open namespace Jikes block



//
// Represents MethodDeclaration, AbstractMethodDeclaration, and Annotation
// method declarations added in JSR 175.
//
	class MethodDeclaration : public AbstractMethodDeclaration
	{
		AstStoragePool* pool;
		AstArray<AstTypeName*>* throws;

	public:
		unsigned int BodyStart() override;

		unsigned int BodyEnd() override;

		static MethodDeclaration* MethodDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<MethodDeclaration*>(node->kind == METHOD ? node : NULL);
		}
		
		AstType* type;
		AstMemberValue* default_value_opt;
		AstMethodBody* method_body_opt;
		Token* semicolon_token_opt;

		MethodDeclaration(AstStoragePool* p);

		~MethodDeclaration() {}

		bool IsValid() { return method_symbol != NULL; }

		bool IsSignature() { return !method_body_opt; }

		  AstTypeName*& Throw(unsigned i) const;

		  unsigned NumThrows();
		  void AllocateThrows(unsigned estimate = 1);
		  void AddThrow(AstTypeName*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken();

		virtual Token* RightToken();
	
		 vector<wstring> getReturnTypeName() override;
	};

	inline MethodDeclaration::MethodDeclaration(AstStoragePool* p)
		: AbstractMethodDeclaration(METHOD) , pool(p), throws(nullptr),
		  type(nullptr),
		 default_value_opt(nullptr),
		method_body_opt(nullptr), semicolon_token_opt(nullptr)
	{}
} // Close namespace Jikes block


#endif // AstMethodDeclaration_INCLUDED
