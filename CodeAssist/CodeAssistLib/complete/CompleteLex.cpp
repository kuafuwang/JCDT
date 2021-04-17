#include "stdafx.h"
#include "CompleteLex.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		CompleteLex::CompleteLex(Option* option_, FileBase* file_symbol_): LexStream(option_, file_symbol_)
		{

		}

		void CompleteLex::ChangeTokenStream(vector<Token*>& _token_stream)
		{
			if(original_token_stream.empty()){
				original_token_stream.swap(token_stream);
			}
			token_stream.swap(_token_stream);
		}

		CompleteLex::~CompleteLex()
		{

		}

	} // Close namespace Jikes block

} // Close namespace CodeAssist block

