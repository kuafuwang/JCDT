
#ifndef AstFound_INCLUDED
#define AstFound_INCLUDED
#include <exception>

namespace Jikes { // Open namespace Jikes block
	class Ast;

namespace CodeAssist{
	class AstFound :public std::exception
	{
		
	public:
		enum AstFoundKind
		{
			FIND_TYPE,

			FIND_IMPORT,

			FIND_PACKAGE,

			FIND_METHOD,

			FIND_CONSTRUCTOR,

			FIND_INITIALIZER
		};
		AstFound(Ast* node_, AstFoundKind _kind)
			:node(node_), kind(_kind)
		{
			
		}
		Ast* node = nullptr;
		AstFoundKind kind;
	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

