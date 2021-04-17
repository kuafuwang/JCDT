// $Id: init.cpp,v 1.28 2004/05/03 21:20:23 elliott-oss Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#include "JCDT_Lib/internal/env/platform.h"
#include  "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include <JCDT_Lib/internal/env/INameEnvironment.h>


namespace Jikes { // Open namespace Jikes block


void Semantic::ProcessVariableInitializer(AstVariableDeclarator* variable_declarator)
{
    VariableSymbol* symbol = variable_declarator -> symbol;

    if (! variable_declarator -> variable_initializer_opt)
    {
        symbol -> MarkInitialized();
        return;
    }

    TypeSymbol* field_type = symbol -> Type();
    Expression* init =
        (Expression*) variable_declarator -> variable_initializer_opt;
    AstArrayInitializer* array_initializer = AstArrayInitializer::ArrayInitializerCast(init);
    if (array_initializer)
        ProcessArrayInitializer(array_initializer, field_type);
    else
    {
        ProcessExpressionOrStringConstant(init);

        if (field_type != init -> Type() && init -> Type() != lookup_env.no_type)
        {
            if (CanAssignmentConvert(field_type, init))
            {
                init = ConvertToType(init, field_type);
                variable_declarator -> variable_initializer_opt = init;
            }
            else if (init -> IsConstant() &&
                     lookup_env.IsSimpleIntegerValueType(init -> Type()) &&
                     lookup_env.IsSimpleIntegerValueType(field_type))
            {
                if (field_type == lookup_env.byte_type)
                    ReportSemErrorWithAst(SemanticError::INVALID_BYTE_VALUE, init);
                else if (field_type == lookup_env.char_type)
                    ReportSemErrorWithAst(SemanticError::INVALID_CHARACTER_VALUE,
                                   init);
                else ReportSemErrorWithAst(SemanticError::INVALID_SHORT_VALUE, init);
                init -> value = NULL;
            }
            else
            {
                ReportSemErrorWithAst(SemanticError::INCOMPATIBLE_TYPE_FOR_ASSIGNMENT,
                               variable_declarator,
                               field_type -> ContainingPackageName(),
                               field_type -> ExternalName(),
                               init -> Type() -> ContainingPackageName(),
                               init -> Type() -> ExternalName());
                init -> value = NULL;
            }
        }

        if (symbol -> ACC_FINAL() &&
            (field_type -> Primitive() || 
			 lookup_env.IsString(field_type) ))
        {
            if (init -> IsConstant())
            {
                symbol -> initial_value = init -> value;
            }
            else if (symbol -> ACC_STATIC() && ThisType() -> IsInner())
            {
                ReportSemErrorWithAst(SemanticError::STATIC_FIELD_IN_INNER_CLASS_NOT_CONSTANT,
                               variable_declarator,
                               (variable_declarator -> LeftToken()->NameString()),
                               ThisType() -> Name(), ThisType() -> FileLoc());
            }
        }
    }

    //
    // A non-static final field initialized to a constant value wastes
    // space in each instance, so warn about it.
    //
    TypeSymbol* containing_type = TypeSymbol::TypeCast(symbol -> owner);
    if (containing_type && ! containing_type -> ACC_INTERFACE() &&
        ! field_type -> IsArray() &&
        symbol -> ACC_FINAL() &&
        ! symbol -> ACC_STATIC() &&
        init && init -> IsConstant())
    {
        ReportSemErrorWithAst(SemanticError::NON_STATIC_FINAL_CONSTANT_FIELD,
                       variable_declarator,
                           (variable_declarator -> LeftToken()->NameString()));
    }

    symbol -> MarkInitialized();
}


void Semantic::ProcessArrayInitializer(AstArrayInitializer* array_initializer,
                                       TypeSymbol* type)
{
    if (! type -> IsArray())
    {
        ReportSemErrorWithAst(SemanticError::INIT_SCALAR_WITH_ARRAY,
                       array_initializer, type -> Name());
    }
    else
    {
        for (unsigned i = 0;
             i < array_initializer -> NumVariableInitializers(); i++)
        {
            AstArrayInitializer* sub_array_initializer = 
				AstArrayInitializer::ArrayInitializerCast(array_initializer ->
					VariableInitializer(i));
            TypeSymbol* array_subtype = type -> ArraySubtype();
            if (sub_array_initializer)
                 ProcessArrayInitializer(sub_array_initializer, array_subtype);
            else
            {
                Expression* init = (Expression*) array_initializer ->
                    VariableInitializer(i);
                ProcessExpressionOrStringConstant(init);

                if (array_subtype != init -> Type())
                {
                    if (CanAssignmentConvert(array_subtype, init))
                        array_initializer -> VariableInitializer(i) =
                            ConvertToType(init, array_subtype);
                    else if (array_subtype -> IsArray() &&
                             init -> Type() -> Primitive())
                    {
                        ReportSemErrorWithAst(SemanticError::INIT_ARRAY_WITH_SCALAR,
                                       init, array_subtype -> Name());
                    }
                    else if (init -> IsConstant() &&
                             lookup_env.IsSimpleIntegerValueType(init -> Type()) &&
                             lookup_env.IsSimpleIntegerValueType(array_subtype))
                    {
                        if (array_subtype == lookup_env.byte_type)
                            ReportSemErrorWithAst(SemanticError::INVALID_BYTE_VALUE,
                                           init);
                        else if (array_subtype == lookup_env.char_type)
                            ReportSemErrorWithAst(SemanticError::INVALID_CHARACTER_VALUE,
                                           init);
                        else ReportSemErrorWithAst(SemanticError::INVALID_SHORT_VALUE,
                                            init);
                    }
                    else
                    {
                        ReportSemErrorWithAst(SemanticError::INCOMPATIBLE_TYPE_FOR_INITIALIZATION,
                                       init,
                                       array_subtype -> ContainingPackageName(),
                                       array_subtype -> ExternalName(),
                                       init -> Type() -> ContainingPackageName(),
                                       init -> Type() -> ExternalName());
                    }
                }
            }
        }
    }
}


void Semantic::ComputeFinalValue(VariableSymbol* variable)
{
    AstVariableDeclarator* variable_declarator = variable -> declarator;
    assert(variable_declarator && variable -> ACC_FINAL());
    if (! variable -> IsInitialized())
    {
        if (variable_declarator -> pending ||
            ! variable_declarator -> variable_initializer_opt)
        {
            //
            // Break loops, and ignore non-initialized fields.
            //
            variable -> MarkInitialized();
            return;
        }

        //
        // Create a clone and process that, to avoid triggering errors now.
        // Later, we will issue the errors for real when processing the field
        // initializer when we get to its source file.
        //
        TypeSymbol* type = variable -> ContainingType();
        Semantic* sem = type -> semantic_environment -> sem;

		if (!sem->error)
			sem->error =
			new SemanticError(lookup_env.option,lookup_env.nameEnv->classpath, sem->source_file_symbol);
		
		sem->error->EnteringClone();
        sem -> state_stack.Push(type -> semantic_environment);
        MethodSymbol* calling_method = sem -> ThisMethod();
        VariableSymbol* calling_var = sem -> ThisVariable();
        sem -> ThisMethod() = NULL;
        sem -> ThisVariable() = variable;
        variable_declarator -> pending = true;

        AstStoragePool pool(variable_declarator -> RightToken() -
                         variable_declarator -> LeftToken());
        AstVariableDeclarator* clone = (AstVariableDeclarator*)
            variable_declarator -> Clone(&pool);
        clone -> symbol = variable;
        sem -> ProcessVariableInitializer(clone);
        assert(variable -> IsInitialized());

        variable_declarator -> pending = false;
        sem -> ThisMethod() = calling_method;
        sem -> ThisVariable() = calling_var;
        sem -> state_stack.Pop();
		sem->error-> ExitingClone();
    }
}


} // Close namespace Jikes block


