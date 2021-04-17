
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
#include <boost/algorithm/string.hpp>

namespace Jikes { // Open namespace Jikes block


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

inline void ReportOverflow(Semantic* semantic, Expression* expr, bool safe)
{
    if (! safe)
    {
        semantic -> ReportSemErrorWithAst(SemanticError::CONSTANT_OVERFLOW, expr,
                                   expr -> Type() -> Name());
    }
}


template <typename T>
static void CheckIntegerNegation(Semantic* semantic, Expression* expr,
                                 const T& x)
{
    T min, max;
    ExtremaForType(min, max);
    ReportOverflow(semantic, expr, (x != min));
}


template <typename T>
inline void CheckIntegerAddition(Semantic* semantic, Expression* expr,
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
static void CheckIntegerSubtraction(Semantic* semantic, Expression* expr,
                                    const T& x, const T& y)
{
    CheckIntegerAddition(semantic, expr, x, T(-y));
}


template <typename T>
static void CheckIntegerMultiplication(Semantic* semantic, Expression* expr,
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
static void CheckIntegerDivision(Semantic* semantic, Expression* expr,
                                 const T& x, const T& y)
{
    const T zero = T(0);
    const T minus_one = T(-1);
    T min, max;
    ExtremaForType(min, max);
    bool safe = (y != zero) && !(x == min && y == minus_one);
    ReportOverflow(semantic, expr, safe);
}


bool Semantic::IsIntValueRepresentableInType(Expression* expr,
                                             const TypeSymbol* type)
{
    if (! expr -> IsConstant() ||
        ! lookup_env.IsSimpleIntegerValueType(expr -> Type()))
    {
        return false;
    }

    IntLiteralValue* literal = DYNAMIC_CAST<IntLiteralValue*> (expr -> value);
    return type == lookup_env.int_type || type == lookup_env.no_type ||
        (type == lookup_env.char_type && (literal -> value >= 0) &&
         (literal -> value <= 65535)) ||
        (type == lookup_env.byte_type && (literal -> value >= -128) &&
         (literal -> value <= 127)) ||
        (type == lookup_env.short_type && (literal -> value >= -32768) &&
         (literal -> value <= 32767));
}


bool Semantic::IsConstantTrue(Expression* expr)
{
    return expr -> IsConstant() && expr -> Type() == lookup_env.boolean_type &&
        DYNAMIC_CAST<IntLiteralValue*> (expr -> value) -> value;
}


bool Semantic::IsConstantFalse(Expression* expr)
{
    return expr -> IsConstant() && expr -> Type() == lookup_env.boolean_type &&
        ! DYNAMIC_CAST<IntLiteralValue*> (expr -> value) -> value;
}


//
// Returns true if source_method is more specific than target_method, which
// is defined as the type that declared the method, as well as all method
// parameter types, being equal or more specific in the source_method.
//
inline bool Semantic::MoreSpecific(MethodSymbol* source_method,
                                   MethodSymbol* target_method)
{
    //
    // Sun bug 4761586: the declaration type is no longer considered when
    // looking for the most specific method.
    //
//      if (! CanMethodInvocationConvert(target_method -> containing_type,
//                                       source_method -> containing_type))
//      {
//          return false;
//      }
    for (int k = target_method -> NumFormalParameters() - 1; k >= 0; k--)
    {
        if (! CanMethodInvocationConvert(target_method -> FormalParameter(k) ->
                                         Type(),
                                         source_method -> FormalParameter(k) ->
                                         Type()))
        {
            return false;
        }
    }
    return true;
}


//
// Returns true if a method is more specific than the current set of maximally
// specific methods.
//
inline bool Semantic::MoreSpecific(MethodSymbol* method,
                                   Tuple<MethodSymbol*>& maximally_specific_method)
{
    for (unsigned i = 0; i < maximally_specific_method.Length(); i++)
    {
        if (! MoreSpecific(method, maximally_specific_method[i]))
            return false;
    }
    return true;
}


//
// Returns true if no method in the current set of maximally specific methods
// is more specific than the given method, meaning that the given method should
// be added to the set.
//
inline bool Semantic::NoMethodMoreSpecific(Tuple<MethodSymbol*>& maximally_specific_method,
                                           MethodSymbol* method)
{
    for (unsigned i = 0; i < maximally_specific_method.Length(); i++)
    {
        if (MoreSpecific(maximally_specific_method[i], method))
            return false;
    }
    return true;
}


//
// Returns true if a method is more specific than the current set of maximally
// specific methods.
//
inline bool Semantic::MoreSpecific(MethodSymbol* method,
                                   Tuple<MethodShadowSymbol*>& maximally_specific_method)
{
    for (unsigned i = 0; i < maximally_specific_method.Length(); i++)
    {
        if (! MoreSpecific(method,
                           maximally_specific_method[i] -> method_symbol))
            return false;
    }
    return true;
}


//
// Returns true if no method in the current set of maximally specific methods
// is more specific than the given method, meaning that the given method should
// be added to the set.
//
inline bool Semantic::NoMethodMoreSpecific(Tuple<MethodShadowSymbol*>& maximally_specific_method,
                                           MethodSymbol* method)
{
    for (unsigned i = 0; i < maximally_specific_method.Length(); i++)
    {
        if (MoreSpecific(maximally_specific_method[i] -> method_symbol, method))
            return false;
    }
    return true;
}


//
// Creates a new wchar_t[] containing the type of the method or constructor
// overload for printing in Report*NotFound. Caller is responsible for
// calling delete[] on the result.
//
wchar_t* Semantic::Header(const NameSymbol* name, AstArguments* args)
{
    unsigned num_arguments = args -> NumArguments();
    int length = name -> NameLength();
    for (unsigned i = 0; i < num_arguments; i++)
    {
        TypeSymbol* arg_type = args -> Argument(i) -> Type();
        // '.' after package_name; ',' and ' ' to separate this argument
        // from the next one
        length += arg_type -> ContainingPackage() -> PackageNameLength() +
            arg_type -> ExternalNameLength() + 3;
    }

    // +1 for (, +1 for ), +1 for '\0'
    wchar_t* header = new wchar_t[length + 3];
    wchar_t* s = header;
    const wchar_t* s2;

    for (s2 = name -> Name(); *s2; s2++)
        *s++ = *s2;
    *s++ = U_LEFT_PARENTHESIS;
    if (num_arguments > 0)
    {
        for (unsigned i = 0; i < num_arguments; i++)
        {
            TypeSymbol* arg_type = args -> Argument(i) -> Type();

            PackageSymbol* package = arg_type -> ContainingPackage();
            wchar_t* package_name = package -> PackageName();
            if (package -> PackageNameLength() > 0 &&
                package_name[0] != U_DOT)
            {
                while (*package_name)
                {
                    *s++ = (*package_name == U_SLASH ? (wchar_t) U_DOT
                            : *package_name);
                    package_name++;
                }
                *s++ = U_DOT;
            }

            for (s2 = arg_type -> ExternalName(); *s2; s2++)
                *s++ = (*s2 == U_DOLLAR ? (wchar_t) U_DOT : *s2);
            *s++ = U_COMMA;
            *s++ = U_SPACE;
        }

        s -= 2; // remove the last ',' and ' '
    }
    *s++ = U_RIGHT_PARENTHESIS;
    *s = U_NULL;
    return header;
}


//
// Called when no accessible method was found. This checks in order: a hidden
// exact match in an enclosing class (for simple names only); an accessible
// method of the same name but different parameter types, favoring methods with
// the same parameter count; an accessible field by the same name (for no-arg
// call only); an inaccessible method in a superclass; a misspelled method
// name; a type by the same name; and finally no method was found. The
// parameter type should be NULL only if method_call represents a simple name.
//
void Semantic::ReportMethodNotFound(AstMethodInvocation* method_call,
                                    TypeSymbol* type)
{
	
	Expression* base = method_call->base_opt;
	SemanticEnvironment* env;
	SemanticEnvironment* top_env = state_stack.Top();
	assert((base == NULL) == (type == NULL));


    Token* id_token = method_call -> identifier_token;
    NameSymbol* name_symbol = method_call->identifier_token->GetNameSymbol();
    MethodShadowSymbol* method_shadow;

    //
    // First, for simple names, search for a hidden method match in an
    // enclosing class.
    //
    for (env = top_env -> previous; ! base && env; env = env -> previous)
    {
        Tuple<MethodShadowSymbol*> others(2);
        SemanticEnvironment* found_other;
        FindMethodInEnvironment(others, found_other, env, method_call);
        if (others.Length() > 0)
        {
            ReportSemErrorWithAst(SemanticError::HIDDEN_METHOD_IN_ENCLOSING_CLASS,
                           method_call, others[0] -> method_symbol -> Header(),
                           others[0] -> method_symbol -> containing_type -> ContainingPackageName(),
                           others[0] -> method_symbol -> containing_type -> ExternalName());
            return;
        }
    }

    //
    // Search for an accessible method with different arguments. Favor the
    // earliest method found with the smallest difference in parameter count.
    // Since the JVMS limits methods to 255 parameters, we initialize our
    // difference detection with 255.
    //
    MethodSymbol* best_match = NULL;
    for (env = top_env; env;
         env = (base ? (SemanticEnvironment*) NULL : env -> previous))
    {
        if (! base)
            type = env -> Type();
        if (! type -> expanded_method_table)
            ComputeMethodsClosure(type, method_call->identifier_token);
        int difference = 255;
        for (method_shadow = type -> expanded_method_table ->
                 FindMethodShadowSymbol(name_symbol);
             method_shadow; method_shadow = method_shadow -> next_method)
        {
            MethodSymbol* method = method_shadow -> method_symbol;

            if (! method -> IsTyped())
                method -> ProcessMethodSignature(this, id_token);
            if (MemberAccessCheck(type, method, base) ||
                method_shadow -> NumConflicts() > 0)
            {
                int diff = method_call -> arguments -> NumArguments() -
                    method -> NumFormalParameters();
                if (diff < 0)
                    diff = - diff;
                if (diff < difference)
                {
                    best_match = method;
                    difference = diff;
                }
            }
        }
        if (best_match)
        {
            wchar_t* header = Header(name_symbol, method_call -> arguments);

			    ReportSemErrorSimple(
						   SemanticError::METHOD_OVERLOAD_NOT_FOUND,
                           method_call->identifier_token,
						   header,
                           best_match -> containing_type -> ContainingPackageName(),
                           best_match -> containing_type -> ExternalName(),
                           best_match -> Header());
            delete [] header;
            return;
        }
    }

    //
    // For a no-arg method, search for an accessible field of the same name.
    //
    if (method_call -> arguments -> NumArguments() == 0)
    {
        for (env = top_env; env;
             env = (base ? (SemanticEnvironment*) NULL : env -> previous))
        {
            if (! base)
                type = env -> Type();
            if (! type -> expanded_field_table)
                ComputeFieldsClosure(type, id_token);
            VariableShadowSymbol* variable_shadow = type ->
                expanded_field_table -> FindVariableShadowSymbol(name_symbol);
            if (variable_shadow)
            {
                VariableSymbol* variable = variable_shadow -> variable_symbol;
                if (MemberAccessCheck(type, variable))
                {
                    TypeSymbol* enclosing_type =
                      TypeSymbol::TypeCast(variable->owner);
                    assert(enclosing_type);
                    ReportSemErrorWithAst(SemanticError::FIELD_NOT_METHOD,
                                   method_call, variable -> Name(),
                                   enclosing_type -> ContainingPackageName(),
                                   enclosing_type -> ExternalName());
                    return;
                }
            }
        }
    }

    //
    // Check if the method is inaccessible.
    //
    for (TypeSymbol* super_type = type;
         super_type; super_type = super_type -> super)
    {
        for (method_shadow = super_type -> expanded_method_table ->
                 FindMethodShadowSymbol(name_symbol);
             method_shadow; method_shadow = method_shadow -> next_method)
        {
            MethodSymbol* method = method_shadow -> method_symbol;
            if (! method -> IsTyped())
                method -> ProcessMethodSignature(this, id_token);

            if (method_call -> arguments -> NumArguments() ==
                method -> NumFormalParameters())
            {
                unsigned i;
                for (i = 0;
                     i < method_call -> arguments -> NumArguments(); i++)
                {
                    Expression* expr =
                        method_call -> arguments -> Argument(i);
                    if (! CanMethodInvocationConvert(method -> FormalParameter(i) -> Type(),
                                                     expr -> Type()))
                    {
                        break;
                    }
                }
                if (i == method_call -> arguments -> NumArguments())
                {
                    //
                    // JLS 9.2: Interfaces do not have protected members,
                    // even though jikes treats interfaces as subtypes of
                    // Object.
                    //
                    if (base && method -> ACC_PROTECTED() &&
                        base -> Type() -> ACC_INTERFACE())
                    {
                        assert(method -> containing_type == lookup_env.Object());
						ReportSemError(SemanticError::PROTECTED_INTERFACE_METHOD_NOT_ACCESSIBLE,
							method_call->identifier_token, method_call->identifier_token, method -> Header());
                    }
                    //
                    // A protected instance method in the superclass is
                    // inaccessible if the base expression is the wrong type.
                    //
                    else if (method -> ACC_PROTECTED() &&
                             ! method -> ACC_STATIC() &&
                             ThisType() -> HasProtectedAccessTo(method -> containing_type))
                    {
                        assert(base);
						ReportSemError(SemanticError::PROTECTED_INSTANCE_METHOD_NOT_ACCESSIBLE,
                                       method_call->identifier_token, method_call->identifier_token,
									   method -> Header(),
                                       method -> containing_type -> ContainingPackageName(),
                                       method -> containing_type -> ExternalName(),
                                       ThisType() -> ContainingPackageName(),
                                       ThisType() -> ExternalName());
                    }
                    else
                    {
   
						ReportSemError(SemanticError::METHOD_NOT_ACCESSIBLE,
							method_call->identifier_token, 
							method_call->identifier_token, method->Header(),
							method->containing_type->ContainingPackageName(),
							method->containing_type->ExternalName(),
							method->AccessString());
                    }
                    return;
                }
            }
        }
    }

    
    // Search for a misspelled method name.
    for (env = top_env; env; env = (base ? (SemanticEnvironment*) NULL : env -> previous)){
        if (! base)
            type = env -> Type();
        best_match = FindMisspelledMethodName(type, method_call, name_symbol);
        if (best_match){
            ReportSemErrorWithAst(SemanticError::METHOD_NAME_MISSPELLED,
                           method_call, name_symbol -> Name(),
                           type -> ContainingPackageName(),
                           type -> ExternalName(), best_match -> Name());
            return;
        }
    }
   
    // Search for a type of the same name.
    if (FindType(id_token))
        ReportSemErrorWithAst(SemanticError::TYPE_NOT_METHOD, method_call,
                       name_symbol -> Name());

    // Give up. We didn't find it.
    else
    {
        if (! base)
            type = ThisType();
        wchar_t* header = Header(name_symbol, method_call -> arguments);
		ReportSemErrorSimple(SemanticError::METHOD_NOT_FOUND,
			method_call->identifier_token,
			header,
			type->ContainingPackageName(),
			type->ExternalName());
        delete [] header;
    }
}


//
// Called when no accessible constructor was found. This checks in order: an
// accessible method of the same name but different parameters, favoring
// constructors with the same parameter count; an inaccessible constructor;
// an accessible method with the same name as the type; and finally no
// constructor was found.
//
void Semantic::ReportConstructorNotFound(Ast* ast, TypeSymbol* type)
{
    AstClassCreationExpression* class_creation =
        AstClassCreationExpression::ClassCreationExpressionCast(ast);
    AstSuperCall* super_call = AstSuperCall::SuperCallCast(ast);
    AstArguments* args;
    Token* left_tok;

    if (class_creation)
    {
        args = class_creation -> arguments;
        left_tok = class_creation -> new_token;
        if (class_creation -> anonymous_type_opt)
            class_creation = NULL;
    }
    else if (super_call)
    {
        args = super_call -> arguments;
        left_tok = super_call -> super_token;
    }
    else
    {
        AstThisCall* this_call = AstThisCall::ThisCallCast(ast);
        assert(this_call);
        args = this_call -> arguments;
        left_tok = this_call -> this_token;
    }
    unsigned num_arguments = args -> NumArguments();
    auto right_tok = args -> right_parenthesis_token;

    //
    // Search for an accessible constructor with different arguments. Favor
    // the earliest ctor found with the smallest difference in parameter count.
    // Since the JVMS limits methods to 255 parameters, we initialize our
    // difference detection with 255.
    //
    MethodSymbol* best_match = NULL;
    MethodSymbol* ctor;
    int difference = 255;
    for (ctor = type -> FindMethodSymbol(lookup_env.nameEnv->init_name_symbol);
         ctor; ctor = ctor -> next_method)
    {
        if (ConstructorAccessCheck(ctor, ! class_creation))
        {
            int diff = num_arguments - ctor -> NumFormalParameters();
            if (diff < 0)
                diff = - diff;
            if (diff < difference)
            {
                best_match = ctor;
                difference = diff;
            }
        }
    }
    if (best_match)
    {
		Token* leftToken;
		Token* rightToken;
		if (class_creation)
		{
			   leftToken =class_creation->class_type->LeftToken();
			   rightToken = class_creation->class_type->RightToken();
		}
		else if (super_call)
		{
			leftToken = super_call->super_token;
			rightToken = super_call->super_token;
		}
		else
		{
			AstThisCall* this_call = AstThisCall::ThisCallCast(ast);		
			leftToken = this_call->this_token;
			rightToken = this_call->this_token;
		}

        wchar_t* header = Header(type -> Identity(), args);
		ReportSemError(SemanticError::CONSTRUCTOR_OVERLOAD_NOT_FOUND, leftToken, rightToken,
                       header, type -> ContainingPackageName(),
                       type -> ExternalName(), best_match -> Header());
        delete [] header;
        return;
    }

    //
    // Check if the constructor is inaccessible.
    //
    for (ctor = type -> FindMethodSymbol(lookup_env.nameEnv->init_name_symbol);
         ctor; ctor = ctor -> next_method)
    {
        if (num_arguments == ctor -> NumFormalParameters())
        {
            unsigned i;
            for (i = 0; i < num_arguments; i++)
            {
                Expression* expr = args -> Argument(i);
                if (! CanMethodInvocationConvert(ctor -> FormalParameter(i) -> Type(),
                                                 expr -> Type()))
                {
                    break;
                }
            }
            if (i == num_arguments) // found a match?
            {
                ReportSemErrorWithAst(SemanticError::CONSTRUCTOR_NOT_ACCESSIBLE, ast,
                               ctor -> Header(),
                               type -> ContainingPackageName(),
                               type -> ExternalName(), ctor -> AccessString());
                return;
            }
        }
    }

    //
    // Search for an accessible method with the same name as the type.
    //
    MethodSymbol* method;
    for (method = type -> FindMethodSymbol(type -> Identity());
         method; method = method -> next_method)
    {
        if (! method -> IsTyped())
            method -> ProcessMethodSignature(this, right_tok);

        if (num_arguments == method -> NumFormalParameters())
        {
            unsigned i;
            for (i = 0; i < num_arguments; i++)
            {
                if (! CanMethodInvocationConvert(method -> FormalParameter(i) -> Type(),
                                                 args -> Argument(i) -> Type()))
                {
                    break;
                }
            }
            if (i == num_arguments)
                break;
        }
    }
    if (method)
    {
        if (method -> declaration)
        {
            MethodDeclaration* method_declaration =
                (MethodDeclaration*) method -> declaration;
            FileLocation loc((method -> containing_type ->
                              semantic_environment -> sem -> lex_stream),
                             (method_declaration -> method_declarator ->
                              identifier_token));
            ReportSemError(SemanticError::METHOD_FOUND_FOR_CONSTRUCTOR,
                           left_tok, right_tok, type -> Name(),
                           loc.location);
        }
        else
        {
            ReportSemError(SemanticError::METHOD_FOUND_FOR_CONSTRUCTOR,
                           left_tok, right_tok, type -> Name(),
                           method -> containing_type -> file_location -> location);
        }
        return;
    }

    //
    // Give up. We didn't find it.
    //
    wchar_t* header = Header(type -> Identity(), args);
    ReportSemErrorWithAst(SemanticError::CONSTRUCTOR_NOT_FOUND, ast, header,
                   type -> ContainingPackageName(), type -> ExternalName());
    delete [] header;
}


MethodSymbol* Semantic::FindConstructor(TypeSymbol* containing_type, Ast* ast,
	Token* left_tok,
                                        Token* right_tok)
{
    if (containing_type == lookup_env.no_type)
        return NULL;

    //
    // If this type is anonymous, we have just generated the constructor,
    // so we know it is the right one.
    //
    if (containing_type -> Anonymous())
    {
        return containing_type -> declaration -> default_constructor ->
            method_symbol;
    }

    AstArguments* args;
    Tuple<MethodSymbol*> constructor_set(2); // Stores constructor overloads.

    AstClassCreationExpression* class_creation =
        AstClassCreationExpression::ClassCreationExpressionCast(ast);
    AstSuperCall* super_call = AstSuperCall::SuperCallCast(ast);

    if (class_creation)
    {
        args = class_creation -> arguments;
        if (class_creation -> anonymous_type_opt)
            class_creation = NULL;
    }
    else if (super_call)
        args = super_call -> arguments;
    else
    {
        AstThisCall* this_call = AstThisCall::ThisCallCast(ast);
        assert(this_call);
        args = this_call -> arguments;
    }

    unsigned num_arguments = args -> NumArguments();
    assert(containing_type -> ConstructorMembersProcessed());
    MethodSymbol* ctor;
    for (ctor = containing_type -> FindMethodSymbol(lookup_env.nameEnv->init_name_symbol);
         ctor; ctor = ctor -> next_method)
    {
        if (! ctor -> IsTyped())
            ctor -> ProcessMethodSignature(this, right_tok);

        if (num_arguments == ctor -> NumFormalParameters() &&
            ConstructorAccessCheck(ctor, ! class_creation))
        {
            unsigned i;
            for (i = 0; i < num_arguments; i++)
            {
                if (! CanMethodInvocationConvert(ctor -> FormalParameter(i) -> Type(),
                                                 args -> Argument(i) -> Type()))
                {
                    break;
                }
            }
            if (i == num_arguments)
            {
                if (MoreSpecific(ctor, constructor_set))
                {
                    constructor_set.Reset();
                    constructor_set.Next() = ctor;
                }
                else if (NoMethodMoreSpecific(constructor_set, ctor))
                    constructor_set.Next() = ctor;
            }
        }
    }

    if (constructor_set.Length() == 0)
    {
        if (! containing_type -> Bad() || NumErrors() == 0)
            ReportConstructorNotFound(ast, containing_type);
        return NULL;
    }
    if (constructor_set.Length() > 1)
    {
        ReportSemError(SemanticError::AMBIGUOUS_CONSTRUCTOR_INVOCATION,
                       left_tok, right_tok, containing_type -> Name(),
                       constructor_set[0] -> Header(),
                       constructor_set[1] -> Header());
    }

    ctor = constructor_set[0];
    if (ctor -> ACC_SYNTHETIC())
    {
        ReportSemError(SemanticError::SYNTHETIC_CONSTRUCTOR_INVOCATION,
                       left_tok, right_tok, ctor -> Header(),
                       containing_type -> ContainingPackageName(),
                       containing_type -> ExternalName());
    }

    //
    // If this constructor came from a class file, make sure that its throws
    // clause has been processed.
    //
    ctor -> ProcessMethodThrows(this, right_tok);

    if (lookup_env.option->deprecation && ctor -> IsDeprecated() &&
        ! InDeprecatedContext())
    {
        ReportSemError(SemanticError::DEPRECATED_CONSTRUCTOR,
                       left_tok, right_tok, ctor -> Header(),
                       ctor -> containing_type -> ContainingPackageName(),
                       ctor -> containing_type -> ExternalName());
    }
    return ctor;
}


//
//
//
VariableSymbol* Semantic::FindMisspelledVariableName(TypeSymbol* type,
                                                     Expression* expr)
{
    FieldReference* field_access = FieldReference::FieldAccessCast(expr);
    AstName* field_name = AstName::NameCast(expr);
    Expression* base =
        field_name ? field_name -> base_opt : field_access -> base;
    VariableSymbol* misspelled_variable = NULL;
    int index = 0;
    auto identifier_token = expr -> RightToken();
    const wchar_t* name =  (identifier_token->NameString());

    for (unsigned k = 0;
         k < type -> expanded_field_table -> symbol_pool.Length(); k++)
    {
        VariableShadowSymbol* variable_shadow =
            type -> expanded_field_table -> symbol_pool[k];
        VariableSymbol* variable = variable_shadow -> variable_symbol;
        if (! variable -> IsTyped())
            variable -> ProcessVariableSignature(this, identifier_token);
        if (! MemberAccessCheck(type, variable, base))
            variable = NULL;
        for (unsigned i = 0;
             ! variable && i < variable_shadow -> NumConflicts(); i++)
        {
            variable = variable_shadow -> Conflict(i);
            if (! variable -> IsTyped())
                variable -> ProcessVariableSignature(this,
                                                     identifier_token);
            if (! MemberAccessCheck(type, variable, base))
                variable = NULL;
        }

        if (variable)
        {
            int new_index = Spell::Index(name, variable -> Name());
            if (new_index > index)
            {
                misspelled_variable = variable;
                index = new_index;
            }
        }
    }

    int length = wcslen(name);
    return (length == 3 && index >= 5) ||
        (length == 4 && index >= 6) ||
        (length >= 5 && index >= 7)
        ? misspelled_variable : (VariableSymbol*) NULL;
}

//
//
//
MethodSymbol* Semantic::FindMisspelledMethodName(
	TypeSymbol* type,
	AstMethodInvocation* method_call,
	NameSymbol* name_symbol, TypeSymbol* thisType
)
{
	Expression* base = method_call->base_opt;
	MethodSymbol* misspelled_method = NULL;
	int index = 0;
	auto identifier_token = method_call->identifier_token;

	for (unsigned k = 0;
		k < type->expanded_method_table->symbol_pool.Length(); k++)
	{
		MethodShadowSymbol* method_shadow =
			type->expanded_method_table->symbol_pool[k];
		MethodSymbol* method = method_shadow->method_symbol;

		if (!method->IsTyped())
			method->ProcessMethodSignature(this, identifier_token);

		if ((method_call->arguments->NumArguments() ==
			method->NumFormalParameters()) &&
			(MemberAccessCheck(type, method, thisType, base) ||
				method_shadow->NumConflicts() > 0))
		{
			unsigned i;
			for (i = 0; i < method_call->arguments->NumArguments(); i++)
			{
				Expression* expr = method_call->arguments->Argument(i);
				if (!CanMethodInvocationConvert(method->FormalParameter(i)->Type(),
					expr->Type()))
				{
					break;
				}
			}
			if (i == method_call->arguments->NumArguments())
			{
				int new_index = Spell::Index(name_symbol->Name(),
					method->Name());
				if (new_index > index)
				{
					misspelled_method = method;
					index = new_index;
				}
			}
		}
	}

	int length = name_symbol->NameLength();
	int num_args = method_call->arguments->NumArguments();

	//
	// If we have a name of length 2, accept >= 30% probality if the function
	// takes at least one argument. If we have a name of length 3,
	// accept >= 50% probality if the function takes at least one argument.
	// Otherwise, if the length of the name is > 3, accept >= 60% probability.
	//
	return index < 3 ? (MethodSymbol*)NULL
		: ((length == 2 && (index >= 3 || num_args > 0)) ||
		(length == 3 && (index >= 5 || num_args > 0)) ||
			(length  > 3 && (index >= 6 || (index >= 5 && num_args > 0))))
		? misspelled_method : (MethodSymbol*)NULL;
}


MethodSymbol* Semantic::FindMisspelledMethodName(TypeSymbol* type,
                                                 AstMethodInvocation* method_call,
                                                 NameSymbol* name_symbol)
{
    Expression* base = method_call -> base_opt;
    MethodSymbol* misspelled_method = NULL;
    int index = 0;
    auto identifier_token = method_call -> identifier_token;

    for (unsigned k = 0;
         k < type -> expanded_method_table -> symbol_pool.Length(); k++)
    {
        MethodShadowSymbol* method_shadow =
            type -> expanded_method_table -> symbol_pool[k];
        MethodSymbol* method = method_shadow -> method_symbol;

        if (! method -> IsTyped())
            method -> ProcessMethodSignature(this, identifier_token);

        if ((method_call -> arguments -> NumArguments() ==
             method -> NumFormalParameters()) &&
            (MemberAccessCheck(type, method, base) ||
             method_shadow -> NumConflicts() > 0))
        {
            unsigned i;
            for (i = 0; i < method_call -> arguments -> NumArguments(); i++)
            {
                Expression* expr = method_call -> arguments -> Argument(i);
                if (! CanMethodInvocationConvert(method -> FormalParameter(i) -> Type(),
                                                 expr -> Type()))
                {
                    break;
                }
            }
            if (i == method_call -> arguments -> NumArguments())
            {
                int new_index = Spell::Index(name_symbol -> Name(),
                                             method -> Name());
                if (new_index > index)
                {
                    misspelled_method = method;
                    index = new_index;
                }
            }
        }
    }

    int length = name_symbol -> NameLength();
    int num_args = method_call -> arguments -> NumArguments();

    //
    // If we have a name of length 2, accept >= 30% probality if the function
    // takes at least one argument. If we have a name of length 3,
    // accept >= 50% probality if the function takes at least one argument.
    // Otherwise, if the length of the name is > 3, accept >= 60% probability.
    //
    return index < 3 ? (MethodSymbol*) NULL
        : ((length == 2 && (index >= 3 || num_args > 0)) ||
           (length == 3 && (index >= 5 || num_args > 0)) ||
           (length  > 3 && (index >= 6 || (index >= 5 && num_args > 0))))
        ? misspelled_method : (MethodSymbol*) NULL;
}


//
// Search the type in question for a method. Note that name_symbol is an
// optional argument. If it was not passed to this function then its default
// value is NULL (see semantic.h) and we assume that the name to search for
// is the name specified in the field_access of the method_call.
//
MethodShadowSymbol* Semantic::FindMethodInType(TypeSymbol* type,
                                               AstMethodInvocation* method_call,
                                               NameSymbol* name_symbol)
{
    Tuple<MethodShadowSymbol*> method_set(2); // Stores method overloads.
    Expression* base = method_call -> base_opt;
  //  TokenIndex id_token = method_call -> identifier_token;
	auto id_token = method_call->identifier_token;
    assert(base);
    if (! name_symbol)
        name_symbol = method_call->identifier_token->GetNameSymbol();
    if (! type -> expanded_method_table)
        ComputeMethodsClosure(type, id_token);

    //
    // Here, we ignore any conflicts in a method declaration. If there are
    // conflicts, they are necessarily abstract methods inherited from
    // interfaces, so either the original method implements them all, or it
    // is also abstract and we are free to choose which one to use.
    //
    for (MethodShadowSymbol* method_shadow = type -> expanded_method_table ->
             FindMethodShadowSymbol(name_symbol);
         method_shadow; method_shadow = method_shadow -> next_method)
    {
        MethodSymbol* method = method_shadow -> method_symbol;

        if (! method -> IsTyped())
            method -> ProcessMethodSignature(this, id_token);

        //
        // If there are method shadow conflicts, they are necessarily public
        // abstract methods inherited from interfaces; and we can skip the
        // member access check because we can always invoke the public version.
        //
        if ((method_call -> arguments -> NumArguments() ==
             method -> NumFormalParameters()) &&
            (MemberAccessCheck(type, method, base) ||
             method_shadow -> NumConflicts() > 0))
        {
            unsigned i;
            for (i = 0; i < method_call -> arguments -> NumArguments(); i++)
            {
                Expression* expr = method_call -> arguments -> Argument(i);
                if (! CanMethodInvocationConvert(method -> FormalParameter(i) -> Type(),
                                                 expr -> Type()))
                {
                    break;
                }
            }
            if (i == method_call -> arguments -> NumArguments())
            {
                if (MoreSpecific(method, method_set))
                {
                    method_set.Reset();
                    method_set.Next() = method_shadow;
                }
                else if (NoMethodMoreSpecific(method_set, method))
                    method_set.Next() = method_shadow;
            }
        }
    }

    if (method_set.Length() == 0)
    {
        ReportMethodNotFound(method_call, type);
        return NULL;
    }
    else if (method_set.Length() > 1)
    {
        ReportSemErrorWithAst(SemanticError::AMBIGUOUS_METHOD_INVOCATION,
                       method_call, name_symbol -> Name(),
                       method_set[0] -> method_symbol -> Header(),
                       method_set[0] -> method_symbol -> containing_type -> ContainingPackageName(),
                       method_set[0] -> method_symbol -> containing_type -> ExternalName(),
                       method_set[1] -> method_symbol -> Header(),
                       method_set[1] -> method_symbol -> containing_type -> ContainingPackageName(),
                       method_set[1] -> method_symbol -> containing_type -> ExternalName());
    }

    MethodSymbol* method = method_set[0] -> method_symbol;
    if (method -> ACC_SYNTHETIC())
    {
        ReportSemErrorWithAst(SemanticError::SYNTHETIC_METHOD_INVOCATION,
                       method_call, method -> Header(),
                       method -> containing_type -> ContainingPackageName(),
                       method -> containing_type -> ExternalName());
    }

    //
    // If this method came from a class file, make sure that its throws clause
    // has been processed.
    //
    method -> ProcessMethodThrows(this, id_token);

    if (lookup_env.option->deprecation && method -> IsDeprecated() &&
        ! InDeprecatedContext())
    {
        ReportSemErrorWithAst(SemanticError::DEPRECATED_METHOD, method_call,
                       method -> Header(),
                       method -> containing_type -> ContainingPackageName(),
                       method -> containing_type -> ExternalName());
    }
    return method_set[0];
}


void Semantic::FindMethodInEnvironment(Tuple<MethodShadowSymbol*>& methods_found,
                                       SemanticEnvironment*& where_found,
                                       SemanticEnvironment* envstack,
                                       AstMethodInvocation* method_call)
{
    assert(! method_call -> base_opt);
    auto id_token = method_call -> identifier_token;
    NameSymbol* name_symbol = method_call->identifier_token->GetNameSymbol();

    for (SemanticEnvironment* env = envstack; env; env = env -> previous)
    {
        TypeSymbol* type = env -> Type();
        if (! type -> expanded_method_table)
            ComputeMethodsClosure(type, id_token);

        methods_found.Reset();
        where_found = NULL;

        //
        // If this environment contained a method with the right name, the
        // search stops:
        //
        //    "Class scoping does not influence overloading: if the inner
        //     class has one print method, the simple method name 'print'
        //     refers to that method, not any of the ten 'print' methods in
        //     the enclosing class."
        //
        MethodShadowSymbol* method_shadow = type -> expanded_method_table ->
            FindMethodShadowSymbol(name_symbol);
        if (method_shadow)
        {
            for ( ; method_shadow;
                  method_shadow = method_shadow -> next_method)
            {
                MethodSymbol* method = method_shadow -> method_symbol;

                if (! method -> IsTyped())
                    method -> ProcessMethodSignature(this, id_token);

                //
                // Since type -> IsOwner(this_type()), i.e., type encloses
                // this_type(), method is accessible, even if it is private.
                //
                if (method_call -> arguments -> NumArguments() ==
                    method -> NumFormalParameters())
                {
                    unsigned i;
                    for (i = 0; i < method_call -> arguments -> NumArguments(); i++)
                    {
                        Expression* expr = method_call -> arguments -> Argument(i);
                        if (! CanMethodInvocationConvert(method -> FormalParameter(i) -> Type(),
                                                         expr -> Type()))
                        {
                            break;
                        }
                    }
                    if (i == method_call -> arguments -> NumArguments())
                    {
                        if (MoreSpecific(method, methods_found))
                        {
                            methods_found.Reset();
                            methods_found.Next() = method_shadow;
                        }
                        else if (NoMethodMoreSpecific(methods_found, method))
                            methods_found.Next() = method_shadow;
                    }
                }
            }

            //
            // If a match was found, save the environment
            //
            where_found = (methods_found.Length() > 0 ? env
                           : (SemanticEnvironment*) NULL);
            break;
        }
    }
}


MethodShadowSymbol* Semantic::FindMethodInEnvironment(SemanticEnvironment*& where_found,
                                                      AstMethodInvocation* method_call)
{
    Tuple<MethodShadowSymbol*> methods_found(2);
    FindMethodInEnvironment(methods_found, where_found, state_stack.Top(),
                            method_call);
    if (methods_found.Length() == 0)
    {
        ReportMethodNotFound(method_call, NULL);
        return NULL;
    }
    MethodSymbol* method_symbol =  methods_found[0] -> method_symbol;
    for (unsigned i = 1; i < methods_found.Length(); i++)
    {
        ReportSemErrorWithAst(SemanticError::AMBIGUOUS_METHOD_INVOCATION,
                       method_call, method_symbol -> Name(),
                       methods_found[0] -> method_symbol -> Header(),
                       method_symbol -> containing_type -> ContainingPackageName(),
                       method_symbol -> containing_type -> ExternalName(),
                       methods_found[i] -> method_symbol -> Header(),
                       methods_found[i] -> method_symbol -> containing_type -> ContainingPackageName(),
                       methods_found[i] -> method_symbol -> containing_type -> ExternalName());
    }

    if (method_symbol -> containing_type != where_found -> Type())
    {
        //
        // The method was inherited.
        //
        if (method_symbol -> ACC_SYNTHETIC())
        {
            ReportSemErrorWithAst(SemanticError::SYNTHETIC_METHOD_INVOCATION,
                           method_call, method_symbol -> Header(),
                           method_symbol -> containing_type -> ContainingPackageName(),
                           method_symbol -> containing_type -> ExternalName());
        }
        else if (lookup_env.option->pedantic)
        {
            //
            // Give a pedantic warning if the inherited method shadowed
            // a method of the same name within an enclosing lexical scope.
            //
            Tuple<MethodShadowSymbol*> others(2);
            SemanticEnvironment* found_other;
            SemanticEnvironment* previous_env = where_found -> previous;
            FindMethodInEnvironment(others, found_other, previous_env,
                                    method_call);

            if (others.Length() > 0 &&
                where_found -> Type() != found_other -> Type())
            {
                for (unsigned i = 0; i < others.Length();  i++)
                {
                    if (others[i] -> method_symbol != method_symbol &&
                        (others[i] -> method_symbol -> containing_type ==
                         found_other -> Type()))
                    {
                        ReportSemErrorWithAst(SemanticError::INHERITANCE_AND_LEXICAL_SCOPING_CONFLICT_WITH_MEMBER,
                                       method_call,
                                       method_symbol -> Name(),
                                       method_symbol -> containing_type -> ContainingPackageName(),
                                       method_symbol -> containing_type -> ExternalName(),
                                       found_other -> Type() -> ContainingPackageName(),
                                       found_other -> Type() -> ExternalName());
                        break; // emit only one error message
                    }
                }
            }
        }
    }

    //
    // If this method came from a class file, make sure that its throws
    // clause has been processed.
    //
    method_symbol -> ProcessMethodThrows(this,
                                         method_call -> identifier_token);
    if (lookup_env.option->deprecation && method_symbol -> IsDeprecated() &&
        ! InDeprecatedContext())
    {
        ReportSemErrorWithAst(SemanticError::DEPRECATED_METHOD,
                       method_call, method_symbol -> Header(),
                       method_symbol -> containing_type -> ContainingPackageName(),
                       method_symbol -> containing_type -> ExternalName());
    }
    return methods_found[0];
}



//
// Search the type in question for a variable. Note that name_symbol is an
// optional argument. If it was not passed to this function then its default
// value is NULL (see semantic.h) and we assume that the name to search for
// is the last identifier specified in the field_access. Error reporting if
// the field is not found is up to the callee, since for qualified names,
// the name may successfully resolve to a nested type.
//
VariableSymbol* Semantic::FindVariableInType(TypeSymbol* type,
                                             Expression* expr,
                                             NameSymbol* name_symbol)
{
    Tuple<VariableSymbol*> variable_set(2); // Stores variable conflicts.
    FieldReference* field_access = FieldReference::FieldAccessCast(expr);
    AstName* name = AstName::NameCast(expr);
    Expression* base = name ? name -> base_opt : field_access -> base;
    assert(base);
    VariableSymbol* variable;
    if (! name_symbol)
        name_symbol = (expr -> RightToken()->GetNameSymbol());
    if (! type -> expanded_field_table)
        ComputeFieldsClosure(type, expr -> RightToken());

    //
    // Find the accessible fields with the correct name in the type.
    //
    VariableShadowSymbol* variable_shadow =
        type -> expanded_field_table -> FindVariableShadowSymbol(name_symbol);

    if (variable_shadow)
    {
        variable = variable_shadow -> variable_symbol;
        if (! variable -> IsTyped())
            variable -> ProcessVariableSignature(this, expr -> RightToken());
        if (MemberAccessCheck(type, variable, base))
            variable_set.Next() = variable;

        for (unsigned i = 0; i < variable_shadow -> NumConflicts(); i++)
        {
            variable = variable_shadow -> Conflict(i);
            if (! variable -> IsTyped())
                variable -> ProcessVariableSignature(this,
                                                     expr -> RightToken());
            if (MemberAccessCheck(type, variable, base))
                variable_set.Next() = variable;
        }
    }

    if (variable_set.Length() == 0)
        return NULL;
    else if (variable_set.Length() > 1)
    {
        ReportSemErrorWithAst(SemanticError::AMBIGUOUS_FIELD, expr,
                       name_symbol -> Name(),
                       variable_set[0] -> ContainingType() -> ContainingPackageName(),
                       variable_set[0] -> ContainingType() -> ExternalName(),
                       variable_set[1] -> ContainingType() -> ContainingPackageName(),
                       variable_set[1] -> ContainingType() -> ExternalName());
    }

    variable = variable_set[0];
    if (variable -> ACC_SYNTHETIC())
    {
        ReportSemErrorWithAst(SemanticError::SYNTHETIC_VARIABLE_ACCESS, expr,
                       variable -> Name(),
                       variable -> ContainingType() -> ContainingPackageName(),
                       variable -> ContainingType() -> ExternalName());
    }

    if (lookup_env.option->deprecation && variable -> IsDeprecated() &&
        ! InDeprecatedContext())
    {
        ReportSemErrorWithAst(SemanticError::DEPRECATED_FIELD, expr,
                       variable -> Name(),
                       variable -> ContainingType() -> ContainingPackageName(),
                       variable -> ContainingType() -> ExternalName());
    }

	if(variable->Type()->Bad()){
		if(variable->declarator)
		{
			auto  typeAst = variable->declarator->Type();
			auto  typeNames = boost::join(typeAst->getTypeName(), L".");
			ReportSemErrorSimple(SemanticError::TYPE_NOT_FOUND, expr->RightToken(),
				typeNames.c_str(),
				L"");
		}
		else
		{
			
			ReportSemErrorSimple(SemanticError::TYPE_NOT_FOUND, expr->RightToken(),
				name_symbol->Name(),
				L"");
		}
		
	}
    return variable;
}


//
// Called when no accessible variable was found. The access must be one of
// FieldReference or AstSimpleName. This checks in order: an accessible no-arg
// method by the same name, an inaccessible field in a superclass, a
// misspelled field name, a type by the same name, and finally the field was
// not found.
//
void Semantic::ReportVariableNotFound(Expression* access, TypeSymbol* type)
{
	//TokenIndex id_token = access->RightToken();
	auto  id_token = access->RightToken();
    NameSymbol* name_symbol = access->RightToken()->GetNameSymbol();
    VariableShadowSymbol* variable_shadow;

    if (! type -> expanded_field_table)
        ComputeFieldsClosure(type, access->RightToken());
    if (! type -> expanded_method_table)
        ComputeMethodsClosure(type, id_token);

    //
    // Search for an accessible no-arg method of the same name.
    //
    MethodShadowSymbol* method_shadow;
    for (method_shadow = type -> expanded_method_table ->
             FindMethodShadowSymbol(name_symbol);
         method_shadow; method_shadow = method_shadow -> next_method)
    {
        MethodSymbol* method = method_shadow -> method_symbol;

        //
        // Make sure that method has been fully prepared.
        //
        if (! method -> IsTyped())
            method -> ProcessMethodSignature(this, id_token);

        if (method -> NumFormalParameters() == 0 &&
            MemberAccessCheck(type, method))
        {
            ReportSemErrorSimple(SemanticError::METHOD_NOT_FIELD,
                           id_token, name_symbol -> Name(),
                           method -> containing_type -> ContainingPackageName(),
                           method -> containing_type -> ExternalName());
            return;
        }
    }

    //
    // Check if the field is inaccessible.
    //
    for (TypeSymbol* super_type = type;
         super_type; super_type = super_type -> super)
    {
        variable_shadow = super_type -> expanded_field_table ->
            FindVariableShadowSymbol(name_symbol);
        if (variable_shadow)
        {
            VariableSymbol* variable = variable_shadow -> variable_symbol;
            TypeSymbol* containing_type =  TypeSymbol::TypeCast(variable->owner);

            //
            // A protected instance field in the superclass is inaccessible if
            // the base expression is the wrong type.
            //
            if (variable -> ACC_PROTECTED() &&
                ! variable -> ACC_STATIC() &&
                ThisType() -> HasProtectedAccessTo(containing_type))
            {
                ReportSemErrorSimple(SemanticError::PROTECTED_INSTANCE_FIELD_NOT_ACCESSIBLE,
                               id_token, name_symbol -> Name(),
                               containing_type -> ContainingPackageName(),
                               containing_type -> ExternalName(),
                               ThisType() -> ContainingPackageName(),
                               ThisType() -> ExternalName());
            }
            else
            {
                ReportSemErrorSimple(SemanticError::FIELD_NOT_ACCESSIBLE,
                               id_token, name_symbol -> Name(),
                               containing_type -> ContainingPackageName(),
                               containing_type -> ExternalName(),
                               variable -> AccessString());
            }
            return;
        }
    }

    //
    // Try various possibilities of what the user might have meant.
    //
    AstName* ast_name = AstName::NameCast(access);
    TypeSymbol* inaccessible_type = (! ast_name || ast_name -> base_opt)
        ? NULL : FindInaccessibleType(ast_name);
    VariableSymbol* variable = FindMisspelledVariableName(type, access);
    if (variable)
    {
        //
        // There is a field with a similar name.
        //
        ReportSemErrorSimple(SemanticError::FIELD_NAME_MISSPELLED,
                       id_token, name_symbol -> Name(),
                       type -> ContainingPackageName(),
                       type -> ExternalName(),
                       variable -> Name());
    }
    else if (FindType(id_token))
    {
        //
        // There is a type or package of the same name.
        //
        ReportSemErrorSimple(SemanticError::TYPE_NOT_FIELD,
                       id_token, name_symbol -> Name());
    }
    else if (inaccessible_type)
    {
        //
        // There is an inaccessible type of the same name.
        //
        ReportTypeInaccessible(ast_name, inaccessible_type);
    }
    else if (		access -> symbol 
				&&   PackageSymbol::PackageCast(access->symbol) )
    {
        ReportSemErrorWithAst(SemanticError::UNKNOWN_AMBIGUOUS_NAME,
                       access, name_symbol -> Name());
    }
    else
    {
        //
        // Give up. We didn't find it.
        //
        ReportSemErrorSimple(SemanticError::FIELD_NOT_FOUND,
                       id_token, name_symbol -> Name(),
                       type -> ContainingPackageName(),
                       type -> ExternalName());
    }
}


void Semantic::FindVariableInEnvironment(Tuple<VariableSymbol*>& variables_found,
                                         SemanticEnvironment*& where_found,
                                         SemanticEnvironment* envstack,
                                         NameSymbol* name_symbol,
                                         Token* identifier_token)
{
    variables_found.Reset();
    where_found = (SemanticEnvironment*) NULL;

    for (SemanticEnvironment* env = envstack; env; env = env -> previous)
    {
        VariableSymbol* variable_symbol =
            env -> symbol_table.FindVariableSymbol(name_symbol);
        if (variable_symbol) // a local variable
        {
            variables_found.Next() = variable_symbol;
            where_found = env;
            break;
        }

        TypeSymbol* type = env -> Type();
        if (! type -> expanded_field_table)
            ComputeFieldsClosure(type, identifier_token);
        VariableShadowSymbol* variable_shadow = type ->
            expanded_field_table -> FindVariableShadowSymbol(name_symbol);
        if (variable_shadow)
        {
            //
            // Since type -> IsOwner(this_type()), i.e., type encloses
            // this_type(), variable_symbol is accessible, even if it is
            // private.
            //
            variables_found.Next() = variable_shadow -> variable_symbol;

            //
            // Recall that even an inaccessible member x of a super class (or
            // interface) S, in addition to not been inherited by a subclass,
            // hides all other occurrences of x that may appear in a super
            // class (or super interface) of S (see 8.3).
            //
            for (unsigned i = 0; i < variable_shadow -> NumConflicts(); i++)
                variables_found.Next() = variable_shadow -> Conflict(i);
            where_found = env;
            break;
        }
    }
}


VariableSymbol* Semantic::FindVariableInEnvironment(SemanticEnvironment*& where_found,
                                                    Token* identifier_token)
{
    Tuple<VariableSymbol*> variables_found(2);
    NameSymbol* name_symbol =  (identifier_token->GetNameSymbol());
    SemanticEnvironment* envstack = state_stack.Top();
    FindVariableInEnvironment(variables_found, where_found, envstack,
                              name_symbol, identifier_token);

    VariableSymbol* variable_symbol =
        (VariableSymbol*) (variables_found.Length() > 0
                            ? variables_found[0] : NULL);

    if (variable_symbol)
    {
        if (variable_symbol -> IsLocal()) // a local variable
        {
            if (where_found != envstack)
            {
                TypeSymbol* type = envstack -> Type();

                if (! variable_symbol -> ACC_FINAL())
                {
                    MethodSymbol* method =
						MethodSymbol::MethodCast(variable_symbol -> owner);

                    //
                    // TODO: What if the method is a constructor ?
                    // if (method -> Identity() != control.init_symbol &&
                    //     method -> Identity() != control.block_init_symbol &&
                    //     method -> Identity() != control.clinit_symbol)
                    //
                    ReportSemErrorSimple(SemanticError::INNER_CLASS_REFERENCE_TO_NON_FINAL_LOCAL_VARIABLE,
                                   identifier_token,
                                   type -> ContainingPackageName(),
                                   type -> ExternalName(),
                                   (identifier_token->NameString()),
                                   method -> ExternalName());
                }
                else if (! variable_symbol -> initial_value)
                {
                    //
                    // The variable is not constant, so we need to insert a
                    // variable shadow in the outermost local class within the
                    // scope of the variable, and use that shadow instead.
                    //
                    variable_symbol = FindLocalVariable(variable_symbol,
                                                        envstack -> Type());
                    TypeSymbol* shadow_owner =
                        variable_symbol -> ContainingType();
                    assert(shadow_owner);
                    where_found = shadow_owner -> semantic_environment;
                }
            }
        }
        else if (variable_symbol -> owner != where_found -> Type())
        {
            //
            // The field was inherited.
            //
            TypeSymbol* type = (TypeSymbol*) variable_symbol -> owner;
            if (variable_symbol -> ACC_SYNTHETIC())
            {
                ReportSemErrorSimple(SemanticError::SYNTHETIC_VARIABLE_ACCESS,
                               identifier_token,
                               variable_symbol -> Name(),
                               type -> ContainingPackageName(),
                               type -> ExternalName());
            }
            else if (lookup_env.option->pedantic)
            {
                //
                // Give a pedantic warning if the inherited field shadowed
                // a field of the same name within an enclosing lexical scope.
                //
                Tuple<VariableSymbol*> others(2);
                SemanticEnvironment* found_other;
                SemanticEnvironment* previous_env = where_found -> previous;
                FindVariableInEnvironment(others, found_other, previous_env,
                                          name_symbol, identifier_token);

                if (others.Length() > 0 &&
                    where_found -> Type() != found_other -> Type())
                {
                    for (unsigned i = 0; i < others.Length(); i++)
                    {
                        if (others[i] != variable_symbol)
                        {
                            MethodSymbol* method =
								MethodSymbol::MethodCast(others[i] -> owner);

                            if (method)
                            {
                                ReportSemErrorSimple(SemanticError::INHERITANCE_AND_LEXICAL_SCOPING_CONFLICT_WITH_LOCAL,
                                               identifier_token,
                                                (identifier_token->NameString()),
                                               type -> ContainingPackageName(),
                                               type -> ExternalName(),
                                               method -> Name());
                                break;
                            }
                            else if (others[i] -> owner == found_other -> Type())
                            {
                                ReportSemErrorSimple(SemanticError::INHERITANCE_AND_LEXICAL_SCOPING_CONFLICT_WITH_MEMBER,
                                               identifier_token,
                                                (identifier_token->NameString()),
                                               type -> ContainingPackageName(),
                                               type -> ExternalName(),
                                               found_other -> Type() -> ContainingPackageName(),
                                               found_other -> Type() -> ExternalName());
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    for (unsigned i = 1; i < variables_found.Length(); i++)
    {
        ReportSemErrorSimple(SemanticError::AMBIGUOUS_FIELD, identifier_token,
                       variable_symbol -> Name(),
                       variable_symbol -> ContainingType() -> ContainingPackageName(),
                       variable_symbol -> ContainingType() -> ExternalName(),
                       variables_found[i] -> ContainingType() -> ContainingPackageName(),
                       variables_found[i] -> ContainingType() -> ExternalName());
    }

    if (variable_symbol)
    {
        if (lookup_env.option->deprecation && variable_symbol -> IsDeprecated() &&
            ! InDeprecatedContext())
        {
            ReportSemErrorSimple(SemanticError::DEPRECATED_FIELD, identifier_token,
                           variable_symbol -> Name(),
                           variable_symbol -> ContainingType() -> ContainingPackageName(),
                           variable_symbol -> ContainingType() -> ExternalName());
        }

        if (! variable_symbol -> IsTyped())
            variable_symbol -> ProcessVariableSignature(this, identifier_token);
    }
    return variable_symbol;
}


//
// Find a variable shadow in the outermost local class within the scope of
// the variable, and return a local variable shadow to it instead.
//
VariableSymbol* Semantic::FindLocalVariable(VariableSymbol* local,
                                            TypeSymbol* type)
{
    while (local -> accessed_local)
        local = local -> accessed_local;
    assert(local -> IsLocal());

    TypeSymbol* containing_type = local -> ContainingType();
    if (type == containing_type)
        return local;

    while (type && type -> ContainingType() != containing_type)
    {
        if (! type -> EnclosingType())
        {
            assert(type -> Anonymous());
            break;
        }
        type = type -> ContainingType();
    }
    assert(type && type -> IsLocal());
    return type -> FindOrInsertLocalShadow(local);
}


//
// Using the this$0 variable, locate the appropriate enclosing instance.
//
Expression* Semantic::FindEnclosingInstance(Expression* base,
                                               TypeSymbol* environment_type,
                                               bool exact)
{
    TypeSymbol* base_type = base -> Type();
    assert(base_type != environment_type &&
           base_type -> HasEnclosingInstance(environment_type, exact));
    VariableSymbol* this0 = base_type -> EnclosingInstance();
    if (! this0)
    {
        //
        // In the case of an anonymous class in an explicit constructor call,
        // when the immediate enclosing class is not yet initialized, other
        // enclosing classes are not accessible (even though they COULD be
        // available through additional constructor parameters) - JLS 8.8.5.1
        //
        assert(base_type -> Anonymous() && base_type -> IsLocal());
        return NULL;
    }

    auto tok = base -> RightToken();

    FieldReference* field_access =
        compilation_unit -> ast_pool -> GenFieldAccess();
    field_access -> base = base;
    field_access -> identifier_token = tok;
    field_access -> symbol = this0;

    if (exact ? (this0 -> Type() == environment_type)
        : (this0 -> Type() -> IsSubclass(environment_type)))
    {
        return field_access;
    }
    return FindEnclosingInstance(field_access, environment_type, exact);
}


//
// Generate access to the correct enclosing instance.
//
Expression* Semantic::CreateAccessToType(Ast* source,
                                            TypeSymbol* environment_type)
{
    TypeSymbol* this_type = ThisType();

    Token* left_tok;
	Token* right_tok;

    AstName* variable = AstName::NameCast(source);
    AstMethodInvocation* method = AstMethodInvocation::MethodInvocationCast(source);
    AstSuperCall* super_call = AstSuperCall::SuperCallCast(source);
    AstThisExpression* this_expr = AstThisExpression::ThisExpressionCast(source);
    AstSuperExpression* super_expr = AstSuperExpression::SuperExpressionCast(source);
    AstClassCreationExpression* class_creation =
        AstClassCreationExpression::ClassCreationExpressionCast(source);
    bool exact = false;

    if (variable)
    {
        assert(! variable -> base_opt);
        left_tok = right_tok = variable -> identifier_token;
        //
        // If this type subclasses the enclosing type, then CreateAccess was
        // called because the simple name was not inherited into this type
        // (ie. the variable is private or else hidden in a superclass). In
        // this case, turn on exact enclosing type checking.
        //
        if (this_type -> IsSubclass(environment_type))
            exact = true;
    }
    else if (method)
    {
        assert(! method -> base_opt);
        left_tok = right_tok = method -> identifier_token;
        //
        // If this type subclasses the enclosing type, then CreateAccess was
        // called because the simple name was not inherited into this type
        // (ie. the method is private or else hidden in a superclass). In
        // this case, turn on exact enclosing type checking.
        //
        if (this_type -> IsSubclass(environment_type))
            exact = true;
    }
    else if (class_creation)
        left_tok = right_tok = class_creation -> new_token;
    else if (super_call)
        left_tok = right_tok = super_call -> super_token;
    else if (this_expr)
    {
        assert(this_expr -> base_opt);
        left_tok = this_expr -> LeftToken();
        right_tok = this_expr -> this_token;
        exact = true;
    }
    else if (super_expr)
    {
        assert(super_expr -> base_opt);
        left_tok = super_expr -> LeftToken();
        right_tok = super_expr -> super_token;
        exact = true;
    }
    else assert(false && "create access to invalid expression");

    Expression* resolution;

    if (! this_type -> HasEnclosingInstance(environment_type, exact))
    {
        ReportSemError((ExplicitConstructorInvocation() &&
                        this_type -> IsSubclass(environment_type)
                        ? SemanticError::ENCLOSING_INSTANCE_ACCESS_FROM_CONSTRUCTOR_INVOCATION
                        : SemanticError::ENCLOSING_INSTANCE_NOT_ACCESSIBLE),
                       left_tok, right_tok,
                       environment_type -> ContainingPackageName(),
                       environment_type -> ExternalName());
        resolution = compilation_unit -> ast_pool -> GenName(left_tok);
        resolution -> symbol = lookup_env.no_type;
    }
    else
    {
        //
        // Collapse everything except qualified this or super to the innermost
        // class. Start from the parameter this$0 in an explicit constructor
        // invocation, else start from this.
        //
        if (ExplicitConstructorInvocation())
        {
            VariableSymbol* variable = LocalSymbolTable().
                FindVariableSymbol(lookup_env.nameEnv->this_name_symbol);
            assert(variable);
            resolution = compilation_unit -> ast_pool -> GenName(left_tok);
            resolution -> symbol = variable;
        }
        else
        {
            resolution =
                compilation_unit -> ast_pool -> GenThisExpression(left_tok);
            resolution -> symbol = this_type;
        }
        TypeSymbol* resolved_type = resolution -> Type();
        if (resolved_type != environment_type &&
            (! resolved_type -> IsSubclass(environment_type) || exact))
        {
            Expression* intermediate =
                FindEnclosingInstance(resolution, environment_type, exact);
            if (! intermediate)
            {
                ReportSemError(SemanticError::ENCLOSING_INSTANCE_ACCESS_ACROSS_STATIC_REGION,
                               left_tok, right_tok,
                               environment_type -> ContainingPackageName(),
                               environment_type -> ExternalName());
                resolution -> symbol = lookup_env.no_type;
            }
            else resolution = intermediate;
        }
    }
    if (super_expr)
        environment_type = environment_type -> super;
    return ConvertToType(resolution, environment_type);
}


void Semantic::CreateAccessToScopedVariable(AstName* name,
                                            TypeSymbol* environment_type)
{
    assert(! name -> base_opt);
    VariableSymbol* variable = (VariableSymbol*) name -> symbol;
    assert(  TypeSymbol::TypeCast(variable->owner));
    Expression* access_expression;
    if (variable -> ACC_STATIC())
    {
        access_expression = compilation_unit -> ast_pool ->
            GenName(name -> identifier_token);
        access_expression -> symbol = environment_type;
    }
    else
    {
        AstThisExpression* this_expr = compilation_unit -> ast_pool ->
            GenThisExpression(name -> identifier_token);
        this_expr -> resolution_opt =
            CreateAccessToType(name, environment_type);
        this_expr -> symbol = this_expr -> resolution_opt -> symbol;
        access_expression = this_expr;
    }

    if (access_expression -> symbol != lookup_env.no_type)
    {
        TypeSymbol* containing_type = variable -> ContainingType();

        if (variable -> ACC_PRIVATE() ||
            (variable -> ACC_PROTECTED() &&
             ! ProtectedAccessCheck(containing_type)))
        {
            assert((variable -> ACC_PRIVATE() &&
                    environment_type == containing_type) ||
                   (variable -> ACC_PROTECTED() &&
                    environment_type -> IsSubclass(containing_type)));

            auto loc = name -> identifier_token;
            AstArguments* args =
                compilation_unit -> ast_pool -> GenArguments(loc, loc);
            if (! variable -> ACC_STATIC())
            {
                // TODO: WARNING: sharing of Ast subtree !!!
                args -> AllocateArguments(1);
                args -> AddArgument(access_expression);
            }

            AstMethodInvocation* accessor =
                compilation_unit -> ast_pool -> GenMethodInvocation(loc);
            accessor -> base_opt = access_expression;
            accessor -> arguments = args;
            // The default base type of the accessor method is appropriate.
            accessor -> symbol =
                environment_type -> GetReadAccessMethod(variable);

            name -> resolution_opt = accessor;
        }
        else
        {
            FieldReference* field_access =
                compilation_unit -> ast_pool -> GenFieldAccess();
            field_access -> base = access_expression;
            field_access -> identifier_token = name -> identifier_token;
            field_access -> symbol = variable;

            name -> resolution_opt = field_access;
        }
    }
}


void Semantic::CreateAccessToScopedMethod(AstMethodInvocation* method_call,
                                          TypeSymbol* environment_type)
{
    assert(environment_type -> IsOwner(ThisType()));
    assert(! method_call -> base_opt);
    MethodSymbol* method = (MethodSymbol*) method_call -> symbol;
    Expression* access_expression;
    if (method -> ACC_STATIC())
    {
        access_expression = compilation_unit -> ast_pool ->
            GenName(method_call -> identifier_token);
        access_expression -> symbol = environment_type;
    }
    else
    {
        AstThisExpression* this_expr = compilation_unit -> ast_pool ->
            GenThisExpression(method_call -> identifier_token);
        this_expr -> resolution_opt =
            CreateAccessToType(method_call, environment_type);
        this_expr -> symbol = this_expr -> resolution_opt -> symbol;
        access_expression = this_expr;
    }

    if (access_expression -> symbol != lookup_env.no_type)
    {
        method_call -> base_opt = access_expression;
        TypeSymbol* containing_type = method -> containing_type;

        if (method -> ACC_PRIVATE() ||
            (method -> ACC_PROTECTED() &&
             ! ProtectedAccessCheck(containing_type)))
        {
            assert((method -> ACC_PRIVATE() &&
                    environment_type == containing_type) ||
                   (method -> ACC_PROTECTED() &&
                    environment_type -> IsSubclass(containing_type)));

            AstArguments* args = compilation_unit -> ast_pool ->
                GenArguments(method_call -> arguments -> left_parenthesis_token,
                             method_call -> arguments -> right_parenthesis_token);
            unsigned num_args = method_call -> arguments -> NumArguments();
            if (! method -> ACC_STATIC())
            {
                args -> AllocateArguments(num_args + 1);
                args -> AddArgument(access_expression);
            }
            else args -> AllocateArguments(num_args);
            for (unsigned i = 0; i < num_args; i++)
                args -> AddArgument(method_call -> arguments -> Argument(i));

            AstMethodInvocation* accessor = compilation_unit -> ast_pool ->
                GenMethodInvocation(method_call -> identifier_token);
            accessor -> base_opt = access_expression;
            accessor -> arguments = args;
            accessor -> symbol =
                // default base type is appropriate
                environment_type -> GetReadAccessMethod(method);

            method_call -> symbol = method;
            method_call -> resolution_opt = accessor;
        }
    }
}


void Semantic::CheckSimpleName(AstName* name, SemanticEnvironment* where_found)
{
	VariableSymbol* variable_symbol = VariableSymbol::VariableCast(name->symbol);
    assert(variable_symbol && ! name -> base_opt);

    if (StaticRegion() && ! ExplicitConstructorInvocation())
    {
        if (! (variable_symbol -> IsLocal() ||
               variable_symbol -> ACC_STATIC()))
        {
            ReportSemErrorSimple(SemanticError::NAME_NOT_CLASS_VARIABLE,
                           name -> identifier_token,
                            (name -> identifier_token->NameString()));
        }
        else if ( TypeSymbol::TypeCast(variable_symbol->owner) &&
                 ! variable_symbol -> IsDeclarationComplete() &&
                 ! ProcessingSimpleAssignment())
        {
            ReportSemErrorSimple(SemanticError::NAME_NOT_YET_AVAILABLE,
                           name -> identifier_token,
                           (name -> identifier_token->NameString()));
        }
    }
    else if (! variable_symbol -> ACC_STATIC()) // an instance variable?
    {
        // an instance field member ?
		TypeSymbol* containing_type = TypeSymbol::TypeCast(variable_symbol->owner);

        // variable must be a field for these next errors to be valid
        if (containing_type && ! variable_symbol -> accessed_local)
        {
            if (containing_type == ThisType() &&
                ! variable_symbol -> IsDeclarationComplete() &&
                ! ProcessingSimpleAssignment()) // forward reference?
            {
                ReportSemErrorSimple(SemanticError::NAME_NOT_YET_AVAILABLE,
                               name -> identifier_token,
                               (name -> identifier_token->NameString()));
            }
            else if (ExplicitConstructorInvocation() &&
                     where_found == state_stack.Top())
            {
                //
                // If the variable in question is an instance variable that is
                // declared in this_type (this_type is definitely a class) or
                // one of its super classes, then we have an error:
                //
                ReportSemErrorSimple(SemanticError::INSTANCE_VARIABLE_IN_EXPLICIT_CONSTRUCTOR,
                               name -> identifier_token,
                                (name -> identifier_token->NameString()),
                               containing_type -> Name());
            }
        }
    }
}


void Semantic::ProcessExpressionOrStringConstant(Expression* expr)
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


void Semantic::ProcessName(Ast* expr)
{
    AstName* name = (AstName*) expr;
    ProcessAmbiguousName(name);
    TypeSymbol* type = name -> Type();
    if (type == lookup_env.no_type)
        return; // ProcessAmbiguousName already reported the error
    if (! type || TypeSymbol::TypeCast(name -> symbol))
    {
        ReportVariableNotFound(name, ThisType());
        name -> symbol = lookup_env.no_type;
    }
}


//
// Returns true if the type is accessible from the current semantic location.
//
bool Semantic::TypeAccessCheck(TypeSymbol* type)
{
    // According to JLS 6.6.1, a type T[] is accessible if T is accessible.
    if (type -> IsArray())
        type = type -> base_type;

    //
    // Outside a class body, only public types from other packages, or
    // non-private types in the current package, are accessible. For a member
    // type, as in T1.T2, this does not check that T1 is also accessible; that
    // requires additional checks by the caller.
    //
    assert(this_package);
    if (type -> ACC_PUBLIC() ||
        (type -> ContainingPackage() == this_package &&
         ! type -> ACC_PRIVATE()))
    {
        return true;
    }
    if (state_stack.Size() > 0)
    {
        //
        // Inside a class body, all types listed above are accessible.
        // Additionally, declared or inherited member types are accessible.
        //
        TypeSymbol* this_type = ThisType();
        assert(this_type -> ContainingPackage() == this_package);
        if (this_type -> outermost_type == type -> outermost_type ||
            (type -> ACC_PROTECTED() &&
             this_type -> HasProtectedAccessTo(type)))
        {
            return true;
        }
    }
    return false;
}


//
// Returns true if the constructor is accessible. The invocation is used to
// distinguish between different rules for class instance creation and explicit
// constructor invocation.
//
bool Semantic::ConstructorAccessCheck(MethodSymbol* constructor,
                                      bool explicit_ctor)
{
    TypeSymbol* this_type = ThisType();
    TypeSymbol* containing_type = constructor -> containing_type;
    if (this_type -> outermost_type != containing_type -> outermost_type &&
        constructor -> ACC_PRIVATE())
    {
        return false;
    }

    //
    // Default constructors are not accessible outside the package, and
    // protected constructors can only be accessed by a call to super(). This
    // includes anonymous classes, where we will later generate a super() call.
    //
    if (containing_type -> ContainingPackage() != this_package &&
        ! constructor -> ACC_PUBLIC())
    {
        return constructor -> ACC_PROTECTED() && explicit_ctor;
    }
    return true;
}
bool Semantic::ConstructorAccessCheck(MethodSymbol* constructor,
	bool explicit_ctor , TypeSymbol* this_type)
{
	
	TypeSymbol* containing_type = constructor->containing_type;
	if (this_type->outermost_type != containing_type->outermost_type &&
		constructor->ACC_PRIVATE())
	{
		return false;
	}

	//
	// Default constructors are not accessible outside the package, and
	// protected constructors can only be accessed by a call to super(). This
	// includes anonymous classes, where we will later generate a super() call.
	//
	if (containing_type->ContainingPackage() != this_package &&
		!constructor->ACC_PUBLIC())
	{
		return constructor->ACC_PROTECTED() && explicit_ctor;
	}
	return true;
}


//
// Returns true if the field or method member symbol can be accessed from this
// semantic point, when the qualifier of the access is base_type. base
// is the qualifying expression for the access, and is NULL for simple names.
//
bool Semantic::MemberAccessCheck(TypeSymbol* base_type, Symbol* symbol, TypeSymbol* this_type,
                                 Expression* base)
{
  
	VariableSymbol* variable_symbol = VariableSymbol::VariableCast(symbol);
	MethodSymbol* method_symbol = MethodSymbol::MethodCast(symbol);
    assert(variable_symbol || method_symbol);

    AccessFlags* flags = (variable_symbol ? (AccessFlags*) variable_symbol
                          : (AccessFlags*) method_symbol);
    TypeSymbol* containing_type = (variable_symbol
                                   ? variable_symbol -> ContainingType()
                                   : method_symbol -> containing_type);
    assert(containing_type);

    //
    // When this function, MemberAccessCheck is invoked, it is assumed that
    // the base type has been checked as follows:
    //
    //    if (! TypeAccessCheck(base_type))
    //        ReportTypeInaccessible(base, base_type);
    //

    if (this_type -> outermost_type != containing_type -> outermost_type)
    {
        if (flags -> ACC_PRIVATE())
            return false;
        else if (flags -> ACC_PROTECTED())
        {
            //
            // Within the same package, protected is accessible. Super access
            // has special priveleges (contrary to JLS2 15.11.2,
            // super.name != ((S)this).name; ). JLS2 6.6.2: When packages
            // differ, subclasses may access protected static members without
            // further restrictions, but accessing instance members requires
            // that the qualifier be the subclass or lower.
            // JLS 9.2: Interfaces have no protected members.
            //
            if (base && base -> Type() -> ACC_INTERFACE())
            {
                // Object has no fields, so this would be the protected
                // methods "inherited" into an interface from Object.
                assert(method_symbol);
                return false;
            }
            if (containing_type -> ContainingPackage() == this_package ||
                (base && AstSuperExpression::SuperExpressionCast(base) ))
            {
                return true;
            }
            if (this_type -> HasProtectedAccessTo(containing_type))
            {
                if (flags -> ACC_STATIC())
                    return true;
                for (SemanticEnvironment* env =
                         this_type -> semantic_environment;
                     env; env = env -> previous)
                {
                    if (base_type -> IsSubclass(env -> Type()))
                        return true;
                }
            }
            return false;
        }
        else if (! flags -> ACC_PUBLIC() &&
                 containing_type -> ContainingPackage() != this_package)
        {
            return false;
        }
    }
    return true;
}


//
// Returns true if the current type can access a protected member declared in
// the containing type, without an accessor method. This does not test
// whether the target type and member are accessible, since those checks are
// assumed to be already done.
//
bool Semantic::ProtectedAccessCheck(TypeSymbol* containing_type)
{
    return ThisType() -> IsSubclass(containing_type) ||
        this_package == containing_type -> ContainingPackage();
}


//
// FindVariableMember resolves a qualified field reference. The parameter
// type is the type of the qualifying expression, field_access is the
// expression being resolved.
//
void Semantic::FindVariableMember(TypeSymbol* type, Expression* expr)
{
    //
    // TypeCast() returns true for super, this, and instance creation as
    // well as true type names, hence the extra check
    //
    FieldReference* field_access = FieldReference::FieldAccessCast(expr);
    AstName* name = AstName::NameCast(expr);
    Expression* base = name ? name -> base_opt : field_access -> base;
    auto id_token = expr -> RightToken();
    bool base_is_type = TypeSymbol::TypeCast(base -> symbol)  && AstName::NameCast(base);

    if (type -> Bad())
    {
        //
        // If no error has been detected so far, report this as an error so
        // that we don't try to generate code later. On the other hand, if an
        // error had been detected prior to this, don't flood the user with
        // spurious messages.
        //
        if (NumErrors() == 0)
            ReportVariableNotFound(expr, type);
        expr -> symbol = lookup_env.no_type;
    }
    else if (type == lookup_env.null_type || type -> Primitive())
    {
        ReportSemErrorWithAst(SemanticError::TYPE_NOT_REFERENCE, base,
                       type -> Name());
        expr -> symbol = lookup_env.no_type;
    }
    else
    {
        TypeSymbol* this_type = ThisType();
        if (! TypeAccessCheck(type))
        {
            ReportTypeInaccessible(base, type);
            expr -> symbol = lookup_env.no_type;
            return;
        }

        VariableSymbol* variable = FindVariableInType(type, expr);
        if (variable)
        {
            assert(variable -> IsTyped());

            if (base_is_type && ! variable -> ACC_STATIC())
            {
                ReportSemErrorSimple(SemanticError::NAME_NOT_CLASS_VARIABLE,
                               id_token,  (id_token->NameString()));
                expr -> symbol = lookup_env.no_type;
                return;
            }
            if (variable -> ACC_STATIC() && ! base_is_type)
            {
                ReportSemErrorSimple(SemanticError::CLASS_FIELD_ACCESSED_VIA_INSTANCE,
                               id_token, (id_token->NameString()));
            }
            //
            // If a variable is FINAL, initialized with a constant expression,
            // and of the form TypeName.Identifier, we substitute the
            // expression here - JLS 15.28. If it is of any other form, we
            // still compute the initial value, which will be inlined in
            // bytecode, but do not treat the expression as a constant - JLS2
            // clarifications.
            //
            if (variable -> ACC_FINAL())
            {
                if (! variable -> IsInitialized())
                    ComputeFinalValue(variable);
                if (base_is_type)
                {
                    assert(variable -> IsInitialized());
                    expr -> value = variable -> initial_value;
                }
            }

            //
            // Access to a private or protected variable in or via an enclosing
            // type? If the base is a super expression, be sure to start from
            // the correct enclosing instance.
            //
            TypeSymbol* containing_type = variable -> ContainingType();
            TypeSymbol* target_type = containing_type;
            if (! variable -> ACC_STATIC() && AstSuperExpression::SuperExpressionCast(base))
            {
                AstSuperExpression* super_expr = (AstSuperExpression*) base;
                if (super_expr -> base_opt)
                    target_type = super_expr -> base_opt -> symbol;
            }
            if (this_type != target_type &&
                (variable -> ACC_PRIVATE() ||
                 (variable -> ACC_PROTECTED() &&
                  (! ProtectedAccessCheck(containing_type) ||
                   target_type != containing_type))))
            {
                if (expr -> IsConstant())
                    expr -> symbol = variable;
                else
                {
                    //
                    // Find the right enclosing class to place the accessor
                    // method in. For private fields, the containing type; for
                    // protected fields, an enclosing class which is related
                    // to the containing type.
                    //
                    TypeSymbol* environment_type = containing_type;
                    if (variable -> ACC_PROTECTED())
                    {
                        for (SemanticEnvironment* env =
                                 this_type -> semantic_environment;
                             env; env = env -> previous)
                        {
                            if (env -> Type() -> IsSubclass(target_type))
                            {
                                environment_type = env -> Type();
                                break;
                            }
                        }
                        assert(environment_type != containing_type &&
                               environment_type != this_type);
                    }

                    AstArguments* args =
                        compilation_unit -> ast_pool -> GenArguments(id_token,
                                                                     id_token);
                    if (! variable -> ACC_STATIC())
                    {
                        args -> AllocateArguments(1);
                        args -> AddArgument(base);
                    }

                    AstMethodInvocation* accessor = compilation_unit ->
                        ast_pool -> GenMethodInvocation(id_token);
                    accessor -> base_opt = base;
                    accessor -> arguments = args;
                    accessor -> symbol = environment_type ->
                        GetReadAccessMethod(variable, base -> Type());

                    if (name)
                        name -> resolution_opt = accessor;
                    else
                        field_access -> resolution_opt = accessor;
                    expr -> symbol = accessor -> symbol;
                }
            }
            else
                expr -> symbol = variable;
        }
        else
        {
            TypeSymbol* inner_type = FindNestedType(type, id_token);
            if (inner_type)
            {
                if (base_is_type)
                {
                    expr -> symbol = inner_type;
                    if (! TypeAccessCheck(inner_type))
                        ReportTypeInaccessible(expr, inner_type);
                }
                else
                {
                    ReportSemErrorSimple(SemanticError::TYPE_NOT_FIELD, id_token,
                                    (id_token->NameString()));
                    expr -> symbol = lookup_env.no_type;
                }
            }
            else
            {
                ReportVariableNotFound(expr, type);
                expr -> symbol = lookup_env.no_type;
            }
        }
    }
}

//
// Note that method names are not processed here but by the function
// ProcessMethodName.
//
void Semantic::ProcessAmbiguousName(AstName* name)
{
    TypeSymbol* this_type = ThisType();
    //
    // JLS2 6.5.2: If the ambiguous name is a simple name,...
    //
    if (! name -> base_opt)
    {
        TypeSymbol* type;
        //
        // ... If the Identifier appears within the scope (6.3) if a local
        // variable declaration (14.3) or parameter declaration (8.4.1,
        // 8.6.1, 14.18) with that name, then the ambiguous name is
        // reclassified as an ExpressionName...
        //
        // ...Otherwise, consider the class or interface C within whose
        // declaration the Identifier occurs. If C has one or more fields
        // with that name, which may be either declared within it or inherited,
        // then the Ambiguous name is reclassified as an ExpressionName....
        //
        SemanticEnvironment* where_found;
        VariableSymbol* variable_symbol =
            FindVariableInEnvironment(where_found, name -> identifier_token);
        if (variable_symbol)
        {
            assert(variable_symbol -> IsTyped());

            //
            // A variable_symbol that is FINAL may have an initial value.
            // If variable_symbol is not final then its initial value is NULL.
            //
            if (variable_symbol -> ACC_FINAL() &&
                ! variable_symbol -> IsInitialized())
            {
                ComputeFinalValue(variable_symbol);
            }
            name -> value = variable_symbol -> initial_value;
            name -> symbol = variable_symbol;

            CheckSimpleName(name, where_found);

            //
            // If the variable belongs to an outer type, add the proper
            // pointer dereferences (and method access in the case of a
            // private variable) necessary to  get to it.
            //
            if (where_found != state_stack.Top() &&
				TypeSymbol::TypeCast(variable_symbol -> owner ))
            {
                CreateAccessToScopedVariable(name, where_found -> Type());
            }
        }
        //
        // ...Otherwise, if a type of that name is declared in the compilation
        // unit (7.3) containing the Identifier, either by a
        // single-type-import declaration (7.5.1) or by a class or interface
        // type declaration (7.6), then the Ambiguous name is reclassified as
        // a TypeName...
        //
        // ...Otherwise, if a type of that name is declared in another
        // compilation unit (7.3) of the package (7.1) of the compilation unit
        // containing the Identifier, then the Ambiguous Name is reclassified
        // as a TypeName...
        //
        // ...Otherwise, if a type of that name is declared by exactly one
        // type-import-on-demand declaration (7.5.2) of the compilation unit
        // containing the Identifier, then the AmbiguousName is reclassified
        // as a TypeName
        //
        // ...Otherwise, if a type of that name is declared by more than one
        // type-import-on-demand declaration of the compilation unit
        // containing the Identifier, then a compile-time error results.
        //
        else if ((type = FindType(name -> identifier_token)))
        {
            name -> symbol = type;
            if (lookup_env.option->deprecation && type -> IsDeprecated() &&
                ! InDeprecatedContext())
            {
                ReportSemErrorSimple(SemanticError::DEPRECATED_TYPE,
                               name -> identifier_token,
                               type -> ContainingPackageName(),
                               type -> ExternalName());
            }
        }
        //
        // ...Otherwise, the Ambiguous name is reclassified as a PackageName.
        // While the JLS claims a later step determines whether or not
        // a package of that name actually exists, it is pointless to defer
        // the error that long, as a package cannot qualify a method or field
        // access, and a subpackage requires the base package to exist.
        //
        else
        {
            NameSymbol* name_symbol =
                 (name -> identifier_token->GetNameSymbol());
            PackageSymbol* package =
                lookup_env.external_table.FindPackageSymbol(name_symbol);
            if (! package)
            {
                //
                // One last check in case the package was not imported.
                //
                package = lookup_env.external_table.InsertPackageSymbol(name_symbol,
                                                                     NULL);
				lookup_env.nameEnv->FindPathsToDirectory(package);
            }
            if (package -> directory.Length())
                name -> symbol = package;
            else
            {
                ReportVariableNotFound(name, this_type);
                name -> symbol = lookup_env.no_type;
            }
        }
    }
    //
    // ...If the ambiguous name is a qualified name,...
    //
    else
    {
        //
        // ...First, classify the name or expression to the left of the '.'...
        //
        AstName* base = name -> base_opt;
        ProcessAmbiguousName(base);

        TypeSymbol* type = base -> Type();
        assert(type|| 
			PackageSymbol::PackageCast(base->symbol) );

        if (type == lookup_env.no_type)
        {
            name -> symbol = lookup_env.no_type;
            return;
        }
       
		PackageSymbol* package =  PackageSymbol::PackageCast(base->symbol);
        if (package)
        {
            //
            // ... If there is a package whose name is the name to the
            // left of the '.' and that package contains a declaration of
            // a type whose name is the same as the Identifier, then the
            // AmbiguousName is reclassified as a TypeName...
            //
            NameSymbol* name_symbol =
                (name -> identifier_token->GetNameSymbol());
            type = package -> FindTypeSymbol(name_symbol);

            if (type)
            {
                if (type -> SourcePending())
                    lookup_env.ProcessFileHeaders(type -> file_symbol);
                name -> symbol = type;
            }
            else
            {
                FileSymbol* file_symbol =
					lookup_env.GetFile(lookup_env.option, package, name_symbol);
                if (file_symbol)
                {
                    type = ReadType(file_symbol, package, name_symbol,
                                    name -> identifier_token);
                    name -> symbol = type;
                }
                //
                // ... Otherwise, this AmbiguousName is reclassified as a
                // PackageName. While the JLS claims a later step
                // determines whether or not a package of that name
                // actually exists, it is pointless to defer the error
                // that long, as a package cannot qualify a method or field
                // access, and a subpackage requires the base package to
                // exist.
                //
                else
                {
                    PackageSymbol* subpackage =
                        package -> FindPackageSymbol(name_symbol);
                    if (! subpackage)
                    {
                        //
                        // One last check in case the subpackage was not
                        // imported.
                        //
                        subpackage =
                            package -> InsertPackageSymbol(name_symbol);
						lookup_env.nameEnv->FindPathsToDirectory(subpackage);
                    }
                    if (subpackage -> directory.Length())
                        name -> symbol = subpackage;
                    else
                    {
                        ReportSemErrorWithAst(SemanticError::UNKNOWN_AMBIGUOUS_NAME,
                                       name, name_symbol -> Name());
                        name -> symbol = lookup_env.no_type;
                    }
                }
            }
        }
        // ...Whether the qualifier is a type name, variable, or method
        // call, this is a regular field access
        //
        else
        {
            FindVariableMember(type, name);
            AddDependence(this_type, type, name -> IsConstant());
        }
    }
}


void Semantic::ProcessFieldAccess(Ast* expr)
{
    FieldReference* field_access = (FieldReference*) expr;
    assert(! AstName::NameCast(field_access->base));
    ProcessExpressionOrStringConstant(field_access -> base);

    TypeSymbol* type = field_access -> base -> Type();
    assert(type);
    if (type == lookup_env.no_type)
    {
        field_access -> symbol = lookup_env.no_type;
        return;
    }
    FindVariableMember(type, field_access);
    AddDependence(ThisType(), type);

    if (field_access -> symbol != lookup_env.no_type)
    {
		PackageSymbol* package = PackageSymbol::PackageCast(field_access->symbol);
        if (package)
        {
            ReportSemErrorWithAst(SemanticError::UNKNOWN_AMBIGUOUS_NAME,
                           field_access, package -> PackageName());
            field_access -> symbol = lookup_env.no_type;
        }
        else if (TypeSymbol::TypeCast(field_access -> symbol))
        {
            type = (TypeSymbol*) field_access -> symbol;
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_FIELD,
                           field_access, type -> Name());
            field_access -> symbol = lookup_env.no_type;
        }
        else
        {
            //
            // Either it's not a variable (an error) or the signature of
            // the variable has been typed
            //
            assert(!VariableSymbol::VariableCast(field_access -> symbol) ||
				VariableSymbol::VariableCast(field_access -> symbol ) -> IsTyped());
        }
    }
}


void Semantic::ProcessCharacterLiteral(Ast* expr)
{
    CharLiteral* char_literal = (CharLiteral*) expr;

    LiteralSymbol* literal =
        (char_literal -> character_literal_token->GetLiteralSymbol());

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


void Semantic::ProcessIntegerLiteral(Ast* expr)
{
    IntegerLiteral* int_literal = (IntegerLiteral*) expr;

    LiteralSymbol* literal =
         (int_literal -> integer_literal_token->GetLiteralSymbol());

	if(!literal)
	{
		ReportSemErrorWithAst(SemanticError::INVALID_INT_VALUE, int_literal);
		int_literal->symbol = lookup_env.no_type;
		return;
	}

    if (! literal -> value)
        lookup_env.nameEnv->int_pool.FindOrInsertInt(literal);
    if (literal -> value == lookup_env.BadValue())
    {
        ReportSemErrorWithAst(SemanticError::INVALID_INT_VALUE, int_literal);
        int_literal -> symbol = lookup_env.no_type;
    }
    else
    {
        int_literal -> value = literal -> value;
        int_literal -> symbol = lookup_env.int_type;
    }
}


void Semantic::ProcessLongLiteral(Ast* expr)
{
    LongLiteral* long_literal = (LongLiteral*) expr;

    LiteralSymbol* literal =
        (long_literal -> long_literal_token->GetLiteralSymbol());
	if (!literal)
	{
		ReportSemErrorWithAst(SemanticError::INVALID_LONG_VALUE, long_literal);
		long_literal->symbol = lookup_env.no_type;
		return;
	}
    if (! literal -> value)
        lookup_env.nameEnv->long_pool.FindOrInsertLong(literal);
    if (literal -> value == lookup_env.BadValue())
    {
        ReportSemErrorWithAst(SemanticError::INVALID_LONG_VALUE, long_literal);
        long_literal -> symbol = lookup_env.no_type;
    }
    else
    {
        long_literal -> value = literal -> value;
        long_literal -> symbol = lookup_env.long_type;
    }
}


void Semantic::ProcessFloatLiteral(Ast* expr)
{
    FloatLiteral* float_literal = (FloatLiteral*) expr;

    LiteralSymbol* literal =
         (float_literal -> float_literal_token->GetLiteralSymbol());
	if (!literal)
	{
		ReportSemErrorWithAst(SemanticError::INVALID_FLOAT_VALUE, float_literal);
		float_literal->symbol = lookup_env.no_type;
		return;
	}
    if (! literal -> value)
        lookup_env.nameEnv->float_pool.FindOrInsertFloat(literal);
    if (lookup_env.option->source < JikesOption::SDK1_5 &&
        (literal -> Name()[1] == U_x || literal -> Name()[1] == U_X))
    {
        ReportSemErrorWithAst(SemanticError::HEX_FLOATING_POINT_UNSUPPORTED,
                       float_literal);
    }
    if (literal -> value == lookup_env.BadValue())
    {
        ReportSemErrorWithAst(SemanticError::INVALID_FLOAT_VALUE, float_literal);
        float_literal -> symbol = lookup_env.no_type;
    }
    else
    {
        float_literal -> value = literal -> value;
        float_literal -> symbol = lookup_env.float_type;
    }
}


void Semantic::ProcessDoubleLiteral(Ast* expr)
{
    DoubleLiteral* double_literal = (DoubleLiteral*) expr;

    LiteralSymbol* literal =
        (double_literal -> double_literal_token->GetLiteralSymbol());

	if (!literal)
	{
		ReportSemErrorWithAst(SemanticError::INVALID_DOUBLE_VALUE, double_literal);
		double_literal->symbol = lookup_env.no_type;
		return;
	}

    if (! literal -> value)
        lookup_env.nameEnv->double_pool.FindOrInsertDouble(literal);
    if (lookup_env.option->source < JikesOption::SDK1_5 &&
        (literal -> Name()[1] == U_x || literal -> Name()[1] == U_X))
    {
        ReportSemErrorWithAst(SemanticError::HEX_FLOATING_POINT_UNSUPPORTED,
                       double_literal);
    }
    if (literal -> value == lookup_env.BadValue())
    {
        ReportSemErrorWithAst(SemanticError::INVALID_DOUBLE_VALUE, double_literal);
        double_literal -> symbol = lookup_env.no_type;
    }
    else
    {
        double_literal -> value = literal -> value;
        double_literal -> symbol = lookup_env.double_type;
    }
}


void Semantic::ProcessTrueLiteral(Ast* expr)
{
    Expression* true_literal = (TrueLiteral*) expr;

    true_literal -> value = lookup_env.nameEnv->int_pool.FindOrInsert((int) 1);
    true_literal -> symbol = lookup_env.boolean_type;
}


void Semantic::ProcessFalseLiteral(Ast* expr)
{
    Expression* false_literal = (FalseLiteral*) expr;

    false_literal -> value = lookup_env.nameEnv->int_pool.FindOrInsert((int) 0);
    false_literal -> symbol = lookup_env.boolean_type;
}


void Semantic::ProcessStringLiteral(Ast* expr)
{
    StringLiteral* string_literal = (StringLiteral*) expr;

    LiteralSymbol* literal =
        (string_literal -> string_literal_token->GetLiteralSymbol());
	if (!literal)
	{
		string_literal->symbol = lookup_env.no_type;
		return;
	}
	
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


void Semantic::ProcessArrayAccess(Ast* expr)
{
    ArrayReference* array_access = (ArrayReference*) expr;

    ProcessExpression(array_access -> base);
    ProcessExpression(array_access -> expression);
    array_access -> expression =
        PromoteUnaryNumericExpression(array_access -> expression);
    if (array_access -> expression -> Type() != lookup_env.int_type)
    {
        TypeSymbol* type = array_access -> expression -> Type();
        if (array_access -> expression -> symbol != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGER,
                           array_access -> expression,
                           type -> ContainingPackageName(),
                           type -> ExternalName());
        array_access -> symbol = lookup_env.no_type;
    }

    TypeSymbol* array_type = array_access -> base -> Type();
    if (array_type -> IsArray())
    {
        if (! array_access -> symbol)
            array_access -> symbol = array_type -> ArraySubtype();
    }
    else
    {
        if (array_type != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_ARRAY,
                           array_access -> base,
                           array_type -> ContainingPackageName(),
                           array_type -> ExternalName());
        array_access -> symbol = lookup_env.no_type;
    }
}


MethodShadowSymbol* Semantic::FindMethodMember(TypeSymbol* type,
                                               AstMethodInvocation* method_call)
{
    Expression* base = method_call -> base_opt;
    auto id_token = method_call -> identifier_token;
    assert(base);
    //
    // TypeCast() returns true for super, this, and instance creation as
    // well as true type names, hence the extra check
    //
    bool base_is_type = TypeSymbol::TypeCast(base -> symbol) && AstName::NameCast(base);
    MethodShadowSymbol* shadow = NULL;

    if (type -> Bad())
    {
        //
        // If no error has been detected so far, report this as an error so
        // that we don't try to generate code later. On the other hand, if an
        // error had been detected prior to this, don't flood the user with
        // spurious messages.
        //
        if (NumErrors() == 0)
            ReportMethodNotFound(method_call, type);
        method_call -> symbol = lookup_env.no_type;
    }
    else if (type == lookup_env.null_type || type -> Primitive())
    {
        ReportSemErrorWithAst(SemanticError::TYPE_NOT_REFERENCE, base,
                       type -> Name());
        method_call -> symbol = lookup_env.no_type;
    }
    else
    {
        TypeSymbol* this_type = ThisType();
        if (! TypeAccessCheck(type))
        {
            ReportTypeInaccessible(base, type);
            method_call -> symbol = lookup_env.no_type;
            return shadow;
        }

        shadow = FindMethodInType(type, method_call);
        MethodSymbol* method = (shadow ? shadow -> method_symbol
                                : (MethodSymbol*) NULL);
        if (method)
        {
            assert(method -> IsTyped());

            if (base_is_type && ! method -> ACC_STATIC())
            {
                ReportSemError(SemanticError::METHOD_NOT_CLASS_METHOD,
                               method_call -> LeftToken(), id_token,
                                (id_token->NameString()));
                method_call -> symbol = lookup_env.no_type;
                return NULL;
            }
            if (method -> ACC_STATIC() && ! base_is_type)
            {
                ReportSemError(SemanticError::CLASS_METHOD_INVOKED_VIA_INSTANCE,
                               method_call -> LeftToken(), id_token,
					(id_token->NameString()));
            }

            //
            // Apply method invocation conversion to the parameters
            //
            MethodInvocationConversion(method_call -> arguments, method);

            //
            // Access to a private or protected variable in or via an enclosing
            // type? If the base is a super expression, be sure to start from
            // the correct enclosing instance.
            //
            TypeSymbol* containing_type = method -> containing_type;
            TypeSymbol* target_type = containing_type;
            if (! method -> ACC_STATIC() && AstSuperExpression::SuperExpressionCast(base))
            {
                AstSuperExpression* super_expr = (AstSuperExpression*) base;
                if (super_expr -> base_opt)
                    target_type = super_expr -> base_opt -> symbol;
            }
            if (this_type != target_type &&
                (method -> ACC_PRIVATE() ||
                 (method -> ACC_PROTECTED() &&
                  ! ProtectedAccessCheck(containing_type)) ||
                 (target_type != containing_type &&
                  target_type != this_type)))
            {
                //
                // Find the right enclosing class to place the accessor method
                // in. For private methods, the containing type; for protected
                // methods or superclass methods, an enclosing class which is
                // related to the containing type.
                //
                TypeSymbol* environment_type = containing_type;
                if (! method -> ACC_PRIVATE())
                {
                    for (SemanticEnvironment* env = this_type -> semantic_environment;
                         env; env = env -> previous)
                    {
                        if (env -> Type() -> IsSubclass(target_type))
                        {
                            environment_type = env -> Type();
                            break;
                        }
                    }
                    assert(environment_type != containing_type &&
                           environment_type != this_type);
                }

                AstArguments* args = compilation_unit -> ast_pool ->
                    GenArguments(method_call -> arguments -> left_parenthesis_token,
                                 method_call -> arguments -> right_parenthesis_token);
                unsigned num_args = method_call -> arguments -> NumArguments();
                if (! method -> ACC_STATIC())
                {
                    args -> AllocateArguments(num_args + 1);
                    args -> AddArgument(base);
                }
                else args -> AllocateArguments(num_args);
                for (unsigned i = 0; i < num_args; i++)
                    args -> AddArgument(method_call -> arguments -> Argument(i));

                AstMethodInvocation* accessor = compilation_unit ->
                    ast_pool -> GenMethodInvocation(id_token);
                // TODO: WARNING: sharing of subtrees...
                accessor -> base_opt = base;
                accessor -> arguments = args;
                accessor -> symbol = environment_type ->
                    GetReadAccessMethod(method, base -> Type());

                method_call -> symbol = method;
                method_call -> resolution_opt = accessor;
            }
            else method_call -> symbol = method;
        }
        else
        {
            method_call -> symbol = lookup_env.no_type;
        }
    }
    return shadow;
}


void Semantic::ProcessMethodName(AstMethodInvocation* method_call)
{
    TypeSymbol* this_type = ThisType();
    Expression* base = method_call -> base_opt;
    auto id_token = method_call -> identifier_token;
    TypeSymbol* base_type;
    MethodShadowSymbol* method_shadow;
    if (! base)
    {
        SemanticEnvironment* where_found;
        method_shadow = FindMethodInEnvironment(where_found, method_call);
        if (! method_shadow)
        {
            method_call -> symbol = lookup_env.no_type;
            base_type = NULL;
        }
        else
        {
            base_type = where_found -> Type();
            MethodSymbol* method = method_shadow -> method_symbol;
            assert(method -> IsTyped());

            if (! method -> ACC_STATIC())
            {
                if (ExplicitConstructorInvocation())
                {
                    if (where_found == state_stack.Top())
                    {
                        //
                        // If the method belongs to this type, including
                        // inherited from an enclosing type, it is not
                        // accessible.
                        //
                        ReportSemErrorWithAst(SemanticError::INSTANCE_METHOD_IN_EXPLICIT_CONSTRUCTOR,
                                       method_call, method -> Header(),
                                       method -> containing_type -> Name());
                        method_call -> symbol = lookup_env.no_type;
                        method_shadow = NULL;
                    }
                }
                else if (StaticRegion())
                {
                    ReportSemErrorWithAst(SemanticError::METHOD_NOT_CLASS_METHOD,
                                   method_call,
						id_token->NameString());
                    method_call -> symbol = lookup_env.no_type;
                    method_shadow = NULL;
                }
            }

            //
            // Apply method invocation conversion to the parameters
            //
            MethodInvocationConversion(method_call -> arguments, method);
            method_call -> symbol = method;

            //
            // If the method is a private method belonging to an outer type,
            // give the ast simple_name access to its read_method.
            //
            if (where_found != state_stack.Top())
                CreateAccessToScopedMethod(method_call, where_found -> Type());
        }
    }
    else
    {
        //
        // ...First, classify the name or expression to the left of the '.'...
        // If there are more names to the left, we short-circuit
        // ProcessFieldAccess, since we already know what context the name
        // is in.
        //
        if (AstName::NameCast(base))
            ProcessAmbiguousName((AstName*) base);
        else // The qualifier might be a complex String constant
            ProcessExpressionOrStringConstant(base);

        if (PackageSymbol::PackageCast(base->symbol))
        {
            ReportSemErrorWithAst(SemanticError::UNKNOWN_AMBIGUOUS_NAME, base,
				PackageSymbol::PackageCast(base->symbol)-> PackageName());
            base -> symbol = lookup_env.no_type;
        }

        base_type = base -> Type();
        assert(base_type);

        if (base_type == lookup_env.no_type)
        {
            method_call -> symbol = lookup_env.no_type;
            method_shadow = NULL;
        }
        else
            method_shadow = FindMethodMember(base_type, method_call);
        if ( AstSuperExpression::SuperExpressionCast(base))
        {
            //
            // JLS2 15.12.3 requires this test
            //
			MethodSymbol* method = MethodSymbol::MethodCast(method_call->symbol);
            if (method && method -> ACC_ABSTRACT())
            {
                ReportSemErrorWithAst(SemanticError::ABSTRACT_METHOD_INVOCATION,
                               method_call,
					id_token->NameString() );
            }
        }
        else AddDependence(this_type, base_type);
    }

    //
    // If we found a candidate, proceed to check the throws clauses. If
    // base_type inherited multiple abstract methods, then this calling
    // environment must merge the throws clauses (although it may invoke an
    // arbitrary method from the set). Be careful of default and protected
    // abstract methods which are not accessible when doing this merge.
    //
    if (method_shadow)
    {
        MethodSymbol* method = (MethodSymbol*) method_call -> symbol;
        if (! MemberAccessCheck(base_type, method, base))
        {
            assert(method_shadow -> NumConflicts() > 0);
            method = method_shadow -> Conflict(0);
            method_call -> symbol = method;
        }

        SymbolSet exceptions(method -> NumThrows());
        int i, j;
        // First, the base set
        for (i = method -> NumThrows(); --i >= 0; )
            exceptions.AddElement(method -> Throws(i));
        // Next, add all subclasses thrown in method conflicts
        for (i = method_shadow -> NumConflicts(); --i >= 0; )
        {
            MethodSymbol* conflict = method_shadow -> Conflict(i);
            conflict -> ProcessMethodThrows(this,
                                            method_call -> identifier_token);
            for (j = conflict -> NumThrows(); --j >= 0; )
            {
                TypeSymbol* candidate = conflict -> Throws(j);
                for (TypeSymbol* ex = (TypeSymbol*) exceptions.FirstElement();
                     ex; ex = (TypeSymbol*) exceptions.NextElement())
                {
                    if (candidate -> IsSubclass(ex))
                    {
                        exceptions.AddElement(candidate);
                        break;
                    }
                }
            }
        }
        // Finally, prune all methods not thrown by all conflicts, and report
        // uncaught exceptions.
        TypeSymbol* ex = (TypeSymbol*) exceptions.FirstElement();
        while (ex)
        {
            bool remove = false;
            for (i = method_shadow -> NumConflicts(); --i >= 0; )
            {
                MethodSymbol* conflict = method_shadow -> Conflict(i);
                for (j = conflict -> NumThrows(); --j >= 0; )
                {
                    TypeSymbol* candidate = conflict -> Throws(j);
                    if (ex -> IsSubclass(candidate))
                        break;
                }
                if (j < 0)
                {
                    remove = true;
                    break;
                }
            }
            TypeSymbol* temp = (TypeSymbol*) exceptions.NextElement();
            if (remove)
                exceptions.RemoveElement(ex);
            else if (UncaughtException(ex))
            {
				auto un_string =  UncaughtExceptionContext();
					ReportSemErrorWithAst(SemanticError::UNCAUGHT_METHOD_EXCEPTION,
						method_call, method->Header(),
						ex->ContainingPackageName(),
						ex->ExternalName(), un_string.c_str());
            }
    
            ex = temp;
        }

        SymbolSet* exception_set = TryExceptionTableStack().Top();
        if (exception_set)
            exception_set -> Union(exceptions);
    }
    else
    {
        //
        // There was no candidate, so we have no idea what can be thrown in
        // a try block if it had been a valid method call.
        //
        SymbolSet* exception_set = TryExceptionTableStack().Top();
        if (exception_set)
            exception_set -> AddElement(lookup_env.no_type);
    }
}


//
// Processes the argument list, returning true if the list contains an
// invalid expression.
//
bool Semantic::ProcessArguments(AstArguments* args)
{
    bool bad_argument = false;
    for (unsigned i = 0; i < args -> NumArguments(); i++)
    {
        Expression* expr = args -> Argument(i);
        ProcessExpressionOrStringConstant(expr);
        if (expr -> symbol == lookup_env.no_type)
            bad_argument = true;
        else if (expr -> Type() == lookup_env.void_type)
        {
            ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID, expr,
                           expr -> Type() -> Name());
            bad_argument = true;
        }
    }
    return bad_argument;
}


void Semantic::ProcessMethodInvocation(Ast* expr)
{
    AstMethodInvocation* method_call = (AstMethodInvocation*) expr;

    if (method_call -> type_arguments_opt)
    {
        ReportSemErrorWithAst(SemanticError::EXPLICIT_TYPE_ARGUMENTS_UNSUPPORTED,
                       method_call -> type_arguments_opt);
    }
    bool bad_argument = ProcessArguments(method_call -> arguments);
    if (bad_argument)
        method_call -> symbol = lookup_env.no_type;

    ProcessMethodName(method_call);
    assert(method_call -> symbol == lookup_env.no_type ||
           ((MethodSymbol*) method_call -> symbol) -> IsTyped());
}


void Semantic::ProcessNullLiteral(Ast* expr)
{
    //
    // Null is not a compile-time constant, so don't give it a value
    //
    NullLiteral* null_literal = (NullLiteral*) expr;
    null_literal -> symbol = lookup_env.null_type;
}


void Semantic::ProcessClassLiteral(Ast* expr)
{
    TypeSymbol* this_type = ThisType();
    ClassLiteralAccess* class_lit = (ClassLiteralAccess*) expr;
    //
    // In a clone, simply return control.no_type. We are in a clone only
    // when doing something like evaluating a forward reference to a final
    // field for its constant value, but a class literal has no constant
    // value. In such cases, this method will again be invoked when we
    // finally reach the field, and then it is finally appropriate to
    // resolve the reference.
    //
	if (error && error->InClone())
    {
        class_lit -> symbol = lookup_env.no_type;
        return;
    }
    ProcessType(class_lit -> type);
    TypeSymbol* type = class_lit -> type -> symbol;
    AddDependence(this_type, type -> BoxedType(lookup_env));
    if (type == lookup_env.no_type)
        class_lit -> symbol = lookup_env.no_type;
    else if (type -> Primitive())
    {
        if (type == lookup_env.int_type)
            class_lit -> symbol = lookup_env.Integer_TYPE_Field();
        else if (type == lookup_env.double_type)
            class_lit -> symbol = lookup_env.Double_TYPE_Field();
        else if (type == lookup_env.char_type)
            class_lit -> symbol = lookup_env.Character_TYPE_Field();
        else if (type == lookup_env.long_type)
            class_lit -> symbol = lookup_env.Long_TYPE_Field();
        else if (type == lookup_env.float_type)
            class_lit -> symbol = lookup_env.Float_TYPE_Field();
        else if (type == lookup_env.byte_type)
            class_lit -> symbol = lookup_env.Byte_TYPE_Field();
        else if (type == lookup_env.short_type)
            class_lit -> symbol = lookup_env.Short_TYPE_Field();
        else if (type == lookup_env.boolean_type)
            class_lit -> symbol = lookup_env.Boolean_TYPE_Field();
        else
        {
            assert(type == lookup_env.void_type);
            class_lit -> symbol = lookup_env.Void_TYPE_Field();
        }
    }
    else if (lookup_env.option->target < JikesOption::SDK1_5)
    {
        //
        // We have already checked that the type is accessible. Older VMs
        // require a helper method to resolve the reference.
        //
        VariableSymbol* var = this_type -> FindOrInsertClassLiteral(type);
        AstName* name = compilation_unit -> ast_pool ->
            GenName(class_lit -> class_token);
        name -> symbol = var;
        class_lit -> symbol = var;
        class_lit -> resolution_opt = name;
    }
    else class_lit -> symbol = lookup_env.Class();
}


void Semantic::ProcessThisExpression(Ast* expr)
{
    TypeSymbol* this_type = ThisType();
    AstThisExpression* this_expression = (AstThisExpression*) expr;
    AstTypeName* base = this_expression -> base_opt;
    if (base)
    {
        ProcessType(base);
        TypeSymbol* enclosing_type = base -> symbol;
        if (enclosing_type == lookup_env.no_type)
            this_expression -> symbol = lookup_env.no_type;
        else if (! enclosing_type)
        {
            ReportSemErrorWithAst(SemanticError::NOT_A_TYPE, base);
            this_expression -> symbol = lookup_env.no_type;
        }
        else if (enclosing_type -> ACC_INTERFACE())
        {
            ReportSemErrorWithAst(SemanticError::NOT_A_CLASS, base,
                           enclosing_type -> ContainingPackageName(),
                           enclosing_type -> ExternalName());
            this_expression -> symbol = lookup_env.no_type;
        }
        else if (ExplicitConstructorInvocation() &&
                 enclosing_type == this_type)
        {
            ReportSemError(SemanticError::SELF_IN_EXPLICIT_CONSTRUCTOR,
                           base -> LeftToken(),
                           this_expression -> this_token,
                           StringConstant::US_this);
            this_expression -> symbol = lookup_env.no_type;
        }
        else if (! this_type -> IsNestedIn(enclosing_type))
        {
            ReportSemError(SemanticError::ILLEGAL_THIS_FIELD_ACCESS,
                           base -> LeftToken(),
                           this_expression -> this_token,
                           enclosing_type -> ContainingPackageName(),
                           enclosing_type -> ExternalName(),
                           this_package -> PackageName(),
                           this_type -> ExternalName());
            this_expression -> symbol = lookup_env.no_type;
        }
        else if (this_type == enclosing_type)
        {
            if (StaticRegion())
            {
                ReportSemError(SemanticError::ENCLOSING_INSTANCE_NOT_ACCESSIBLE,
                               base -> LeftToken(),
                               this_expression -> this_token,
                               enclosing_type -> ContainingPackageName(),
                               enclosing_type -> ExternalName());
                this_expression -> symbol = lookup_env.no_type;
            }
            else this_expression -> symbol = this_type;
        }
        else
        {
            this_expression -> resolution_opt =
                CreateAccessToType(this_expression, enclosing_type);
            this_expression -> symbol =
                this_expression -> resolution_opt -> symbol;
        }
    }
    else // unqualified
    {
        if (ExplicitConstructorInvocation())
        {
            ReportSemErrorSimple(SemanticError::SELF_IN_EXPLICIT_CONSTRUCTOR,
                           this_expression -> this_token,
                           StringConstant::US_this);
            this_expression -> symbol = lookup_env.no_type;
        }
        else if (StaticRegion())
        {
            ReportSemErrorSimple(SemanticError::MISPLACED_THIS_EXPRESSION,
                           this_expression -> this_token);
            this_expression -> symbol = lookup_env.no_type;
        }
        else this_expression -> symbol = this_type;
    }
}


void Semantic::ProcessSuperExpression(Ast* expr)
{
    TypeSymbol* this_type = ThisType();
    AstSuperExpression* super_expression = (AstSuperExpression*) expr;
    AstTypeName* base = super_expression -> base_opt;
    if (base)
    {
        ProcessType(base);
        TypeSymbol* enclosing_type = base -> symbol;
        if (enclosing_type == lookup_env.no_type)
            super_expression -> symbol = lookup_env.no_type;
        else if (! enclosing_type)
        {
            ReportSemErrorWithAst(SemanticError::NOT_A_TYPE, base);
            super_expression -> symbol = lookup_env.no_type;
        }
        else if (enclosing_type -> ACC_INTERFACE())
        {
            ReportSemErrorWithAst(SemanticError::NOT_A_CLASS, base,
                           enclosing_type -> ContainingPackageName(),
                           enclosing_type -> ExternalName());
            super_expression -> symbol = lookup_env.no_type;
        }
        else if (this_type == lookup_env.Object())
        {
            ReportSemError(SemanticError::OBJECT_HAS_NO_SUPER_TYPE,
                           base -> LeftToken(),
                           super_expression -> super_token);
            super_expression -> symbol = lookup_env.no_type;
        }
        else if (ExplicitConstructorInvocation() &&
                 enclosing_type == this_type)
        {
            ReportSemError(SemanticError::SELF_IN_EXPLICIT_CONSTRUCTOR,
                           base -> LeftToken(),
                           super_expression -> super_token,
                           StringConstant::US_super);
            super_expression -> symbol = lookup_env.no_type;
        }
        else if (! this_type -> IsNestedIn(enclosing_type))
        {
            ReportSemError(SemanticError::ILLEGAL_THIS_FIELD_ACCESS,
                           base -> LeftToken(),
                           super_expression -> super_token,
                           enclosing_type -> ContainingPackageName(),
                           enclosing_type -> ExternalName(),
                           this_package -> PackageName(),
                           this_type -> ExternalName());
            super_expression -> symbol = lookup_env.no_type;
        }
        else if (this_type == enclosing_type)
        {
            if (StaticRegion())
            {
                ReportSemError(SemanticError::ENCLOSING_INSTANCE_NOT_ACCESSIBLE,
                               base -> LeftToken(),
                               super_expression -> super_token,
                               enclosing_type -> ContainingPackageName(),
                               enclosing_type -> ExternalName());
                super_expression -> symbol = lookup_env.no_type;
            }
            else super_expression -> symbol = this_type -> super;
        }
        else
        {
            super_expression -> resolution_opt =
                CreateAccessToType(super_expression, enclosing_type);
            super_expression -> symbol =
                super_expression -> resolution_opt -> symbol;
        }
    }
    else // unqualified
    {
        if (ThisType() == lookup_env.Object())
        {
            ReportSemErrorSimple(SemanticError::OBJECT_HAS_NO_SUPER_TYPE,
                           super_expression -> super_token);
            super_expression -> symbol = lookup_env.no_type;
        }
        else if (ExplicitConstructorInvocation())
        {
            ReportSemErrorSimple(SemanticError::SELF_IN_EXPLICIT_CONSTRUCTOR,
                           super_expression -> super_token,
                           StringConstant::US_super);
            super_expression -> symbol = lookup_env.no_type;
        }
        else if (StaticRegion())
        {
            ReportSemErrorSimple(SemanticError::MISPLACED_SUPER_EXPRESSION,
                           super_expression -> super_token);
            super_expression -> symbol = lookup_env.no_type;
        }
        else super_expression -> symbol = ThisType() -> super;
    }
}


void Semantic::ProcessParenthesizedExpression(Ast* expr)
{
    AstParenthesizedExpression* parenthesized =
        (AstParenthesizedExpression*) expr;

    //
    // Do not use ProcessExpressionOrStringConstant here, to avoid generating
    // intermediate Strings - see CheckConstantString in lookup.cpp
    //
    ProcessExpression(parenthesized -> expression);
    if (parenthesized -> expression -> Type() == lookup_env.void_type)
    {
        ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID,
                       parenthesized -> expression,
                       lookup_env.void_type -> Name());
        parenthesized -> symbol = lookup_env.no_type;
    }
    else
    {
        parenthesized -> value = parenthesized -> expression -> value;
        parenthesized -> symbol = parenthesized -> expression -> symbol;
    }
}


void Semantic::UpdateLocalConstructors(TypeSymbol* inner_type)
{
    assert(inner_type -> IsLocal() &&
           (! inner_type -> Anonymous() || ! inner_type -> EnclosingType()));

    //
    // Update the constructor signatures to account for local shadow
    // parameters.
    //
    inner_type -> MarkLocalClassProcessingCompleted();
    unsigned param_count = inner_type -> NumConstructorParameters();
    if (param_count)
    {
        MethodSymbol* ctor;
        for (ctor = inner_type -> FindMethodSymbol(lookup_env.nameEnv->init_name_symbol);
             ctor; ctor = ctor -> next_method)
        {
            ctor -> SetSignature(lookup_env);
        }
        for (unsigned j = 0;
             j < inner_type -> NumPrivateAccessConstructors(); j++)
        {
            inner_type -> PrivateAccessConstructor(j) ->
                SetSignature(lookup_env, (inner_type -> outermost_type ->
                                       GetPlaceholderType()));
        }
    }

    //
    // Update all constructor call contexts that were pending on this class.
    // These calls are necessarily located within the body of inner_type, and
    // are calling a constructor in inner_type.
    //
    for (unsigned i = 0;
         i < inner_type -> NumLocalConstructorCallEnvironments(); i++)
    {
        SemanticEnvironment* env =
            inner_type -> LocalConstructorCallEnvironment(i);
        state_stack.Push(env);
        AstArguments* args = env -> args;

        args -> AllocateLocalArguments(param_count);
        for (unsigned k = 0; k < param_count; k++)
        {
            AstName* name = compilation_unit ->
                ast_pool -> GenName(args -> right_parenthesis_token);
            VariableSymbol* accessor =
                FindLocalVariable(inner_type -> ConstructorParameter(k),
                                  ThisType());
            name -> symbol = accessor;
            TypeSymbol* owner = accessor -> ContainingType();
            if (owner != ThisType())
                CreateAccessToScopedVariable(name, owner);
            args -> AddLocalArgument(name);
        }
        if (ThisType() -> Anonymous() &&
            ! ThisType() -> LocalClassProcessingCompleted())
        {
            UpdateLocalConstructors(ThisType());
        }
        state_stack.Pop();
    }
}


//
// This creates the default constructor for an anonymous class, and sets
// the resolution_opt field of the original to a generated instance creation
// expression that has been adjusted for compilation purposes.
//
void Semantic::GetAnonymousConstructor(AstClassCreationExpression* class_creation,
                                       TypeSymbol* anonymous_type)
{
    auto left_loc = class_creation -> class_type -> LeftToken();
	auto right_loc =
        class_creation -> arguments -> right_parenthesis_token;

    state_stack.Push(anonymous_type -> semantic_environment);
    TypeSymbol* super_type = anonymous_type -> super;
    MethodSymbol* super_constructor =   FindConstructor(super_type,
                                                      class_creation,
                                                      left_loc, right_loc);
    if (! super_constructor)
    {
        class_creation -> class_type -> symbol = lookup_env.no_type;
        state_stack.Pop();
        return;
    }
    assert(super_constructor -> IsTyped());

    //
    // Make replacement class instance creation expression.
    //
    AstArguments* resolution_args = compilation_unit -> ast_pool ->
        GenArguments(class_creation -> arguments -> left_parenthesis_token,
                     right_loc);

    AstClassCreationExpression* resolution =
        compilation_unit -> ast_pool -> GenClassCreationExpression();
    resolution -> new_token = class_creation -> new_token;
    // TODO: WARNING: sharing of subtrees...
    resolution -> class_type = class_creation -> class_type;
    resolution -> arguments = resolution_args;
    resolution -> symbol = anonymous_type;
    class_creation -> resolution_opt = resolution;

    //
    // Make constructor symbol. The associated symbol table will not contain
    // too many elements...
    //
    BlockSymbol* block_symbol =
        new BlockSymbol(super_constructor -> NumFormalParameters() + 3);
    block_symbol -> max_variable_index = 1; // A spot for "this".

    MethodSymbol* constructor =
        anonymous_type -> InsertMethodSymbol(lookup_env.nameEnv->init_name_symbol);
	constructor->MarkConstructor();
    constructor -> SetType(anonymous_type);
    constructor -> SetContainingType(anonymous_type);
    constructor -> SetBlockSymbol(block_symbol);

    //
    // Anonymous class constructors may throw any exception listed in the
    // superclass; but this list may be expanded later since the anonymous
    // constructor also throws anything possible in instance initializers.
    //
    for (unsigned i = 0; i < super_constructor -> NumThrows(); i++)
        constructor -> AddThrows(super_constructor -> Throws(i));

    //
    // If we are in a static region, the anonymous constructor does not need
    // a this$0 argument. Otherwise, a this$0 argument that points to an
    // instance of the immediately enclosing class is required.
    //
    if (anonymous_type -> EnclosingType())
    {
        VariableSymbol* this0_variable =
            block_symbol -> InsertVariableSymbol(lookup_env.nameEnv->this_name_symbol);
        this0_variable -> SetType(anonymous_type -> EnclosingType());
        this0_variable -> SetOwner(constructor);
        this0_variable -> SetFlags(AccessFlags::ACCESS_FINAL |
                                   AccessFlags::ACCESS_SYNTHETIC);
        this0_variable -> SetLocalVariableIndex(block_symbol ->
                                                max_variable_index++);
        this0_variable -> MarkComplete();
        AstThisExpression* this0_expression =
            compilation_unit -> ast_pool -> GenThisExpression(left_loc);
        this0_expression -> symbol = anonymous_type -> EnclosingType();
        resolution -> base_opt = this0_expression;
    }

    //
    // Create an explicit call to the superconstructor, passing any necessary
    // shadow variables or enclosing instances.
    //
    AstArguments* super_args = compilation_unit -> ast_pool ->
        GenArguments(class_creation -> arguments -> left_parenthesis_token,
                     right_loc);

    AstSuperCall* super_call = compilation_unit -> ast_pool -> GenSuperCall();
    if (super_constructor -> ACC_PRIVATE())
    {
        super_constructor =
            super_type -> GetReadAccessConstructor(super_constructor);
        super_args -> AddNullArgument();
    }

    // Use initial base_opt.
    super_call -> base_opt = class_creation -> base_opt;
    super_call -> super_token = class_creation -> new_token;
    super_call -> arguments = super_args;
    super_call -> semicolon_token = right_loc;
    super_call -> symbol = super_constructor;

	AstClassBody* class_body = nullptr;
	if(  class_creation->anonymous_type_opt)
	{
		class_body = class_creation->anonymous_type_opt->class_body;
	};

    //
    // Construct the default constructor of the anonymous type.
    //
    AstMethodBody* constructor_block =
        compilation_unit -> ast_pool -> GenMethodBody();
    // This symbol table will be empty.
    constructor_block -> block_symbol =
        constructor -> block_symbol -> InsertBlockSymbol(0);
    constructor_block -> left_brace_token = class_body -> left_brace_token;
    constructor_block -> right_brace_token = class_body -> left_brace_token;
    constructor_block -> explicit_constructor_opt = super_call;
    constructor_block -> AllocateStatements(1); // for the generated return

    AstMethodDeclarator* method_declarator =
        compilation_unit -> ast_pool -> GenMethodDeclarator();
    method_declarator -> identifier_token = left_loc;
    method_declarator -> left_parenthesis_token =
        class_creation -> arguments -> left_parenthesis_token;
    method_declarator -> right_parenthesis_token = right_loc;

    ConstructorDeclaration* constructor_declaration  =
        compilation_unit -> ast_pool -> GenConstructorDeclaration();
    constructor_declaration ->method_declarator = method_declarator;
    constructor_declaration -> constructor_body = constructor_block;
	constructor_block->owner_ast = constructor_declaration;

    constructor_declaration ->method_symbol = constructor;

    constructor -> declaration = constructor_declaration;
    class_body -> default_constructor = constructor_declaration;


    //
    // Update the enclosing instance of the supertype.
    //
    unsigned num_args = class_creation -> arguments -> NumArguments();
    if (class_creation -> base_opt)
    {
        VariableSymbol* super_this0_variable =
            block_symbol -> InsertVariableSymbol(lookup_env.MakeParameter(0));
        super_this0_variable -> SetACC_SYNTHETIC();
        super_this0_variable -> SetType(super_call -> base_opt -> Type());
        super_this0_variable -> SetOwner(constructor);
        super_this0_variable -> SetLocalVariableIndex(block_symbol ->
                                                      max_variable_index++);
        super_this0_variable -> MarkComplete();

        resolution_args -> AllocateArguments(num_args + 1);
        resolution_args -> AddArgument(class_creation -> base_opt);
        constructor -> AddFormalParameter(super_this0_variable);

        AstName* name = compilation_unit -> ast_pool ->
            GenName(class_creation -> new_token);
        name -> symbol = super_this0_variable;
        super_call -> base_opt = name;
    }
    else resolution_args -> AllocateArguments(num_args);
    super_args -> AllocateArguments(super_constructor ->
                                    NumFormalParameters());

    //
    // Next, simply pass all parameters through to the superclass.
    //
    for (unsigned j = 0; j < super_constructor -> NumFormalParameters(); j++)
    {
        VariableSymbol* param = super_constructor -> FormalParameter(j);
        VariableSymbol* symbol =
            block_symbol -> InsertVariableSymbol(param -> Identity());
        symbol -> SetType(param -> Type());
        symbol -> SetOwner(constructor);
        symbol -> SetLocalVariableIndex(block_symbol -> max_variable_index++);
        symbol -> MarkComplete();
        if (lookup_env.IsDoubleWordType(symbol -> Type()))
            block_symbol -> max_variable_index++;

        resolution_args -> AddArgument(class_creation -> arguments -> Argument(j));
        constructor -> AddFormalParameter(symbol);
        AstName* name = compilation_unit -> ast_pool ->
            GenName(class_creation -> new_token);
        name -> symbol = symbol;
        super_args -> AddArgument(name);
    }

    //
    // Worry about shadow variables in the super type
    //
    if (super_type -> IsLocal())
    {
        unsigned param_count = super_type -> NumConstructorParameters();
        if (super_type -> LocalClassProcessingCompleted() && param_count)
        {
            super_args -> AllocateLocalArguments(param_count);
            for (unsigned k = 0; k < param_count; k++)
            {
                //
                // We may need to create a shadow in the outermost
                // local class enclosing the variable.
                //
                AstName* name = compilation_unit ->
                    ast_pool -> GenName(super_call -> super_token);
                VariableSymbol* accessor =
                    FindLocalVariable(super_type -> ConstructorParameter(k),
                                      anonymous_type);
                name -> symbol = accessor;
                TypeSymbol* owner = accessor -> ContainingType();
                if (owner != anonymous_type)
                    CreateAccessToScopedVariable(name, owner);
                super_args -> AddLocalArgument(name);
            }
        }
        else
        {
            //
            // We are within body of super_type; save processing for
            // later, since not all shadows may be known yet. See
            // ProcessClassDeclaration.
            //
            super_type -> AddLocalConstructorCallEnvironment
                (GetEnvironment(super_call -> arguments));
        }
    }
    //
    // We set the signature of the constructor now, although it may be modified
    // later if this is in a local constructor call environment.
    //
    constructor -> SetSignature(lookup_env);
    state_stack.Pop();
}

//
// super_type is the type specified in the anonymous constructor,
// which is the supertype of the created anonymous type.
//
TypeSymbol* Semantic::GetAnonymousType(AstClassCreationExpression* class_creation,
                                       TypeSymbol* super_type)
{
    //
    // In a clone, simply return control.no_type. We are in a clone only when
    // doing something like evaluating a forward reference to a final field for
    // its constant value, but an anonymous class has no constant value. In
    // such cases, this method will again be invoked when we finally reach the
    // field, and then it is finally appropriate to create the class.
    //
	if (error && error->InClone())
        return lookup_env.no_type;

    TypeSymbol* this_type = ThisType();
	AstClassBody* class_body = nullptr;

	class_body = class_creation->anonymous_type_opt->class_body;

    assert(class_body);
    TypeSymbol* outermost_type = this_type -> outermost_type;

    //
    // Anonymous and local classes can clash if we don't use both when
    // determining the id number of this class.
    //
    IntToWstring value(this_type -> NumLocalTypes() +
                       this_type -> NumAnonymousTypes() + 1);

    int length = this_type -> ExternalNameLength() + 1 +
        value.Length(); // +1 for $
    wchar_t* anonymous_name = new wchar_t[length + 1]; // +1 for '\0'
    wcscpy(anonymous_name, this_type -> ExternalName());
    wcscat(anonymous_name, (lookup_env.option->target < JikesOption::SDK1_5
                            ? StringConstant::US_DS : StringConstant::US_MI));
    wcscat(anonymous_name, value.String());

    NameSymbol* name_symbol = lookup_env.nameEnv->FindOrInsertName(anonymous_name, length);
    delete [] anonymous_name;

    assert(! ThisMethod() || LocalSymbolTable().Top());

    TypeSymbol* anon_type =
        this_type -> InsertAnonymousTypeSymbol(name_symbol);
    anon_type -> MarkAnonymous();
    anon_type -> outermost_type = outermost_type;
    anon_type -> supertypes_closure = new SymbolSet;
    anon_type -> subtypes_closure = new SymbolSet;
    anon_type -> semantic_environment =
        new SemanticEnvironment(this, anon_type, state_stack.Top());
    anon_type -> declaration = class_body;

	class_creation->anonymous_type_opt->symbol = anon_type;
	
    anon_type -> declaration -> semantic_environment =
        anon_type -> semantic_environment;
    anon_type -> file_symbol = source_file_symbol;
    if (ThisMethod())
        anon_type -> SetOwner(ThisMethod());
    else if (ThisVariable())
    {
        //
        // Creating an anonymous class in a field initializer necessarily
        // requires non-trivial code, so the initializer method should
        // exist as the owner of this type.
        //
        assert(ThisVariable() -> ACC_STATIC()
               ? this_type -> static_initializer_method
               : (this_type -> FindMethodSymbol(lookup_env.nameEnv->
                                                block_init_name_symbol)));
        anon_type ->
            SetOwner(ThisVariable() -> ACC_STATIC()
                     ? this_type -> static_initializer_method
                     : (this_type ->
                        FindMethodSymbol(lookup_env.nameEnv->block_init_name_symbol)));
    }
    else
    {
        assert(class_creation -> generated);
        anon_type -> SetOwner(this_type);
    }

    //
    // Add 3 extra elements for padding. Need a default constructor and
    // other support elements.
    //
    anon_type -> SetSymbolTable(class_body -> NumClassBodyDeclarations() + 3);
    anon_type -> SetLocation();
    anon_type -> SetSignature(lookup_env);

    //
    // By JLS2 15.9.5, an anonymous class is implicitly final, but never
    // static. However, the anonymous class only needs access to its enclosing
    // instance if it is not in a static context.
    //
    anon_type -> SetACC_FINAL();
    if (! StaticRegion())
        anon_type -> InsertThis0();

    if (super_type -> ACC_INTERFACE())
    {
        anon_type -> AddInterface(super_type);
        anon_type -> super = lookup_env.Object();
        lookup_env.Object() -> subtypes -> AddElement(anon_type);
    }
    else anon_type -> super = super_type;
    AddDependence(anon_type, super_type);
    super_type -> subtypes -> AddElement(anon_type);
    if (super_type -> ACC_FINAL())
    {
         ReportSemErrorWithAst(SemanticError::SUPER_IS_FINAL,
                        class_creation -> class_type,
                        super_type -> ContainingPackageName(),
                        super_type -> ExternalName());
         anon_type -> MarkBad();
    }
    else if (super_type -> Bad())
        anon_type -> MarkBad();

    this_type -> AddAnonymousType(anon_type);

    //
    // Provide the default constructor. For now, we don't worry about accessors
    // to final local variables; those are inserted later when completing
    // the class instance creation processing. Also, the throws clause may
    // expand after processing instance initializer blocks. We keep on
    // processing, even if the constructor failed, to detect other semantic
    // errors in the anonymous class body.
    //
    GetAnonymousConstructor(class_creation, anon_type);

    //
    // Now process the body of the anonymous class !!!
    //
    CheckNestedMembers(anon_type, class_body);
    ProcessTypeHeaders(class_body, anon_type);

    //
    // If the class body has not yet been parsed, do so now.
    //
    if (AstClassBody::UnparsedClassBodyCast(class_body))
    {

       /* if (! control.parser -> InitializerParse(source_file_symbol, class_body))
             compilation_unit -> MarkBad();
        else
        {
            ProcessMembers(class_body);
            CompleteSymbolTable(class_body);
        }

        if (! control.parser -> BodyParse(source_file_symbol, class_body))
            compilation_unit -> MarkBad();
        else ProcessExecutableBodies(class_body);*/

		if (!class_body->initializerParseBad) {
			ProcessMembers(class_body);
			CompleteSymbolTable(class_body);
			if (!class_body->bodyParseBad) {
				ProcessExecutableBodies(class_body);
			}
			else {
				// Mark that syntax errors were detected.
				// compilation_unit -> MarkBad();;
			}

		}
		else
		{
			// Mark that syntax errors were detected.
			//compilation_unit->MarkBad(); 已经标记过了
		}


    }
    else // The relevant bodies have already been parsed
    {
        ProcessMembers(class_body);
        CompleteSymbolTable(class_body);
        ProcessExecutableBodies(class_body);
    }

    //
    // If we failed to provide a default constructor, this is as far as
    // we can go.
    //
    if (class_creation -> class_type -> symbol == lookup_env.no_type)
        return lookup_env.no_type;
	else if(!(class_creation->class_type->symbol))
	{
		class_creation->class_type->symbol =  lookup_env.no_type;
		return lookup_env.no_type;
	}
    //
    // Finally, mark the class complete, in order to add any shadow variable
    // parameters to the constructor.
    //
    if (! super_type -> IsLocal() ||
        super_type -> LocalClassProcessingCompleted() ||
        anon_type -> EnclosingType())
    {
        if (anon_type -> NumConstructorParameters() && ! anon_type -> Bad())
        {
            class_body -> default_constructor -> method_symbol ->
                SetSignature(lookup_env);
        }
        anon_type -> MarkLocalClassProcessingCompleted();
    }
    return anon_type;
}


void Semantic::ProcessClassCreationExpression(Ast* expr)
{
    AstClassCreationExpression* class_creation =
        (AstClassCreationExpression*) expr;
    unsigned i;

    //
    // For an anonymous type, the qualifier determines the enclosing instance
    // of the supertype; as the enclosing instance of the anonymous class (if
    // present) is the current class. We update actual_type after this.
    //
    AstName* actual_type = class_creation -> class_type -> name;
    TypeSymbol* type;
    if (class_creation -> base_opt)
    {
        ProcessExpression(class_creation -> base_opt);
        TypeSymbol* enclosing_type = class_creation -> base_opt -> Type();
        if (! enclosing_type -> IsSubclass(lookup_env.Object()))
        {
            if (enclosing_type != lookup_env.no_type)
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_REFERENCE,
                               class_creation -> base_opt,
                               enclosing_type -> ExternalName());
            enclosing_type = lookup_env.no_type;
        }

        //
        // The grammar guarantees that the actual type is a simple name.
        //
        type = MustFindNestedType(enclosing_type, actual_type);
        if (type -> ACC_INTERFACE())
        {
            ReportSemErrorWithAst(SemanticError::INTERFACE_NOT_INNER_CLASS,
                           actual_type, type -> ContainingPackageName(),
                           type -> ExternalName());
            type = lookup_env.no_type;
        }
        else if (type -> ACC_STATIC())
        {
            ReportSemErrorWithAst(SemanticError::STATIC_NOT_INNER_CLASS,
                           actual_type, type -> ContainingPackageName(),
                           type -> ExternalName());
            type = lookup_env.no_type;
        }
    }
    else
    {
        ProcessType(class_creation -> class_type);
        type = class_creation -> class_type -> symbol;
        if (type -> EnclosingType())
        {
            AstThisExpression* this_expr = compilation_unit -> ast_pool ->
                GenThisExpression(class_creation -> new_token);
            this_expr -> resolution_opt =
                CreateAccessToType(class_creation, type -> EnclosingType());
            this_expr -> symbol = this_expr -> resolution_opt -> symbol;
            class_creation -> base_opt = this_expr;
        }
    }

    //
    // Check the arguments to the constructor.
    //
    if (class_creation -> type_arguments_opt)
    {
        ReportSemErrorWithAst(SemanticError::EXPLICIT_TYPE_ARGUMENTS_UNSUPPORTED,
                       class_creation -> type_arguments_opt);
    }
    ProcessArguments(class_creation -> arguments);

    //
    // Create the anonymous class now, if needed; then check that the type
    // can be constructed. A side effect of creating the anonymous class is
    // building a resolution constructor invocation that does not have a body;
    // this new constructor is necessary to call parameters in the correct
    // order, when the superclass of the anonymous class has an enclosing
    // instance.
    //
    if (type -> IsEnum())
    {
        ReportSemErrorWithAst(SemanticError::CANNOT_CONSTRUCT_ENUM, actual_type,
                       type -> ContainingPackageName(),
                       type -> ExternalName());
        type = lookup_env.no_type;
    }
    else if (class_creation -> anonymous_type_opt)
    {
        type = GetAnonymousType(class_creation, type);
        class_creation -> symbol = type;
        if (type != lookup_env.no_type)
            class_creation = class_creation -> resolution_opt;
    }
    else if (type -> ACC_INTERFACE())
    {
        ReportSemErrorWithAst(SemanticError::NOT_A_CLASS, actual_type,
                       type -> ContainingPackageName(),
                       type -> ExternalName());
        type = lookup_env.no_type;
    }
    else if (type -> ACC_ABSTRACT())
    {
        ReportSemErrorWithAst(SemanticError::ABSTRACT_TYPE_CREATION, actual_type,
                       type -> ExternalName());
    }

    MethodSymbol* ctor =
        FindConstructor(type, class_creation, actual_type -> LeftToken(),
                        class_creation -> arguments -> right_parenthesis_token);
    //
    // Convert the arguments to the correct types.
    //
    if (ctor)
    {
        assert(ctor -> IsTyped());
        class_creation -> symbol = ctor;

        if (class_creation -> base_opt)
        {
            assert(CanAssignmentConvertReference(ctor -> containing_type -> EnclosingType(),
                                                 class_creation -> base_opt -> Type()));
            class_creation -> base_opt =
                ConvertToType(class_creation -> base_opt,
                              ctor -> containing_type -> EnclosingType());
        }
        MethodInvocationConversion(class_creation -> arguments, ctor);

        //
        // Process the throws clause.
        //
        SymbolSet* exception_set = TryExceptionTableStack().Top();
        for (i = 0; i < ctor -> NumThrows(); i++)
        {
            TypeSymbol* exception = ctor -> Throws(i);
            if (exception_set)
                exception_set -> AddElement(exception);

            if (UncaughtException(exception))
            {
				auto un_string = UncaughtExceptionContext();
				ReportSemErrorWithAst((class_creation->anonymous_type_opt
					? SemanticError::UNCAUGHT_ANONYMOUS_CONSTRUCTOR_EXCEPTION
					: SemanticError::UNCAUGHT_CONSTRUCTOR_EXCEPTION),
					actual_type, type->ExternalName(),
					exception->ContainingPackageName(),
					exception->ExternalName(),
					un_string.c_str());
            }
               
        }

        if (ctor -> ACC_PRIVATE() && ThisType() != type)
        {
            //
            // Add extra argument for read access constructor.
            //
            assert(ThisType() -> outermost_type == type -> outermost_type);
            ctor = type -> GetReadAccessConstructor(ctor);
            class_creation -> symbol = ctor;
            class_creation -> arguments -> AddNullArgument();
        }
    }
    else
    {
        //
        // No constructor was found (possibly because the type was not found),
        // so we don't know what exceptions could be thrown if the user fixes
        // the prior errors.
        //
        SymbolSet* exception_set = TryExceptionTableStack().Top();
        if (exception_set)
            exception_set -> AddElement(lookup_env.no_type);
        class_creation -> symbol = lookup_env.no_type;
    }

    //
    // A local type may use enclosed local variables. If so, we must add
    // the parameters which allow the local type to initialize its shadows.
    //
    if (type -> IsLocal())
    {
        if (type -> LocalClassProcessingCompleted())
        {
            unsigned param_count = type -> NumConstructorParameters();
            class_creation -> arguments -> AllocateLocalArguments(param_count);
            for (i = 0; i < param_count; i++)
            {
                //
                // Are we currently within the body of the method that
                // contains the local variable in question? If not, we may need
                // to create a shadow in the outermost local class enclosing
                // the variable.
                //
                AstName* name = compilation_unit ->
                    ast_pool -> GenName(class_creation -> new_token);
                VariableSymbol* accessor =
                    FindLocalVariable(type -> ConstructorParameter(i),
                                      ThisType());
                name -> symbol = accessor;
                TypeSymbol* owner = accessor -> ContainingType();
                if (owner != ThisType())
                    CreateAccessToScopedVariable(name, owner);
                class_creation -> arguments -> AddLocalArgument(name);
            }
        }
        else
        {
            //
            // We are within body of type; save processing for later, since
            // not all shadows may be known yet. See ProcessClassDeclaration
            // in body.cpp.
            //
            type -> AddLocalConstructorCallEnvironment
                (GetEnvironment(class_creation -> arguments));
        }
    }
}


void Semantic::ProcessArrayCreationExpression(Ast* expr)
{
    AstArrayCreationExpression* array_creation =
        (AstArrayCreationExpression*) expr;
    //
    // Either we have an initializer, or we have dimension expressions and
    // optional brackets.
    //
    assert(array_creation -> array_initializer_opt ?
           (! array_creation -> NumDimExprs() &&
            ! array_creation -> NumBrackets())
           : array_creation -> NumDimExprs());
    ProcessType(array_creation -> array_type);
    TypeSymbol* type = array_creation -> array_type -> symbol;
    unsigned dims = type -> num_dimensions +
        array_creation -> NumDimExprs() + array_creation -> NumBrackets();
    type = type -> GetArrayType(this, dims);
    array_creation -> symbol = type;

    for (unsigned i = 0; i < array_creation -> NumDimExprs(); i++)
    {
        AstDimExpr* dim_expr = array_creation -> DimExpr(i);
        ProcessExpression(dim_expr -> expression);
        Expression* expr =
            PromoteUnaryNumericExpression(dim_expr -> expression);
        if (expr -> Type() != lookup_env.int_type &&
            expr -> symbol != lookup_env.no_type)
        {
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGER,
                           dim_expr -> expression,
                           expr -> Type() -> ContainingPackageName(),
                           expr -> Type() -> ExternalName());
            array_creation -> symbol = lookup_env.no_type;
        }
        dim_expr -> expression = expr;
        if (expr -> IsConstant() &&
            expr -> Type() == lookup_env.int_type &&
            (DYNAMIC_CAST<IntLiteralValue*> (expr -> value)) -> value < 0)
        {
            ReportSemErrorWithAst(SemanticError::NEGATIVE_ARRAY_SIZE,
                           dim_expr -> expression);
        }
    }

    if (array_creation -> array_initializer_opt)
        ProcessArrayInitializer(array_creation -> array_initializer_opt, type);
}


void Semantic::ProcessPostUnaryExpression(Ast* expr)
{
    AstPostUnaryExpression* postfix_expression =
        (AstPostUnaryExpression*) expr;
    Expression* expression = postfix_expression -> expression;

    ProcessExpression(expression);
    postfix_expression -> symbol = expression -> symbol;

    //
    // JLS2 added ability for parenthesized variable to remain a variable.
    //
    if (  AstParenthesizedExpression::ParenthesizedExpressionCast(expression))
    {
        ReportSemErrorWithAst(SemanticError::UNNECESSARY_PARENTHESIS, expression);
        while ( AstParenthesizedExpression::ParenthesizedExpressionCast(expression))
            expression = ((AstParenthesizedExpression*) expression) ->
                expression;
    }

    if (expression -> symbol != lookup_env.no_type)
    {
        if (! expression -> IsLeftHandSide())
        {
            ReportSemErrorWithAst(SemanticError::NOT_A_NUMERIC_VARIABLE,
                           postfix_expression -> expression,
                           postfix_expression -> expression -> Type() -> Name());
            postfix_expression -> symbol = lookup_env.no_type;
        }
        else if (! lookup_env.IsNumeric(expression -> Type()))
        {
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_NUMERIC,
                           postfix_expression -> expression,
                           expression -> Type() -> ContainingPackageName(),
                           expression -> Type() -> ExternalName());
            postfix_expression -> symbol = lookup_env.no_type;
        }
        else if (! ArrayReference::ArrayAccessCast(expression)) // some kind of name
        {
            MethodSymbol* read_method = NULL;
            AstName* name = AstName::NameCast(expression);
            if (name)
            {
				if (name->resolution_opt)
					read_method =
					MethodSymbol::MethodCast(name->resolution_opt->symbol);
            }
            else
            {
                FieldReference* field_access = (FieldReference*) expression;
                if (field_access -> resolution_opt)
                    read_method = MethodSymbol::MethodCast(field_access -> resolution_opt -> symbol);
            }

            if (read_method)
            {
                postfix_expression -> write_method =
                    read_method -> containing_type ->
                    GetWriteAccessFromReadAccess(read_method);
            }
        }
    }
}


void Semantic::ProcessPLUS(AstPreUnaryExpression* expr)
{
    ProcessExpression(expr -> expression);
    expr -> expression = PromoteUnaryNumericExpression(expr -> expression);
    expr -> value = expr -> expression -> value;
    expr -> symbol = expr -> expression -> symbol;
}


void Semantic::ProcessMINUS(AstPreUnaryExpression* expr)
{
    IntegerLiteral* int_literal =
       IntegerLiteral::IntegerLiteralCast(expr->expression);
    LongLiteral* long_literal = LongLiteral::LongLiteralCast(expr->expression);

    if (int_literal)
    {
        LiteralSymbol* literal =  
            (int_literal -> integer_literal_token->GetLiteralSymbol() );

        expr -> value = lookup_env.nameEnv->int_pool.FindOrInsertNegativeInt(literal);
        if (expr -> value == lookup_env.BadValue())
        {
            ReportSemErrorWithAst(SemanticError::INVALID_INT_VALUE, expr);
            expr -> symbol = lookup_env.no_type;
        }
        else expr -> symbol = lookup_env.int_type;
    }
    else if (long_literal)
    {
        LiteralSymbol* literal =  (long_literal -> long_literal_token->GetLiteralSymbol());

        expr -> value = lookup_env.nameEnv->long_pool.FindOrInsertNegativeLong(literal);
        if (expr -> value == lookup_env.BadValue())
        {
            ReportSemErrorWithAst(SemanticError::INVALID_LONG_VALUE, expr);
            expr -> symbol = lookup_env.no_type;
        }
        else expr -> symbol = lookup_env.long_type;
    }
    else
    {
        ProcessExpression(expr -> expression);

        expr -> expression = PromoteUnaryNumericExpression(expr -> expression);
        expr -> symbol = expr -> expression -> symbol;
        if (expr -> expression -> IsConstant())
        {
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
}


void Semantic::ProcessTWIDDLE(AstPreUnaryExpression* expr)
{
    ProcessExpression(expr -> expression);
    expr -> expression = PromoteUnaryNumericExpression(expr -> expression);
    expr -> symbol = expr -> expression -> symbol;

    if (! lookup_env.IsIntegral(expr -> expression -> Type()))
    {
        TypeSymbol* type = expr -> expression -> Type();
        if (expr -> expression -> symbol != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                           expr -> expression,
                           type -> ContainingPackageName(),
                           type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
    }
    else if (expr -> expression -> IsConstant())
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


void Semantic::ProcessNOT(AstPreUnaryExpression* expr)
{
    ProcessExpression(expr -> expression);

    if (expr -> expression -> Type() != lookup_env.boolean_type)
    {
        TypeSymbol* type = expr -> expression -> Type();
        if (expr -> expression -> symbol != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                           expr -> expression,
                           type -> ContainingPackageName(),
                           type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
    }
    else
    {
        if (expr -> expression -> IsConstant())
            expr -> value = lookup_env.nameEnv->int_pool
                .FindOrInsert(IsConstantTrue(expr -> expression) ? 0 : 1);
        expr -> symbol = lookup_env.boolean_type;
    }
}


void Semantic::ProcessPLUSPLUSOrMINUSMINUS(AstPreUnaryExpression* prefix_expression)
{
    Expression* expression = prefix_expression -> expression;

    ProcessExpression(expression);
    prefix_expression -> symbol = expression -> symbol;

    //
    // JLS2 added ability for parenthesized variable to remain a variable.
    //
    if ( AstParenthesizedExpression::ParenthesizedExpressionCast(expression))
    {
        ReportSemErrorWithAst(SemanticError::UNNECESSARY_PARENTHESIS, expression);
        while (AstParenthesizedExpression::ParenthesizedExpressionCast(expression))
            expression = ((AstParenthesizedExpression*) expression) ->
                expression;
    }

    if (expression -> symbol != lookup_env.no_type)
    {
        if (! expression -> IsLeftHandSide())
        {
            ReportSemErrorWithAst(SemanticError::NOT_A_NUMERIC_VARIABLE,
                           prefix_expression -> expression,
                           prefix_expression -> expression -> Type() -> Name());
            prefix_expression -> symbol = lookup_env.no_type;
        }
        else if (! lookup_env.IsNumeric(expression -> Type()))
        {
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_NUMERIC,
                           prefix_expression -> expression,
                           expression -> Type() -> ContainingPackageName(),
                           expression -> Type() -> ExternalName());
            prefix_expression -> symbol = lookup_env.no_type;
        }
        else if (! ArrayReference::ArrayAccessCast(expression)) // some kind of name
        {
            MethodSymbol* read_method = NULL;
            AstName* name = AstName::NameCast(expression);
            if (name)
            {
				if (name->resolution_opt)
					read_method =
					MethodSymbol::MethodCast(name->resolution_opt->symbol);
            }
            else
            {
                FieldReference* field_access = (FieldReference*) expression;
				if (field_access->resolution_opt)
					read_method = MethodSymbol::MethodCast(field_access->resolution_opt->symbol);
            }

            if (read_method)
            {
                prefix_expression -> write_method =
                    read_method -> containing_type ->
                    GetWriteAccessFromReadAccess(read_method);
            }
        }
    }
}


void Semantic::ProcessPreUnaryExpression(Ast* expr)
{
    AstPreUnaryExpression* prefix_expression = (AstPreUnaryExpression*) expr;
    (this ->* ProcessPreUnaryExpr[prefix_expression -> Tag()])
        (prefix_expression);
}


//
// Returns true if both types are primitive, and the source type can be
// widened into the target type.
//
inline bool Semantic::CanWideningPrimitiveConvert(const TypeSymbol* target_type,
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
inline bool Semantic::CanNarrowingPrimitiveConvert(const TypeSymbol* target_type,
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


//
// Returns true if the source type can be converted to the target type in a
// method invocation - this includes identity and widening conversions.
//
bool Semantic::CanMethodInvocationConvert(const TypeSymbol* target_type,
                                          const TypeSymbol* source_type)
{
    if (target_type == lookup_env.no_type) // Don't convert any class to bad type.
        return false;
    if (source_type == lookup_env.no_type) // Allow bad type to match anything.
        return true;

    if (source_type -> Primitive())
    {
        return target_type -> Primitive() &&
            (target_type == source_type ||
             CanWideningPrimitiveConvert(target_type, source_type));
    }

    if (target_type -> Primitive())
        return false;
    return source_type == lookup_env.null_type ||
        source_type -> IsSubtype(target_type);
}


//
// Returns true if the reference source type can be automatically converted to
// the target type in assignments. This works only for references (including
// null), but allows a bad target type while method invocation does not.
//
bool Semantic::CanAssignmentConvertReference(const TypeSymbol* target_type,
                                             const TypeSymbol* source_type)
{
    return target_type == lookup_env.no_type ||
        CanMethodInvocationConvert(target_type, source_type);
}


//
// Returns true if the source expression can be automatically converted to the
// target type. This includes all method invocation conversions, and
// additionally allows narrowing conversions of primitive constants.
//
bool Semantic::CanAssignmentConvert(const TypeSymbol* target_type,
                                    Expression* expr)
{
    return target_type == lookup_env.no_type ||
        expr -> symbol == lookup_env.no_type ||
        CanMethodInvocationConvert(target_type, expr -> Type()) ||
        IsIntValueRepresentableInType(expr, target_type);
}


//
// Returns true if the source type can be cast into the target type, via an
// identity, narrowing, or widening conversion. The lexical token is needed
// in case an error is encountered when resolving the target type.
//
bool Semantic::CanCastConvert(TypeSymbol* target_type, TypeSymbol* source_type,
                              Token* tok)
{
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

    // Now that primitives are removed, check if one subtypes the other.
    if (source_type == lookup_env.null_type ||
        target_type -> IsSubtype(source_type) ||
        source_type -> IsSubtype(target_type))
    {
        return true;
    }

    // If we are left with arrays, see if the base types are compatible.
    if (source_type -> IsArray() || target_type -> IsArray())
    {
        if (source_type -> num_dimensions != target_type -> num_dimensions)
            return false;
        source_type = source_type -> base_type;
        target_type = target_type -> base_type;
        if (source_type -> Primitive() || target_type -> Primitive())
            return false;
    }

    //
    // Here, we are left with two reference types. Two classes are not
    // compatible at this point, and final classes do not implement
    // interfaces. Otherwise, a class can implement an interface (even with
    // conflicting signatures), but two interfaces must be compatible.
    //
    if (source_type -> ACC_FINAL() || target_type -> ACC_FINAL() ||
        (! source_type -> ACC_INTERFACE() && ! target_type -> ACC_INTERFACE()))
    {
         return false;
    }
    if (! source_type -> ACC_INTERFACE() || ! target_type -> ACC_INTERFACE())
        return true;
    if (! source_type -> expanded_method_table)
        ComputeMethodsClosure(source_type, tok);
    if (! target_type -> expanded_method_table)
        ComputeMethodsClosure(target_type, tok);
    ExpandedMethodTable* source_method_table =
        source_type -> expanded_method_table;
    unsigned i;
    for (i = 0; i < source_method_table -> symbol_pool.Length(); i++)
    {
        MethodSymbol* method1 =
            source_method_table -> symbol_pool[i] -> method_symbol;
        MethodShadowSymbol* method_shadow2 =
            target_type -> expanded_method_table ->
            FindOverloadMethodShadow(method1, this, tok);
        if (method_shadow2)
        {
            if (! method1 -> IsTyped())
                method1 -> ProcessMethodSignature(this, tok);

            MethodSymbol* method2 = method_shadow2 -> method_symbol;
            if (! method2 -> IsTyped())
                method2 -> ProcessMethodSignature(this, tok);
            if (method1 -> Type() != method2 -> Type())
                return false;
        }
    }
    return true; // All the methods passed the test.
}


//
// Transfer a constant value across a primitive or String cast statement,
// whether explicit or generated.
//
LiteralValue* Semantic::CastValue(const TypeSymbol* target_type,
                                  Expression* expr)
{
    TypeSymbol* source_type = expr -> Type();

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


void Semantic::ProcessCastExpression(Ast* expr)
{
    CastExpression* cast_expression = (CastExpression*) expr;

    //
    // Do not use ProcessExpressionOrStringConstant here, to avoid generating
    // intermediate Strings - see CheckConstantString in lookup.cpp
    //
    ProcessType(cast_expression -> type);
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
    else
    {
        ReportSemErrorWithAst(SemanticError::INVALID_CAST_CONVERSION,
                       cast_expression -> expression,
                       source_type -> ContainingPackageName(),
                       source_type -> Name(),
                       target_type -> ContainingPackageName(),
                       target_type -> Name());
        cast_expression -> symbol = lookup_env.no_type;
    }
}


//
// Inserts a widening conversion, if necessary.
//
Expression* Semantic::ConvertToType(Expression* expr,
                                       TypeSymbol* target_type)
{
    TypeSymbol* source_type = expr -> Type();
    if (source_type == lookup_env.null_type || source_type == target_type ||
        source_type == lookup_env.no_type || target_type -> Bad())
    {
        return expr;
    }

    auto loc = expr -> LeftToken();

    CastExpression* result =
        compilation_unit -> ast_pool -> GenCastExpression();
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


Expression* Semantic::PromoteUnaryNumericExpression(Expression* unary_expression)
{
    TypeSymbol* type = unary_expression -> Type();

    if (type == lookup_env.no_type)
        return unary_expression;

    if (! lookup_env.IsNumeric(type))
    {
        ReportSemErrorWithAst(SemanticError::TYPE_NOT_NUMERIC, unary_expression,
                       type -> ContainingPackageName(),
                       type -> ExternalName());
        unary_expression -> symbol = lookup_env.no_type;
        return unary_expression;
    }
    return (type == lookup_env.byte_type || type == lookup_env.short_type ||
            type == lookup_env.char_type)
        ? ConvertToType(unary_expression, lookup_env.int_type) : unary_expression;
}


void Semantic::BinaryNumericPromotion(BinaryExpression* binary_expression)
{
    binary_expression -> symbol =
        BinaryNumericPromotion(binary_expression -> left_expression,
                               binary_expression -> right_expression);
}


void Semantic::BinaryNumericPromotion(AstAssignmentExpression* assignment_expression)
{
    Expression* left_expr = assignment_expression -> left_hand_side;
    while (AstParenthesizedExpression::ParenthesizedExpressionCast(left_expr))
        left_expr = ((AstParenthesizedExpression*) left_expr) -> expression;
    TypeSymbol* type =
        BinaryNumericPromotion(left_expr, assignment_expression -> expression);
    assignment_expression -> left_hand_side = left_expr;
    if (type == lookup_env.no_type)
        assignment_expression -> symbol = lookup_env.no_type;
}


void Semantic::BinaryNumericPromotion(ConditionalExpression* conditional_expression)
{
    conditional_expression -> symbol =
        BinaryNumericPromotion(conditional_expression -> true_expression,
                               conditional_expression -> false_expression);
}


TypeSymbol* Semantic::BinaryNumericPromotion(Expression*& left_expr,
                                             Expression*& right_expr)
{
    TypeSymbol* left_type = left_expr -> Type();
    TypeSymbol* right_type = right_expr -> Type();

    if (! lookup_env.IsNumeric(left_type) || ! lookup_env.IsNumeric(right_type))
    {
        if (left_type != lookup_env.no_type && ! lookup_env.IsNumeric(left_type))
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_NUMERIC, left_expr,
                           left_type -> ContainingPackageName(),
                           left_type -> ExternalName());
        if (right_type != lookup_env.no_type && ! lookup_env.IsNumeric(right_type))
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_NUMERIC, right_expr,
                           right_type -> ContainingPackageName(),
                           right_type -> ExternalName());
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


void Semantic::MethodInvocationConversion(AstArguments* args,
                                          MethodSymbol* method)
{
    assert(args -> NumArguments() == method -> NumFormalParameters());
    for (unsigned i = 0; i < args -> NumArguments(); i++)
    {
        Expression* expr = args -> Argument(i);
        if (expr -> Type() != method -> FormalParameter(i) -> Type())
        {
            args -> Argument(i) =
                ConvertToType(expr, method -> FormalParameter(i) -> Type());
        }
    }
}


void Semantic::ProcessPLUS(BinaryExpression* expr)
{
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

    if (left_type == lookup_env.no_type || right_type == lookup_env.no_type)
        expr -> symbol = lookup_env.no_type;
    else if (
		  (lookup_env.IsString(left_type))
		||(lookup_env.IsString(right_type))
		)
    {
        //
        // Convert the left expression if necessary.
        //
        if (!lookup_env.IsString(left_type))
        {
            AddDependence(ThisType(), left_type -> BoxedType(lookup_env));
            if (left_type == lookup_env.void_type)
            {
                ReportSemErrorWithAst(SemanticError::VOID_TO_STRING, left);
                expr -> symbol = lookup_env.no_type;
            }
            else if (left_type == lookup_env.null_type || left -> IsConstant())
            {
                left -> value = CastValue(lookup_env.String(), left);
                left -> symbol = lookup_env.String();
            }
        }

        //
        // Convert the right expression if necessary.
        //
        if (!lookup_env.IsString(right_type))
        {
            AddDependence(ThisType(), right_type -> BoxedType(lookup_env));
            if (right_type == lookup_env.void_type)
            {
                ReportSemErrorWithAst(SemanticError::VOID_TO_STRING, right);
                expr -> symbol = lookup_env.no_type;
            }
            else if (right_type == lookup_env.null_type || right -> IsConstant())
            {
                right -> value = CastValue(lookup_env.String(), right);
                right -> symbol = lookup_env.String();
            }
        }

        AddDependence(ThisType(), lookup_env.option->target >= JikesOption::SDK1_5
                      ? lookup_env.StringBuilder() : lookup_env.StringBuffer());

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
        if (expr -> symbol != lookup_env.no_type)
            expr -> symbol = lookup_env.String();
    }
    else
    {
        BinaryNumericPromotion(expr);
        left = expr -> left_expression;
        right = expr -> right_expression;

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


void Semantic::ProcessShift(BinaryExpression* expr)
{
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (! lookup_env.IsIntegral(left_type))
    {
        if (left_type != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                           expr -> left_expression,
                           left_type -> ContainingPackageName(),
                           left_type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
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
        if (right_type != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                           expr -> right_expression,
                           right_type -> ContainingPackageName(),
                           right_type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
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
void Semantic::ProcessShiftCount(TypeSymbol* left_type, Expression* expr)
{
    if (! expr -> IsConstant())
        return;

    IntLiteralValue* literal = DYNAMIC_CAST<IntLiteralValue*>(expr -> value);
    i4 count = literal -> value;
    IntToWstring count_text(count);

    if (count < 0)
    {
        ReportSemErrorWithAst(SemanticError::NEGATIVE_SHIFT_COUNT,
                       expr,
                       count_text.String());
    }

    int width = (left_type == lookup_env.long_type) ? 64 : 32;
    if (count >= width)
    {
        IntToWstring width_text(width);
        ReportSemErrorWithAst(SemanticError::SHIFT_COUNT_TOO_LARGE,
                       expr,
                       count_text.String(),
                       width_text.String());
    }
}


void Semantic::ProcessLEFT_SHIFT(BinaryExpression* expr)
{
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

            expr -> value = lookup_env.nameEnv->long_pool.FindOrInsert(left -> value <<
                                                           (right -> value &
                                                            LONG_SHIFT_MASK));
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value = lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <<
                                                          (right -> value &
                                                           INT_SHIFT_MASK));
        }
    }
}


void Semantic::ProcessRIGHT_SHIFT(BinaryExpression* expr)
{
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

            expr -> value = lookup_env.nameEnv->long_pool.FindOrInsert(left -> value >>
                                                           (right -> value &
                                                            LONG_SHIFT_MASK));
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value = lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >>
                                                          (right -> value &
                                                           INT_SHIFT_MASK));
        }
    }
}


void Semantic::ProcessUNSIGNED_RIGHT_SHIFT(BinaryExpression* expr)
{
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

            expr -> value = lookup_env.nameEnv->long_pool.FindOrInsert((LongInt)
                ((ULongInt) left -> value >> (right -> value & LONG_SHIFT_MASK)));
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value = lookup_env.nameEnv->int_pool.FindOrInsert((i4)
                ((u4) left -> value >> (right -> value & INT_SHIFT_MASK)));
        }
    }
}


void Semantic::ProcessLESS(BinaryExpression* expr)
{
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

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <
                                              right -> value ? 1 : 0);
        }
    }
}


void Semantic::ProcessGREATER(BinaryExpression* expr)
{
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

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >
                                              right -> value ? 1 : 0);
        }
    }
}


void Semantic::ProcessLESS_EQUAL(BinaryExpression* expr)
{
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

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <=
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <=
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <=
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value <=
                                              right -> value ? 1 : 0);
        }
    }
}


void Semantic::ProcessGREATER_EQUAL(BinaryExpression* expr)
{
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

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >=
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >=
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >=
                                              right -> value ? 1 : 0);
        }
        else if (left_type == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value >=
                                              right -> value ? 1 : 0);
        }
    }
}


void Semantic::ProcessAND(BinaryExpression* expr)
{
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.boolean_type ||
        right_type == lookup_env.boolean_type)
    {
        if (left_type != lookup_env.boolean_type)
        {
            if (left_type != lookup_env.no_type)
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                               expr -> left_expression,
                               left_type -> ContainingPackageName(),
                               left_type -> ExternalName());
            expr -> symbol = lookup_env.no_type;
        }
        else if (right_type != lookup_env.boolean_type)
        {
            if (right_type != lookup_env.no_type)
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                               expr -> right_expression,
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
            expr -> symbol = lookup_env.no_type;
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
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                               expr -> left_expression,
                               left_type -> ContainingPackageName(),
                               left_type -> ExternalName());
            }
            if (! lookup_env.IsIntegral(right_type) &&
                right_type != lookup_env.no_type)
            {
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                               expr -> right_expression,
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
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

                expr -> value = lookup_env.nameEnv->long_pool.FindOrInsert(left -> value &
                                                               right -> value);
            }
            else if (expr_type == lookup_env.int_type)
            {
                IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> left_expression -> value);
                IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> right_expression -> value);

                expr -> value = lookup_env.nameEnv->int_pool.FindOrInsert(left -> value &
                                                              right -> value);
            }
        }
    }
}


void Semantic::ProcessXOR(BinaryExpression* expr)
{
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.boolean_type ||
        right_type == lookup_env.boolean_type)
    {
        if (left_type != lookup_env.boolean_type)
        {
            if (left_type != lookup_env.no_type)
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                               expr -> left_expression,
                               left_type -> ContainingPackageName(),
                               left_type -> ExternalName());
            expr -> symbol = lookup_env.no_type;
        }
        else if (right_type != lookup_env.boolean_type)
        {
            if (right_type != lookup_env.no_type)
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                               expr -> right_expression,
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
            expr -> symbol = lookup_env.no_type;
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
            if (! lookup_env.IsIntegral(left_type) &&
                left_type != lookup_env.no_type)
            {
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                               expr -> left_expression,
                               left_type -> ContainingPackageName(),
                               left_type -> ExternalName());
            }
            if (! lookup_env.IsIntegral(right_type) &&
                right_type != lookup_env.no_type)
            {
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                               expr -> right_expression,
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
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

                expr -> value = lookup_env.nameEnv->long_pool.FindOrInsert(left -> value ^
                                                               right -> value);
            }
            else if (expr_type == lookup_env.int_type)
            {
                IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> left_expression -> value);
                IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> right_expression -> value);

                expr -> value = lookup_env.nameEnv->int_pool.FindOrInsert(left -> value ^
                                                              right -> value);
            }
        }
    }
}


void Semantic::ProcessIOR(BinaryExpression* expr)
{
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.boolean_type ||
        right_type == lookup_env.boolean_type)
    {
        if (left_type != lookup_env.boolean_type)
        {
            if (left_type != lookup_env.no_type)
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                               expr -> left_expression,
                               left_type -> ContainingPackageName(),
                               left_type -> ExternalName());
            expr -> symbol = lookup_env.no_type;
        }
        else if (right_type != lookup_env.boolean_type)
        {
            if (right_type != lookup_env.no_type)
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                               expr -> right_expression,
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
            expr -> symbol = lookup_env.no_type;
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
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                               expr -> left_expression,
                               left_type -> ContainingPackageName(),
                               left_type -> ExternalName());
            }
            if (! lookup_env.IsIntegral(right_type) &&
                right_type != lookup_env.no_type)
            {
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                               expr -> right_expression,
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
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

                expr -> value = lookup_env.nameEnv->long_pool.FindOrInsert(left -> value |
                                                               right -> value);
            }
            else if (expr_type == lookup_env.int_type)
            {
                IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> left_expression -> value);
                IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                    (expr -> right_expression -> value);

                expr -> value = lookup_env.nameEnv->int_pool.FindOrInsert(left -> value |
                                                              right -> value);
            }
        }
    }
}


void Semantic::ProcessAND_AND(BinaryExpression* expr)
{
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type != lookup_env.boolean_type)
    {
        if (left_type != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                           expr -> left_expression,
                           left_type -> ContainingPackageName(),
                           left_type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
    }
    if (right_type != lookup_env.boolean_type)
    {
        if (right_type != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                           expr -> right_expression,
                           right_type -> ContainingPackageName(),
                           right_type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
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


void Semantic::ProcessOR_OR(BinaryExpression* expr)
{
    ProcessExpression(expr -> left_expression);
    ProcessExpression(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type != lookup_env.boolean_type)
    {
        if (left_type != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                           expr -> left_expression,
                           left_type -> ContainingPackageName(),
                           left_type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
    }
    if (right_type != lookup_env.boolean_type)
    {
        if (right_type != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                           expr -> right_expression,
                           right_type -> ContainingPackageName(),
                           right_type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
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
    if (expr -> symbol != lookup_env.no_type)
        expr -> symbol = lookup_env.boolean_type;
}


void Semantic::ProcessEQUAL_EQUAL(BinaryExpression* expr)
{
    ProcessExpressionOrStringConstant(expr -> left_expression);
    ProcessExpressionOrStringConstant(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.void_type || right_type == lookup_env.void_type)
    {
        if (left_type == lookup_env.void_type)
            ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID,
                           expr -> left_expression,
                           left_type -> Name());
        if (right_type == lookup_env.void_type)
            ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID,
                           expr -> right_expression,
                           right_type -> Name());
        expr -> symbol = lookup_env.no_type;
    }
    else if (left_type -> Primitive() && right_type -> Primitive())
    {
        if (left_type == lookup_env.boolean_type ||
            right_type == lookup_env.boolean_type)
        {
            if (left_type != right_type)
            {
                ReportSemErrorWithAst(SemanticError::INCOMPATIBLE_TYPE_FOR_BINARY_EXPRESSION,
                               expr, left_type -> ContainingPackageName(),
                               left_type -> ExternalName(),
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
                expr -> symbol = lookup_env.no_type;
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
        ReportSemErrorWithAst(SemanticError::INCOMPATIBLE_TYPE_FOR_BINARY_EXPRESSION,
                       expr, left_type -> ContainingPackageName(),
                       left_type -> ExternalName(),
                       right_type -> ContainingPackageName(),
                       right_type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
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
            DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> left_expression -> value);
            DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> right_expression -> value);
            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value ==
                                              right -> value ? 1 : 0);
        }
        else if (expr -> left_expression -> Type() == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);
            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value ==
                                              right -> value ? 1 : 0);
        }
        else expr -> value =
                 lookup_env.nameEnv->int_pool.FindOrInsert(left == right ? 1 : 0);
    }
}


void Semantic::ProcessNOT_EQUAL(BinaryExpression* expr)
{
    ProcessExpressionOrStringConstant(expr -> left_expression);
    ProcessExpressionOrStringConstant(expr -> right_expression);

    TypeSymbol* left_type = expr -> left_expression -> Type();
    TypeSymbol* right_type = expr -> right_expression -> Type();

    if (left_type == lookup_env.void_type || right_type == lookup_env.void_type)
    {
        if (left_type == lookup_env.void_type)
            ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID,
                           expr -> left_expression,
                           left_type -> Name());
        if (right_type == lookup_env.void_type)
            ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID,
                           expr -> right_expression,
                           right_type -> Name());
        expr -> symbol = lookup_env.no_type;
    }
    else if (left_type -> Primitive() && right_type -> Primitive())
    {
        if (left_type == lookup_env.boolean_type ||
            right_type == lookup_env.boolean_type)
        {
            if (left_type != right_type)
            {
                ReportSemErrorWithAst(SemanticError::INCOMPATIBLE_TYPE_FOR_BINARY_EXPRESSION,
                               expr, left_type -> ContainingPackageName(),
                               left_type -> ExternalName(),
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
                expr -> symbol = lookup_env.no_type;
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
        ReportSemErrorWithAst(SemanticError::INCOMPATIBLE_TYPE_FOR_BINARY_EXPRESSION,
                       expr, left_type -> ContainingPackageName(),
                       left_type -> ExternalName(),
                       right_type -> ContainingPackageName(),
                       right_type -> ExternalName());
        expr -> symbol = lookup_env.no_type;
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
            DoubleLiteralValue* left = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> left_expression -> value);
            DoubleLiteralValue* right = DYNAMIC_CAST<DoubleLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value !=
                                              right -> value ? 1 : 0);
        }
        else if (expr -> left_expression -> Type() == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value !=
                                              right -> value ? 1 : 0);
        }
        else expr -> value =
                 lookup_env.nameEnv->int_pool.FindOrInsert(left != right ? 1 : 0);
    }
}


void Semantic::ProcessSTAR(BinaryExpression* expr)
{
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

            expr -> value =
                lookup_env.nameEnv->double_pool.FindOrInsert(left -> value *
                                                 right -> value);
        }
        else if (expr -> Type() == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->float_pool.FindOrInsert(left -> value *
                                                right -> value);
        }
        else if (expr -> Type() == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);
            CheckIntegerMultiplication(this, expr,
                                       left -> value, right -> value);
            expr -> value =
                lookup_env.nameEnv->long_pool.FindOrInsert(left -> value *
                                               right -> value);
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
            CheckIntegerMultiplication(this, expr,
                                       left -> value, right -> value);
            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value *
                                              right -> value);
        }
    }
}


void Semantic::ProcessMINUS(BinaryExpression* expr)
{
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

            expr -> value =
                lookup_env.nameEnv->double_pool.FindOrInsert(left -> value -
                                                 right -> value);
        }
        else if (expr -> Type() == lookup_env.float_type)
        {
            FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> left_expression -> value);
            FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                (expr -> right_expression -> value);

            expr -> value =
                lookup_env.nameEnv->float_pool.FindOrInsert(left -> value -
                                                right -> value);
        }
        else if (expr -> Type() == lookup_env.long_type)
        {
            LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> left_expression -> value);
            LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                (expr -> right_expression -> value);
            CheckIntegerSubtraction(this, expr, left -> value, right -> value);
            expr -> value =
                lookup_env.nameEnv->long_pool.FindOrInsert(left -> value -
                                               right -> value);
        }
        else if (expr -> Type() == lookup_env.int_type)
        {
            IntLiteralValue* left = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> left_expression -> value);
            IntLiteralValue* right = DYNAMIC_CAST<IntLiteralValue*>
                (expr -> right_expression -> value);
            CheckIntegerSubtraction(this, expr, left -> value, right -> value);
            expr -> value =
                lookup_env.nameEnv->int_pool.FindOrInsert(left -> value - right -> value);
        }
    }
}


void Semantic::ProcessSLASH(BinaryExpression* expr)
{
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
            ReportSemErrorWithAst(SemanticError::ZERO_DIVIDE_CAUTION, expr);
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
                    lookup_env.nameEnv->double_pool.FindOrInsert(left -> value /
                                                     right -> value);
            }
            else if (expr -> Type() == lookup_env.float_type)
            {
                FloatLiteralValue* left = DYNAMIC_CAST<FloatLiteralValue*>
                    (left_expression -> value);
                FloatLiteralValue* right = DYNAMIC_CAST<FloatLiteralValue*>
                    (right_expression -> value);

                expr -> value =
                    lookup_env.nameEnv->float_pool.FindOrInsert(left -> value /
                                                    right -> value);
            }
            else if (expr -> Type() == lookup_env.long_type)
            {
                LongLiteralValue* left = DYNAMIC_CAST<LongLiteralValue*>
                    (left_expression -> value);
                LongLiteralValue* right = DYNAMIC_CAST<LongLiteralValue*>
                    (right_expression -> value);
                CheckIntegerDivision(this, expr, left -> value,
                                     right -> value);
                expr -> value =
                    lookup_env.nameEnv->long_pool.FindOrInsert(left -> value /
                                                   right -> value);
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
                expr -> value = lookup_env.nameEnv->int_pool
                    .FindOrInsert(right -> value == -1
                                  ? -(left -> value)
                                  : left -> value / right -> value);
            }
        }
    }
}


void Semantic::ProcessMOD(BinaryExpression* expr)
{
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
            ReportSemErrorWithAst(SemanticError::ZERO_DIVIDE_CAUTION, expr);
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


void Semantic::ProcessBinaryExpression(Ast* expr)
{
    BinaryExpression* binary_expression = (BinaryExpression*) expr;
    (this ->* ProcessBinaryExpr[binary_expression -> Tag()])
        (binary_expression);
}


void Semantic::ProcessInstanceofExpression(Ast* expr)
{
    InstanceofExpression* instanceof = (InstanceofExpression*) expr;
    ProcessExpressionOrStringConstant(instanceof -> expression);
    ProcessType(instanceof -> type);

    TypeSymbol* left_type = instanceof -> expression -> Type();
    TypeSymbol* right_type = instanceof -> type -> symbol;

    if (left_type -> Primitive())
    {
        ReportSemErrorWithAst(SemanticError::TYPE_NOT_REFERENCE,
                       instanceof -> expression,
                       left_type -> Name());
        instanceof -> symbol = lookup_env.no_type;
    }
    // can left_type (source) be cast into right_type
    else if (! CanCastConvert(right_type, left_type,
                              instanceof -> instanceof_token))
    {
        ReportSemErrorWithAst(SemanticError::INVALID_INSTANCEOF_CONVERSION,
                       expr, left_type -> ContainingPackageName(),
                       left_type -> ExternalName(),
                       right_type -> ContainingPackageName(),
                       right_type -> ExternalName());
        instanceof -> symbol = lookup_env.no_type;
    }
    else instanceof -> symbol = lookup_env.boolean_type;
}


void Semantic::ProcessConditionalExpression(Ast* expr)
{
    ConditionalExpression* conditional_expression =
        (ConditionalExpression*) expr;

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

    if (test_type != lookup_env.boolean_type)
    {
        if (test_type != lookup_env.no_type)
            ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                           conditional_expression -> test_expression,
                           test_type -> ContainingPackageName(),
                           test_type -> ExternalName());
        conditional_expression -> symbol = lookup_env.no_type;
    }
    if (true_type == lookup_env.void_type)
    {
        ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID,
                       conditional_expression -> true_expression,
                       true_type -> Name());
        true_type = lookup_env.no_type;
    }
    if (false_type == lookup_env.void_type)
    {
        ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID,
                       conditional_expression -> false_expression,
                       false_type -> Name());
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
            ReportSemError(SemanticError::INCOMPATIBLE_TYPE_FOR_CONDITIONAL_EXPRESSION,
                           conditional_expression -> true_expression -> LeftToken(),
                           conditional_expression -> false_expression -> RightToken(),
                           true_type -> ContainingPackageName(),
                           true_type -> ExternalName(),
                           false_type -> ContainingPackageName(),
                           false_type -> ExternalName());
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
        if (CanAssignmentConvert(false_type,
                                 conditional_expression -> true_expression))
        {
            conditional_expression -> true_expression =
                ConvertToType(conditional_expression -> true_expression,
                              false_type);
            conditional_expression -> symbol = false_type;
        }
        else if (CanAssignmentConvert(true_type,
                                      conditional_expression -> false_expression))
        {
            conditional_expression -> false_expression =
                ConvertToType(conditional_expression -> false_expression,
                              true_type);
            conditional_expression -> symbol = true_type;
        }
        else
        {
            ReportSemError(SemanticError::INCOMPATIBLE_TYPE_FOR_CONDITIONAL_EXPRESSION,
                           conditional_expression -> true_expression -> LeftToken(),
                           conditional_expression -> false_expression -> RightToken(),
                           true_type -> ContainingPackageName(),
                           true_type -> ExternalName(),
                           false_type -> ContainingPackageName(),
                           false_type -> ExternalName());
            conditional_expression -> symbol = lookup_env.no_type;
        }

        //
        // If all the subexpressions are constants, compute the results and
        // set the value of the expression accordingly.
        //
        // Since null should not be a compile-time constant, the assert
        // should not need to check for null type.
        //
        if (conditional_expression -> true_expression -> IsConstant() &&
            conditional_expression -> false_expression -> IsConstant())
        {
            assert(  (lookup_env.IsString(conditional_expression->symbol)) ||
                       conditional_expression -> symbol == lookup_env.no_type);

            if (IsConstantTrue(conditional_expression -> test_expression))
                conditional_expression -> value =
                    conditional_expression -> true_expression -> value;
            else if (IsConstantFalse(conditional_expression -> test_expression))
                conditional_expression -> value =
                    conditional_expression -> false_expression -> value;
        }
    }
}


void Semantic::ProcessAssignmentExpression(Ast* expr)
{
    AstAssignmentExpression* assignment_expression =
        (AstAssignmentExpression*) expr;
    ProcessExpressionOrStringConstant(assignment_expression -> expression);

    Expression* left_hand_side = assignment_expression -> left_hand_side;
    //
    // JLS2 added ability for parenthesized variable to remain a variable.
    // Therefore, the grammar was changed to accept all expressions, to avoid
    // ambiguities, and we must filter out invalid left-hand sides.
    //
    if ( AstParenthesizedExpression::ParenthesizedExpressionCast(left_hand_side))
    {
        ReportSemErrorWithAst(SemanticError::UNNECESSARY_PARENTHESIS, left_hand_side);
        while ( AstParenthesizedExpression::ParenthesizedExpressionCast(left_hand_side))
            left_hand_side = ((AstParenthesizedExpression*) left_hand_side) ->
                expression;
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
        ProcessingSimpleAssignment() = true;
    }

    ProcessExpression(left_hand_side);
    ProcessingSimpleAssignment() = false;

    if (! left_hand_side -> IsLeftHandSide())
    {
        ReportSemErrorWithAst(SemanticError::NOT_A_VARIABLE, left_hand_side);
        left_hand_side -> symbol = lookup_env.no_type;
        assignment_expression -> symbol = lookup_env.no_type;
    }

    TypeSymbol* left_type = left_hand_side -> Type();
    TypeSymbol* right_type = assignment_expression -> expression -> Type();

    if (left_type == lookup_env.no_type ||
        right_type == lookup_env.no_type || right_type == lookup_env.void_type)
    {
        if (right_type == lookup_env.void_type)
            ReportSemErrorWithAst(SemanticError::TYPE_IS_VOID,
                           assignment_expression -> expression,
                           right_type -> Name());
        assignment_expression -> symbol = lookup_env.no_type;
        return;
    }
    assignment_expression -> symbol = left_type;

    if (! ArrayReference::ArrayAccessCast(left_hand_side)) // the left-hand-side is a name
    {
        MethodSymbol* read_method = NULL;
        AstName* name = AstName::NameCast(left_hand_side);
        FieldReference* field_access = FieldReference::FieldAccessCast(left_hand_side);
        if (name)
        {
			if (name->resolution_opt)
				read_method =
				MethodSymbol::MethodCast(name->resolution_opt->symbol);
        }
        else if (field_access)
        {
			if (field_access->resolution_opt)
				read_method =
				MethodSymbol::MethodCast(field_access->resolution_opt->symbol);
        }

        if (read_method)
            assignment_expression -> write_method = read_method ->
                containing_type -> GetWriteAccessFromReadAccess(read_method);
    }

    if (assignment_expression -> Tag() ==
        AssignmentExpressionEnum::SIMPLE_EQUAL)
    {
        if (left_type != right_type)
        {
            if (CanAssignmentConvert(left_type,
                                     assignment_expression -> expression))
            {
                assignment_expression -> expression =
                    ConvertToType(assignment_expression -> expression,
                                  left_type);
            }
            else if (assignment_expression -> expression -> IsConstant() &&
                     lookup_env.IsSimpleIntegerValueType(left_type) &&
                     lookup_env.IsSimpleIntegerValueType(right_type))
            {
                if (left_type == lookup_env.byte_type)
                    ReportSemErrorWithAst(SemanticError::INVALID_BYTE_VALUE,
                                   assignment_expression -> expression);
                else if (left_type == lookup_env.short_type)
                    ReportSemErrorWithAst(SemanticError::INVALID_SHORT_VALUE,
                                   assignment_expression -> expression);
                else
                {
                    assert(left_type == lookup_env.char_type);
                    ReportSemErrorWithAst(SemanticError::INVALID_CHARACTER_VALUE,
                                   assignment_expression -> expression);
                }
                assignment_expression -> symbol = lookup_env.no_type;
            }
            else
            {
                ReportSemErrorWithAst(SemanticError::INCOMPATIBLE_TYPE_FOR_ASSIGNMENT,
                               assignment_expression,
                               left_type -> ContainingPackageName(),
                               left_type -> ExternalName(),
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
                assignment_expression -> symbol = lookup_env.no_type;
            }
        }
        return;
    }

    //
    // In JLS 2, it states that the only reference type on the left can
    // be String, for +=.  However, some compilers accept any type on the left
    // that can be assigned a String, provided the right side is a String.
    // In the process, that means an array access could then throw an
    // ArrayStoreException when the left type is not String.
    //
    // TODO: Get the definative answer from Sun which behavior is correct
    //
    if (  lookup_env.IsString(left_type) &&
        (assignment_expression -> Tag() ==
         AssignmentExpressionEnum::PLUS_EQUAL))
    {
        if ( !lookup_env.IsString(right_type) )
        {
            if (right_type == lookup_env.void_type)
            {
                 ReportSemErrorWithAst(SemanticError::VOID_TO_STRING,
                                assignment_expression -> expression);
                 assignment_expression -> symbol = lookup_env.no_type;
            }
            else
            {
                assignment_expression -> expression -> value =
                    CastValue(lookup_env.String(),
                              assignment_expression -> expression);
                if (assignment_expression -> expression -> IsConstant())
                {
                    assignment_expression -> expression -> symbol =
                        lookup_env.String();
                }
            }
        }
        return;
    }

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
                    //
                    // If the type of the expression is integral and the right
                    // hand side is constant 0 then issue an error message.
                    //
                    if ((right_expression -> Type() == lookup_env.int_type &&
                         DYNAMIC_CAST<IntLiteralValue*>
                         (right_expression -> value) -> value == 0) ||
                        (right_expression -> Type() == lookup_env.long_type &&
                         DYNAMIC_CAST<LongLiteralValue*>
                         (right_expression -> value) -> value == 0))
                    {
                        //
                        // This will guarantee a runtime exception, but the
                        // clarifications to JLS2 insist it is legal code.
                        //
                        ReportSemErrorWithAst(SemanticError::ZERO_DIVIDE_CAUTION,
                                       assignment_expression);
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
                if (assignment_expression -> left_hand_side -> symbol !=
                    lookup_env.no_type)
                {
                    ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                                   assignment_expression -> left_hand_side,
                                   left_type -> ContainingPackageName(),
                                   left_type -> ExternalName());
                }
                assignment_expression -> symbol = lookup_env.no_type;
            }
            //
            // This call captures both unary numeric conversion (widening) of
            // byte, char, or short, and narrowing of long, since the bytecode
            // requires an int shift amount.
            //
            if (! lookup_env.IsIntegral(right_type))
            {
                ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                               assignment_expression -> expression,
                               right_type -> ContainingPackageName(),
                               right_type -> ExternalName());
                assignment_expression -> symbol = lookup_env.no_type;
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
                    ReportSemErrorWithAst(SemanticError::TYPE_NOT_BOOLEAN,
                                   assignment_expression -> expression,
                                   right_type -> ContainingPackageName(),
                                   right_type -> ExternalName());
                    assignment_expression -> symbol = lookup_env.no_type;
                }
            }
            else
            {
                if (! lookup_env.IsIntegral(left_type))
                {
                    ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                                   left_hand_side,
                                   left_type -> ContainingPackageName(),
                                   left_type -> ExternalName());
                    assignment_expression -> symbol = lookup_env.no_type;
                }
                if (! lookup_env.IsIntegral(right_type))
                {
                    ReportSemErrorWithAst(SemanticError::TYPE_NOT_INTEGRAL,
                                   assignment_expression -> expression,
                                   right_type -> ContainingPackageName(),
                                   right_type -> ExternalName());
                    assignment_expression -> symbol = lookup_env.no_type;
                }
                BinaryNumericPromotion(assignment_expression);
            }
            break;
        default:
            assert(false);
            break;
    }
}


} // Close namespace Jikes block


