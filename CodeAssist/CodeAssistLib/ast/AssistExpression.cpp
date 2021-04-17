#include "stdafx.h"
#include "AssistExpression.h"
#include "SymbolInfo.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
	

		AssistExpression::AssistExpression(AstKind _kind): symbol(nullptr), session_value(nullptr), kind(_kind)
		{
		}

		AssistExpression::~AssistExpression()
		{
			if (symbol)
				delete symbol;
			if (session_value)
				delete session_value;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

