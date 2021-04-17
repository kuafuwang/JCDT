
#ifndef SemanticForWatch_Jikes_CodeAssist_INCLUDED_INCLUDED
#define SemanticForWatch_Jikes_CodeAssist_INCLUDED_INCLUDED

namespace Jikes { // Open namespace Jikes block
	class Expression;
	class TypeSymbol;
namespace CodeAssist{
	class SemanticForWatch
	{
	public:
		LookupEnvironment& lookup_env; // The control object performing the compilation
		AstStoragePool* ast_pool;
		bool unreachable = false;
		bool IsIntValueRepresentableInType(Expression* expr, const TypeSymbol* type);
		bool IsConstantTrue(Expression* expr);
		bool IsConstantFalse(Expression* expr);
		void ProcessExpressionOrStringConstant(Expression* expr);
		void ProcessName(Ast* expr);
		void ProcessAmbiguousName(AstName* name);
		void ProcessFieldAccess(Ast* expr);
		void ProcessCharacterLiteral(Ast* expr);
		void ProcessIntegerLiteral(Ast* expr);
		void ProcessLongLiteral(Ast* expr);
		void ProcessFloatLiteral(Ast* expr);
		void ProcessDoubleLiteral(Ast* expr);
		void ProcessTrueLiteral(Ast* expr);
		void ProcessFalseLiteral(Ast* expr);
		void ProcessStringLiteral(Ast* expr);
		void ProcessArrayAccess(Ast* expr);
		void ProcessMethodInvocation(Ast*);
		void ProcessNullLiteral(Ast* expr);
		void ProcessClassLiteral(Ast* expr);
		void ProcessThisExpression(Ast* expr);
		void ProcessSuperExpression(Ast* expr);
		void ProcessParenthesizedExpression(Ast* expr);
		void ProcessClassCreationExpression(Ast* expr);
		void ProcessArrayCreationExpression(Ast* expr);
		void ProcessPostUnaryExpression(Ast* expr);
		SemanticForWatch(LookupEnvironment& control_);
		inline void ProcessExpression(Expression* expr);


		void (SemanticForWatch::*ProcessPreUnaryExpr[AstPreUnaryExpression::_num_kinds])(AstPreUnaryExpression*);
		void ProcessPLUS(AstPreUnaryExpression*);
		void ProcessMINUS(AstPreUnaryExpression*);
		void ProcessTWIDDLE(AstPreUnaryExpression*);
		void ProcessNOT(AstPreUnaryExpression*);
		void ProcessPLUSPLUSOrMINUSMINUS(AstPreUnaryExpression*);
		void ProcessPreUnaryExpression(Ast* expr);
		bool CanWideningPrimitiveConvert(const TypeSymbol* target_type, const TypeSymbol* source_type);
		bool CanNarrowingPrimitiveConvert(const TypeSymbol* target_type, const TypeSymbol* source_type);
		bool CanCastConvert(TypeSymbol* target_type, TypeSymbol* source_type, Token* tok);
		void (SemanticForWatch::*ProcessBinaryExpr[BinaryExpression::_num_kinds])(BinaryExpression*);

		//
		// An array of member methods, to dispatch the various expressions and
		// statements for processing.
		//
		void (SemanticForWatch::*ProcessExprOrStmt[Ast::_num_expr_or_stmt_kinds])(Ast*);

		LiteralValue* CastValue(const TypeSymbol* target_type, Expression* expr);
		void ProcessCastExpression(Ast* expr);
		Expression* ConvertToType(Expression* expr, TypeSymbol* target_type);
		Expression* PromoteUnaryNumericExpression(Expression* unary_expression);
		void BinaryNumericPromotion(BinaryExpression* binary_expression);
		void BinaryNumericPromotion(AstAssignmentExpression* assignment_expression);
		void BinaryNumericPromotion(ConditionalExpression* conditional_expression);
		TypeSymbol* BinaryNumericPromotion(Expression*& left_expr, Expression*& right_expr);
		void ProcessPLUS(BinaryExpression*);
		void ProcessShift(BinaryExpression*);
		void ProcessShiftCount(TypeSymbol*, Expression*);
		void ProcessLEFT_SHIFT(BinaryExpression*);
		void ProcessRIGHT_SHIFT(BinaryExpression*);
		void ProcessUNSIGNED_RIGHT_SHIFT(BinaryExpression*);
		void ProcessLESS(BinaryExpression*);
		void ProcessGREATER(BinaryExpression*);
		void ProcessLESS_EQUAL(BinaryExpression*);
		void ProcessGREATER_EQUAL(BinaryExpression*);
		void ProcessAND(BinaryExpression*);
		void ProcessXOR(BinaryExpression*);
		void ProcessIOR(BinaryExpression*);
		void ProcessAND_AND(BinaryExpression*);
		void ProcessOR_OR(BinaryExpression*);
		void ProcessEQUAL_EQUAL(BinaryExpression*);
		void ProcessNOT_EQUAL(BinaryExpression*);
		void ProcessSTAR(BinaryExpression*);
		void ProcessMINUS(BinaryExpression*);
		void ProcessSLASH(BinaryExpression*);
		void ProcessMOD(BinaryExpression*);
		void ProcessBinaryExpression(Ast* expr);
		void ProcessInstanceofExpression(Ast* expr);
		void ProcessConditionalExpression(Ast* expr);
		void ProcessAssignmentExpression(Ast* expr);
	};
	inline void SemanticForWatch::ProcessExpression(Expression* expr)
	{
		if (unreachable ||!expr)
			return;
		 (this->*ProcessExprOrStmt[expr->kind])(expr);
	}
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

