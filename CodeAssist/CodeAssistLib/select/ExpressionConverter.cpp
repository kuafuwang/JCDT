#include "stdafx.h"
#include <CodeAssistLib/select/ExpressionConverter.h>
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
#include <CodeAssistLib/ast/SymbolInfo.h>

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	AssistVariable* convertToAssistVariable(VariableSymbol* variable_symbol)
	{
	
		wstring varName;
		const wchar_t*  temp = variable_symbol->Name();
		if (temp) {
			varName.assign(temp, temp + variable_symbol->NameLength());
		}

		auto  assist_variable = new AssistVariable(varName);
		assist_variable->SetFlags(variable_symbol->Flags());

		auto _type = variable_symbol->Type();
		if (_type) {
			assist_variable->m_strTypeDesc = _type->getNameString();
		}

		auto  contain_type = variable_symbol->ContainingType();
		if (contain_type){
			assist_variable->claName = contain_type->readableName('/');
			auto pkg = contain_type->ContainingPackage();
			if (pkg) {
				assist_variable->pkgName = pkg->PackageNameString();
			}	
		}

		
		assist_variable->SetFlags(variable_symbol->Flags());
		return assist_variable;
	}

	AssistExpression* ExpressionConverter::ProcessSimpleNameVariable(VariableSymbol* variable_symbol)
	{
		if (!variable_symbol) {
			return nullptr;
		}
		auto assist_expr = new AssistAstName();
		if (variable_symbol->initial_value) {
			assist_expr->session_value = variable_symbol->initial_value->Clone();
		}

		assist_expr->symbol = convertToAssistVariable(variable_symbol);

		return assist_expr;
	}
	ExpressionConverter::ExpressionConverter(LookupEnvironment& _env)
	:lookup_env(_env)
	{
		// ExpressProcess processing of expressions.
		ProcessExprOrStmt[AssistExpression::NAME] = &ExpressionConverter::ProcessName;
		ProcessExprOrStmt[AssistExpression::DOT] = &ExpressionConverter::ProcessFieldAccess;
		ProcessExprOrStmt[AssistExpression::INTEGER_LITERAL] =
			&ExpressionConverter::ProcessIntegerLiteral;
		ProcessExprOrStmt[AssistExpression::LONG_LITERAL] = &ExpressionConverter::ProcessLongLiteral;
		ProcessExprOrStmt[AssistExpression::FLOAT_LITERAL] = &ExpressionConverter::ProcessFloatLiteral;
		ProcessExprOrStmt[AssistExpression::DOUBLE_LITERAL] =
			&ExpressionConverter::ProcessDoubleLiteral;
		ProcessExprOrStmt[AssistExpression::TRUE_LITERAL] = &ExpressionConverter::ProcessTrueLiteral;
		ProcessExprOrStmt[AssistExpression::FALSE_LITERAL] = &ExpressionConverter::ProcessFalseLiteral;
		ProcessExprOrStmt[AssistExpression::STRING_LITERAL] =
			&ExpressionConverter::ProcessStringLiteral;
		ProcessExprOrStmt[AssistExpression::CHARACTER_LITERAL] =
			&ExpressionConverter::ProcessCharacterLiteral;
		ProcessExprOrStmt[AssistExpression::NULL_LITERAL] = &ExpressionConverter::ProcessNullLiteral;
		ProcessExprOrStmt[AssistExpression::CLASS_LITERAL] =
			&ExpressionConverter::ProcessClassLiteral;
		ProcessExprOrStmt[AssistExpression::ARRAY_ACCESS] = &ExpressionConverter::ProcessArrayAccess;
		ProcessExprOrStmt[AssistExpression::CALL] = &ExpressionConverter::ProcessMethodInvocation;
		ProcessExprOrStmt[AssistExpression::THIS_EXPRESSION] =
			&ExpressionConverter::ProcessThisExpression;
		ProcessExprOrStmt[AssistExpression::SUPER_EXPRESSION] =
			&ExpressionConverter::ProcessSuperExpression;
		ProcessExprOrStmt[AssistExpression::PARENTHESIZED_EXPRESSION] =
			&ExpressionConverter::ProcessParenthesizedExpression;
		ProcessExprOrStmt[AssistExpression::CLASS_CREATION] =
			&ExpressionConverter::ProcessClassCreationExpression;
		ProcessExprOrStmt[AssistExpression::ARRAY_CREATION] =
			&ExpressionConverter::ProcessArrayCreationExpression;
		ProcessExprOrStmt[AssistExpression::POST_UNARY] =
			&ExpressionConverter::ProcessPostUnaryExpression;
		ProcessExprOrStmt[AssistExpression::PRE_UNARY] =
			&ExpressionConverter::ProcessPreUnaryExpression;
		ProcessExprOrStmt[AssistExpression::CAST] = &ExpressionConverter::ProcessCastExpression;
		ProcessExprOrStmt[AssistExpression::BINARY] = &ExpressionConverter::ProcessBinaryExpression;
		ProcessExprOrStmt[AssistExpression::INSTANCEOF] =
			&ExpressionConverter::ProcessInstanceofExpression;
		ProcessExprOrStmt[AssistExpression::CONDITIONAL] =
			&ExpressionConverter::ProcessConditionalExpression;
		ProcessExprOrStmt[AssistExpression::ASSIGNMENT] =
			&ExpressionConverter::ProcessAssignmentExpression;
	}

	AssistExpression* ExpressionConverter::ProcessName(Expression*  expr)
	{
		if(unreachable) return nullptr;
		AstName* name = (AstName*)expr;
		if (name->symbol == lookup_env.no_type)
		{
			unreachable = true;
			return nullptr;
		}
		
		VariableSymbol* variable_symbol = VariableSymbol::VariableCast(name->symbol);;
		if (!variable_symbol){
			unreachable = true;
			return nullptr;
		}

		auto assist_expr = new AssistAstName();
		if (variable_symbol->initial_value) {
			assist_expr->session_value = variable_symbol->initial_value->Clone();
		}

		assist_expr->symbol = convertToAssistVariable(variable_symbol);
		if (name->base_opt) {
			assist_expr->base_opt = (AssistAstName*)ProcessExpression(name->base_opt);
		}
		return assist_expr;
	}

	AssistExpression* ExpressionConverter::ProcessFieldAccess(Expression*  expr)
	{
		
		if(unreachable) return nullptr;
		FieldReference* field_access = static_cast<FieldReference*>(expr);
		if (field_access->symbol == lookup_env.no_type){
			unreachable = true;
			return nullptr;
		}

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
		auto symbol = VariableSymbol::VariableCast(field_access->symbol);
		if (!symbol){
			delete symbol_in_base;
			return nullptr;
		}
			
		auto assist_exp = new AssistFieldReference();
		assist_exp->base = symbol_in_base;
		assist_exp->symbol = convertToAssistVariable(symbol);
		return assist_exp;
	}

	AssistExpression* ExpressionConverter::ProcessIntegerLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		IntegerLiteral* int_literal = static_cast<IntegerLiteral*>(expr);
	
		if (!int_literal->symbol || int_literal->symbol == lookup_env.no_type ||  !int_literal->IsConstant()){
			unreachable = true;
			return nullptr;
		}

		auto assist_expr = new AssistIntegerLiteral();
		wstring name;
		auto temp = int_literal->symbol->Name();
		if (temp) {
			name.assign(temp, temp + int_literal->symbol->NameLength());
		}
		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = expr->value->Clone();
		return assist_expr;
	}

	AssistExpression* ExpressionConverter::ProcessLongLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;

		LongLiteral* long_literal = (LongLiteral*)expr;
		auto symbol = long_literal->symbol;

		if (!symbol || symbol == lookup_env.no_type || !long_literal->IsConstant()) {
			unreachable = true;
			return nullptr;
		}
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
	AssistExpression* ExpressionConverter::ProcessFloatLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		FloatLiteral* float_literal = (FloatLiteral*)expr;
		auto symbol = float_literal->symbol;

		if (symbol == lookup_env.no_type || !float_literal->IsConstant()) {
			unreachable = true;
			return nullptr;
		}
	
		AssistFloatLiteral*  assist_expr = new AssistFloatLiteral();
		

		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = float_literal->value->Clone();
		return assist_expr;
	}

	AssistExpression* ExpressionConverter::ProcessDoubleLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		DoubleLiteral* double_literal = (DoubleLiteral*)expr;	
		
		auto symbol = double_literal->symbol;

		if (!symbol || symbol == lookup_env.no_type || !double_literal->IsConstant()) {
			unreachable = true;
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


	AssistExpression* ExpressionConverter::ProcessTrueLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
	
		Expression* true_literal = (TrueLiteral*)expr;

		auto symbol = true_literal->symbol;

		auto assist_expr = new AssistTrueLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = expr->value->Clone();
		return assist_expr;
	}

	AssistExpression* ExpressionConverter::ProcessFalseLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		Expression* true_literal = (FalseLiteral*)expr;

		auto symbol = true_literal->symbol;

		auto assist_expr = new AssistFalseLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = expr->value->Clone();
		return assist_expr;
	}

	AssistExpression* ExpressionConverter::ProcessStringLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		StringLiteral* string_literal = (StringLiteral*)expr;
		if (string_literal->symbol == lookup_env.no_type ||  !string_literal->IsConstant()) {
			unreachable = true;
			return nullptr;
		}

		auto symbol = string_literal->symbol;

		auto assist_expr = new AssistStringLiteral();
		wstring name;
		auto temp = symbol->Name();
		if (temp) {
			name.assign(temp, temp + symbol->NameLength());
		}

		assist_expr->symbol = new AssistConstantInfo(name);
		assist_expr->session_value = expr->value->Clone();
		return assist_expr;
	}
	AssistExpression* ExpressionConverter::ProcessCharacterLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		CharLiteral* char_literal = (CharLiteral*)expr;
		if (char_literal->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}

		auto symbol = char_literal->symbol;

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
	AssistExpression* ExpressionConverter::ProcessArrayAccess(Expression*  expr)
	{
		if(unreachable) return nullptr;
		ArrayReference* array_access = (ArrayReference*)expr;
		if (array_access->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}
		auto assist_expr_in_base = ProcessExpression(array_access->base);
		if (!assist_expr_in_base) {
			unreachable = true;
			return nullptr;
		}
		auto assist_expr_in_expr = ProcessExpression(array_access->expression);
		return new AssistArrayReference(assist_expr_in_base, assist_expr_in_expr);

	}

	AssistExpression* ExpressionConverter::ProcessMethodInvocation(Expression*  expr)
	{
		unreachable = true;
		return nullptr;
	}
	AssistExpression* ExpressionConverter::ProcessNullLiteral(Expression*  expr)
	{
		if(unreachable) return nullptr;
		return new AssistNullLiteral();
	}
	AssistExpression* ExpressionConverter::ProcessClassLiteral(Expression*  expr)
	{
		unreachable = true;
		return nullptr;
	}


	AssistExpression* ExpressionConverter::ProcessThisExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		AstThisExpression* this_expression = (AstThisExpression*)expr;
	
		if (!expr->symbol || expr->symbol == lookup_env.no_type ) {
			unreachable = true;
			return nullptr;
		}
		auto symbol = TypeSymbol::TypeCast(expr->symbol);
		if(!symbol){
			unreachable = true;
			return nullptr;
		}

		auto assist_expr = new AssistAstThisExpression();
		wstring typeName = symbol->readableName('/');
		

		auto symbol_info = new AssistVariable(L"this");
		symbol_info->claName = typeName;
		auto pkg_symbol =  symbol->ContainingPackage();
		wstring pkgName;
		if(pkg_symbol)
		{
			auto pkg_temp = pkg_symbol->PackageName();
			if(pkg_temp){
				pkgName.assign(pkg_temp, pkg_temp + pkg_symbol->PackageNameLength());
			}	
		}
		symbol_info->m_strTypeDesc = typeName;
		symbol_info->pkgName = pkgName;
		assist_expr->symbol = symbol_info;
		return assist_expr;

	}
	AssistExpression* ExpressionConverter::ProcessSuperExpression(Expression*  expr)
	{
		
		if (unreachable) return nullptr;
		AstSuperExpression* super_expression = (AstSuperExpression*)expr;

		if (!expr->symbol || expr->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}
		auto symbol = TypeSymbol::TypeCast(expr->symbol);
		if (!symbol) {
			unreachable = true;
			return nullptr;
		}

		auto assist_expr = new AssistAstSuperExpression();
		wstring typeName= symbol->readableName('/');
	

		auto symbol_info = new AssistVariable(L"super");
		symbol_info->claName = typeName;
		
		auto pkg_symbol = symbol->ContainingPackage();
		wstring pkgName;
		if (pkg_symbol){
			 pkgName = pkg_symbol->PackageNameString();	
		}
		symbol_info->m_strTypeDesc = typeName;
		symbol_info->pkgName = pkgName;
		assist_expr->symbol = symbol_info;
		return assist_expr;


	}
	AssistExpression* ExpressionConverter::ProcessParenthesizedExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		AstParenthesizedExpression* parenthesized =
			(AstParenthesizedExpression*)expr;
		if (parenthesized->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}
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
	AssistExpression* ExpressionConverter::ProcessClassCreationExpression(Expression*  expr)
	{
		unreachable = true;// 属于 类似函数调用，无法查询debug的时候的值。
		return nullptr;
	}
	AssistExpression* ExpressionConverter::ProcessArrayCreationExpression(Expression*  expr)
	{
		unreachable = true;// 属于 类似函数调用，无法查询debug的时候的值。
		return nullptr;
	}
	AssistExpression* ExpressionConverter::ProcessPostUnaryExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;

		
		AstPostUnaryExpression* postfix_expression =
			(AstPostUnaryExpression*)expr;

		if (postfix_expression->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}
	
		auto exp_in_epression = ProcessExpression(postfix_expression->expression);
		if (!exp_in_epression) {
			unreachable = true;
			return nullptr;
		}

		auto assist_expr = new AssistAstPostUnaryExpression(postfix_expression->Tag());
		assist_expr->expression = exp_in_epression;
		return assist_expr;
	}
	AssistExpression* ExpressionConverter::ProcessPreUnaryExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		AstPreUnaryExpression* prefix_expression = (AstPreUnaryExpression*)expr;

		if (prefix_expression->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}

		if(prefix_expression->IsConstant())
		{
			auto assist_expr = new AssistAstPreUnaryExpression(prefix_expression->Tag());
			assist_expr->symbol = new AssistConstantInfo(L"");
			assist_expr->session_value = expr->value->Clone();
			return assist_expr;
		}

		auto exp_in_epression = ProcessExpression(prefix_expression->expression);
		if (!exp_in_epression) {
			unreachable = true;
			return nullptr;
		}
		auto assist_expr = new AssistAstPreUnaryExpression(prefix_expression->Tag());
		assist_expr->expression = exp_in_epression;
		return assist_expr;
	}

	AssistExpression* ExpressionConverter::ProcessCastExpression(Expression*  expr)
	{
	
		if(unreachable) return nullptr;
		CastExpression* cast_expression = (CastExpression*)expr;
		if (cast_expression->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}
		auto assist_expr = new AssistCastExpression();
		auto exp_in_epression = ProcessExpression(cast_expression->expression);
		if (!exp_in_epression) {
			unreachable = true;
			return nullptr;
		}
		assist_expr->expression = exp_in_epression;
		return assist_expr;
	}

	AssistExpression* ExpressionConverter::ProcessBinaryExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		BinaryExpression* binary_expression = (BinaryExpression*)expr;
		if (binary_expression->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}

		if (binary_expression->IsConstant())
		{
			auto assist_expr = new AssistBinaryExpression(binary_expression->Tag());
			assist_expr->symbol = new AssistConstantInfo(L"");
			assist_expr->session_value = expr->value->Clone();
			return assist_expr;
		}

		auto left_expr = ProcessExpression(binary_expression->left_expression);
		if(!left_expr)
		{
			unreachable = true;
			return nullptr;
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
	AssistExpression* ExpressionConverter::ProcessInstanceofExpression(Expression*  expr)
	{
		unreachable = true;
		return nullptr;
	}
	AssistExpression* ExpressionConverter::ProcessConditionalExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;
		ConditionalExpression* conditional_expression =
			(ConditionalExpression*)expr;
		if (conditional_expression->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}
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
	AssistExpression* ExpressionConverter::ProcessAssignmentExpression(Expression*  expr)
	{
		if(unreachable) return nullptr;

		AstAssignmentExpression* assignment_expression =
			(AstAssignmentExpression*)expr;
		if (assignment_expression->symbol == lookup_env.no_type) {
			unreachable = true;
			return nullptr;
		}

		AssistExpression* left_hand_side = ProcessExpression(assignment_expression->left_hand_side);
		if(left_hand_side){
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
		else if(expression->IsConstant()){
			assist_expr->session_value = expression->session_value;
			expression->session_value = nullptr;
		}
		return assist_expr;

	}

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block




