#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#ifndef AstMemberValue_def_INCLUDED
#define AstMemberValue_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstMemberValue.h"

namespace Jikes { // Open namespace Jikes block



	TypeSymbol* AstMemberValue::Type()
	{
		return !symbol ? (TypeSymbol*)NULL
			: symbol->Kind() == Symbol::TYPE
			? (TypeSymbol*)symbol
			: symbol->Kind() == Symbol::VARIABLE
			? ((VariableSymbol*)symbol)->Type()
			: symbol->Kind() == Symbol::METHOD
			? ((MethodSymbol*)symbol)->Type()
			: (TypeSymbol*)NULL;
	}


} // Close namespace Jikes block


#endif // AstMemberValue_def_INCLUDED
