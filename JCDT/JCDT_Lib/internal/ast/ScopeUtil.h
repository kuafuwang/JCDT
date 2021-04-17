#ifndef ScopeUtil_Jikes_INCLUDED
#define ScopeUtil_Jikes_INCLUDED

namespace Jikes { // Open namespace Jikes block
	class CompilationUnitDeclaration;
	class TypeDeclaration;
	class AstNodeScope;
	class TypeSymbol;
	class ScopeUtil
	{
	public:
		static CompilationUnitDeclaration* compilationUnitScope(AstNodeScope*);
		/* Answer the receiver's enclosing source type.
		*/
		static TypeSymbol* enclosingSourceType(AstNodeScope*);

		static AstNodeScope* enclosingMethod(AstNodeScope*);

		static AstNodeScope* enclosingType(AstNodeScope*);

		static AstNodeScope* getInitializerBodyScop(AstNodeScope*);

	}; // Close namespace Jikes block
}
#endif
