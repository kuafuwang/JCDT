#ifndef AstClassDeclaration_INCLUDED
#define AstClassDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclaredType.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block


	//
	// Represents a class declaration.
	//
	class ClassTypeDeclaration : public TypeDeclaration
	{
	public:
		AstStoragePool* pool;
	
		AstArray<AstTypeName*>* interfaces;

		virtual TypeDeclarationKind getTypeDeclarationKind()
		{
			return CLASS_DECL;
		}
		Token* class_token;
		AstTypeParameters* type_parameters_opt;
		AstTypeName* super_opt;
		static ClassTypeDeclaration* ClassDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<ClassTypeDeclaration*>(node->kind == CLASS ? node : NULL);
		}

		ClassTypeDeclaration(AstStoragePool* p);

		~ClassTypeDeclaration() {}

		  AstTypeName*& Interface(unsigned i);

		  unsigned NumInterfaces();
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
	inline AstTypeName*& ClassTypeDeclaration::Interface(unsigned i)
	{
		return (*interfaces)[i];
	}

	inline  unsigned ClassTypeDeclaration::NumInterfaces()
	{
		return interfaces ? interfaces->Length() : 0;
	}



	inline void ClassTypeDeclaration::AddInterface(AstTypeName* interf)
	{
		assert(interfaces);
		interfaces->Next() = interf;
	}



} // Close namespace Jikes block


#endif // AstClassDeclaration_INCLUDED
