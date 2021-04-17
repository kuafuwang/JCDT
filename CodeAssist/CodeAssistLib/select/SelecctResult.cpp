#include "stdafx.h"
#include  "SelecctResult.h"
#include  "SelectionWhenDebug.h"
#include "CodeAssistLib/ast/AssistExpression.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block

	
namespace CodeAssist{
	SelecctResult::EnumType SelecctResult::GetType()
	{
		return SELECT_FOR_EDIT_TYPE;
	}

	SelecctResult::EnumType SelectForDebug::GetType()
	{
		return SELECT_FOR_DEBUG_TYPE;
	}

	SelectForDebug::~SelectForDebug()
	{
		delete expr;
	}

	AssistExpression* SelectForDebug::TransferExpr()
	{
		auto temp = expr;
		expr = nullptr;
		return temp;
	}

	SelecctResult::EnumType SelectForScope::GetType()
	{
		return SELECT_FOR_SCOPE_TYPE;
	}

	SelectForScope::~SelectForScope()
	{
		delete range;
	}

	JavaModel::ISourceRange* SelectForScope::TransferRange()
	{
		auto temp = range;
		range = nullptr;
		return temp;
	}
	 SelectionWhenDebug::SelectionWhenDebug(Ast* _ast, ReferenceKind kind_, AstNodeScope* _scope) 
	: node(_ast), kind(kind_), scope(_scope)
	{

	}
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block



