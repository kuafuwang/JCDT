
#ifndef CompleteLex_Jikes_CodeAssist_INCLUDED
#define CompleteLex_Jikes_CodeAssist_INCLUDED
#include <JCDT_Lib/internal/lex/LexStream.h>
namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class CompleteLex :public LexStream
	{
		
	public:
		CompleteLex(Option* option_, FileBase* file_symbol_);

		virtual void ChangeTokenStream(vector<Token*>& _token_stream) override;
		
		virtual ~CompleteLex() override;

		vector<Token*> original_token_stream;
		vector<Token*>& getOriginalTokenStream()
		{
			if(original_token_stream.empty())
			{
				return token_stream;
			}
			else
			{
				return original_token_stream;
			}
		}

		inline int FindGoodTokenIndexInOrigianlTokenStream(unsigned int pos)
		{
			if(original_token_stream.empty())
			{
				auto _size = token_stream.size();
				if (_size)
				{
					return BinarySearch<Token>(token_stream.data(), _size, pos);
				}
			}
			else
			{
				auto _size = original_token_stream.size();
				if (_size){
					return BinarySearch<Token>(original_token_stream.data(), _size, pos);
				}
			}
			

			return PositionNode::NO_FOUND;
		}


	};


}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

