
#ifndef Jikes_Jikes_ExpresionConverterForWatch_INCLUDED
#define Jikes_Jikes_ExpresionConverterForWatch_INCLUDED
#include <exception>
#include <CodeAssistLib/ast/AssistExpression.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>


namespace Jikes { // Open namespace Jikes block
	class LookupEnvironment;
	class LexStream;
namespace CodeAssist{
	class AssistAstName;

	class ExpresionConverterForWatch{
		public:
			LexStream* lex_stream;
			LookupEnvironment& lookup_env;
			bool unreachable = false;
			explicit ExpresionConverterForWatch(LookupEnvironment& _env);

		inline AssistExpression* ProcessExpression(Expression* expr);


			//  expression processing
			AssistExpression* (ExpresionConverterForWatch::*ProcessExprOrStmt[Expression::_num_expression_kinds])(Expression*);
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
			AssistAstName* ConverterTypeNameToName(AstTypeName* typeName);
			
	};

	
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // Jikes_Jikes_ExpressProcess_INCLUDED

