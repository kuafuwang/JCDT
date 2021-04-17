
#ifndef FindAstNode_INCLUDED
#define FindAstNode_INCLUDED
namespace Jikes {// Open namespace Jikes block
	class LexStream;
	class CompilationUnitDeclaration;
	class Ast;

namespace CodeAssist{
	
	class FindAstNode
	{
		Ast* find_node = nullptr;
		void Find(CompilationUnitDeclaration*, LexStream*,unsigned int position);
	};
	
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // FindAstNode_INCLUDED

