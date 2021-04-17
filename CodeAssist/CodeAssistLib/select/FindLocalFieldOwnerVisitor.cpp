#include "stdafx.h"
#include "FindLocalFieldOwnerVisitor.h"
#include "AstFound.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{

	bool FindLocalFieldOwnerVisitor::visit(AstBlock* node, AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		return true;
	}

	bool FindLocalFieldOwnerVisitor::visit(CompilationUnitDeclaration* node, AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		return true;
	}

	bool FindLocalFieldOwnerVisitor::visit(AstClassBody* node, AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		return true;
	}

	bool FindLocalFieldOwnerVisitor::visit(ClassTypeDeclaration* node, AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node))
			return false;

		return true;
	}
	void  FindLocalFieldOwnerVisitor::endVisit(ClassTypeDeclaration * node, AstNodeScope* scope)
	{
		type_stack.pop_back();
	}

	bool FindLocalFieldOwnerVisitor::visit(MethodDeclaration* node, AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		throw AstFound(node,AstFound::FIND_METHOD);	
	}

	bool FindLocalFieldOwnerVisitor::visit(ConstructorDeclaration* node, AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		throw AstFound(node, AstFound::FIND_CONSTRUCTOR);
	}

	bool FindLocalFieldOwnerVisitor::visit(EnumTypeDeclaration* node, AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node))
			return false;

		return true;
	}
	void  FindLocalFieldOwnerVisitor::endVisit(EnumTypeDeclaration * node, AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool FindLocalFieldOwnerVisitor::visit(InterfaceTypeDeclaration* node, AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node))
			return false;

		return true;
	}
	void  FindLocalFieldOwnerVisitor::endVisit(InterfaceTypeDeclaration * node, AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool FindLocalFieldOwnerVisitor::visit(AnnotationTypeDeclaration* node, AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node))
			return false;

		return true;
	}
	void  FindLocalFieldOwnerVisitor::endVisit(AnnotationTypeDeclaration * node, AstNodeScope* scope){
		type_stack.pop_back();
	}

	bool FindLocalFieldOwnerVisitor::visit(Initializer* node, AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		throw AstFound(node, AstFound::FIND_INITIALIZER);
	}
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block



