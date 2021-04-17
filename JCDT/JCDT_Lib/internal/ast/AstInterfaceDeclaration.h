#ifndef AstInterfaceDeclaration_INCLUDED
#define AstInterfaceDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclaredType.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block


		//
		// Represents an interface type.
		//
	class InterfaceTypeDeclaration : public TypeDeclaration
	{
	public:
		AstStoragePool* pool;
	
		AstArray<AstTypeName*>* interfaces;

		static InterfaceTypeDeclaration* InterfaceDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<InterfaceTypeDeclaration*>
				(node->kind == INTERFACE_TYPE ? node : NULL);
		}
		Token* interface_token;
		AstTypeParameters* type_parameters_opt;
		virtual TypeDeclarationKind getTypeDeclarationKind()
		{
			return INTERFACE_DECL;
		}

		InterfaceTypeDeclaration(AstStoragePool* p);

		~InterfaceTypeDeclaration() {}

		  AstTypeName*& Interface(unsigned i)
		{
			return (*interfaces)[i];
		}
		  unsigned NumInterfaces()
		{
			return interfaces ? interfaces->Length() : 0;
		}
		  void AllocateInterfaces(unsigned estimate = 1);
		  void AddInterface(AstTypeName*);

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


	inline InterfaceTypeDeclaration::InterfaceTypeDeclaration(AstStoragePool* p): TypeDeclaration(INTERFACE_TYPE)
	                                                                              , pool(p), interfaces(nullptr), interface_token(nullptr), type_parameters_opt(nullptr)
	{}


	inline    void InterfaceTypeDeclaration::AddInterface(AstTypeName* interf)
	{
		assert(interfaces);
		interfaces->Next() = interf;
	}


} // Close namespace Jikes block


#endif // AstInterfaceDeclaration_INCLUDED
