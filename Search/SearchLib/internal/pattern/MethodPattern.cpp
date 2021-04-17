#include "stdafx.h"
#include "MethodPattern.h"
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lex/LexStream.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	MethodPattern::MethodPattern(int _matchMode, bool _isCaseSensitive): SearchPattern(_matchMode, _isCaseSensitive), parameterQualifications(nullptr), parameterSimpleNames(nullptr)
	{
	}

	bool MethodPattern::IsMayInTheFile(BaseLexStream* lex)
	{
		if (!lex)
			return false;

		auto data_base_prefix = selector.data();
		auto size_base_prefix = selector.size();
		auto  tokens = lex->token_stream.data();
		auto token_size = lex->token_stream.size();
		for (auto i = 0; i < token_size; ++i)
		{
			auto token = tokens[i];
			if (token && token->_kind == TK_Identifier && token->additional_info.name_symbol)
			{

				if (matchesName(
					data_base_prefix,
					size_base_prefix,
					token->additional_info.name_symbol->Name(), token->additional_info.name_symbol->NameLength()
				))
				{
					auto next_token = tokens[i + 1];
					if(next_token && next_token->_kind  == TK_LPAREN)
					{
						return true;
					}
					
				}
			}

		}

		return false;
	}
}


} // Close namespace Jikes block


