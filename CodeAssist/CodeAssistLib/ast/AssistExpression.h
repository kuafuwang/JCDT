#ifndef Jikes_CodeAssist_AssistExpression_INCLUDED
#define Jikes_CodeAssist_AssistExpression_INCLUDED
#include <string>
using std::wstring;
namespace Jikes { // Open namespace Jikes block
	class LiteralValue;
   namespace CodeAssist{
	   struct SymbolInfo;
	  class   AssistExpression
	{
	public:
		


		enum AstKind
		{
			PLACE_HOLDER, // must be first
			// Expressions
			NAME,
			DOT,
			INTEGER_LITERAL,
			LONG_LITERAL,
			FLOAT_LITERAL,
			DOUBLE_LITERAL,
			TRUE_LITERAL,
			FALSE_LITERAL,
			STRING_LITERAL,
			CHARACTER_LITERAL,
			NULL_LITERAL,
			CLASS_LITERAL,
			THIS_EXPRESSION,
			SUPER_EXPRESSION,
			PARENTHESIZED_EXPRESSION,
			ARRAY_ACCESS,
			CALL,
			CLASS_CREATION,
			ARRAY_CREATION,
			POST_UNARY,
			PRE_UNARY,
			CAST,
			BINARY,
			INSTANCEOF,
			CONDITIONAL,
			ASSIGNMENT,
			_num_expression_kinds,
			
		};
	
		bool IsConstant() { return session_value != NULL; }
		SymbolInfo* symbol;
		
		LiteralValue* session_value ;//用来保存一些常量值
		  AssistExpression(AstKind _kind);

		  virtual ~AssistExpression();
		  const AstKind kind ;
	
		
	};

   }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistExpression_INCLUDED
