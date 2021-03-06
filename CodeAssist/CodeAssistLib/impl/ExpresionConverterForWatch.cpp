#include "stdafx.h"
#include <CodeAssistLib/impl/ExpresionConverterForWatch.h>
#include <JCDT_Lib/internal/ast/AstFieldAccess.h>
#include <JCDT_Lib/internal/ast/AstThisExpression.h>

#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <CodeAssistLib/ast/CodeAssistFieldAccess.h>
#include <CodeAssistLib/ast/CodeAssistIntegerLiteral.h>
#include <CodeAssistLib/ast/CodeAssistLongLiteral.h>
#include <CodeAssistLib/ast/CodeAssistDoubleLiteral.h>
#include <CodeAssistLib/ast/CodeAssistTrueLiteral.h>
#include <CodeAssistLib/ast/CodeAssistArrayAccess.h>
#include <CodeAssistLib/ast/CodeAssistNullLiteral.h>
#include <CodeAssistLib/ast/CodeAssistThisExpression.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <CodeAssistLib/ast/CodeAssistSuperExpression.h>
#include <CodeAssistLib/ast/CodeAssistParenthesizedExpression.h>
#include <CodeAssistLib/ast/CodeAssistPostUnaryExpression.h>
#include <CodeAssistLib/ast/CodeAssistPreUnaryExpression.h>
#include <CodeAssistLib/ast/CodeAssistCastExpression.h>
#include <CodeAssistLib/ast/CodeAssistBinaryExpression.h>
#include <CodeAssistLib/ast/CodeAssistConditionalExpression.h>
#include <CodeAssistLib/ast/CodeAssistAssignmentExpression.h>
#include <CodeAssistLib/ast/CodeAssistName.h>
#include <CodeAssistLib/ast/CodeAssistFalseLiteral.h>
#include <CodeAssistLib/ast/CodeAssistFloatLiteral.h>
#include <CodeAssistLib/ast/CodeAssistStringLiteral.h>
#include <CodeAssistLib/ast/CodeAssistCharacterLiteral.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <CodeAssistLib/ast/SymbolInfo.h>
#include <CodeAssistLib/ast/AssistExpression.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>
namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	
	
	ExpresionConverterForWatch::ExpresionConverterForWatch(LookupEnvironment& _env)
	:lookup_env(_env)
	{
		// ExpressProcess processing of expressions.
		ProcessExprOrStmt[AssistExpression::NAME] = &ExpresionConverterForWatch::ProcessName;
		ProcessExprOrStmt[AssistExpression::DOT] = &ExpresionConverterForWatch::ProcessFieldAccess;
		ProcessExprOrStmt[AssistExpression::INTEGER_LITERAL] =
			&ExpresionConverterForWatch::ProcessIntegerLiteral;
		ProcessExprOrStmt[AssistExpression::LONG_LITERAL] = &ExpresionConverterForWatch::ProcessLongLiteral;
		ProcessExprOrStmt[AssistExpression::FLOAT_LITERAL] = &ExpresionConverterForWatch::ProcessFloatLiteral;
		ProcessExprOrStmt[AssistExpression::DOUBLE_LITERAL] =
			&ExpresionConverterForWatch::ProcessDoubleLiteral;
		ProcessExprOrStmt[AssistExpression::TRUE_LITERAL] = &ExpresionConverterForWatch::ProcessTrueLiteral;
		ProcessExprOrStmt[AssistExpression::FALSE_LITERAL] = &ExpresionConverterForWatch::ProcessFalseLiteral;
		ProcessExprOrStmt[AssistExpression::STRING_LITERAL] =
			&ExpresionConverterForWatch::ProcessStringLiteral;
		ProcessExprOrStmt[AssistExpression::CHARACTER_LITERAL] =
			&ExpresionConverterForWatch::ProcessCharacterLiteral;
		ProcessExprOrStmt[AssistExpression::NULL_LITERAL] = &ExpresionConverterForWatch::ProcessNullLiteral;
		ProcessExprOrStmt[AssistExpression::CLASS_LITERAL] =
			&ExpresionConverterForWatch::ProcessClassLiteral;
		ProcessExprOrStmt[AssistExpression::ARRAY_ACCESS] = &ExpresionConverterForWatch::ProcessArrayAccess;
		ProcessExprOrStmt[AssistExpression::CALL] = &ExpresionConverterForWatch::ProcessMethodInvocation;
		ProcessExprOrStmt[AssistExpression::THIS_EXPRESSION] =
			&ExpresionConverterForWatch::ProcessThisExpression;
		ProcessExprOrStmt[AssistExpression::SUPER_EXPRESSION] =
			&ExpresionConverterForWatch::ProcessSuperExpression;
		ProcessExprOrStmt[AssistExpression::PARENTHESIZED_EXPRESSION] =
			&ExpresionConverterForWatch::ProcessParenthesizedExpression;
		ProcessExprOrStmt[AssistExpression::CLASS_CREATION] =
			&ExpresionConverterForWatch::ProcessClassCreationExpression;
		ProcessExprOrStmt[AssistExpression::ARRAY_CREATION] =
			&ExpresionConverterForWatch::ProcessArrayCreationExpression;
		ProcessExprOrStmt[AssistExpression::POST_UNARY] =
			&ExpresionConverterForWatch::ProcessPostUnaryExpression;
		ProcessExprOrStmt[AssistExpression::PRE_UNARY] =
			&ExpresionConverterForWatch::ProcessPreUnaryExpression;
		ProcessExprOrStmt[AssistExpression::CAST] = &ExpresionConverterForWatch::ProcessCastExpression;
		ProcessExprOrStmt[AssistExpression::BINARY] = &ExpresionConverterForWatch::ProcessBinaryExpression;
		ProcessExprOrStmt[AssistExpression::INSTANCEOF] =
			&ExpresionConverterForWatch::ProcessInstanceofExpression;
		ProcessExprOrStmt[AssistExpression::CONDITIONAL] =
			&ExpresionConverterForWatch::ProcessConditionalExpression;
		ProcessExprOrStmt[AssistExpression::ASSIGNMENT] =
			&ExpresionConverterForWatch::ProcessAssignmentExpression;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessExpression(Expression* expr)
	{
		if (!expr)
			return nullptr;

		if (!expr->symbol)
		{
			// already processed, make sure it was compiler-generated
			assert(!expr->generated);
			return nullptr;
		}
		return (this ->* ProcessExprOrStmt[expr->kind])(expr);
	}

	AssistExpression* ExpresionConverterForWatch::ProcessName(Expression*  expr)
	{
		if(unreachable) return nullptr;
		AstName* name = (AstName*)expr;
		auto assist_expr = new AssistAstName();
		if (name->base_opt) {
			assist_expr->base_opt = (AssistAstName*)ProcessExpression(name->base_opt);
		}
		if(name->identifier_token)
		{
			assist_expr->identifier_token = name->identifier_token->getName();
		}


		return assist_expr;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessFieldAccess(Expression*  expr)
	{
		
		if(unreachable) return nullptr;
		FieldReference* field_access = static_cast<FieldReference*>(expr);
	
		AssistExpression* symbol_in_base = nullptr;
		if (field_access->base) {
			symbol_in_base = ProcessExpression(field_access->base);
			if (!symbol_in_base) {
				unreachable = true;
				return nullptr;
			}
		}
		else
		{
			assert(false);
			unreachable = true;
			return nullptr;
			
		}
		
		auto assist_exp = new AssistFieldReference();
		assist_exp->base = symbol_in_base;
		return assist_exp;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessIntegerLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		IntegerLiteral* int_literal = static_cast<IntegerLiteral*>(expr);

		if(!int_literal->integer_literal_token)
		{
			return nullptr;
		}
		LiteralSymbol* literal = int_literal->integer_literal_token->GetLiteralSymbol();

		if(!literal)
		{
			return nullptr;
		}

		if (!literal->value)
			lookup_env.nameEnv->int_pool.FindOrInsertInt(literal);
		if (literal->value == lookup_env.BadValue())
		{
			unreachable = true;
			return nullptr;
		}
		else
		{
			int_literal->value = literal->value;
			int_literal->symbol = lookup_env.int_type;
		}

		if(!int_literal->symbol)
		{
			return nullptr;
		}

		auto assist_expr = new AssistIntegerLiteral();
		wstring name;
		auto temp = int_literal->symbol->Name();
		if (temp) {
			name.assign(temp, temp + int_literal->symbol->NameLength());
		}
		assist_expr->symbol = new AssistConstantInfo(name);
		if(expr->value)
		{
			assist_expr->session_value = expr->value->Clone();
		}
		
		return assist_expr;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessLongLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;

		LongLiteral* long_literal = (LongLiteral*)expr;
		
		LiteralSymbol* literal =
			long_literal->long_literal_token->GetLiteralSymbol();

		if (!literal)
			return nullptr;

		if (!literal->value)
			lookup_env.nameEnv->long_pool.FindOrInsertLong(literal);
		if (literal->value == lookup_env.BadValue())
		{
			unreachable = true;
			return nullptr;
		}
		else
		{
			long_literal->value = literal->value;
			long_literal->symbol = lookup_env.long_type;
		}
		auto symbol = long_literal->symbol;
		if (!symbol)
			return nullptr;

		auto assist_expr = new AssistLongLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = expr->value->Clone();
		return assist_expr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessFloatLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;

		FloatLiteral* float_literal = (FloatLiteral*)expr;
		LiteralSymbol* literal = float_literal->float_literal_token->GetLiteralSymbol();

		if (!literal->value)
			lookup_env.nameEnv->float_pool.FindOrInsertFloat(literal);
		
		if (literal->value == lookup_env.BadValue())
		{
			unreachable = true;
			return nullptr;
		}
		else
		{
			float_literal->value = literal->value;
			float_literal->symbol = lookup_env.float_type;
		}
	
		AssistFloatLiteral*  assist_expr = new AssistFloatLiteral();
		
		auto symbol = float_literal->symbol;

		if (!symbol)
			return nullptr;

		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = float_literal->value->Clone();
		return assist_expr;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessDoubleLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		DoubleLiteral* double_literal = (DoubleLiteral*)expr;	
	
		LiteralSymbol* literal = double_literal->double_literal_token->GetLiteralSymbol();

		if (!literal->value)
			lookup_env.nameEnv->double_pool.FindOrInsertDouble(literal);
	
		if (literal->value == lookup_env.BadValue())
		{
			unreachable = true;
			return nullptr;
		}
		else
		{
			double_literal->value = literal->value;
			double_literal->symbol = lookup_env.double_type;
		}
		auto symbol = double_literal->symbol;
		if(!symbol)
		{
			return nullptr;
		}
		
		auto assist_expr = new AssistDoubleLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = expr->value->Clone();
		return assist_expr;
	}


	AssistExpression* ExpresionConverterForWatch::ProcessTrueLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
	
		Expression* true_literal = (TrueLiteral*)expr;
		true_literal->value = lookup_env.nameEnv->int_pool.FindOrInsert((int)1);
		true_literal->symbol = lookup_env.boolean_type;

		auto symbol = true_literal->symbol;

		auto assist_expr = new AssistTrueLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		if(expr->value)
		{
			assist_expr->session_value = expr->value->Clone();
		}
		
		return assist_expr;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessFalseLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		Expression* false_literal = (FalseLiteral*)expr;
		false_literal->value = lookup_env.nameEnv->int_pool.FindOrInsert((int)0);
		false_literal->symbol = lookup_env.boolean_type;
		auto symbol = false_literal->symbol;
		if (!symbol)
		{
			return nullptr;
		}
		auto assist_expr = new AssistFalseLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		if(expr->value)
		{
			assist_expr->session_value = expr->value->Clone();
		}
	
		return assist_expr;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessStringLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		StringLiteral* string_literal = (StringLiteral*)expr;
		LiteralSymbol* literal =
			(string_literal->string_literal_token->GetLiteralSymbol());

		if (!literal->value)
			lookup_env.nameEnv->Utf8_pool.FindOrInsertString(literal);
		if (literal->value == lookup_env.BadValue())
		{
			unreachable = true;
			return nullptr;
		}
		else
		{
			string_literal->value = literal->value;
			string_literal->symbol = lookup_env.String();
		}

		auto symbol = string_literal->symbol;
		if (!symbol)
		{
			return nullptr;
		}
		auto assist_expr = new AssistStringLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);

		if( expr->value )
		{
			assist_expr->session_value = expr->value->Clone();
		}
		
		return assist_expr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessCharacterLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		CharLiteral* char_literal = (CharLiteral*)expr;
	
		LiteralSymbol* literal =  char_literal->character_literal_token->GetLiteralSymbol();

		if (!literal->value)
			lookup_env.nameEnv->int_pool.FindOrInsertChar(literal);
		if (literal->value == lookup_env.BadValue())
		{
			unreachable = true;
			return nullptr;
		}
		else
		{
			char_literal->value = literal->value;
			char_literal->symbol = lookup_env.char_type;
		}


		auto symbol = char_literal->symbol;
		if (!symbol)
		{
			return nullptr;
		}
		auto assist_expr = new AssistCharLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = expr->value->Clone();
		return assist_expr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessArrayAccess(Expression*  expr)
	{
		if(unreachable) return nullptr;
		ArrayReference* array_access = (ArrayReference*)expr;
	
		auto assist_expr_in_base = ProcessExpression(array_access->base);
		if (!assist_expr_in_base) {
			unreachable = true;
			return nullptr;
		}
		auto assist_expr_in_expr = ProcessExpression(array_access->expression);
		return new AssistArrayReference(assist_expr_in_base, assist_expr_in_expr);

	}

	AssistExpression* ExpresionConverterForWatch::ProcessMethodInvocation(Expression*  expr)
	{
		unreachable = true;
		return nullptr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessNullLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		return new AssistNullLiteral();
	}
	AssistExpression* ExpresionConverterForWatch::ProcessClassLiteral(Expression*  expr)
	{
		unreachable = true;
		return nullptr;
	}


	AssistExpression* ExpresionConverterForWatch::ProcessThisExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		AstThisExpression* this_expression = (AstThisExpression*)expr;
	
		
		

		auto assist_expr = new AssistAstThisExpression();
		

		auto symbol_info = new AssistVariable(L"this");
		
		return assist_expr;

	}
	AssistAstName* ExpresionConverterForWatch::ConverterTypeNameToName(AstTypeName* typeName)
	{
		if (unreachable) return nullptr;
		auto assist_expr = new AssistAstName();
		if (typeName->base_opt) {
			assist_expr->base_opt = (AssistAstName*)ConverterTypeNameToName(typeName->base_opt);
		}
		if(typeName->name && typeName->name->identifier_token)
		{
			assist_expr->identifier_token = typeName->name->identifier_token->getName();
		}
		

		return assist_expr;
		
	}
	AssistExpression* ExpresionConverterForWatch::ProcessSuperExpression(Expression*  expr)
	{
		
		if (unreachable) return nullptr;
		AstSuperExpression* super_expression = (AstSuperExpression*)expr;

		auto assist_expr = new AssistAstSuperExpression();
		if(super_expression->base_opt)
			assist_expr->base_opt  = ConverterTypeNameToName(super_expression->base_opt);
	
		return assist_expr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessParenthesizedExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		AstParenthesizedExpression* parenthesized =
			(AstParenthesizedExpression*)expr;
	
		auto expre_in_Parenthesized = ProcessExpression(parenthesized->expression);
		if(!expre_in_Parenthesized)
		{
			unreachable = true;
			return nullptr;
		}
		auto assist_expr = new AssistAstParenthesizedExpression();
		assist_expr->expression = expre_in_Parenthesized;
		return assist_expr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessClassCreationExpression(Expression*  expr)
	{
		unreachable = true;// ???? ??????????????????????debug????????????
		return nullptr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessArrayCreationExpression(Expression*  expr)
	{
		unreachable = true;// ???? ??????????????????????debug????????????
		return nullptr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessPostUnaryExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;

		
		AstPostUnaryExpression* postfix_expression =
			(AstPostUnaryExpression*)expr;

		
	
		auto exp_in_epression = ProcessExpression(postfix_expression->expression);
		if (!exp_in_epression) {
			unreachable = true;
			return nullptr;
		}

		auto assist_expr = new AssistAstPostUnaryExpression(postfix_expression->Tag());
		assist_expr->expression = exp_in_epression;
		return assist_expr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessPreUnaryExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		AstPreUnaryExpression* prefix_expression = (AstPreUnaryExpression*)expr;

	
		auto exp_in_epression = ProcessExpression(prefix_expression->expression);
		if (!exp_in_epression) {
			unreachable = true;
			return nullptr;
		}

		if (prefix_expression->IsConstant())
		{
			auto assist_expr = new AssistAstPreUnaryExpression(prefix_expression->Tag());
			assist_expr->symbol = new AssistConstantInfo(L"");
			if(expr->value)
			{
				assist_expr->session_value = expr->value->Clone();
			}
			
			return assist_expr;
		}


		auto assist_expr = new AssistAstPreUnaryExpression(prefix_expression->Tag());
		assist_expr->expression = exp_in_epression;
		return assist_expr;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessCastExpression(Expression*  expr)
	{
	
		if(unreachable) return nullptr;
		CastExpression* cast_expression = (CastExpression*)expr;
	
		auto assist_expr = new AssistCastExpression();
		auto exp_in_epression = ProcessExpression(cast_expression->expression);
		if (!exp_in_epression) {
			unreachable = true;
			return nullptr;
		}
		assist_expr->expression = exp_in_epression;
		return assist_expr;
	}

	AssistExpression* ExpresionConverterForWatch::ProcessBinaryExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		BinaryExpression* binary_expression = (BinaryExpression*)expr;
		
		auto left_expr = ProcessExpression(binary_expression->left_expression);
		if(!left_expr)
		{
			unreachable = true;
			return nullptr;
		}

		if (binary_expression->IsConstant())
		{
			auto assist_expr = new AssistBinaryExpression(binary_expression->Tag());
			assist_expr->symbol = new AssistConstantInfo(L"");
			if(expr->value)
			{
				assist_expr->session_value = expr->value->Clone();
			}
		
			return assist_expr;
		}


		auto right_expr = ProcessExpression(binary_expression->right_expression);
		if (!right_expr){
			delete left_expr;
			unreachable = true;
			return nullptr;
		}

		auto assist_expr = new AssistBinaryExpression(binary_expression->Tag());
		assist_expr->left_expression = left_expr;
		assist_expr->right_expression = right_expr;
		return assist_expr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessInstanceofExpression(Expression*  expr)
	{
		unreachable = true;
		return nullptr;
	}
	AssistExpression* ExpresionConverterForWatch::ProcessConditionalExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		ConditionalExpression* conditional_expression =
			(ConditionalExpression*)expr;
		
		auto test_expr = ProcessExpression(conditional_expression->test_expression);
		if(!test_expr)
		{
			unreachable = true;
			return nullptr;
		}

		
		auto false_expr = ProcessExpression(conditional_expression->false_expression);
		auto true_expr = ProcessExpression(conditional_expression->true_expression);

		if(!false_expr && !true_expr){
			delete test_expr;
			return nullptr;
		}

		auto assist_expr = new AssistConditionalExpression();
		assist_expr->test_expression = test_expr;
		assist_expr->false_expression = false_expr;
		assist_expr->true_expression = true_expr;
		return assist_expr;


	}
	AssistExpression* ExpresionConverterForWatch::ProcessAssignmentExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;

		AstAssignmentExpression* assignment_expression =
			(AstAssignmentExpression*)expr;
		

		AssistExpression* left_hand_side = ProcessExpression(assignment_expression->left_hand_side);
		if(!left_hand_side){
			unreachable = true;
			return nullptr;
		}
		AssistExpression* expression  = ProcessExpression(assignment_expression->expression);
		auto tag = assignment_expression->Tag();
		auto assist_expr = new AssistAstAssignmentExpression(tag);
		assist_expr->left_hand_side = left_hand_side;
		assist_expr->expression = expression;


		if(left_hand_side->IsConstant())
		{
			assist_expr->session_value = left_hand_side->session_value;
			left_hand_side->session_value = nullptr;
		}
		else if(expression && expression->IsConstant()){
			assist_expr->session_value = expression->session_value;
			expression->session_value = nullptr;
		}
		return assist_expr;

	}

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block




