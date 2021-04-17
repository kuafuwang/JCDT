
#ifndef SelectionWhenDebug_INCLUDED
#define SelectionWhenDebug_INCLUDED
#include <exception>

namespace Jikes { // Open namespace Jikes block
	class Ast;
	class AstNodeScope;
namespace CodeAssist{
	class SelectionWhenDebug :public std::exception
	{
	public:
		enum ReferenceKind
		{
			OnAstVariableDeclarator,
			ArrayReferenceExepression,
			AstSuperExpressionOnSuper,
			AstThisExpressionOnThis,
			AstExpressOnFieldReference,
			AstExpressOnName,
		};

		SelectionWhenDebug(Ast* _ast, ReferenceKind kind_, AstNodeScope* _scope);
		Ast* node = nullptr;
		ReferenceKind kind;
		AstNodeScope* scope;
	};

	
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

