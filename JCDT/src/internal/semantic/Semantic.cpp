#include "JCDT_Lib/internal/semantic/semantic.h"
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/impl/option.h>

namespace Jikes { // Open namespace Jikes block

				  // The constructor
	Semantic::Semantic(LookupEnvironment& control_, FileSymbol* file_symbol_)
		: lookup_env(control_),
		source_file_symbol(file_symbol_),
		lex_stream(file_symbol_->lex_stream),
		compilation_unit(file_symbol_ -> compilation_unit),
		directory_symbol(file_symbol_ -> directory_symbol),
		return_code(0),

		this_package(file_symbol_ -> package)
	{
#ifdef JIKES_DEBUG
		int i;
		for (i = 0; i < Ast::_num_expr_or_stmt_kinds; i++)
		{
			ProcessExprOrStmt[i] = &Semantic::ProcessInvalidKind;
			DefiniteStmt[i] = &Semantic::DefiniteInvalidStatement;
		}
		for (i = 0; i < Ast::_num_expression_kinds; i++)
			DefiniteExpr[i] = &Semantic::DefiniteInvalidExpression;
		for (i = 0; i < PreUnaryExpressionEnum::_num_kinds; i++)
		{
			ProcessPreUnaryExpr[i] = NULL;
			DefinitePreUnaryExpr[i] = NULL;
		}
		for (i = 0; i < BinaryExpressionEnum::_num_kinds; i++)
		{
			ProcessBinaryExpr[i] = NULL;
			DefiniteBinaryExpr[i] = NULL;
		}
#endif // JIKES_DEBUG
		// Semantic processing of expressions.
		ProcessExprOrStmt[Ast::NAME] = &Semantic::ProcessName;
		ProcessExprOrStmt[Ast::DOT] = &Semantic::ProcessFieldAccess;
		ProcessExprOrStmt[Ast::INTEGER_LITERAL] =
			&Semantic::ProcessIntegerLiteral;
		ProcessExprOrStmt[Ast::LONG_LITERAL] = &Semantic::ProcessLongLiteral;
		ProcessExprOrStmt[Ast::FLOAT_LITERAL] = &Semantic::ProcessFloatLiteral;
		ProcessExprOrStmt[Ast::DOUBLE_LITERAL] =
			&Semantic::ProcessDoubleLiteral;
		ProcessExprOrStmt[Ast::TRUE_LITERAL] = &Semantic::ProcessTrueLiteral;
		ProcessExprOrStmt[Ast::FALSE_LITERAL] = &Semantic::ProcessFalseLiteral;
		ProcessExprOrStmt[Ast::STRING_LITERAL] =
			&Semantic::ProcessStringLiteral;
		ProcessExprOrStmt[Ast::CHARACTER_LITERAL] =
			&Semantic::ProcessCharacterLiteral;
		ProcessExprOrStmt[Ast::NULL_LITERAL] = &Semantic::ProcessNullLiteral;
		ProcessExprOrStmt[Ast::CLASS_LITERAL] =
			&Semantic::ProcessClassLiteral;
		ProcessExprOrStmt[Ast::ARRAY_ACCESS] = &Semantic::ProcessArrayAccess;
		ProcessExprOrStmt[Ast::CALL] = &Semantic::ProcessMethodInvocation;
		ProcessExprOrStmt[Ast::THIS_EXPRESSION] =
			&Semantic::ProcessThisExpression;
		ProcessExprOrStmt[Ast::SUPER_EXPRESSION] =
			&Semantic::ProcessSuperExpression;
		ProcessExprOrStmt[Ast::PARENTHESIZED_EXPRESSION] =
			&Semantic::ProcessParenthesizedExpression;
		ProcessExprOrStmt[Ast::CLASS_CREATION] =
			&Semantic::ProcessClassCreationExpression;
		ProcessExprOrStmt[Ast::ARRAY_CREATION] =
			&Semantic::ProcessArrayCreationExpression;
		ProcessExprOrStmt[Ast::POST_UNARY] =
			&Semantic::ProcessPostUnaryExpression;
		ProcessExprOrStmt[Ast::PRE_UNARY] =
			&Semantic::ProcessPreUnaryExpression;
		ProcessExprOrStmt[Ast::CAST] = &Semantic::ProcessCastExpression;
		ProcessExprOrStmt[Ast::BINARY] = &Semantic::ProcessBinaryExpression;
		ProcessExprOrStmt[Ast::INSTANCEOF] =
			&Semantic::ProcessInstanceofExpression;
		ProcessExprOrStmt[Ast::CONDITIONAL] =
			&Semantic::ProcessConditionalExpression;
		ProcessExprOrStmt[Ast::ASSIGNMENT] =
			&Semantic::ProcessAssignmentExpression;
		// Semantic processing of statements.
		ProcessExprOrStmt[Ast::LOCAL_CLASS] =
			&Semantic::ProcessClassDeclaration;
		ProcessExprOrStmt[Ast::THIS_CALL] = &Semantic::ProcessInvalidKind;
		ProcessExprOrStmt[Ast::SUPER_CALL] = &Semantic::ProcessInvalidKind;
		ProcessExprOrStmt[Ast::BLOCK] = &Semantic::ProcessBlock;
		ProcessExprOrStmt[Ast::LOCAL_VARIABLE_DECLARATION] =
			&Semantic::ProcessLocalVariableStatement;
		ProcessExprOrStmt[Ast::IF] = &Semantic::ProcessIfStatement;
		ProcessExprOrStmt[Ast::EMPTY_STATEMENT] =
			&Semantic::ProcessEmptyStatement;
		ProcessExprOrStmt[Ast::EXPRESSION_STATEMENT] =
			&Semantic::ProcessExpressionStatement;
		ProcessExprOrStmt[Ast::SWITCH] = &Semantic::ProcessSwitchStatement;
		ProcessExprOrStmt[Ast::WHILE] = &Semantic::ProcessWhileStatement;
		ProcessExprOrStmt[Ast::DO] = &Semantic::ProcessDoStatement;
		ProcessExprOrStmt[Ast::FOR] = &Semantic::ProcessForStatement;
		ProcessExprOrStmt[Ast::FOREACH] = &Semantic::ProcessForeachStatement;
		ProcessExprOrStmt[Ast::BREAK] = &Semantic::ProcessBreakStatement;
		ProcessExprOrStmt[Ast::CONTINUE] = &Semantic::ProcessContinueStatement;
		ProcessExprOrStmt[Ast::RETURN] = &Semantic::ProcessReturnStatement;
		ProcessExprOrStmt[Ast::THROW] = &Semantic::ProcessThrowStatement;
		ProcessExprOrStmt[Ast::SYNCHRONIZED_STATEMENT] =
			&Semantic::ProcessSynchronizedStatement;
		ProcessExprOrStmt[Ast::ASSERT_KIND] = &Semantic::ProcessAssertStatement;
		ProcessExprOrStmt[Ast::TRY_KIND] = &Semantic::ProcessTryStatement;

		//
		// Check statements for definite assignment.
		//
		// This and super calls are not normally statements, so
		// DefiniteThisCall and DefiniteSuperCall should be invoked manually
		// from DefiniteConstructorBody rather than automatically by
		// DefiniteStatement. Therefore, they are diverted to the no-op
		// DefiniteDefaultStatement.
		//
		DefiniteStmt[Ast::THIS_CALL] = &Semantic::DefiniteDefaultStatement;
		DefiniteStmt[Ast::SUPER_CALL] = &Semantic::DefiniteDefaultStatement;
		DefiniteStmt[Ast::BLOCK] = &Semantic::DefiniteBlock;
		DefiniteStmt[Ast::LOCAL_CLASS] =
			&Semantic::DefiniteLocalClassStatement;
		DefiniteStmt[Ast::LOCAL_VARIABLE_DECLARATION] =
			&Semantic::DefiniteLocalVariableStatement;
		DefiniteStmt[Ast::IF] = &Semantic::DefiniteIfStatement;
		DefiniteStmt[Ast::EMPTY_STATEMENT] =
			&Semantic::DefiniteDefaultStatement;
		DefiniteStmt[Ast::EXPRESSION_STATEMENT] =
			&Semantic::DefiniteExpressionStatement;
		DefiniteStmt[Ast::SWITCH] = &Semantic::DefiniteSwitchStatement;
		DefiniteStmt[Ast::WHILE] = &Semantic::DefiniteWhileStatement;
		DefiniteStmt[Ast::DO] = &Semantic::DefiniteDoStatement;
		DefiniteStmt[Ast::FOR] = &Semantic::DefiniteForStatement;
		DefiniteStmt[Ast::FOREACH] = &Semantic::DefiniteForeachStatement;
		DefiniteStmt[Ast::BREAK] = &Semantic::DefiniteBreakStatement;
		DefiniteStmt[Ast::CONTINUE] = &Semantic::DefiniteContinueStatement;
		DefiniteStmt[Ast::RETURN] = &Semantic::DefiniteReturnStatement;
		DefiniteStmt[Ast::THROW] = &Semantic::DefiniteThrowStatement;
		DefiniteStmt[Ast::SYNCHRONIZED_STATEMENT] =
			&Semantic::DefiniteSynchronizedStatement;
		DefiniteStmt[Ast::ASSERT_KIND] = &Semantic::DefiniteAssertStatement;
		DefiniteStmt[Ast::TRY_KIND] = &Semantic::DefiniteTryStatement;

		DefiniteExpr[Ast::NAME] = &Semantic::DefiniteName;
		DefiniteExpr[Ast::DOT] = &Semantic::DefiniteFieldAccess;
		DefiniteExpr[Ast::ARRAY_ACCESS] = &Semantic::DefiniteArrayAccess;
		DefiniteExpr[Ast::CALL] = &Semantic::DefiniteMethodInvocation;
		DefiniteExpr[Ast::PARENTHESIZED_EXPRESSION] =
			&Semantic::DefiniteParenthesizedExpression;
		DefiniteExpr[Ast::CLASS_CREATION] =
			&Semantic::DefiniteClassCreationExpression;
		DefiniteExpr[Ast::ARRAY_CREATION] =
			&Semantic::DefiniteArrayCreationExpression;
		DefiniteExpr[Ast::POST_UNARY] = &Semantic::DefinitePostUnaryExpression;
		DefiniteExpr[Ast::PRE_UNARY] = &Semantic::DefinitePreUnaryExpression;
		DefiniteExpr[Ast::CAST] = &Semantic::DefiniteCastExpression;
		DefiniteExpr[Ast::BINARY] = &Semantic::DefiniteBinaryExpression;
		DefiniteExpr[Ast::INSTANCEOF] =
			&Semantic::DefiniteInstanceofExpression;
		DefiniteExpr[Ast::CONDITIONAL] =
			&Semantic::DefiniteConditionalExpression;
		DefiniteExpr[Ast::ASSIGNMENT] =
			&Semantic::DefiniteAssignmentExpression;
		DefiniteExpr[Ast::INTEGER_LITERAL] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::LONG_LITERAL] = &Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::FLOAT_LITERAL] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::DOUBLE_LITERAL] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::TRUE_LITERAL] = &Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::FALSE_LITERAL] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::STRING_LITERAL] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::CHARACTER_LITERAL] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::NULL_LITERAL] = &Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::CLASS_LITERAL] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::THIS_EXPRESSION] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::SUPER_EXPRESSION] =
			&Semantic::DefiniteDefaultExpression;
		DefiniteExpr[Ast::TYPE] = &Semantic::DefiniteDefaultExpression;

		DefiniteBinaryExpr[BinaryExpressionEnum::PLUS] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::LEFT_SHIFT] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::RIGHT_SHIFT] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::UNSIGNED_RIGHT_SHIFT] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::LESS] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::GREATER] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::LESS_EQUAL] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::GREATER_EQUAL] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::AND] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::XOR] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::IOR] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::EQUAL_EQUAL] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::NOT_EQUAL] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::AND_AND] =
			&Semantic::DefiniteAND_AND;
		DefiniteBinaryExpr[BinaryExpressionEnum::OR_OR] =
			&Semantic::DefiniteOR_OR;
		DefiniteBinaryExpr[BinaryExpressionEnum::STAR] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::MINUS] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::SLASH] =
			&Semantic::DefiniteDefaultBinaryExpression;
		DefiniteBinaryExpr[BinaryExpressionEnum::MOD] =
			&Semantic::DefiniteDefaultBinaryExpression;

		DefinitePreUnaryExpr[PreUnaryExpressionEnum::PLUS] =
			&Semantic::DefiniteDefaultPreUnaryExpression;
		DefinitePreUnaryExpr[PreUnaryExpressionEnum::MINUS] =
			&Semantic::DefiniteDefaultPreUnaryExpression;
		DefinitePreUnaryExpr[PreUnaryExpressionEnum::TWIDDLE] =
			&Semantic::DefiniteDefaultPreUnaryExpression;
		DefinitePreUnaryExpr[PreUnaryExpressionEnum::NOT] =
			&Semantic::DefiniteNOT;
		DefinitePreUnaryExpr[PreUnaryExpressionEnum::PLUSPLUS] =
			&Semantic::DefinitePLUSPLUSOrMINUSMINUS;
		DefinitePreUnaryExpr[PreUnaryExpressionEnum::MINUSMINUS] =
			&Semantic::DefinitePLUSPLUSOrMINUSMINUS;

		ProcessBinaryExpr[BinaryExpressionEnum::PLUS] = &Semantic::ProcessPLUS;
		ProcessBinaryExpr[BinaryExpressionEnum::LEFT_SHIFT] =
			&Semantic::ProcessLEFT_SHIFT;
		ProcessBinaryExpr[BinaryExpressionEnum::RIGHT_SHIFT] =
			&Semantic::ProcessRIGHT_SHIFT;
		ProcessBinaryExpr[BinaryExpressionEnum::UNSIGNED_RIGHT_SHIFT] =
			&Semantic::ProcessUNSIGNED_RIGHT_SHIFT;
		ProcessBinaryExpr[BinaryExpressionEnum::LESS] = &Semantic::ProcessLESS;
		ProcessBinaryExpr[BinaryExpressionEnum::GREATER] =
			&Semantic::ProcessGREATER;
		ProcessBinaryExpr[BinaryExpressionEnum::LESS_EQUAL] =
			&Semantic::ProcessLESS_EQUAL;
		ProcessBinaryExpr[BinaryExpressionEnum::GREATER_EQUAL] =
			&Semantic::ProcessGREATER_EQUAL;
		ProcessBinaryExpr[BinaryExpressionEnum::AND] = &Semantic::ProcessAND;
		ProcessBinaryExpr[BinaryExpressionEnum::XOR] = &Semantic::ProcessXOR;
		ProcessBinaryExpr[BinaryExpressionEnum::IOR] = &Semantic::ProcessIOR;
		ProcessBinaryExpr[BinaryExpressionEnum::AND_AND] =
			&Semantic::ProcessAND_AND;
		ProcessBinaryExpr[BinaryExpressionEnum::OR_OR] =
			&Semantic::ProcessOR_OR;
		ProcessBinaryExpr[BinaryExpressionEnum::EQUAL_EQUAL] =
			&Semantic::ProcessEQUAL_EQUAL;
		ProcessBinaryExpr[BinaryExpressionEnum::NOT_EQUAL] =
			&Semantic::ProcessNOT_EQUAL;
		ProcessBinaryExpr[BinaryExpressionEnum::STAR] = &Semantic::ProcessSTAR;
		ProcessBinaryExpr[BinaryExpressionEnum::MINUS] =
			&Semantic::ProcessMINUS;
		ProcessBinaryExpr[BinaryExpressionEnum::SLASH] =
			&Semantic::ProcessSLASH;
		ProcessBinaryExpr[BinaryExpressionEnum::MOD] = &Semantic::ProcessMOD;

		ProcessPreUnaryExpr[PreUnaryExpressionEnum::PLUS] =
			&Semantic::ProcessPLUS;
		ProcessPreUnaryExpr[PreUnaryExpressionEnum::MINUS] =
			&Semantic::ProcessMINUS;
		ProcessPreUnaryExpr[PreUnaryExpressionEnum::TWIDDLE] =
			&Semantic::ProcessTWIDDLE;
		ProcessPreUnaryExpr[PreUnaryExpressionEnum::NOT] =
			&Semantic::ProcessNOT;
		ProcessPreUnaryExpr[PreUnaryExpressionEnum::PLUSPLUS] =
			&Semantic::ProcessPLUSPLUSOrMINUSMINUS;
		ProcessPreUnaryExpr[PreUnaryExpressionEnum::MINUSMINUS] =
			&Semantic::ProcessPLUSPLUSOrMINUSMINUS;
#ifdef JIKES_DEBUG
		for (i = 1; i < PreUnaryExpressionEnum::_num_kinds; i++)
		{
			assert(ProcessPreUnaryExpr[i]);
			assert(DefinitePreUnaryExpr[i]);
		}
		for (i = 1; i < BinaryExpressionEnum::_num_kinds; i++)
		{
			assert(ProcessBinaryExpr[i]);
			assert(DefiniteBinaryExpr[i]);
		}
#endif // JIKES_DEBUG
	}

	Semantic::~Semantic()
	{
		delete error;
	}

	// Report a multi-token semantic warning or error.
	 void Semantic::ReportSemError(SemanticError::SemanticErrorKind kind,
		 Token* ltok, Token* rtok,
		const wchar_t* s1, const wchar_t* s2,
		const wchar_t* s3, const wchar_t* s4,
		const wchar_t* s5, const wchar_t* s6,
		const wchar_t* s7, const wchar_t* s8,
		const wchar_t* s9)
	{
		if (!lookup_env.option->errors)
			return;
		
		Token*  left_token = nullptr;
		if (ltok != nullptr)
		{
			if (lex_stream)
			{
				auto count = lex_stream->NumTokens();
				if (ltok->index < count)
					//left_token = lex_stream->getTokenObjNoIndexNoNext(ltok);
					left_token = (ltok);
			}
		}
		Token*  right_token = nullptr;
		if (rtok != nullptr)
		{
			if (lex_stream) {
				auto count = lex_stream->NumTokens();
				if (ltok->index < count)
					//	right_token = lex_stream->getTokenObjNoIndexNoNext(rtok);
					right_token = rtok;
			}
		}
		if (!error)
			error = new SemanticError(lookup_env.option, lookup_env.nameEnv->classpath, source_file_symbol);

		error->Report(kind, left_token, right_token, s1, s2, s3, s4, s5, s6, s7, s8, s9);
	}

	// Report a semantic warning or error on a syntax tree branch.
	 void Semantic::ReportSemErrorWithAst(SemanticError::SemanticErrorKind kind, Ast* ast,
		const wchar_t* s1, const wchar_t* s2,
		const wchar_t* s3, const wchar_t* s4,
		const wchar_t* s5, const wchar_t* s6,
		const wchar_t* s7, const wchar_t* s8,
		const wchar_t* s9)
	{
		if (!lookup_env.option->errors)
			return;
		if (!error)
			error = new SemanticError(lookup_env.option,lookup_env.nameEnv->classpath, source_file_symbol);
	
		Token* left = nullptr;
		Token* right = nullptr;
		if(ast)
		{
			left = ast->LeftToken();
			right = ast->RightToken();
		}

		error->Report(kind,left,right ,
			s1, s2, s3, s4, s5, s6, s7, s8, s9);
	}

} // Close namespace Jikes block



