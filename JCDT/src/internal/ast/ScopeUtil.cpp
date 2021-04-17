#include "JCDT_Lib/internal/ast/ScopeUtil.h"
#include <JCDT_Lib/internal/ast/AstNodeScope.h>
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>
#include <JCDT_Lib/internal/ast/CompilationUnitDeclaration.h>
namespace Jikes { // Open namespace Jikes block


	CompilationUnitDeclaration* ScopeUtil::compilationUnitScope(AstNodeScope* scope)
	{
		
		while (scope)
		{
			if (scope->GetScopeKind() == AstNodeScope::COMPILATION_UNIT_SCOPE)
				return  dynamic_cast<CompilationUnitDeclaration*>(scope);

			scope = scope->parent_scope;
		}
		return nullptr;
	}

	TypeSymbol* ScopeUtil::enclosingSourceType(AstNodeScope*scope)
	{
		while (scope)
		{
			if (scope->GetScopeKind() == AstNodeScope::CLASS_SCOPE)
				return  dynamic_cast<TypeDeclaration*>(scope)->symbol;
			scope = scope->parent_scope;
		} 
		return nullptr;
	}

	AstNodeScope* ScopeUtil::enclosingMethod(AstNodeScope* scope)
	{
	
		while (scope)
		{
			switch ((scope->GetScopeKind()))
			{
			case AstNodeScope::CLASS_SCOPE:
			case AstNodeScope::COMPILATION_UNIT_SCOPE:
				break;
			case AstNodeScope::METHOD_SCOPE:
				return scope;
			}
			scope = scope->parent_scope;
		} 
		return nullptr;
	}

	AstNodeScope* ScopeUtil::enclosingType(AstNodeScope* scope)
	{
		while (scope){
			switch ((scope->GetScopeKind()))
			{
			case AstNodeScope::METHOD_SCOPE:
			case AstNodeScope::COMPILATION_UNIT_SCOPE:
				break;
			case AstNodeScope::CLASS_SCOPE:
				return scope;
			}
			scope = scope->parent_scope;
		}
		return nullptr;
	}

	AstNodeScope* ScopeUtil::getInitializerBodyScop(AstNodeScope*scope)
	{
		AstNodeScope*  prev_scope = scope;
		while (scope) {
			
			switch ((scope->GetScopeKind()))
			{
			case AstNodeScope::METHOD_SCOPE:
			case AstNodeScope::COMPILATION_UNIT_SCOPE:
				break;
			case AstNodeScope::CLASS_SCOPE:
				return prev_scope;
			}
			prev_scope = scope;
			scope = scope->parent_scope;
		}
		return nullptr;
	}
} // Close namespace Jikes block



