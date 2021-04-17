#ifndef MatchLocatorParserVisitor_JIKES_SERACH_INCLUDE
#define MatchLocatorParserVisitor_JIKES_SERACH_INCLUDE

#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <stack>

namespace Jikes { // Open namespace Jikes block

	namespace Search {
		class MatchSet;
	
		 class MatchLocatorParserVisitor :public ASTVisitor
		 {
		 public:
			 MatchSet* matchSet;
			 MatchLocatorParserVisitor();
			 bool visit(AstClassCreationExpression * node, AstNodeScope* scope)override;
			 bool visit(AstAssignmentExpression * node, AstNodeScope* scope)override;

			 bool visit(AstSuperCall * node, AstNodeScope* scope)override;
			 bool visit(AstThisCall * node, AstNodeScope* scope)override;
			 bool visit(FieldReference * node, AstNodeScope* scope)override;
			 bool visit(AstMethodInvocation * node, AstNodeScope* scope)override;

			 bool visit(AstImportDeclaration * node, AstNodeScope* scope)override;

			 bool visit(AstPostUnaryExpression * node, AstNodeScope* scope)override;
			 bool visit(AstPreUnaryExpression * node, AstNodeScope* scope)override;

			 bool visit(AstTypeName * node, AstNodeScope* scope)override;
			 bool visit(AstArrayType * node, AstNodeScope* scope)override;
			 bool visit(AstName * node, AstNodeScope* scope)override;



			bool visit(ClassTypeDeclaration * node, AstNodeScope* scope)override;
			bool visit(InterfaceTypeDeclaration * node, AstNodeScope* scope)override;
			bool visit(AnnotationTypeDeclaration * node, AstNodeScope* scope)override;

			bool visit(EnumTypeDeclaration * node, AstNodeScope* scope);
			
			 bool visit(ConstructorDeclaration * node, AstNodeScope* scope)override;
			 // 相当于 jdt里面的FieldDeclaration
			 bool visit(AstVariableDeclarator * node, AstNodeScope* scope)override;

			 bool visit(Initializer * node, AstNodeScope* scope)override;

			 bool visit(AstLocalClassStatement * node, AstNodeScope* scope)override;
			
			 bool visit(MethodDeclaration * node, AstNodeScope* scope)override;

		 };

		
  }//namespace CodeAssist

} // Close namespace Jikes block


#endif // ASTVisitor_INCLUDED

