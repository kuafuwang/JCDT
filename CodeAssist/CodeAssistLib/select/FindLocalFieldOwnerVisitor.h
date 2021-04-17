
#ifndef Jikes_CodeAssist_MethodVisitor_INCLUDED
#define Jikes_CodeAssist_MethodVisitor_INCLUDED
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class FindLocalFieldOwnerVisitor : public ASTVisitor
	{
	public :
		 unsigned pos;
		 std::vector<TypeDeclaration*> type_stack;
		 bool IsInRange(Ast*) const;
		 bool visit(AstBlock * node, AstNodeScope* scope)override;


		 bool visit(CompilationUnitDeclaration * node, AstNodeScope* scope)override;

		 bool visit(AstClassBody * node, AstNodeScope* scope)override;


		 bool visit(ClassTypeDeclaration * node, AstNodeScope* scope)override;
		void endVisit(ClassTypeDeclaration* node, AstNodeScope* scope) override;
		bool visit(MethodDeclaration * node, AstNodeScope* scope)override;

		 bool visit(ConstructorDeclaration * node, AstNodeScope* scope)override;


		 bool visit(EnumTypeDeclaration * node, AstNodeScope* scope)override;
		void endVisit(EnumTypeDeclaration* node, AstNodeScope* scope) override;


		bool visit(InterfaceTypeDeclaration * node, AstNodeScope* scope)override;
		void endVisit(InterfaceTypeDeclaration* node, AstNodeScope* scope) override;


		bool visit(AnnotationTypeDeclaration * node, AstNodeScope* scope)override;
		void endVisit(AnnotationTypeDeclaration* node, AstNodeScope* scope) override;

		bool visit(Initializer* node, AstNodeScope* scope) override;
	};
	inline bool FindLocalFieldOwnerVisitor::IsInRange(Ast* node) const
	{
		auto start = node->LeftToken()->start_location;
		if (pos < start){
			return false;
		}
		auto end = node->RightToken()->EndLocation();
		if (pos > end){
			return false;
		}
		return true;
	}

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

