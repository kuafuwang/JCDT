
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/impl/double.h"
#include "JCDT_Lib/internal/parser/parser.h"
#include  "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/lookup/table.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/util/spell.h"
#include "JCDT_Lib/internal/impl/option.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "JCDT_Lib/internal/impl/FileLocation.h"

#include "JCDT_Lib/internal/lookup/BlockSymbol.h"
#include "JCDT_Lib/internal/lookup/VariableSymbol.h"
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>
#include "expr.h"


namespace Jikes { // Open namespace Jikes block

	namespace CodeAssist {
template <typename T> inline void ExtremaForType(T& min, T& max);

template <> inline void ExtremaForType(i4& min, i4& max)
{
    min = Int::MIN_INT();
    max = Int::MAX_INT();
}

template <> inline void ExtremaForType(LongInt& min, LongInt& max)
{
    min = LongInt::MIN_LONG();
    max = LongInt::MAX_LONG();
}

inline void ReportOverflow(SemanticForWatch* semantic, Expression* expr, bool safe)
{
    if (! safe)
    {
       /* semantic -> ReportSemErrorWithAst(SemanticError::CONSTANT_OVERFLOW, expr,
                                   expr -> Type() -> Name());*/
    }
}


template <typename T>
static void CheckIntegerNegation(SemanticForWatch* semantic, Expression* expr,
                                 const T& x)
{
    T min, max;
    ExtremaForType(min, max);
    ReportOverflow(semantic, expr, (x != min));
}


template <typename T>
inline void CheckIntegerAddition(SemanticForWatch* semantic, Expression* expr,
                                 const T& x, const T& y)
{
    const T zero = T(0);
    T min, max;
    ExtremaForType(min, max);
    bool safe = x == zero ||
                y == zero ||
                x < zero && y < zero && x >= (min - y) ||
                x < zero && y > zero ||
                x > zero && y < zero ||
                x > zero && y > zero && x <= (max - y);
    ReportOverflow(semantic, expr, safe);
}


template <typename T>
static void CheckIntegerSubtraction(SemanticForWatch* semantic, Expression* expr,
                                    const T& x, const T& y)
{
    CheckIntegerAddition(semantic, expr, x, T(-y));
}


template <typename T>
static void CheckIntegerMultiplication(SemanticForWatch* semantic, Expression* expr,
                                       const T& x, const T& y)
{
    const T zero = T(0);
    const T one = T(1);
    const T minus_one = T(-1);
    T min, max;
    ExtremaForType(min, max);
    bool safe = x > minus_one && x <= one ||
                y > minus_one && y <= one ||
                x < zero && y < zero && T(-x) <= max/-y ||
                x < zero && y > zero && x >= min/y ||
                x > zero && y < zero && y >= min/x ||
                x > zero && y > zero && x <= max/y;
    ReportOverflow(semantic, expr, safe);
}


template <typename T>
static void CheckIntegerDivision(SemanticForWatch* semantic, Expression* expr,
                                 const T& x, const T& y)
{
    const T zero = T(0);
    const T minus_one = T(-1);
    T min, max;
    ExtremaForType(min, max);
    bool safe = (y != zero) && !(x == min && y == minus_one);
    ReportOverflow(semantic, expr, safe);
}


bool SemanticForWatch::IsIntValueRepresentableInType(Expression* expr,
                                             const TypeSymbol* type)
{
    if (! expr -> IsConstant() ||
        ! lookup_env.IsSimpleIntegerValueType(expr -> Type()))
    {
        return false;
    }

    IntLiteralValue* literal = DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
	if (!literal)
		return false;

    return type == lookup_env.int_type || type == lookup_env.no_type ||
        (type == lookup_env.char_type && (literal -> value >= 0) &&
         (literal -> value <= 65535)) ||
        (type == lookup_env.byte_type && (literal -> value >= -128) &&
         (literal -> value <= 127)) ||
        (type == lookup_env.short_type && (literal -> value >= -32768) &&
         (literal -> value <= 32767));
}


bool SemanticForWatch::IsConstantTrue(Expression* expr)
{
    return  expr && expr -> IsConstant() && expr -> Type() == lookup_env.boolean_type &&
		    expr->value && DYNAMIC_CAST<IntLiteralValue*> (expr -> value) -> value;
}


bool SemanticForWatch::IsConstantFalse(Expression* expr)
{
    return expr && expr -> IsConstant() && expr -> Type() == lookup_env.boolean_type &&
		expr->value && ! DYNAMIC_CAST<IntLiteralValue*> (expr -> value) -> value;
}


void SemanticForWatch::ProcessExpressionOrStringConstant(Expression* expr)
{
    ProcessExpression(expr);
    //
    // If the expression is of type String, check whether or not it is
    // constant, and if so, compute the result.
    //
    if (! expr -> IsConstant() && lookup_env.IsString(expr->symbol))
    {
		lookup_env.nameEnv->Utf8_pool.CheckStringConstant(expr);
    }
        
}


void SemanticForWatch::ProcessName(Ast* expr){
    AstName* name = (AstName*) expr;
	if(name->base_opt){
		ProcessName(name->base_opt);
	}
    name -> symbol = lookup_env.no_type;
}

void SemanticForWatch::ProcessFieldAccess(Ast* expr){
    FieldReference* field_access = (FieldReference*) expr;
    assert(! AstName::NameCast(field_access->base));
    ProcessExpressionOrStringConstant(field_access -> base);
}


void SemanticForWatch::ProcessCharacterLiteral(Ast* expr)
{
    CharLiteral* char_literal = (CharLiteral*) expr;
	if (!expr || !char_literal->character_literal_token)
		return;

    LiteralSymbol* literal =
        (char_literal -> character_literal_token->GetLiteralSymbol());

	if (!literal)
		return;

    if (! literal -> value)
        lookup_env.nameEnv->int_pool.FindOrInsertChar(literal);
    if (literal -> value == lookup_env.BadValue())
        char_literal -> symbol = lookup_env.no_type;
    else
    {
        char_literal -> value = literal -> value;
        char_literal -> symbol = lookup_env.char_type;
    }
}


void SemanticForWatch::ProcessIntegerLiteral(Ast* expr)
{
    IntegerLiteral* int_literal = (IntegerLiteral*) expr;

	if(!int_literal || !int_literal->integer_literal_token)
	{
		return;
	}
    LiteralSymbol* literal =
         (int_literal -> integer_literal_token->GetLiteralSymbol());
	if (!literal)
		return;

    if (! literal -> value)
        lookup_env.nameEnv->int_pool.FindOrInsertInt(literal);
    if (literal -> value == lookup_env.BadValue())
    {
		unreachable = true;
    }
    else
    {
        int_literal -> value = literal -> value;
        int_literal -> symbol = lookup_env.int_type;
    }
}


void SemanticForWatch::ProcessLongLiteral(Ast* expr)
{
    LongLiteral* long_literal = (LongLiteral*) expr;

	if(!long_literal ||!long_literal->long_literal_token)
	{
		return;
	}

    LiteralSymbol* literal =
        (long_literal -> long_literal_token->GetLiteralSymbol());

	if (!literal)
		return;

    if (! literal -> value)
        lookup_env.nameEnv->long_pool.FindOrInsertLong(literal);
    if (literal -> value == lookup_env.BadValue())
    {
		unreachable = true;
    }
    else
    {
        long_literal -> value = literal -> value;
        long_literal -> symbol = lookup_env.long_type;
    }
}


void SemanticForWatch::ProcessFloatLiteral(Ast* expr)
{
    FloatLiteral* float_literal = (FloatLiteral*) expr;

	if (!float_literal || !float_literal->float_literal_token)
		return;

    LiteralSymbol* literal =
         (float_literal -> float_literal_token->GetLiteralSymbol());

	if (!literal)
		return;

    if (! literal -> value)
        lookup_env.nameEnv->float_pool.FindOrInsertFloat(literal);

    if (literal -> value == lookup_env.BadValue())
    {
		unreachable = true;
    }
    else
    {
        float_literal -> value = literal -> value;
        float_literal -> symbol = lookup_env.float_type;
    }
}


void SemanticForWatch::ProcessDoubleLiteral(Ast* expr)
{
    DoubleLiteral* double_literal = (DoubleLiteral*) expr;

	if (!expr || !double_literal->double_literal_token)
		return;

    LiteralSymbol* literal =
        (double_literal -> double_literal_token->GetLiteralSymbol());

    if (! literal -> value)
        lookup_env.nameEnv->double_pool.FindOrInsertDouble(literal);

    if (literal -> value == lookup_env.BadValue())
    {
		unreachable = true;
    }
    else
    {
        double_literal -> value = literal -> value;
        double_literal -> symbol = lookup_env.double_type;
    }
}


void SemanticForWatch::ProcessTrueLiteral(Ast* expr)
{
    Expression* true_literal = (TrueLiteral*) expr;
	if (!expr)
		return;
    true_literal -> value = lookup_env.nameEnv->int_pool.FindOrInsert((int) 1);
    true_literal -> symbol = lookup_env.boolean_type;
}


void SemanticForWatch::ProcessFalseLiteral(Ast* expr)
{
    Expression* false_literal = (FalseLiteral*) expr;
	if (!expr)
		return;
    false_literal -> value = lookup_env.nameEnv->int_pool.FindOrInsert((int) 0);
    false_literal -> symbol = lookup_env.boolean_type;
}


void SemanticForWatch::ProcessStringLiteral(Ast* expr)
{
	if (!expr)
		return;
    StringLiteral* string_literal = (StringLiteral*) expr;
	if (!string_literal->string_literal_token)
		return;
    LiteralSymbol* literal =
        (string_literal -> string_literal_token->GetLiteralSymbol());

    if (! literal -> value)
        lookup_env.nameEnv->Utf8_pool.FindOrInsertString(literal);
    if (literal -> value == lookup_env.BadValue())
        string_literal -> symbol = lookup_env.no_type;
    else
    {
        string_literal -> value = literal -> value;
        string_literal -> symbol = lookup_env.String();
    }
}


void SemanticForWatch::ProcessArrayAccess(Ast* expr)
{
    ArrayReference* array_access = (ArrayReference*) expr;

    ProcessExpression(array_access -> base);
    ProcessExpression(array_access -> expression);
    array_access -> expression =
        PromoteUnaryNumericExpression(array_access -> expression);
  
}


void SemanticForWatch::ProcessMethodInvocation(Ast*)
{
	unreachable = true;
}

void SemanticForWatch::ProcessNullLiteral(Ast* expr)
{
    //
    // Null is not a compile-time constant, so don't give it a value
    //
    NullLiteral* null_literal = (NullLiteral*) expr;
    null_literal -> symbol = lookup_env.null_type;
}


void SemanticForWatch::ProcessClassLiteral(Ast* expr)
{
	unreachable = true;
}


void SemanticForWatch::ProcessThisExpression(Ast* expr)
{
   
    AstThisExpression* this_expression = (AstThisExpression*) expr;
    AstTypeName* base = this_expression -> base_opt;
    
}


void SemanticForWatch::ProcessSuperExpression(Ast* expr)
{
    AstSuperExpression* super_expression = (AstSuperExpression*) expr;
    AstTypeName* base = super_expression -> base_opt;
}


void SemanticForWatch::ProcessParenthesizedExpression(Ast* expr)
{
    AstParenthesizedExpression* parenthesized =
        (AstParenthesizedExpression*) expr;

    //
    // Do not use ProcessExpressionOrStringConstant here, to avoid generating
    // intermediate Strings - see CheckConstantString in lookup.cpp
    //
	if(!parenthesized || !parenthesized->expression)
	{
		return;
	}
	ProcessExpression(parenthesized->expression);
	
    parenthesized -> value = parenthesized -> expression -> value;
    parenthesized -> symbol = parenthesized -> expression -> symbol;
    
}



void SemanticForWatch::ProcessClassCreationExpression(Ast* expr)
{
	unreachable = true;// ???? ??????????????????????debug????????????
}


void SemanticForWatch::ProcessArrayCreationExpression(Ast* expr)
{
	unreachable = true;// ???? ??????????????????????debug????????????
}


void SemanticForWatch::ProcessPostUnaryExpression(Ast* expr)
{
    AstPostUnaryExpression* postfix_expression =
        (AstPostUnaryExpression*) expr;
    Expression* expression = postfix_expression -> expression;

    ProcessExpression(expression);
    postfix_expression -> symbol = expression -> symbol;

    //
    // JLS2 added ability for parenthesized variable to remain a variable.
    //
    if (  AstParenthesizedExpression::ParenthesizedExpressionCast(expression) )
    {
		unreachable = true;
		return;
    }

	if (!expression->IsLeftHandSide())
	{
		unreachable = true;
		return;
	}
	else if (!lookup_env.IsNumeric(expression->Type()))
	{
		unreachable = true;
		return;
	}


}


SemanticForWatch::SemanticForWatch(LookupEnvironment& control_) : lookup_env(control_), ast_pool(nullptr)
                                                                
		{
		}

void SemanticForWatch::ProcessPLUS(AstPreUnaryExpression* expr)
{
	if (!expr->expression)
		return;

    ProcessExpression(expr -> expression);
    expr -> expression = PromoteUnaryNumericExpression(expr -> expression);
    expr -> value = expr -> expression -> value;
    expr -> symbol = expr -> expression -> symbol;
}


void SemanticForWatch::ProcessMINUS(AstPreUnaryExpression* expr)
{

    IntegerLiteral* int_literal =
       IntegerLiteral::IntegerLiteralCast(expr->expression);
    LongLiteral* long_literal = LongLiteral::LongLiteralCast(expr->expression);

    if (int_literal)
    {
		if (!int_literal->integer_literal_token)
			return;

        LiteralSymbol* literal =  
            (int_literal -> integer_literal_token->GetLiteralSymbol() );

        expr -> value = lookup_env.nameEnv->int_pool.FindOrInsertNegativeInt(literal);
        if (expr -> value == lookup_env.BadValue())
        {
			unreachable = true;
			return;
        }
        else expr -> symbol = lookup_env.int_type;
    }
    else if (long_literal)
    {
		if (!long_literal->long_literal_token)
			return;

        LiteralSymbol* literal =  (long_literal -> long_literal_token->GetLiteralSymbol());

        expr -> value = lookup_env.nameEnv->long_pool.FindOrInsertNegativeLong(literal);
        if (expr -> value == lookup_env.BadValue())
        {
			unreachable = true;
			return;
        }
        else expr -> symbol = lookup_env.long_type;
    }
    else
    {
		if (!expr->expression)
			return;

        ProcessExpression(expr -> expression);

        expr -> expression = PromoteUnaryNumericExpression(expr -> expression);
        expr -> symbol = expr -> expression -> symbol;

		if (!expr->expression->IsConstant() || !expr->expression->value)
			return;
        
        TypeSymbol* type = expr -> Type();

        if (type == lookup_env.double_type)
        {
            DoubleLiteralValue* literal = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> expression -> value);
            expr -> value =
                lookup_env.nameEnv->double_pool.FindOrInsert(-literal -> value);
        }
        else if (type == lookup_env.float_type)
        {
            FloatLiteralValue* literal = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> expression -> value);
            expr -> value =
                lookup_env.nameEnv->float_pool.FindOrInsert(-literal -> value);
        }
        else if (type == lookup_env.long_type)
        {
            LongLiteralValue* literal = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> expression -> value);
            CheckIntegerNegation(this, expr, literal -> value);
            expr -> value =
                lookup_env.nameEnv->long_pool.FindOrInsert(-literal -> value);
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* literal = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> expression -> value);
            CheckIntegerNegation(this, expr, literal -> value);
            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(-literal -> value);
        }
    
    }
}


void SemanticForWatch::ProcessTWIDDLE(AstPreUnaryExpression* expr)
{
	if (unreachable || !expr->expression)
		return;
    ProcessExpression(expr -> expression);
    expr -> expression = PromoteUnaryNumericExpression(expr -> expression);
    expr -> symbol = expr -> expression -> symbol;

    if (expr -> expression -> IsConstant())
    {
        if (expr -> expression -> Type() == lookup_env.long_type)
        {
            LongLiteralValue* literal = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> expression -> value);
            expr -> value = lookup_env.nameEnv->long_pool.FindOrInsert(~literal -> value);
        }
        else // assert(expr -> expression -> Type() == control.int_type)
        {
            IntLiteralValue* literal = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> expression -> value);
            expr -> value = lookup_env.nameEnv->int_pool.FindOrInsert(~literal -> value);
        }
    }
}


void SemanticForWatch::ProcessNOT(AstPreUnaryExpression* expr)
{
	if (!expr->expression)
		return;
    ProcessExpression(expr -> expression);
	if (expr->expression->IsConstant())
		expr->value = lookup_env.nameEnv->int_pool
		.FindOrInsert(IsConstantTrue(expr->expression) ? 0 : 1);
	expr->symbol = lookup_env.boolean_type;

}


void SemanticForWatch::ProcessPLUSPLUSOrMINUSMINUS(AstPreUnaryExpression* prefix_expression)
{
	if (!prefix_expression->expression)
		return;
    Expression* expression = prefix_expression -> expression;

    ProcessExpression(expression);
    prefix_expression -> symbol = expression -> symbol;
	prefix_expression->value = expression->value;
    //
    // JLS2 added ability for parenthesized variable to remain a variable.
    //
    if ( AstParenthesizedExpression::ParenthesizedExpressionCast(expression))
    {
		unreachable = true; 
		return;
    }
	if (!expression->IsLeftHandSide())
	{
		unreachable = true;
		return;
	}

     
}


void SemanticForWatch::ProcessPreUnaryExpression(Ast* expr)
{
	if (!expr)
		return;

    AstPreUnaryExpression* prefix_expression = (AstPreUnaryExpression*) expr;
    (this ->* ProcessPreUnaryExpr[prefix_expression -> Tag()])
        (prefix_expression);
}


//
// Returns true if both types are primitive, and the source type can be
// widened into the target type.
//
inline bool SemanticForWatch::CanWideningPrimitiveConvert(const TypeSymbol* target_type,
                                                  const TypeSymbol* source_type)
{
    if (target_type == lookup_env.double_type)
        return source_type == lookup_env.float_type ||
            source_type == lookup_env.long_type ||
            source_type == lookup_env.int_type ||
            source_type == lookup_env.char_type ||
            source_type == lookup_env.short_type ||
            source_type == lookup_env.byte_type;
    if (target_type == lookup_env.float_type)
        return source_type == lookup_env.long_type ||
            source_type == lookup_env.int_type ||
            source_type == lookup_env.char_type ||
            source_type == lookup_env.short_type ||
            source_type == lookup_env.byte_type;
    if (target_type == lookup_env.long_type)
        return source_type == lookup_env.int_type ||
            source_type == lookup_env.char_type ||
            source_type == lookup_env.short_type ||
            source_type == lookup_env.byte_type;
    if (target_type == lookup_env.int_type)
        return source_type == lookup_env.char_type ||
            source_type == lookup_env.short_type ||
            source_type == lookup_env.byte_type;
    if (target_type == lookup_env.short_type)
        return source_type == lookup_env.byte_type;
    return false;
}


//
// Returns true if both types are primitive, and the source type can be
// narrowed to the target type.
//
inline bool SemanticForWatch::CanNarrowingPrimitiveConvert(const TypeSymbol* target_type,
                                                   const TypeSymbol* source_type)
{
    if (target_type == lookup_env.byte_type)
        return source_type == lookup_env.double_type ||
            source_type == lookup_env.float_type ||
            source_type == lookup_env.long_type ||
            source_type == lookup_env.int_type ||
            source_type == lookup_env.char_type ||
            source_type == lookup_env.short_type;
    if (target_type == lookup_env.char_type)
        return source_type == lookup_env.double_type ||
            source_type == lookup_env.float_type ||
            source_type == lookup_env.long_type ||
            source_type == lookup_env.int_type ||
            source_type == lookup_env.short_type ||
            source_type == lookup_env.byte_type;
    if (target_type == lookup_env.short_type)
        return source_type == lookup_env.double_type ||
            source_type == lookup_env.float_type ||
            source_type == lookup_env.long_type ||
            source_type == lookup_env.int_type ||
            source_type == lookup_env.char_type;
    if (target_type == lookup_env.int_type)
        return source_type == lookup_env.double_type ||
            source_type == lookup_env.float_type ||
            source_type == lookup_env.long_type;
    if (target_type == lookup_env.long_type)
        return source_type == lookup_env.double_type ||
            source_type == lookup_env.float_type;
    if (target_type == lookup_env.float_type)
        return source_type == lookup_env.double_type;
    return false;
}


// Returns true if the source type can be cast into the target type, via an
// identity, narrowing, or widening conversion. The lexical token is needed
// in case an error is encountered when resolving the target type.
//
bool SemanticForWatch::CanCastConvert(TypeSymbol* target_type, TypeSymbol* source_type,
                              Token* tok)
{
	if (!source_type || !target_type)
	{
		return false;
	}
    if (target_type == lookup_env.null_type)
        return false;
    if (source_type == target_type || source_type == lookup_env.no_type ||
        target_type == lookup_env.no_type)
    {
        return true;
    }

    if (source_type -> Primitive())
    {
        return target_type -> Primitive() &&
            (CanWideningPrimitiveConvert(target_type, source_type) ||
             CanNarrowingPrimitiveConvert(target_type, source_type));
    }

    if (target_type -> Primitive())
        return false;

   
    return false;
}


//
// Transfer a constant value across a primitive or String cast statement,
// whether explicit or generated.
//
LiteralValue* SemanticForWatch::CastValue(const TypeSymbol* target_type,
                                  Expression* expr)
{
	if(!target_type || !expr || !expr->value)
	{
		return nullptr;
	}

    TypeSymbol* source_type = expr -> Type();
	if(!source_type)
	{
		return nullptr;
	}

    if (target_type == source_type || source_type == lookup_env.no_type ||
        ! expr -> IsConstant())
    {
        assert(target_type == source_type || ! expr -> value);
        return expr -> value;
    }
    if ( lookup_env.IsString(source_type))
        return NULL; // A string cast to a supertype is not constant.

    LiteralValue* literal_value = NULL;
    if (lookup_env.IsString(target_type))
    {
        if (source_type == lookup_env.double_type)
        {
            DoubleLiteralValue* literal =
                DYNAMIC_CAST<DoubleLiteralValue*> (expr -> value);
            DoubleToString ieee_double(literal -> value);
            literal_value =
                lookup_env.nameEnv->Utf8_pool.FindOrInsert(ieee_double.String(),
                                               ieee_double.Length());
        }
        else if (source_type == lookup_env.float_type)
        {
            FloatLiteralValue* literal =
                DYNAMIC_CAST<FloatLiteralValue*> (expr -> value);
            FloatToString ieee_float(literal -> value);
            literal_value =
                lookup_env.nameEnv->Utf8_pool.FindOrInsert(ieee_float.String(),
                                               ieee_float.Length());
        }
        else if (source_type == lookup_env.long_type)
        {
            LongLiteralValue* literal =
                DYNAMIC_CAST<LongLiteralValue*> (expr -> value);
            LongToString long_integer(literal -> value);
            literal_value =
                lookup_env.nameEnv->Utf8_pool.FindOrInsert(long_integer.String(),
                                               long_integer.Length());
        }
        else if (source_type == lookup_env.char_type)
        {
            IntLiteralValue* literal =
                DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
            literal_value = lookup_env.nameEnv->Utf8_pool.FindOrInsert(literal -> value);
        }
        else if (source_type == lookup_env.boolean_type)
        {
            if (IsConstantFalse(expr))
                literal_value = lookup_env.nameEnv->false_name_symbol -> Utf8_literal;
            else
            {
                assert(IsConstantTrue(expr));
                literal_value = lookup_env.nameEnv->true_name_symbol -> Utf8_literal;
            }
        }
        else if (lookup_env.IsSimpleIntegerValueType(source_type))
        {
            IntLiteralValue* literal =
                DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
            IntToString integer(literal -> value);
            literal_value =
                lookup_env.nameEnv->Utf8_pool.FindOrInsert(integer.String(),
                                               integer.Length());
        }
    }
    else if (target_type == lookup_env.double_type)
    {
        if (source_type == lookup_env.float_type)
        {
            FloatLiteralValue* literal =
                DYNAMIC_CAST<FloatLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->double_pool.FindOrInsert(literal -> value.DoubleValue());
        }
        else if (source_type == lookup_env.long_type)
        {
            LongLiteralValue* literal =
                DYNAMIC_CAST<LongLiteralValue*> (expr -> value);
            IEEEdouble value(literal -> value);
            literal_value = lookup_env.nameEnv->double_pool.FindOrInsert(value);
        }
        else
        {
            IntLiteralValue* literal =
                DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
            IEEEdouble value(literal -> value);
            literal_value = lookup_env.nameEnv->double_pool.FindOrInsert(value);
        }
    }
    else if (target_type == lookup_env.float_type)
    {
        if (source_type == lookup_env.double_type)
        {
            DoubleLiteralValue* literal =
                DYNAMIC_CAST<DoubleLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->float_pool.FindOrInsert(literal -> value.FloatValue());
        }
        else if (source_type == lookup_env.long_type)
        {
            LongLiteralValue* literal =
                DYNAMIC_CAST<LongLiteralValue*> (expr -> value);
            IEEEfloat value(literal -> value);
            literal_value = lookup_env.nameEnv->float_pool.FindOrInsert(value);
        }
        else
        {
            IntLiteralValue* literal =
                DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
            IEEEfloat value(literal -> value);
            literal_value = lookup_env.nameEnv->float_pool.FindOrInsert(value);
        }
    }
    else if (target_type == lookup_env.long_type)
    {
        if (source_type == lookup_env.double_type)
        {
            DoubleLiteralValue* literal =
                DYNAMIC_CAST<DoubleLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->long_pool.FindOrInsert(literal -> value.LongValue());
        }
        else if (source_type == lookup_env.float_type)
        {
            FloatLiteralValue* literal =
                DYNAMIC_CAST<FloatLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->long_pool.FindOrInsert(literal -> value.LongValue());
        }
        else
        {
            IntLiteralValue* literal =
                DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->long_pool.FindOrInsert((LongInt) literal -> value);
        }
    }
    else if (target_type == lookup_env.int_type)
    {
        if (source_type == lookup_env.double_type)
        {
            DoubleLiteralValue* literal =
                DYNAMIC_CAST<DoubleLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((literal -> value).IntValue());
        }
        else if (source_type == lookup_env.float_type)
        {
            FloatLiteralValue* literal =
                DYNAMIC_CAST<FloatLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert(literal -> value.IntValue());
        }
        else if (source_type == lookup_env.long_type)
        {
            LongLiteralValue* literal =
                DYNAMIC_CAST<LongLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (literal -> value).LowWord());
        }
        else literal_value = expr -> value;
    }
    else if (target_type == lookup_env.char_type)
    {
        if (source_type == lookup_env.double_type)
        {
            DoubleLiteralValue* literal =
                DYNAMIC_CAST<DoubleLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (u2) (literal -> value.IntValue()));
        }
        else if (source_type == lookup_env.float_type)
        {
            FloatLiteralValue* literal =
                DYNAMIC_CAST<FloatLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (u2) (literal -> value.IntValue()));
        }
        else if (source_type == lookup_env.long_type)
        {
            LongLiteralValue* literal =
                DYNAMIC_CAST<LongLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (u2) (literal -> value).LowWord());
        }
        else
        {
            IntLiteralValue* literal =
                DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (u2) literal -> value);
        }
    }
    else if (target_type == lookup_env.short_type)
    {
        if (source_type == lookup_env.double_type)
        {
            DoubleLiteralValue* literal =
                DYNAMIC_CAST<DoubleLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (i2) (literal -> value.IntValue()));
        }
        else if (source_type == lookup_env.float_type)
        {
            FloatLiteralValue* literal =
                DYNAMIC_CAST<FloatLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (i2) (literal -> value.IntValue()));
        }
        else if (source_type == lookup_env.long_type)
        {
            LongLiteralValue* literal =
                DYNAMIC_CAST<LongLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (i2) (literal -> value).LowWord());
        }
        else
        {
            IntLiteralValue* literal =
                DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (i2) literal -> value);
        }
    }
    else if (target_type == lookup_env.byte_type)
    {
        if (source_type == lookup_env.double_type)
        {
            DoubleLiteralValue* literal =
                DYNAMIC_CAST<DoubleLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (i1) (literal -> value.IntValue()));
        }
        else if (source_type == lookup_env.float_type)
        {
            FloatLiteralValue* literal =
                DYNAMIC_CAST<FloatLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (i1) (literal -> value.IntValue()));
        }
        else if (source_type == lookup_env.long_type)
        {
            LongLiteralValue* literal =
                DYNAMIC_CAST<LongLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (i1)
                                              (literal -> value).LowWord());
        }
        else
        {
            IntLiteralValue* literal =
                DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
            literal_value =
                lookup_env.nameEnv->int_pool.FindOrInsert((i4) (i1) literal -> value);
        }
    }

    assert(literal_value);
    return literal_value;
}


void SemanticForWatch::ProcessCastExpression(Ast* expr)
{
	if (!expr)
		return;

    CastExpression* cast_expression = (CastExpression*) expr;

    //
    // Do not use ProcessExpressionOrStringConstant here, to avoid generating
    // intermediate Strings - see CheckConstantString in lookup.cpp
    //
	if (!cast_expression->expression || !cast_expression->type)
		return;

    ProcessExpression(cast_expression -> expression);

    TypeSymbol* source_type = cast_expression -> expression -> Type();
    TypeSymbol* target_type = cast_expression -> type -> symbol;

    if (CanCastConvert(target_type, source_type,
                       cast_expression -> right_parenthesis_token))
    {
        cast_expression -> symbol = target_type;
        cast_expression -> value = CastValue(target_type,
                                             cast_expression -> expression);
    }
   
}


//
// Inserts a widening conversion, if necessary.
//
Expression* SemanticForWatch::ConvertToType(Expression* expr,
                                       TypeSymbol* target_type)
{
	if(!expr || !target_type)
	{
		return nullptr;
	}

    TypeSymbol* source_type = expr -> Type();
    if (source_type == lookup_env.null_type || source_type == target_type ||
        source_type == lookup_env.no_type || target_type -> Bad())
    {
        return expr;
    }

    auto loc = expr -> LeftToken();

    CastExpression* result =
         ast_pool -> GenCastExpression();
    result -> left_parenthesis_token = loc;
    //
    // Rather than generate an AstType, we leave this NULL and rely
    // on the resolved symbol for the type.
    //
    result -> type = NULL;
    result -> right_parenthesis_token = loc;
    result -> expression = expr;
    result -> symbol = target_type;
    result -> value = CastValue(target_type, expr);
    return result;
}


Expression* SemanticForWatch::PromoteUnaryNumericExpression(Expression* unary_expression)
{
	if (!unary_expression)
		return nullptr;;

    TypeSymbol* type = unary_expression -> Type();
    return (type == lookup_env.byte_type || type == lookup_env.short_type ||
            type == lookup_env.char_type)
        ? ConvertToType(unary_expression, lookup_env.int_type) : unary_expression;
}


void SemanticForWatch::BinaryNumericPromotion(BinaryExpression* binary_expression)
{
	if (!binary_expression)
		return ;
    binary_expression -> symbol =
        BinaryNumericPromotion(binary_expression -> left_expression,
                               binary_expression -> right_expression);
}


void SemanticForWatch::BinaryNumericPromotion(AstAssignmentExpression* assignment_expression)
{
	if (!assignment_expression)
		return;

    Expression* left_expr = assignment_expression -> left_hand_side;
    while (AstParenthesizedExpression::ParenthesizedExpressionCast(left_expr))
        left_expr = ((AstParenthesizedExpression*) left_expr) -> expression;

    TypeSymbol* type =
        BinaryNumericPromotion(left_expr, assignment_expression -> expression);

    assignment_expression -> left_hand_side = left_expr;
    if (type == lookup_env.no_type)
        assignment_expression -> symbol = lookup_env.no_type;
}


void SemanticForWatch::BinaryNumericPromotion(ConditionalExpression* conditional_expression)
{
	if (!conditional_expression)
		return;

    conditional_expression -> symbol =
        BinaryNumericPromotion(conditional_expression -> true_expression,
                               conditional_expression -> false_expression);
}


TypeSymbol* SemanticForWatch::BinaryNumericPromotion(Expression*& left_expr,
                                             Expression*& right_expr)
{

	if (!left_expr || !right_expr)
		return lookup_env.no_type;;


    TypeSymbol* left_type = left_expr -> Type();
    TypeSymbol* right_type = right_expr -> Type();

    if (! lookup_env.IsNumeric(left_type) || ! lookup_env.IsNumeric(right_type)){
        return lookup_env.no_type;
    }
    if (left_type == lookup_env.double_type)
    {
        right_expr = ConvertToType(right_expr, lookup_env.double_type);
        return lookup_env.double_type;
    }
    if (right_type == lookup_env.double_type)
    {
        left_expr = ConvertToType(left_expr, lookup_env.double_type);
        return lookup_env.double_type;
    }
    if (left_type == lookup_env.float_type)
    {
        right_expr = ConvertToType(right_expr, lookup_env.float_type);
        return lookup_env.float_type;
    }
    if (right_type == lookup_env.float_type)
    {
        left_expr = ConvertToType(left_expr, lookup_env.float_type);
        return lookup_env.float_type;
    }
    if (left_type == lookup_env.long_type)
    {
        right_expr = ConvertToType(right_expr, lookup_env.long_type);
        return lookup_env.long_type;
    }
    if (right_type == lookup_env.long_type)
    {
        left_expr = ConvertToType(left_expr, lookup_env.long_type);
        return lookup_env.long_type;
    }
    left_expr = ConvertToType(left_expr, lookup_env.int_type);
    right_expr = ConvertToType(right_expr, lookup_env.int_type);
    return lookup_env.int_type;
}




void SemanticForWatch::ProcessPLUS(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
		return;
    //
    // Do not use ProcessExpressionOrStringConstant here, to avoid generating
    // intermediate Strings - see CheckConstantString in lookup.cpp
    //
    Expression* left = expr -> left_expression;
    Expression* right = expr -> right_expression;


    ProcessExpression(left);
    ProcessExpression(right);

    TypeSymbol* left_type = left -> Type();
    TypeSymbol* right_type = right -> Type();

    if (
		  (lookup_env.IsString(left_type))
		||(lookup_env.IsString(right_type))
		)
    {
        //
        // Convert the left expression if necessary.
        //
        if (!lookup_env.IsString(left_type))
        {
            if (left_type == lookup_env.void_type)
            {
             
            }
            else if (left_type == lookup_env.null_type || left -> IsConstant())
            {
                left -> value = CastValue(lookup_env.String(), left);
                left -> symbol = lookup_env.String();
            }
        }

      

        //
        // If both subexpressions are string constants, identify the result as
        // as a string constant, but do not perform the concatenation here. The
        // reason being that if we have a long expression of the form
        //
        //  s1 + s2 + ... + sn
        //
        // where each subexpression s(i) is a string constant, we want to
        // perform one concatenation and enter a single result into the
        // constant pool instead of n-1 subresults. See CheckStringConstant
        // in lookup.cpp.
        //
         expr -> symbol = lookup_env.String();
    }
    else
    {
        BinaryNumericPromotion(expr);
        left = expr -> left_expression;
        right = expr -> right_expression;

		if(!left || !right)
		{
			return;
		}

        if (left -> IsConstant() && right -> IsConstant())
        {
            if (expr -> Type() == lookup_env.double_type)
            {
                DoubleLiteralValue* left_value =
                    DYNAMIC_CAST<DoubleLiteralValue*> (left -> value);
                DoubleLiteralValue* right_value =
                    DYNAMIC_CAST<DoubleLiteralValue*> (right -> value);

                expr -> value =
                    lookup_env.nameEnv->double_pool.FindOrInsert(left_value -> value +
                                                     right_value -> value);
            }
            else if (expr -> Type() == lookup_env.float_type)
            {
                FloatLiteralValue* left_value =
                    DYNAMIC_CAST<FloatLiteralValue*> (left -> value);
                FloatLiteralValue* right_value =
                    DYNAMIC_CAST<FloatLiteralValue*> (right -> value);
                expr -> value =
                    lookup_env.nameEnv->float_pool.FindOrInsert(left_value -> value +
                                                    right_value -> value);
            }
            else if (expr -> Type() == lookup_env.long_type)
            {
                LongLiteralValue* left_value =
                    DYNAMIC_CAST<LongLiteralValue*> (left -> value);
                LongLiteralValue* right_value =
                    DYNAMIC_CAST<LongLiteralValue*> (right -> value);

                CheckIntegerAddition(this, expr, left_value -> value,
                                     right_value -> value);
                expr -> value =
                    lookup_env.nameEnv->long_pool.FindOrInsert(left_value -> value +
                                                   right_value -> value);
            }
            else if (expr -> Type() == lookup_env.int_type)
            {
                IntLiteralValue* left_value =
                    DYNAMIC_CAST<IntLiteralValue*> (left -> value);
                IntLiteralValue* right_value =
                    DYNAMIC_CAST<IntLiteralValue*> (right -> value);
                CheckIntegerAddition(this, expr, left_value -> value,
                                     right_value -> value);
                expr -> value =
                    lookup_env.nameEnv->int_pool.FindOrInsert(left_value -> value +
                                                  right_value -> value);
            }
        }
    }
}


void SemanticForWatch::ProcessShift(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
		return;

    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (! lookup_env.IsIntegral(left_type))
    {
        
    }
    else
    {
        expr -> left_expression =
            PromoteUnaryNumericExpression(expr -> left_expression);
    }
    //
    // This call captures both unary numeric conversion (widening) of
    // byte, char, or short, and narrowing of long, since the bytecode
    // requires an int shift amount.
    //
    if (! lookup_env.IsIntegral(right_type))
    {
       
    }
    else
    {
        expr -> right_expression = ConvertToType(expr -> right_expression,
                                                 lookup_env.int_type);
        if (expr -> symbol != lookup_env.no_type)
            expr -> symbol = expr -> left_expression -> symbol;

        ProcessShiftCount(left_type, expr -> right_expression);
    }
}


//
// Checks whether 'expr' is a suitable shift count for something of type
// 'left_type'. JLS2 15.19 is quite clear about the meaning of code with
// with a negative or out-of-range shift count, so it's still valid code,
// but the behavior is probably not what the author was expecting.
//
void SemanticForWatch::ProcessShiftCount(TypeSymbol* left_type, Expression* expr)
{
    if (!expr || ! expr -> IsConstant() || !expr->value)
        return;

    IntLiteralValue* literal = DYNAMIC_CAST<IntLiteralValue*>(expr -> value);
    i4 count = literal -> value;
    IntToWstring count_text(count);

    if (count < 0)
    {
       /* ReportSemErrorWithAst(SemanticError::NEGATIVE_SHIFT_COUNT,
                       expr,
                       count_text.String());*/
		unreachable = true;
		return;
    }

    int width = (left_type == lookup_env.long_type) ? 64 : 32;
    if (count >= width)
    {
		unreachable = true;
		return;
    }
}


void SemanticForWatch::ProcessLEFT_SHIFT(BinaryExpression* expr)
{
	if(!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}

    ProcessShift(expr);

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (expr -> Type() == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right)
			{
				expr->value = lookup_env.nameEnv->long_pool.FindOrInsert(left->value <<
					(right->value &
						Semantic::LONG_SHIFT_MASK));
			}
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if(left && right)
			{
				expr->value = lookup_env.nameEnv->int_pool.FindOrInsert(left->value <<
					(right->value &
						Semantic::INT_SHIFT_MASK));
			}

        }
    }
}


void SemanticForWatch::ProcessRIGHT_SHIFT(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
		return;

    ProcessShift(expr);

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (expr -> Type() == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right)
			{
				expr->value = lookup_env.nameEnv->long_pool.FindOrInsert(left->value >>
					(right->value &
						Semantic::LONG_SHIFT_MASK));
			}
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right)
			{
				expr->value = lookup_env.nameEnv->int_pool.FindOrInsert(left->value >>
					(right->value &
						Semantic::INT_SHIFT_MASK));
			}
        }
    }
}


void SemanticForWatch::ProcessUNSIGNED_RIGHT_SHIFT(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
		return;
    ProcessShift(expr);

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (expr -> Type() == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value = lookup_env.nameEnv->long_pool.FindOrInsert((LongInt)
					((ULongInt)left->value >> (right->value & Semantic::LONG_SHIFT_MASK)));
			}
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value = lookup_env.nameEnv->int_pool.FindOrInsert((i4)
					((u4)left->value >> (right->value & Semantic::INT_SHIFT_MASK)));
			}
        }
    }
}


void SemanticForWatch::ProcessLESS(BinaryExpression* expr)
{
	if(!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    BinaryNumericPromotion(expr);
    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    expr -> symbol = (left_type == lookup_env.no_type ||
                      right_type == lookup_env.no_type)
        ? lookup_env.no_type : lookup_env.boolean_type;

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (left_type == lookup_env.double_type)
        {
            DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> left_expression -> value);
            DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value <
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.float_type)
        {

            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value <
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value <
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value <
						right->value ? 1 : 0);
			}
        }
    }
}


void SemanticForWatch::ProcessGREATER(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    BinaryNumericPromotion(expr);
    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    expr -> symbol = (left_type == lookup_env.no_type ||
                      right_type == lookup_env.no_type)
        ? lookup_env.no_type : lookup_env.boolean_type;

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (left_type == lookup_env.double_type)
        {
            DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> left_expression -> value);
            DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value >
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value >
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value >
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value >
						right->value ? 1 : 0);
			}
        }
    }
}


void SemanticForWatch::ProcessLESS_EQUAL(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    BinaryNumericPromotion(expr);
    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    expr -> symbol = (left_type == lookup_env.no_type ||
                      right_type == lookup_env.no_type)
        ? lookup_env.no_type : lookup_env.boolean_type;

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (left_type == lookup_env.double_type)
        {
            DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> left_expression -> value);
            DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value <=
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value <=
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value <=
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value <=
						right->value ? 1 : 0);
			}
        }
    }
}


void SemanticForWatch::ProcessGREATER_EQUAL(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    BinaryNumericPromotion(expr);
    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    expr -> symbol = (left_type == lookup_env.no_type ||
                      right_type == lookup_env.no_type)
        ? lookup_env.no_type : lookup_env.boolean_type;

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (left_type == lookup_env.double_type)
        {
            DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> left_expression -> value);
            DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value >=
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value >=
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value >=
						right->value ? 1 : 0);
			}
        }
        else if (left_type == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value >= right->value ? 1 : 0);
			}
        }
    }
}


void SemanticForWatch::ProcessAND(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.boolean_type ||
        right_type == lookup_env.boolean_type)
    {
        if (left_type != lookup_env.boolean_type)
        {
            
        }
        else if (right_type != lookup_env.boolean_type)
        {
         
        }
        else expr -> symbol = lookup_env.boolean_type;

        if (expr -> left_expression -> IsConstant() &&
            expr -> right_expression -> IsConstant())
        {
            expr -> value = lookup_env.nameEnv->int_pool
                .FindOrInsert((IsConstantTrue(expr -> left_expression) &&
                               IsConstantTrue(expr -> right_expression))
                              ? 1 : 0);
        }
    }
    else
    {
        BinaryNumericPromotion(expr);
        TypeSymbol* expr_type = expr -> Type();

        if (! lookup_env.IsIntegral(expr_type))
        {
            if (! lookup_env.IsIntegral(left_type) &&
                left_type != lookup_env.no_type)
            {
				unreachable = true;
				return;
            }
            if (! lookup_env.IsIntegral(right_type) &&
                right_type != lookup_env.no_type)
            {
				unreachable = true;
				return;
            }
            expr -> symbol = lookup_env.no_type;
        }
        if (expr -> left_expression -> IsConstant() &&
            expr -> right_expression -> IsConstant())
        {
            if (expr_type == lookup_env.long_type)
            {
                LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                    (expr -> left_expression -> value);
                LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                    (expr -> right_expression -> value);
				if (left && right) {
					expr->value = lookup_env.nameEnv->long_pool.FindOrInsert(left->value &
						right->value);
				}
            }
            else if (expr_type == lookup_env.int_type)
            {
                IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> left_expression -> value);
                IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> right_expression -> value);
				if (left && right) {
					expr->value = lookup_env.nameEnv->int_pool.FindOrInsert(left->value &
						right->value);
				}
            }
        }
    }
}


void SemanticForWatch::ProcessXOR(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.boolean_type ||
        right_type == lookup_env.boolean_type)
    {
        if (left_type != lookup_env.boolean_type)
        {
          
        }
        else if (right_type != lookup_env.boolean_type)
        {
           
        }
        else expr -> symbol = lookup_env.boolean_type;
        if (expr -> left_expression -> IsConstant() &&
            expr -> right_expression -> IsConstant())
        {
            expr -> value = lookup_env.nameEnv->int_pool
                .FindOrInsert((IsConstantTrue(expr -> left_expression) !=
                               IsConstantTrue(expr -> right_expression))
                              ? 1 : 0);
        }
    }
    else
    {
        BinaryNumericPromotion(expr);
        TypeSymbol* expr_type = expr -> Type();

        if (! lookup_env.IsIntegral(expr_type))
        {
            if (! lookup_env.IsIntegral(left_type))
            {
				unreachable = true;
				return;
            }
            if (! lookup_env.IsIntegral(right_type) )
            {
				unreachable = true;
				return;
            }
            expr -> symbol = lookup_env.no_type;
        }
        if (expr -> left_expression -> IsConstant() &&
            expr -> right_expression -> IsConstant())
        {
            if (expr_type == lookup_env.long_type)
            {
                LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                    (expr -> left_expression -> value);
                LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                    (expr -> right_expression -> value);
				if (left && right) {
					expr->value = lookup_env.nameEnv->long_pool.FindOrInsert(left->value ^
						right->value);
				}
            }
            else if (expr_type == lookup_env.int_type)
            {
                IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> left_expression -> value);
                IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> right_expression -> value);
				if (left && right) {
					expr->value = lookup_env.nameEnv->int_pool.FindOrInsert(left->value ^
						right->value);
				}
            }
        }
    }
}


void SemanticForWatch::ProcessIOR(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.boolean_type ||
        right_type == lookup_env.boolean_type)
    {
        if (left_type != lookup_env.boolean_type)
        {
           
        }
        else if (right_type != lookup_env.boolean_type)
        {
           
        }
        else expr -> symbol = lookup_env.boolean_type;
        if (expr -> left_expression -> IsConstant() &&
            expr -> right_expression -> IsConstant())
        {
            expr -> value = lookup_env.nameEnv->int_pool
                .FindOrInsert((IsConstantTrue(expr -> left_expression) ||
                               IsConstantTrue(expr -> right_expression))
                              ? 1 : 0);
        }
    }
    else
    {
        BinaryNumericPromotion(expr);
        TypeSymbol* expr_type = expr -> Type();

        if (! lookup_env.IsIntegral(expr_type))
        {
            if (! lookup_env.IsIntegral(left_type) &&
                left_type != lookup_env.no_type)
            {
              
            }
            if (! lookup_env.IsIntegral(right_type) &&
                right_type != lookup_env.no_type)
            {
            
            }
            expr -> symbol = lookup_env.no_type;
        }
        if (expr -> left_expression -> IsConstant() &&
            expr -> right_expression -> IsConstant())
        {
            if (expr_type == lookup_env.long_type)
            {
                LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                    (expr -> left_expression -> value);
                LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                    (expr -> right_expression -> value);
				if (left && right) {
					expr->value = lookup_env.nameEnv->long_pool.FindOrInsert(left->value | right->value);
				}
            }
            else if (expr_type == lookup_env.int_type)
            {
                IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> left_expression -> value);
                IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> right_expression -> value);
				if (left && right) {
					expr->value = lookup_env.nameEnv->int_pool.FindOrInsert(left->value | right->value);
				}
            }
        }
    }
}


void SemanticForWatch::ProcessAND_AND(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type != lookup_env.boolean_type)
    {
    
    }
    if (right_type != lookup_env.boolean_type)
    {
       
    }
    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        //
        // Even when evaluating false && x, x must be constant for && to
        // be a constant expression according to JLS2 15.28.
        //
        expr -> value = lookup_env.nameEnv->int_pool.
            FindOrInsert((IsConstantTrue(expr -> left_expression) &&
                          IsConstantTrue(expr -> right_expression))
                         ? 1 : 0);
    }
    if (expr -> symbol != lookup_env.no_type)
        expr -> symbol = lookup_env.boolean_type;
}


void SemanticForWatch::ProcessOR_OR(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type != lookup_env.boolean_type)
    {
        
    }
    if (right_type != lookup_env.boolean_type)
    {
       
    }
    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        //
        // Even when evaluating true || x, x must be constant for || to
        // be a constant expression according to JLS2 15.28.
        //
        expr -> value = lookup_env.nameEnv->int_pool.
            FindOrInsert((IsConstantTrue(expr -> left_expression) ||
                          IsConstantTrue(expr -> right_expression))
                         ? 1 : 0);
    }
 
}


void SemanticForWatch::ProcessEQUAL_EQUAL(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpressionOrStringConstant(expr -> left_expression);
    ProcessExpressionOrStringConstant(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

	if (!left_type || !right_type)
		return;

    if (left_type == lookup_env.void_type || right_type == lookup_env.void_type)
	{
		
    }
    else if (left_type -> Primitive() && right_type -> Primitive())
    {
        if (left_type == lookup_env.boolean_type ||
            right_type == lookup_env.boolean_type)
        {
            if (left_type != right_type)
            {
              
            }
        }
        else BinaryNumericPromotion(expr);
        if (expr -> symbol != lookup_env.no_type)
            expr -> symbol = lookup_env.boolean_type;
    }
    else if (CanCastConvert(left_type, right_type,
                            expr -> binary_operator_token) ||
             (left_type == lookup_env.null_type &&
              (right_type == lookup_env.null_type ||
               right_type -> IsSubclass(lookup_env.Object()))))
    {
        expr -> symbol = lookup_env.boolean_type;
    }
    else
    {
        
    }

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        LiteralValue* left = expr -> left_expression -> value;
        LiteralValue* right = expr -> right_expression -> value;

        //
        // Check double and float separately from long, int, and String; since
        // 0.0 and NaNs cause weird behavior.
        //
        if (expr -> left_expression -> Type() == lookup_env.double_type)
        {
            DoubleLiteralValue* ad_left = DYNAMIC_CAST<DoubleLiteralValue*>
                (left);
            DoubleLiteralValue* ad_right = DYNAMIC_CAST<DoubleLiteralValue*>
                (right);

			if(ad_left && ad_right)
			{
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(ad_left->value ==
						ad_right->value ? 1 : 0);
			}

        }
        else if (expr -> left_expression -> Type() == lookup_env.float_type)
        {
            FloatLiteralValue* ad_left = DYNAMIC_CAST<FloatLiteralValue*>
                (left);
            FloatLiteralValue* ad_right = DYNAMIC_CAST<FloatLiteralValue*>
                (right);
			if (ad_left && ad_right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(ad_left->value ==
						ad_right->value ? 1 : 0);
			}
        }
        else expr -> value =
                 lookup_env.nameEnv->int_pool.FindOrInsert(left == right ? 1 : 0);
    }
}


void SemanticForWatch::ProcessNOT_EQUAL(BinaryExpression* expr)
{

	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpressionOrStringConstant(expr -> left_expression);
    ProcessExpressionOrStringConstant(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.void_type || right_type == lookup_env.void_type)
    {
       
    }
    else if (left_type -> Primitive() && right_type -> Primitive())
    {
        if (left_type == lookup_env.boolean_type ||
            right_type == lookup_env.boolean_type)
        {
            if (left_type != right_type)
            {
                
            }
        }
        else BinaryNumericPromotion(expr);
        if (expr -> symbol != lookup_env.no_type)
            expr -> symbol = lookup_env.boolean_type;
    }
    else if (CanCastConvert(left_type, right_type,
                            expr -> binary_operator_token) ||
             (left_type == lookup_env.null_type &&
              (right_type == lookup_env.null_type ||
               right_type -> IsSubclass(lookup_env.Object()))))
    {
        expr -> symbol = lookup_env.boolean_type;
    }
    else
    {
       
    }

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        LiteralValue* left = expr -> left_expression -> value;
        LiteralValue* right = expr -> right_expression -> value;

        //
        // Check double and float separately from long, int, and String; since
        // 0.0 and NaNs cause weird behavior.
        //
        if (expr -> left_expression -> Type() == lookup_env.double_type)
        {
            DoubleLiteralValue* ad_left = DYNAMIC_CAST<DoubleLiteralValue*>(left);
             
            DoubleLiteralValue* ad_right = DYNAMIC_CAST<DoubleLiteralValue*>(right);
 
            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(ad_left -> value !=
                                              ad_right -> value ? 1 : 0);
        }
        else if (expr -> left_expression -> Type() == lookup_env.float_type)
        {
            FloatLiteralValue* ad_left = DYNAMIC_CAST<FloatLiteralValue*>
                (left);
            FloatLiteralValue* ad_right = DYNAMIC_CAST<FloatLiteralValue*>
                (right);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(ad_left -> value !=
                                              ad_right -> value ? 1 : 0);
        }
        else expr -> value =
                 lookup_env.nameEnv->int_pool.FindOrInsert(left != right ? 1 : 0);
    }
}


void SemanticForWatch::ProcessSTAR(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);
    BinaryNumericPromotion(expr);

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (expr -> Type() == lookup_env.double_type)
        {
            DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> left_expression -> value);
            DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->double_pool.FindOrInsert(left->value *
						right->value);
			}
        }
        else if (expr -> Type() == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
            expr -> value =
                lookup_env.nameEnv->float_pool.FindOrInsert(left -> value *
                                                right -> value);
			}
        }
        else if (expr -> Type() == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);
            CheckIntegerMultiplication(this, expr,
                                       left -> value, right -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->long_pool.FindOrInsert(left->value *
						right->value);
			}
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
            CheckIntegerMultiplication(this, expr,
                                       left -> value, right -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value *
						right->value);
			}
        }
    }
}


void SemanticForWatch::ProcessMINUS(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);
    BinaryNumericPromotion(expr);

    if (expr -> left_expression -> IsConstant() &&
        expr -> right_expression -> IsConstant())
    {
        if (expr -> Type() == lookup_env.double_type)
        {
            DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> left_expression -> value);
            DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->double_pool.FindOrInsert(left->value -
						right->value);
			}
        }
        else if (expr -> Type() == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->float_pool.FindOrInsert(left->value -
						right->value);
			}
        }
        else if (expr -> Type() == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);
            CheckIntegerSubtraction(this, expr, left -> value, right -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->long_pool.FindOrInsert(left->value -
						right->value);
			}
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
            CheckIntegerSubtraction(this, expr, left -> value, right -> value);
			if (left && right) {
				expr->value =
					lookup_env.nameEnv->int_pool.FindOrInsert(left->value - right->value);
			}
        }
    }
}


void SemanticForWatch::ProcessSLASH(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);
    BinaryNumericPromotion(expr);

    Expression* left_expression = expr -> left_expression;
    Expression* right_expression = expr -> right_expression;
    if (right_expression -> IsConstant())
    {
        //
        // If the type of the expression is int or long and the right-hand
        // side is 0 then issue an error message. Otherwise, if both
        // subexpressions are constant, calculate result.
        //
        if ((expr -> Type() == lookup_env.int_type &&
             DYNAMIC_CAST<IntLiteralValue*> (right_expression -> value) -> value == 0) ||
            (expr -> Type() == lookup_env.long_type &&
             DYNAMIC_CAST<LongLiteralValue*> (right_expression -> value) -> value == 0))
        {
            //
            // This will guarantee a runtime exception, but the
            // clarifications to JLS2 insist it is legal code.
            //
          
        }
        else if (left_expression -> IsConstant())
        {
            if (expr -> Type() == lookup_env.double_type)
            {
                DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                    (left_expression -> value);
                DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                    (right_expression -> value);
				if (left && right) {
					expr->value =
						lookup_env.nameEnv->double_pool.FindOrInsert(left->value /
							right->value);
				}
            }
            else if (expr -> Type() == lookup_env.float_type)
            {
                FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                    (left_expression -> value);
                FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                    (right_expression -> value);
				if (left && right) {
					expr->value =
						lookup_env.nameEnv->float_pool.FindOrInsert(left->value /
							right->value);
				}
            }
            else if (expr -> Type() == lookup_env.long_type)
            {
                LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                    (left_expression -> value);
                LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                    (right_expression -> value);
                CheckIntegerDivision(this, expr, left -> value,
                                     right -> value);
				if (left && right) {
					expr->value =
						lookup_env.nameEnv->long_pool.FindOrInsert(left->value /
							right->value);
				}
            }
            else if (expr -> Type() == lookup_env.int_type)
            {
                IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                    (left_expression -> value);
                IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                    (right_expression -> value);
                CheckIntegerDivision(this, expr, left -> value,
                                     right -> value);
                //
                // There is a bug in the intel hardware where if one tries
                // to compute ((2**32-1) / -1), he gets a ZeroDivide
                // exception. Thus, instead of using the straightforward
                // code below, we use the short-circuited one that follows:
                //
                //  expr -> value = control.nameEnv->int_pool
                //      .FindOrInsert(left -> value / right -> value);
                //
				if (left && right) {
					expr->value = lookup_env.nameEnv->int_pool
						.FindOrInsert(right->value == -1
							? -(left->value)
							: left->value / right->value);
				}
            }
        }
    }
}


void SemanticForWatch::ProcessMOD(BinaryExpression* expr)
{
	if (!expr || !expr->left_expression || !expr->right_expression
		||       !expr->left_expression->value || !expr->right_expression->value)
	{
		return;
	}
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);
    BinaryNumericPromotion(expr);

    Expression* left_expression = expr -> left_expression;
    Expression* right_expression = expr -> right_expression;

    if (right_expression -> IsConstant())
    {
        //
        // If the type of the expression is int or long and the right-hand
        // side is 0 then issue an error message. Otherwise, if both
        // subexpressions are constant, calculate result.
        //
        if ((expr -> Type() == lookup_env.int_type &&
             DYNAMIC_CAST<IntLiteralValue*> (right_expression -> value) -> value == 0) ||
            (expr -> Type() == lookup_env.long_type &&
             DYNAMIC_CAST<LongLiteralValue*> (right_expression -> value) -> value == 0))
        {
			unreachable = true;
			return;
        }
        else if (left_expression -> IsConstant())
        {
            if (expr -> Type() == lookup_env.double_type)
            {
                DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                    (left_expression -> value);
                DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                    (right_expression -> value);

                expr -> value =
                    lookup_env.nameEnv->double_pool.FindOrInsert(left -> value %
                                                     right -> value);
            }
            else if (expr -> Type() == lookup_env.float_type)
            {
                FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                    (left_expression -> value);
                FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                    (right_expression -> value);

                expr -> value =
                    lookup_env.nameEnv->float_pool.FindOrInsert(left -> value %
                                                    right -> value);
            }
            else if (expr -> Type() == lookup_env.long_type)
            {
                LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                    (left_expression -> value);
                LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                    (right_expression -> value);

                expr -> value =
                    lookup_env.nameEnv->long_pool.FindOrInsert(left -> value %
                                                   right -> value);
            }
            else if (expr -> Type() == lookup_env.int_type)
            {
                IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                    (left_expression -> value);
                IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                    (right_expression -> value);

                //
                // There is a bug in the intel hardware where if one tries
                // to compute ((2**32-1) / -1), he gets a ZeroDivide
                // exception. Thus, instead of using the straightforward
                // code below, we use the short-circuited one that follows:
                //
                // expr -> value = control.nameEnv->int_pool
                //     .FindOrInsert(left -> value % right -> value);
                //
                expr -> value = lookup_env.nameEnv->int_pool
                    .FindOrInsert((left -> value  == (signed) 0x80000000 &&
                                   right -> value == (signed) 0xffffffff)
                                  ? 0 : left -> value % right -> value);
            }
        }
    }
}


void SemanticForWatch::ProcessBinaryExpression(Ast* expr)
{
	if (!expr)
		return;

    BinaryExpression* binary_expression = (BinaryExpression*) expr;
    (this ->* ProcessBinaryExpr[binary_expression -> Tag()])
        (binary_expression);
}


void SemanticForWatch::ProcessInstanceofExpression(Ast* expr)
{
	unreachable = true;
}



void SemanticForWatch::ProcessConditionalExpression(Ast* expr)
{
    ConditionalExpression* conditional_expression =
        (ConditionalExpression*) expr;

	if (!expr || !conditional_expression->test_expression 
		|| !conditional_expression->true_expression || conditional_expression->false_expression)
		return;

    ProcessExpression(conditional_expression -> test_expression);
    //
    // TODO: Should we delay calculating results of true/false expressions
    // until CheckStringConstant in lookup.cpp to put fewer intermediate
    // strings in the storage pools?
    //
    ProcessExpressionOrStringConstant(conditional_expression ->
                                      true_expression);
    ProcessExpressionOrStringConstant(conditional_expression ->
                                      false_expression);

    TypeSymbol* test_type =
        conditional_expression -> test_expression -> Type();
    TypeSymbol* true_type =
        conditional_expression -> true_expression -> Type();
    TypeSymbol* false_type =
        conditional_expression -> false_expression -> Type();

	if (!test_type || !true_type || false_type)
		return;

    if (test_type != lookup_env.boolean_type)
    {
        conditional_expression -> symbol = lookup_env.no_type;
    }
    if (true_type == lookup_env.void_type)
    {
        true_type = lookup_env.no_type;
    }
    if (false_type == lookup_env.void_type)
    {
        false_type = lookup_env.no_type;
    }
    if (true_type == lookup_env.no_type || false_type == lookup_env.no_type)
        conditional_expression -> symbol = lookup_env.no_type;
    else if (true_type -> Primitive())
    {
        if (! false_type -> Primitive() ||
            (true_type != false_type &&
             (true_type == lookup_env.boolean_type ||
              false_type == lookup_env.boolean_type)))
        {
            conditional_expression -> symbol = lookup_env.no_type;
        }
        else // must be a primitive type
        {
            if (true_type == false_type)
            {
                if (conditional_expression -> symbol != lookup_env.no_type)
                    conditional_expression -> symbol = true_type;
            }
            else // must be mixed numeric types
            {
                if (true_type == lookup_env.byte_type &&
                    false_type == lookup_env.short_type)
                {
                    conditional_expression -> true_expression =
                        ConvertToType(conditional_expression -> true_expression,
                                      lookup_env.short_type);
                    conditional_expression -> symbol = lookup_env.short_type;
                }
                else if (true_type == lookup_env.short_type &&
                         false_type == lookup_env.byte_type)
                {
                    conditional_expression -> false_expression =
                        ConvertToType(conditional_expression -> false_expression,
                                      lookup_env.short_type);
                    conditional_expression -> symbol = lookup_env.short_type;
                }
                else if (true_type == lookup_env.int_type &&
                         lookup_env.IsSimpleIntegerValueType(true_type) &&
                         IsIntValueRepresentableInType(conditional_expression -> true_expression,
                                                       false_type))
                {
                    conditional_expression -> true_expression =
                        ConvertToType(conditional_expression -> true_expression,
                                      false_type);
                    conditional_expression -> symbol = false_type;
                }
                else if (false_type == lookup_env.int_type &&
                         lookup_env.IsSimpleIntegerValueType(false_type) &&
                         IsIntValueRepresentableInType(conditional_expression -> false_expression,
                                                       true_type))
                {
                    conditional_expression -> false_expression =
                        ConvertToType(conditional_expression -> false_expression,
                                      true_type);
                    conditional_expression -> symbol = true_type;
                }
                else BinaryNumericPromotion(conditional_expression);
            }

            //
            // Even when evaluating 'true ? constant : x' or
            // 'false ? x : constant', x must be constant for ?: to be a
            // constant expression according to JLS2 15.28.
            //
            if (conditional_expression -> true_expression -> IsConstant() &&
                conditional_expression -> false_expression -> IsConstant())
            {
                if (IsConstantTrue(conditional_expression -> test_expression))
                    conditional_expression -> value =
                        conditional_expression -> true_expression -> value;
                else if (IsConstantFalse(conditional_expression -> test_expression))
                    conditional_expression -> value =
                        conditional_expression -> false_expression -> value;
            }
        }
    }
    else // true_type is reference
    {

        if (conditional_expression -> true_expression -> IsConstant() &&
            conditional_expression -> false_expression -> IsConstant())
        {
            if (IsConstantTrue(conditional_expression -> test_expression))
                conditional_expression -> value =
                    conditional_expression -> true_expression -> value;
            else if (IsConstantFalse(conditional_expression -> test_expression))
                conditional_expression -> value =
                    conditional_expression -> false_expression -> value;
        }
    }
}


void SemanticForWatch::ProcessAssignmentExpression(Ast* expr)
{
    AstAssignmentExpression* assignment_expression =
        (AstAssignmentExpression*) expr;

	if(!expr)
		return;
    ProcessExpressionOrStringConstant(assignment_expression -> expression);

    Expression* left_hand_side = assignment_expression -> left_hand_side;
	if (!left_hand_side)
		return;
    //
    // JLS2 added ability for parenthesized variable to remain a variable.
    // Therefore, the grammar was changed to accept all expressions, to avoid
    // ambiguities, and we must filter out invalid left-hand sides.
    //
    if ( AstParenthesizedExpression::ParenthesizedExpressionCast(left_hand_side))
    {
		unreachable = true;
		return;
    }

    //
    // JLS2 8.3.2.3 permits simple assignment to a variable that has not
    // yet been declared in an initializer.  If the left_hand_side is a
    // variable, we use ProcessingSimpleAssignment() to inform
    // CheckSimpleName() to treat it specially.
    //
    if ((assignment_expression -> Tag() ==
         AssignmentExpressionEnum::SIMPLE_EQUAL) &&
        AstName::NameCast(left_hand_side) &&
        ! AstName::NameCast(left_hand_side) -> base_opt)
    {
       
    }

    ProcessExpression(left_hand_side);
  

	
	TypeSymbol* left_type = left_hand_side->Type();
	TypeSymbol* right_type = assignment_expression->expression->Type();

    assignment_expression -> symbol = left_type;

   
   
   
   

    switch (assignment_expression -> Tag())
    {
        case AssignmentExpressionEnum::PLUS_EQUAL:
        case AssignmentExpressionEnum::STAR_EQUAL:
        case AssignmentExpressionEnum::MINUS_EQUAL:
            BinaryNumericPromotion(assignment_expression);
            break;
        case AssignmentExpressionEnum::SLASH_EQUAL:
        case AssignmentExpressionEnum::MOD_EQUAL:
            BinaryNumericPromotion(assignment_expression);
            {
                Expression* right_expression =
                    assignment_expression -> expression;
                if (right_expression -> IsConstant())
                {
					if(right_expression->value)
					{
						//
						// If the type of the expression is integral and the right
						// hand side is constant 0 then issue an error message.
						//
						if ((right_expression->Type() == lookup_env.int_type &&
							DYNAMIC_CAST<IntLiteralValue*>
							(right_expression->value)->value == 0) ||
							(right_expression->Type() == lookup_env.long_type &&
								DYNAMIC_CAST<LongLiteralValue*>
								(right_expression->value)->value == 0))
						{


						}
					}

                }
            }
            break;
        case AssignmentExpressionEnum::LEFT_SHIFT_EQUAL:
        case AssignmentExpressionEnum::RIGHT_SHIFT_EQUAL:
        case AssignmentExpressionEnum::UNSIGNED_RIGHT_SHIFT_EQUAL:
            assignment_expression -> left_hand_side
                = PromoteUnaryNumericExpression(left_hand_side);
            if (! lookup_env.IsIntegral(left_type))
            {
                
            }
            //
            // This call captures both unary numeric conversion (widening) of
            // byte, char, or short, and narrowing of long, since the bytecode
            // requires an int shift amount.
            //
            if (! lookup_env.IsIntegral(right_type))
            {
             
            }
            assignment_expression -> expression =
                ConvertToType(assignment_expression -> expression,
                              lookup_env.int_type);
            ProcessShiftCount(left_type, assignment_expression -> expression);
            break;
        case AssignmentExpressionEnum::AND_EQUAL:
        case AssignmentExpressionEnum::XOR_EQUAL:
        case AssignmentExpressionEnum::IOR_EQUAL:
            if (left_type == lookup_env.boolean_type)
            {
                if (right_type != lookup_env.boolean_type)
                {
                   
                }
            }
            else
            {
                if (! lookup_env.IsIntegral(left_type))
                {
                   
                }
                if (! lookup_env.IsIntegral(right_type))
                {
                  
                }
                BinaryNumericPromotion(assignment_expression);
            }
            break;
        default:
            assert(false);
            break;
    }
}
}

} // Close namespace Jikes block


