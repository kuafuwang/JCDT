
#ifndef Jikes_Jikes_ExpressProcess_INCLUDED
#define Jikes_Jikes_ExpressProcess_INCLUDED
#include <exception>
#include <CodeAssistLib/ast/AssistExpression.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>


namespace Jikes { // Open namespace Jikes block
	class LookupEnvironment;

namespace CodeAssist{

	class ExpressionConverter{
		public:
			LookupEnvironment& lookup_env;
			bool unreachable = false;
			explicit ExpressionConverter(LookupEnvironment& _env);

			inline AssistExpression*  ProcessExpression(Expression* expr)
			{
				if (!expr)
					return nullptr;

				if (!expr->symbol) {
					// already processed, make sure it was compiler-generated
					assert(!expr->generated);
					return nullptr;
				}
				return (this->*ProcessExprOrStmt[expr->kind])(expr);
			}

			static AssistExpression* ProcessSimpleNameVariable(VariableSymbol*);

			//  expression processing
			AssistExpression* (ExpressionConverter::*ProcessExprOrStmt[Expression::_num_expression_kinds])(Expression*);
			AssistExpression* ProcessName(Expression*);	
			AssistExpression* ProcessFieldAccess(Expression*);
			AssistExpression* ProcessIntegerLiteral(Expression*);
			AssistExpression* ProcessLongLiteral(Expression*);
			AssistExpression* ProcessFloatLiteral(Expression*);
			AssistExpression* ProcessDoubleLiteral(Expression*);
			AssistExpression* ProcessTrueLiteral(Expression*);
			AssistExpression* ProcessFalseLiteral(Expression*);
			AssistExpression* ProcessStringLiteral(Expression*);
			AssistExpression* ProcessCharacterLiteral(Expression*);
			AssistExpression* ProcessArrayAccess(Expression*);
			AssistExpression* ProcessMethodInvocation(Expression*);
			AssistExpression* ProcessNullLiteral(Expression*);
			AssistExpression* ProcessClassLiteral(Expression*);
			AssistExpression* ProcessThisExpression(Expression*);
			AssistExpression* ProcessSuperExpression(Expression*);
			AssistExpression* ProcessParenthesizedExpression(Expression*);
			AssistExpression* ProcessClassCreationExpression(Expression*);
			AssistExpression* ProcessArrayCreationExpression(Expression*);
			AssistExpression* ProcessPostUnaryExpression(Expression*);
			AssistExpression* ProcessPreUnaryExpression(Expression*);
			AssistExpression* ProcessCastExpression(Expression*);
			AssistExpression* ProcessBinaryExpression(Expression*);
			AssistExpression* ProcessInstanceofExpression(Expression*);
			AssistExpression* ProcessConditionalExpression(Expression*);
			AssistExpression* ProcessAssignmentExpression(Expression*);
	};

	
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // Jikes_Jikes_ExpressProcess_INCLUDED

