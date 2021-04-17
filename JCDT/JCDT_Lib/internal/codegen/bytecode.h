// $Id: bytecode.h,v 1.71 2004/03/05 13:10:53 ericb Exp $ -*- c++ -*-
//
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef bytecode_INCLUDED
#define bytecode_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/classfmt/class.h"
#include "JCDT_Lib/internal/classfmt/Operators.h"
#include "JCDT_Lib/internal/util/segment.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "Label.h"
#include "MethodStack.h"

#include  <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include  <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include  <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include  <JCDT_Lib/internal/lookup/PathSymbol.h>
namespace Jikes { // Open namespace Jikes block


class TypeSymbol;
class LookupEnvironment;
class Semantic;


class ByteCode : public ClassFile, public StringConstant, public Operators
{
    //
    // A heuristic level for generating code to handle conditional branches
    // crossing more than 32767 bytes of code. In one test case, 54616 was
    // required to generate that much code, so 10000 seems like a conservative
    // value.
    //
    enum { TOKEN_WIDTH_REQUIRING_GOTOW = 10000 };

    LookupEnvironment& lookupEnv;
    Semantic& semantic;
    TypeSymbol* unit_type;

    int line_number,
        last_label_pc,        // pc for last (closest to end) label
        last_op_pc,           // pc of last operation emitted
        stack_depth,          // current stack depth;
        max_stack,
        max_block_depth;
    MethodStack* method_stack;

    bool string_overflow,
         library_method_not_found,
         last_op_goto;        // set if last operation was GOTO or GOTO_W.
    //
    // This variable is non-zero only in constructors of local classes; it
    // gives the offset where variable shadow parameters begin.
    //
    u2 shadow_parameter_offset;

    CodeAttribute* code_attribute; // code for current method
    LineNumberTableAttribute* line_number_table_attribute;
    LocalVariableTableAttribute* local_variable_table_attribute;
    InnerClassesAttribute* inner_classes_attribute;

    void MethodInitialization()
    {
        last_label_pc = 0;
        last_op_pc = 0;
        last_op_goto = false;
        stack_depth = 0;
        max_stack = 0;
    }

    bool ProcessAbruptExit(unsigned, u2, TypeSymbol* = NULL);
    void CompleteLabel(Label& lab);
    void DefineLabel(Label& lab);
    void UseLabel(Label& lab, int length, int op_offset);

    bool IsLabelUsed(Label& lab)
    {
        return lab.uses.Length() > 0;
    }


    //
    // Does p refer to a non-null reference type?
    //
    bool IsReferenceType(TypeSymbol* p)
    {
        return ! p -> Primitive() && p != lookupEnv.null_type;
    }
    //
    // Does p refer to an array type that can be assigned arrays?
    //
    bool IsMultiDimensionalArray(TypeSymbol* p)
    {
        return p -> num_dimensions > 1 ||
            p -> base_type == lookupEnv.Object() /*||
            p -> base_type == lookupEnv.Cloneable() ||
            p -> base_type == lookupEnv.Serializable()*/;
    }


    //
    // See if operand is constant zero (including -0.0).
    //
    bool IsZero(Expression* p)
    {
        TypeSymbol* type = p -> Type();
        if (p -> IsConstant() && (  !lookupEnv.IsString(type) ))
        {
            if (lookupEnv.IsSimpleIntegerValueType(type) ||
                type == lookupEnv.boolean_type)
            {
                return (DYNAMIC_CAST<IntLiteralValue*> (p -> value)) ->
                    value == 0;
            }
            else if (type == lookupEnv.long_type)
            {
                return (DYNAMIC_CAST<LongLiteralValue*> (p -> value)) ->
                    value == 0;
            }
            else if (type == lookupEnv.float_type)
            {
                return (DYNAMIC_CAST<FloatLiteralValue*> (p -> value)) ->
                    value == 0;
            }
            else
            {
                assert(type == lookupEnv.double_type);
                return (DYNAMIC_CAST<DoubleLiteralValue*> (p -> value)) ->
                    value == 0;
            }
        }
        return false;
    }


    //
    // See if operand is constant one.
    //
    bool IsOne(Expression* p)
    {
        TypeSymbol* type = p -> Type();
        if (p -> IsConstant() && !lookupEnv.IsString(type ) )
        {
            if (lookupEnv.IsSimpleIntegerValueType(type) ||
                type == lookupEnv.boolean_type)
            {
                return (DYNAMIC_CAST<IntLiteralValue*> (p -> value)) ->
                    value == 1;
            }
            else if (type == lookupEnv.long_type)
            {
                return (DYNAMIC_CAST<LongLiteralValue*> (p -> value)) ->
                    value == 1;
            }
            else if (type == lookupEnv.float_type)
            {
                return (DYNAMIC_CAST<FloatLiteralValue*> (p -> value)) ->
                    value == 1;
            }
            else
            {
                assert(type == lookupEnv.double_type);
                return (DYNAMIC_CAST<DoubleLiteralValue*> (p -> value)) ->
                    value == 1;
            }
        }
        return false;
    }


    //
    // memory access: reference either
    // constant (literal)
    // name (includes local varable, or class variable, or field access)
    // array
    //
    enum VariableCategory
    {
        LOCAL_VAR = 0, // local variable
        ARRAY_VAR = 1, // array (of any kind)
        FIELD_VAR = 2, // instance variable
        STATIC_VAR = 3, // class variable
        ACCESSED_VAR = 4 // enclosing class field via accessor method
    };

    VariableCategory GetVariableKind(Expression* expression)
    {
        AstAssignmentExpression* assignment =
            AstAssignmentExpression::AssignmentExpressionCast(expression);
        AstPreUnaryExpression* pre = AstPreUnaryExpression::PreUnaryExpressionCast(expression);
        AstPostUnaryExpression* post = AstPostUnaryExpression::PostUnaryExpressionCast(expression);

        //
        // Get the expression containing the variable (avoid accessor methods).
        //
        Expression* lhs = (assignment
                              ? (assignment -> write_method
                                 ? (Expression*) NULL
                                 : assignment -> left_hand_side)
                              : pre
                              ? (pre -> write_method
                                 ? (Expression*) NULL : pre -> expression)
                              : post
                              ? (post -> write_method
                                 ? (Expression*) NULL : post -> expression)
                              : expression);

        //
        // Find symbol that is associated with expression. If the
        // subexpression has to be referenced via an access method then the
        // symbol is null.
        //
        if (lhs && CastExpression::CastExpressionCast(lhs))
            lhs = ((CastExpression*) lhs) -> expression;
        while (lhs && AstParenthesizedExpression::ParenthesizedExpressionCast(lhs))
            lhs = ((AstParenthesizedExpression*) lhs) -> expression;
        Symbol* sym = lhs ? lhs -> symbol : (Symbol*) NULL;

        //
        // If the expression associated with the left-hand side is null,
        // then we have an access method. Otherwise, a left-hand side is
        // either an array access, a field access or a name. In the case of
        // a FieldAccess or name, the left-hand side is resolved into a
        // variable. For an array access, it is resolved into a type.
        //
        VariableSymbol* var = (sym ? VariableSymbol::VariableCast(sym)
                               : (VariableSymbol*) NULL);
        return (! lhs ? ACCESSED_VAR
                : ! var ? ARRAY_VAR
                : MethodSymbol::MethodCast(var -> owner)? LOCAL_VAR
                : var -> ACC_STATIC() ? STATIC_VAR
                : FIELD_VAR);
    }


    int GetTypeWords(const TypeSymbol* type)
    {
        return lookupEnv.IsDoubleWordType(type) ? 2 : 1;
    }


    //
    // methods to load and store values
    //
    void LoadLocal(int varno, const TypeSymbol*);
    void StoreLocal(int varno, const TypeSymbol*);
    void LoadLiteral(LiteralValue*, const TypeSymbol*);
    void LoadImmediateInteger(i4);
    int LoadVariable(VariableCategory, Expression*, bool = true);
    int LoadArrayElement(const TypeSymbol*);
    void StoreArrayElement(const TypeSymbol*);
    void StoreField(Expression*);
    void StoreVariable(VariableCategory, Expression*);

    void LoadConstantAtIndex(u2 index)
    {
        if (index <= 255)
        {
            PutOp(OP_LDC);
            PutU1((u1) index);
        }
        else
        {
            PutOp(OP_LDC_W);
            PutU2(index);
        }
    }

    //
    // These pools are sets that keep track of elements that have
    // already been inserted in the constant pool.
    //
    SegmentPool segment_pool;

    Pair* double_constant_pool_index;
    Pair* integer_constant_pool_index;
    Pair* long_constant_pool_index;
    Pair* float_constant_pool_index;
    Pair* string_constant_pool_index;

    Pair utf8_constant_pool_index;
    Pair class_constant_pool_index;

    Triplet* name_and_type_constant_pool_index;
    Triplet* fieldref_constant_pool_index;
    Triplet* methodref_constant_pool_index;

    u2 RegisterNameAndType(Utf8LiteralValue* name, Utf8LiteralValue* type_name)
    {
        assert(name && type_name && "null argument to RegisterNameAndType");

        if (! name_and_type_constant_pool_index)
            name_and_type_constant_pool_index =
                new Triplet(segment_pool,
                            lookupEnv.nameEnv->Utf8_pool.symbol_pool.Length());

        u2 index = name_and_type_constant_pool_index ->
            Image(name -> index, type_name -> index);
        if (index == 0)
        {
            //
            // Either the triplet is not in the constant pool, or the constant
            // pool overflowed.
            //
            u2 name_index = RegisterUtf8(name);
            u2 type_index = RegisterUtf8(type_name);
            index = (u2) constant_pool.Length();
            name_and_type_constant_pool_index ->
                Image(name -> index, type_name -> index) = index;
            constant_pool.SetNext(new CPNameAndTypeInfo(name_index,
                                                        type_index));
        }
        return index;
    }


    //
    // Register a variable, including full control over the qualifying type
    // emitted (which is not necessarily the variable's owner).
    //
    u2 RegisterFieldref(const TypeSymbol* type, const VariableSymbol* variable)
    {
        Utf8LiteralValue* class_name = type -> fully_qualified_name;
        Utf8LiteralValue* field_name =
            variable -> ExternalIdentity() -> Utf8_literal;
        Utf8LiteralValue* field_type_name = variable -> signature;
        assert( TypeSymbol::TypeCast(variable->owner));
        assert(class_name && field_name && field_type_name &&
               "null argument to RegisterFieldref");

        if (! fieldref_constant_pool_index)
            fieldref_constant_pool_index =
                new Triplet(segment_pool,
                            lookupEnv.nameEnv->Utf8_pool.symbol_pool.Length());

        u2 name_type_index = RegisterNameAndType(field_name, field_type_name);
        u2 index = fieldref_constant_pool_index ->
            Image(class_name -> index, name_type_index);
        if (index == 0)
        {
            //
            // Either the triplet is not in the constant pool, or the constant
            // pool overflowed.
            //
            u2 class_index = RegisterClass(type);
            index = (u2) constant_pool.Length();
            fieldref_constant_pool_index ->
                Image(class_name -> index, name_type_index) = index;
            constant_pool.SetNext(new CPMemberInfo(CPInfo::CONSTANT_Fieldref,
                                                   class_index,
                                                   name_type_index));
        }
        return index;
    }

    //
    // Shortcut when we want the variable's containing type.
    //
    u2 RegisterFieldref(const VariableSymbol* variable)
    {
        return RegisterFieldref(variable -> ContainingType(), variable);
    }

    u2 RegisterMethodref(const TypeSymbol* type, const MethodSymbol* method)
    {
        CPInfo::ConstantPoolTag kind = type -> ACC_INTERFACE()
            ? CPInfo::CONSTANT_InterfaceMethodref
            : CPInfo::CONSTANT_Methodref;
        Utf8LiteralValue* class_name = type -> fully_qualified_name;
        Utf8LiteralValue* method_name =
            method -> ExternalIdentity() -> Utf8_literal;
        Utf8LiteralValue* method_type = method -> signature;
        assert(class_name && method_name && method_type &&
               "null argument to RegisterMethodref");

        if (! methodref_constant_pool_index)
            methodref_constant_pool_index =
                new Triplet(segment_pool,
                            lookupEnv.nameEnv->Utf8_pool.symbol_pool.Length());

        u2 name_type_index = RegisterNameAndType(method_name, method_type);
        u2 index = methodref_constant_pool_index ->
            Image(class_name -> index, name_type_index);
        if (index == 0)
        {
            //
            // Either the triplet is not in the constant pool, or the constant
            // pool overflowed.
            //
            u2 class_name_index = RegisterClass(type);
            index = (u2) constant_pool.Length();
            methodref_constant_pool_index -> Image(class_name -> index,
                                                   name_type_index) = index;
            constant_pool.SetNext(new CPMemberInfo(kind, class_name_index,
                                                   name_type_index));
        }
        return index;
    }

    u2 RegisterLibraryMethodref(const MethodSymbol* method)
    {
        //
        // The library method must exist. If it does not, flag an error.
        //
        if (method)
            return RegisterMethodref(method -> containing_type, method);
        library_method_not_found = true;
        return 0;
    }

    u2 RegisterDouble(DoubleLiteralValue* lit)
    {
        assert((lit != NULL) && "null argument to RegisterDouble");

        if (! double_constant_pool_index)
            double_constant_pool_index =
                new Pair(segment_pool,
                         lookupEnv.nameEnv->double_pool.symbol_pool.Length());

        u2 index = (*double_constant_pool_index)[lit -> index];
        if (index == 0)
        {
            //
            // Either the pair is not in the constant pool, or the constant
            // pool overflowed.
            //
            index = (u2) constant_pool.Length();
            (*double_constant_pool_index)[lit -> index] = index;
            constant_pool.SetNext(new CPDoubleInfo(lit -> value.HighWord(),
                                                   lit -> value.LowWord()));
        }
        return index;
    }


    u2 RegisterInteger(IntLiteralValue* lit)
    {
        assert((lit != NULL) && "null argument to RegisterInteger");

        if (! integer_constant_pool_index)
            integer_constant_pool_index =
                new Pair(segment_pool, lookupEnv.nameEnv->int_pool.symbol_pool.Length());

        u2 index = (*integer_constant_pool_index)[lit -> index];
        if (index == 0)
        {
            //
            // Either the pair is not in the constant pool, or the constant
            // pool overflowed.
            //
            index = (u2) constant_pool.Length();
            (*integer_constant_pool_index)[lit -> index] = index;
            constant_pool.SetNext(new CPIntegerInfo((u4) lit -> value));
        }
        return index;
    }


    u2 FindInteger(IntLiteralValue* lit)
    {
        return (lit && integer_constant_pool_index
                ? (*integer_constant_pool_index)[lit -> index] : 0);
    }


    u2 RegisterLong(LongLiteralValue* lit)
    {
        assert((lit != NULL) && "null argument to RegisterLong");

        if (! long_constant_pool_index)
            long_constant_pool_index =
                new Pair(segment_pool, lookupEnv.nameEnv->long_pool.symbol_pool.Length());

        u2 index = (*long_constant_pool_index)[lit -> index];
        if (index == 0)
        {
            //
            // Either the pair is not in the constant pool, or the constant
            // pool overflowed.
            //
            index = (u2) constant_pool.Length();
            (*long_constant_pool_index)[lit -> index] = index;
            constant_pool.SetNext(new CPLongInfo(lit -> value.HighWord(),
                                                 lit -> value.LowWord()));
        }
        return index;
    }


    u2 RegisterFloat(FloatLiteralValue* lit)
    {
        assert((lit != NULL) && "null argument to RegisterFloat");

        if (! float_constant_pool_index)
            float_constant_pool_index =
                new Pair(segment_pool, lookupEnv.nameEnv->float_pool.symbol_pool.Length());

        u2 index = (*float_constant_pool_index)[lit -> index];
        if (index == 0)
        {
            //
            // Either the pair is not in the constant pool, or the constant
            // pool overflowed.
            //
            index = (u2) constant_pool.Length();
            (*float_constant_pool_index)[lit -> index] = index;
            constant_pool.SetNext(new CPFloatInfo(lit -> value.Word()));
        }
        return index;
    }


    u2 RegisterUtf8(Utf8LiteralValue* lit)
    {
        assert(lit != NULL && "null argument to RegisterUtf8");

        u2 index = utf8_constant_pool_index[lit -> index];
        if (index == 0)
        {
            //
            // Either the pair is not in the constant pool, or the constant
            // pool overflowed.
            //
            index = (u2) constant_pool.Length();
            utf8_constant_pool_index[lit -> index] = index;
            constant_pool.SetNext(new CPUtf8Info(lit -> value,
                                                 lit -> length));
            if (lit -> length > 0xffff)
                string_overflow = true;
        }
        return index;
    }

    u2 RegisterName(const NameSymbol* sym)
    {
        return RegisterUtf8(sym -> Utf8_literal);
    }

    u2 RegisterString(Utf8LiteralValue* lit)
    {
        assert(lit && "null argument to RegisterString");
        if (! string_constant_pool_index)
            string_constant_pool_index =
                new Pair(segment_pool, lookupEnv.nameEnv->Utf8_pool.symbol_pool.Length());

        u2 index = (*string_constant_pool_index)[lit -> index];
        if (index == 0)
        {
            //
            // Either the pair is not in the constant pool, or the constant
            // pool overflowed.
            //
            u2 utf_index = RegisterUtf8(lit);
            index = (u2) constant_pool.Length();
            (*string_constant_pool_index)[lit -> index] = index;
            constant_pool.SetNext(new CPStringInfo(utf_index));
        }
        return index;
    }

    u2 RegisterClass(const TypeSymbol* type)
    {
        Utf8LiteralValue* lit = type -> num_dimensions
            ? type -> signature : type -> fully_qualified_name;
        assert(type && lit && "null argument to RegisterClass");
        u2 index = class_constant_pool_index[lit -> index];
        if (index == 0)
        {
            //
            // Either the pair is not in the constant pool, or the constant
            // pool overflowed.
            //
            u2 utf_index = RegisterUtf8(lit);
            index = (u2) constant_pool.Length();
            class_constant_pool_index[lit -> index] = index;
            constant_pool.SetNext(new CPClassInfo(utf_index));
            //
            // All nested classes must appear in the InnerClasses attribute, as
            // well.
            //
            if (type -> IsNested())
            {
                if (! inner_classes_attribute)
                {
                    inner_classes_attribute = new InnerClassesAttribute
                        (RegisterUtf8(lookupEnv.nameEnv->InnerClasses_literal));
                    AddAttribute(inner_classes_attribute);
                }
                AccessFlags flags = type -> Flags();
                //
                // Types are never marked strictfp in .class files. This can be
                // reverse engineered from <init> and <clinit> methods; the
                // only time when these methods don't exist is in interfaces
                // that have no runtime expression evaluations, so no
                // information is lost by clearing the flag.
                //
                flags.ResetACC_STRICTFP();
                inner_classes_attribute ->
                    AddInnerClass(index, type -> IsLocal() ? 0
                                  : RegisterClass(type -> ContainingType()),
                                  type -> Anonymous() ? 0
                                  : RegisterName(type -> name_symbol), flags);
            }
        }
        return index;
    }


    //
    //  Methods to write out the byte code
    //
    DeprecatedAttribute* CreateDeprecatedAttribute()
    {
        return new DeprecatedAttribute(RegisterUtf8
                                       (lookupEnv.nameEnv->Deprecated_literal));
    }

    SyntheticAttribute* CreateSyntheticAttribute()
    {
        return new SyntheticAttribute(RegisterUtf8(lookupEnv.nameEnv->Synthetic_literal));
    }

    EnclosingMethodAttribute* CreateEnclosingMethodAttribute(MethodSymbol* sym)
    {
        u2 attr_name = RegisterUtf8(lookupEnv.nameEnv->EnclosingMethod_literal);
        u2 type_index = RegisterClass(sym -> containing_type);
        u2 name_type_index =
            (sym -> name_symbol == lookupEnv.nameEnv->block_init_name_symbol ||
             sym -> name_symbol == lookupEnv.nameEnv->clinit_name_symbol) ? 0
            : RegisterNameAndType(sym -> ExternalIdentity() -> Utf8_literal,
                                  sym -> signature);
        return new EnclosingMethodAttribute(attr_name, type_index,
                                            name_type_index);
    }


    //
    // Methods to generate expressions.
    //
    int EmitExpression(Expression*, bool = true);
    int EmitName(AstName*, bool);
    int EmitArrayCreationExpression(AstArrayCreationExpression*, bool = true);
    int EmitAssignmentExpression(AstAssignmentExpression*, bool);
    int EmitBinaryExpression(BinaryExpression*, bool);
    int EmitInstanceofExpression(InstanceofExpression*, bool);
    int EmitCastExpression(CastExpression*, bool);
    void EmitCast(TypeSymbol*, TypeSymbol*);
    int EmitClassCreationExpression(AstClassCreationExpression*, bool);
    int EmitConditionalExpression(ConditionalExpression*, bool);
    int EmitFieldAccess(FieldReference*, bool = true);
    Expression* VariableExpressionResolution(Expression*);
    TypeSymbol* VariableTypeResolution(Expression*, VariableSymbol*);
    TypeSymbol* MethodTypeResolution(Expression*, MethodSymbol*);
    void EmitFieldAccessLhsBase(Expression*);
    void EmitFieldAccessLhs(Expression*);
    int EmitMethodInvocation(AstMethodInvocation*, bool);
    void EmitNewArray(unsigned, const TypeSymbol*);
    int EmitPostUnaryExpression(AstPostUnaryExpression*, bool);
    void EmitPostUnaryExpressionArray(AstPostUnaryExpression*, bool);
    void EmitPostUnaryExpressionField(VariableCategory,
                                      AstPostUnaryExpression*, bool);
    void EmitPostUnaryExpressionSimple(VariableCategory,
                                       AstPostUnaryExpression*, bool);
    int EmitPreUnaryExpression(AstPreUnaryExpression*, bool);
    void EmitPreUnaryIncrementExpression(AstPreUnaryExpression*, bool);
    void EmitPreUnaryIncrementExpressionArray(AstPreUnaryExpression*, bool);
    void EmitPreUnaryIncrementExpressionField(VariableCategory,
                                              AstPreUnaryExpression*, bool);
    void EmitPreUnaryIncrementExpressionSimple(VariableCategory,
                                               AstPreUnaryExpression*, bool);
    void EmitThisInvocation(AstThisCall*);
    void EmitSuperInvocation(AstSuperCall*);
    void ConcatenateString(BinaryExpression*, bool);
    void AppendString(Expression*, bool);
    void EmitStringAppendMethod(TypeSymbol*);
    void ChangeStack(int);
    void ResolveAccess(Expression*);
    int GenerateClassAccess(ClassLiteralAccess*, bool);
    void GenerateClassAccessMethod();
    void GenerateAssertVariableInitializer(TypeSymbol*, VariableSymbol*);
    void EmitCheckForNull(Expression* expr, bool = true);

    //
    // Methods to process statements
    //
    void CompileConstructor(ConstructorDeclaration*,
                            Tuple<AstVariableDeclarator*>&, bool);

    void BeginMethod(int, MethodSymbol*);
    void EndMethod(int, MethodSymbol*);
    void DeclareField(VariableSymbol*);
    void InitializeVariable(AstVariableDeclarator*);
    void InitializeArray(const TypeSymbol*, AstArrayInitializer*, bool = true);
    void DeclareLocalVariable(AstVariableDeclarator*);
    bool EmitStatement(ReachComleteAbleStatement*);
    void EmitReturnStatement(ReturnStatement*);
    bool EmitSynchronizedStatement(SynchronizedStatement*);
    bool EmitBlockStatement(AstBlock*);
    void EmitStatementExpression(Expression*);
    bool EmitSwitchStatement(SwitchStatement*);
    bool EmitSwitchBlockStatement(AstSwitchBlockStatement*, bool);
    void CloseSwitchLocalVariables(AstBlock*, u2 op_start);
    void EmitTryStatement(TryStatement*);
    void EmitAssertStatement(AssertStatement*);
    void EmitForeachStatement(ForeachStatement*);
    void EmitBranchIfExpression(Expression*, bool, Label&,
                                ReachComleteAbleStatement* = NULL);
    void EmitBranch(Opcode, Label&, ReachComleteAbleStatement* = NULL);
    int CompleteCall(MethodSymbol*, int, bool = true, TypeSymbol* = NULL);

    Expression* StripNops(Expression*);
    bool IsNop(AstBlock*);

    void EmitArrayAccessLhs(ArrayReference* expression);
    int EmitArrayAccessRhs(ArrayReference* expression)
    {
        EmitArrayAccessLhs(expression); // get array address and index
        return LoadArrayElement(expression -> Type());
    }

    // Return the OP_IF... bytecode that has the opposite meaning
    Opcode InvertIfOpCode(Opcode opc)
    {
        //
        // Unfortunately, the JVMS does not nicely specify symmetric opcodes;
        // we must treat even-odd and odd-even pairs differently.
        //
        if (opc >= OP_IFNULL)
        {
            assert(opc <= OP_IFNONNULL);
            return (Opcode) (opc ^ 1);
        }
        assert(OP_IFEQ <= opc && opc <= OP_IF_ACMPNE);
        return (Opcode) (((opc + 1) ^ 1) - 1);
    }

    void GenerateReturn(TypeSymbol* type)
    {
        PutOp((lookupEnv.IsSimpleIntegerValueType(type) ||
               type == lookupEnv.boolean_type) ? OP_IRETURN
              : type == lookupEnv.long_type ? OP_LRETURN
              : type == lookupEnv.float_type ? OP_FRETURN
              : type == lookupEnv.double_type ? OP_DRETURN
              : OP_ARETURN);
    }


    void PutOp(Opcode);

    void PutOpWide(Opcode, u2 var);

    void PutOpIINC(u2 var, int val);

    //
    //  Methods to insert values into byte code
    //
    void PutI1(i1 i)
    {
        code_attribute -> AddCode(i & 0xff);
    }

    void PutI2(i2 i)
    {
        code_attribute -> AddCode((i >> 8) & 0xff);
        code_attribute -> AddCode(i & 0xff);
    }

    void PutU1(u1 u)
    {
        code_attribute -> AddCode(u & 0xff);
    }

    void PutU2(u2 u)
    {
        code_attribute -> AddCode((u >> 8) & 0xff);
        code_attribute -> AddCode(u & 0xff);
    }

    void PutU4(u4 u)
    {
        code_attribute -> AddCode((u >> 24));
        code_attribute -> AddCode((u >> 16) & 0xff);
        code_attribute -> AddCode((u >>  8) & 0xff);
        code_attribute -> AddCode(u & 0xff);
    }

    void FinishCode();

    void Reset()
    {
        constant_pool.Reset();
        fields.Reset();
        methods.Reset();
        attributes.Reset();
        this_class = super_class = 0;
    }

public:
    ByteCode(TypeSymbol*);

    ~ByteCode()
    {
        delete double_constant_pool_index;
        delete integer_constant_pool_index;
        delete long_constant_pool_index;
        delete float_constant_pool_index;
        delete string_constant_pool_index;

        delete name_and_type_constant_pool_index;
        delete fieldref_constant_pool_index;
        delete methodref_constant_pool_index;
    }

    void GenerateCode(Ostream& os);
};


} // Close namespace Jikes block


#endif // bytecode_INCLUDED

