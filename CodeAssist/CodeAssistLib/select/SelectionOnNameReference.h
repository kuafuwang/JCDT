
#ifndef SelectionOnFieldReference_INCLUDED
#define SelectionOnFieldReference_INCLUDED
#include <exception>

namespace Jikes { // Open namespace Jikes block
	class Ast;
	class AstNodeScope;

namespace CodeAssist{
	class SelectionOnNameReference :public std::exception
	{
	public:
		enum ReferenceKind
		{

			SelectionOnExpression,
			
			
			SelectionOnSuperCall,
			SelectionOnThisCall,

			SelectionOnMethod,
			SelectionOnConstructor, 
			SelectionOnField,
			SelectionEnumConstant	,
			SelectionClassCreation
		};
		SelectionOnNameReference(Ast* _ast, ReferenceKind kind_, AstNodeScope* _scope):node(_ast), kind(kind_), scope(_scope)
		{
			
		}
		Ast* node = nullptr;
		ReferenceKind kind;
		AstNodeScope* scope;
	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

