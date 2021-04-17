
#ifndef AstDeclaredType_INCLUDED
#define AstDeclaredType_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include <JCDT_Lib/internal/impl/ReferenceContext.h>
#include "JCDT_Lib/internal/ast/AstDeclared.h"
#include "AstNodeScope.h"

namespace Jikes { // Open namespace Jikes block


	//
	// This class adds some type safety. It represents all type declarations.
	// See ClassTypeDeclaration, EnumTypeDeclaration, InterfaceTypeDeclaration,
	// AnnotationTypeDeclaration, and EmptyTypeDeclaration.
	//
	class TypeSymbol;
	class TypeDeclaration : public AstDeclared, public AstNodeScope
	{
	public:
		enum TypeDeclarationKind
		{
			CLASS_DECL = 1,
			INTERFACE_DECL = 2,
			ENUM_DECL = 3,
			ANNOTATION_TYPE_DECL = 4,
			EMPTY_TYPE_DECLARE = 5,
			ANONYMOUS_TYPE_DECLARE = 6
		};
		enum TypeLevel
		{
			TOP_LEVEL,
			LOCAL_LEVEL,
			MEMBER_LEVEL,
			ANONYMOUS_LEVEL,
		};
         void MarkTopLevel()
		{
			_level = TOP_LEVEL;
		}
		 void MarkLocalLevel()
         {
			 _level = LOCAL_LEVEL;
         }
		 void MarkMemberLevel()
         {
			 _level = MEMBER_LEVEL;
         }
		 void MarkAnomynousLevel()
         {
			 _level = ANONYMOUS_LEVEL;
         }

		 bool IsToplevelType()const {
			 return _level == TOP_LEVEL;
		 }
		 bool IsMemberType()const {
			 return _level == MEMBER_LEVEL;
		 }
		 bool IsLocalType()const
         {
			 return _level == LOCAL_LEVEL;
         }
		 bool IsInnerType()const
         {
			 return IsMemberType() || IsLocalType() || IsAnonymousType();
         }
		 bool IsAnonymousType() const
         {
			 return _level == ANONYMOUS_LEVEL;
         }
		virtual TypeDeclarationKind getTypeDeclarationKind() = 0;
		TypeLevel  _level;
		AstClassBody* class_body;
		TypeSymbol* symbol;
		TypeDeclaration(AstKind k);

		~TypeDeclaration() {}
		/*virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;*/
		  bool IsValid();
		virtual wstring getName();
		int GetScopeKind() override {
			return		CLASS_SCOPE;
		}
	
		Symbol* GetSymbol() override;
	};


	


} // Close namespace Jikes block


#endif // AstDeclaredType_INCLUDED

