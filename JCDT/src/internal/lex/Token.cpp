#include "JCDT_Lib/internal/lex/Token.h"

namespace Jikes { // Open namespace Jikes block



Token::Token(unsigned _location)
{
	ResetInfoAndSetLocation(_location);
}


Token::~Token()
{
	
}

} // Close namespace Jikes block
