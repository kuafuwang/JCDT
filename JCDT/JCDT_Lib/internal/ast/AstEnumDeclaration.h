#ifndef AstEnumDeclaration_INCLUDED
#define AstEnumDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclaredType.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block


	//
	// Represents an enum type, added by JSR 201.
	//
	class EnumTypeDeclaration : public TypeDeclaration
	{
		AstStoragePool* pool;
		AstArray<AstTypeName*>* interfaces;
		AstArray<AstEnumConstant*>* enum_constants;

	public:
		static EnumTypeDeclaration* EnumDeclarationCast(Ast*node)
		{
			return DYNAMIC_CAST<EnumTypeDeclaration*>(node->kind == ENUM_TYPE ? node : NULL);
		}
		Token* enum_token;
		virtual TypeDeclarationKind getTypeDeclarationKind()
		{
			return ENUM_DECL;
		}

		EnumTypeDeclaration(AstStoragePool* p);

		~EnumTypeDeclaration() {}

		  AstTypeName*& Interface(unsigned i);

		  unsigned NumInterfaces() const;
		  void AllocateInterfaces(unsigned estimate = 1);
		  void AddInterface(AstTypeName*);

		  AstEnumConstant*& EnumConstant(unsigned i);

		  unsigned NumEnumConstants();
		  void AllocateEnumConstants(unsigned estimate = 1);
		  void AddEnumConstant(AstEnumConstant*);

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

	
	inline void EnumTypeDeclaration::AddInterface(AstTypeName* interf)
	{
		assert(interfaces);
		interfaces->Next() = interf;
	}

	

	inline AstTypeName*& EnumTypeDeclaration::Interface(unsigned i)
	{
		return (*interfaces)[i];
	}

	inline unsigned EnumTypeDeclaration::NumInterfaces() const
	{
		return interfaces ? interfaces->Length() : 0;
	}

	inline AstEnumConstant*& EnumTypeDeclaration::EnumConstant(unsigned i)
	{
		return (*enum_constants)[i];
	}

	inline unsigned EnumTypeDeclaration::NumEnumConstants()
	{
		return enum_constants ? enum_constants->Length() : 0;
	}


} // Close namespace Jikes block


#endif // AstEnumDeclaration_INCLUDED
