// $Id: class.cpp,v 1.17 2004/12/12 04:35:52 elliott-oss Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
#include "stdafx.h"
#include "JCDT_Lib/internal/classfmt/class.h"
#include "JCDT_Lib/internal/codegen/code.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/classfmt/Operators.h"
#include "JCDT_Lib/internal/impl/option.h"

#include  "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/internal/util/zip/zip.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"


namespace Jikes { // Open namespace Jikes block



void ConstantPool::SetNext(CPInfo* constant)
{
    pool.push_back(constant);
    if (constant -> Large())
		pool.push_back(NULL); // entries following Long or Double are unused
}

bool ConstantPool::Check() const
{
    for (unsigned i = 1; i < pool.size(); i++)
    {
        CPInfo* entry = pool[i];
        if (entry && ! entry -> Check(*this))
            return false;
    }
    return true;
}


CPInfo* CPInfo::AllocateCPInfo(ClassFile& buffer)
{
    u1 tag = buffer.GetU1();
    switch (tag)
    {
    case CONSTANT_Utf8:
        return new CPUtf8Info(buffer);
    case CONSTANT_Integer:
        return new CPIntegerInfo(buffer);
    case CONSTANT_Float:
        return new CPFloatInfo(buffer);
    case CONSTANT_Long:
        return new CPLongInfo(buffer);
    case CONSTANT_Double:
        return new CPDoubleInfo(buffer);
    case CONSTANT_Class:
        return new CPClassInfo(buffer);
    case CONSTANT_String:
        return new CPStringInfo(buffer);
    case CONSTANT_Fieldref:
    case CONSTANT_Methodref:
    case CONSTANT_InterfaceMethodref:
        return new CPMemberInfo((ConstantPoolTag) tag, buffer);
    case CONSTANT_NameAndType:
        return new CPNameAndTypeInfo(buffer);
    default:
        return new CPInfo((ConstantPoolTag) tag);
    }
}


CPClassInfo::CPClassInfo(ClassFile& buffer)
    : CPInfo(CONSTANT_Class)
    , name_index(buffer.GetU2())
    , type(NULL)
{}


CPMemberInfo::CPMemberInfo(ConstantPoolTag _tag, ClassFile& buffer)
    : CPInfo(_tag)
    , class_index(buffer.GetU2())
    , name_and_type_index(buffer.GetU2())
{}


CPStringInfo::CPStringInfo(ClassFile& buffer)
    : CPInfo(CONSTANT_String)
    , string_index(buffer.GetU2())
{}


CPIntegerInfo::CPIntegerInfo(ClassFile& buffer)
    : CPInfo(CONSTANT_Integer)
    , bytes(buffer.GetU4())
{}


CPFloatInfo::CPFloatInfo(ClassFile& buffer)
    : CPInfo(CONSTANT_Float)
    , value(buffer.GetU4())
{}


CPLongInfo::CPLongInfo(ClassFile& buffer)
    : CPInfo(CONSTANT_Long)
    , value(buffer.GetU8())
{}


CPDoubleInfo::CPDoubleInfo(ClassFile& buffer)
    : CPInfo(CONSTANT_Double)
    , value(buffer.GetU8())
{}


CPNameAndTypeInfo::CPNameAndTypeInfo(ClassFile& buffer)
    : CPInfo(CONSTANT_NameAndType)
    , name_index(buffer.GetU2())
    , descriptor_index(buffer.GetU2())
{}


CPUtf8Info::CPUtf8Info(ClassFile& buffer)
    : CPInfo(CONSTANT_Utf8)
    , len(buffer.GetU2())
    , valid(true)
{
    u2 size = (u2) buffer.GetN(bytes, len);
    if (size != len)
        valid = false;
    Init(size);
    if (! valid)
        buffer.MarkInvalid("bad CPUtf8Info");
}

void CPUtf8Info::Init(u2 size)
{
#ifdef JIKES_DEBUG
    const char* tmp;
#endif // JIKES_DEBUG
    for (u2 i = 0; i < size; i++)
    {
        switch (bytes[i])
        {
        case 0x00:
        case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5:
        case 0xf6: case 0xf7: case 0xf8: case 0xf9: case 0xfa: case 0xfb:
        case 0xfc: case 0xfd: case 0xfe: case 0xff: // invalid
            valid = false;
#ifdef JIKES_DEBUG
            contents.push_back( '\\');
            contents.push_back('x');
            tmp = IntToString(bytes[i], 2).String();
            contents.push_back(tmp[0]);
            contents.push_back(tmp[1]);
#endif // JIKES_DEBUG
            break;
#ifdef JIKES_DEBUG
        case 0x09:
            contents.push_back('\\');
            contents.push_back('t');
            break;
        case 0x0a:
            contents.push_back('\\');
            contents.push_back('n');
            break;
        case 0x0d:
            contents.push_back('\\');
            contents.push_back('r');
            break;
        case 0x22:
            contents.push_back('\\');
            contents.push_back('"');
            break;
        case 0x5c:
            contents.push_back('\\');
            contents.push_back('\\');
            break;
        case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06:
        case 0x07: case 0x08: case 0x0b: case 0x0c: case 0x0e: case 0x0f:
        case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15:
        case 0x16: case 0x17: case 0x18: case 0x19: case 0x1a: case 0x1b:
        case 0x1c: case 0x1d: case 0x1e: case 0x1f: case 0x7f:
            // non-printing ASCII
            contents.push_back('\\');
            contents.push_back( 'u');
            tmp = IntToString(bytes[i], 4).String();
            contents.push_back( tmp[0]);
            contents.push_back( tmp[1]);
            contents.push_back( tmp[2]);
            contents.push_back( tmp[3]);
            break;
#endif // JIKES_DEBUG
        default:
            if (bytes[i] <= 0x7f) // 1-byte (printing ASCII, if JIKES_DEBUG)
            {
#ifdef JIKES_DEBUG
                contents.push_back( bytes[i]);
#endif // JIKES_DEBUG
            }
            else if (bytes[i] <= 0xdf) // 2-byte source
            {
                if (i + 1 == size || (bytes[i + 1] & 0xc0) != 0x80)
                {
                    valid = false;
#ifdef JIKES_DEBUG
                    contents.push_back( '\\');
                    contents.push_back( 'x');
                    tmp = IntToString(bytes[i], 2).String();
                    contents.push_back( tmp[0]);
                    contents.push_back( tmp[1]);
#endif // JIKES_DEBUG
                    break;
                }
                ++i;
#ifdef JIKES_DEBUG
                u2 value = (bytes[i - 1] & 0x1f) << 6;
                value |= bytes[i] & 0x3f;
                contents.push_back( '\\');
                contents.push_back( 'u');
                tmp = IntToString(value, 4).String();
                contents.push_back( tmp[0]);
                contents.push_back( tmp[1]);
                contents.push_back( tmp[2]);
                contents.push_back( tmp[3]);
#endif // JIKES_DEBUG
            }
            else // 3-byte source
            {
                assert((bytes[i] & 0xf0) == 0xe0);
                if (i + 2 >= size ||
                    (bytes[i + 1] & 0xc0) != 0x80 ||
                    (bytes[i + 2] & 0xc0) != 0x80)
                {
                    valid = false;
#ifdef JIKES_DEBUG
                    contents.push_back( '\\');
                    contents.push_back( 'x');
                    tmp = IntToString(bytes[i], 2).String();
                    contents.push_back( tmp[0]);
                    contents.push_back( tmp[1]);
#endif // JIKES_DEBUG
                    break;
                }
                i += 2;
#ifdef JIKES_DEBUG
                u2 value = (bytes[i - 2] & 0x0f) << 12;
                value |= (bytes[i - 1] & 0x3f) << 6;
                value |= bytes[i] & 0x3f;
                contents.push_back( '\\');
                contents.push_back( 'u');
                tmp = IntToString(value, 4).String();
                contents.push_back( tmp[0]);
                contents.push_back( tmp[1]);
                contents.push_back( tmp[2]);
                contents.push_back( tmp[3]);
#endif // JIKES_DEBUG
            }
        }
    }
#ifdef JIKES_DEBUG
    if (! valid)
    {
        contents.push_back( '\\');
        contents.push_back( 'i');
        contents.push_back( 'n');
        contents.push_back( 'v');
        contents.push_back( 'a');
        contents.push_back( 'l');
        contents.push_back( 'i');
        contents.push_back( 'd');
        contents.push_back( '\\');
    }
    contents.push_back( 0);
#endif // JIKES_DEBUG
}


FieldInfo::FieldInfo(ClassFile& buffer)
    : AccessFlags(buffer.GetU2())
    , name_index(buffer.GetU2())
    , descriptor_index(buffer.GetU2())
    , attr_synthetic(NULL)
    , attr_deprecated(NULL)
    , attr_signature(NULL)
    , attr_constantvalue(NULL)
    , attr_visible_annotations(NULL)
    , attr_invisible_annotations(NULL)
{
    unsigned count = buffer.GetU2();
    while (count--)
    {
        AttributeInfo* attr = AttributeInfo::AllocateAttributeInfo(buffer);
        attributes.Next() = attr;
        switch (attr -> Tag())
        {
        case AttributeInfo::ATTRIBUTE_Synthetic:
            if (attr_synthetic)
                buffer.MarkInvalid("duplicate synthetic attribute");
            attr_synthetic = (SyntheticAttribute*) attr;
            SetACC_SYNTHETIC();
            break;
        case AttributeInfo::ATTRIBUTE_Deprecated:
            if (attr_deprecated)
                buffer.MarkInvalid("duplicate deprecated attribute");
            attr_deprecated = (DeprecatedAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_ConstantValue:
            if (attr_constantvalue)
                buffer.MarkInvalid("duplicate ConstantValue attribute");
            if (! ACC_FINAL())
                buffer.MarkInvalid("ConstantValue attribute without final");
            attr_constantvalue = (ConstantValueAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Signature:
            if (attr_signature)
                buffer.MarkInvalid("duplicate signature attribute");
            attr_signature = (SignatureAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
            if (attr_visible_annotations)
                buffer.MarkInvalid("duplicate signature attribute");
            attr_visible_annotations = (AnnotationsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
            if (attr_invisible_annotations)
                buffer.MarkInvalid("duplicate invisible attribute");
            attr_invisible_annotations = (AnnotationsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Generic:
            // ignore
            break;
        default:
            // invalid field attribute
            buffer.MarkInvalid("invalid field attribute");
        }
    }
}

const char* FieldInfo::Signature(const ConstantPool& pool,
                                  const Option* /*option*/) const
{
    assert(pool[descriptor_index] -> Tag() == CPInfo::CONSTANT_Utf8);
    const CPUtf8Info* sig =
        /*(option->source >= JikesOption::SDK1_5 && attr_signature)
        ? attr_signature -> Signature(pool)
        :*/ (const CPUtf8Info*) pool[descriptor_index];
    return sig -> Bytes();
}

u2 FieldInfo::SignatureLength(const ConstantPool& pool,
                              const Option* /*option*/) const
{
    assert(pool[descriptor_index] -> Tag() == CPInfo::CONSTANT_Utf8);
    const CPUtf8Info* sig =
        /*(option->source >= JikesOption::SDK1_5 && attr_signature)
        ? attr_signature -> Signature(pool)
        :*/ (const CPUtf8Info*) pool[descriptor_index];
    return sig -> Length();
}


MethodInfo::MethodInfo(ClassFile& buffer)
    : AccessFlags(buffer.GetU2())
    , name_index(buffer.GetU2())
    , descriptor_index(buffer.GetU2())
    , attr_synthetic(NULL)
    , attr_deprecated(NULL)
    , attr_signature(NULL)
    , attr_bridge(NULL)
    , attr_code(NULL)
    , attr_exceptions(NULL)
    , attr_visible_annotations(NULL)
    , attr_invisible_annotations(NULL)
    , attr_param_visible_annotations(NULL)
    , attr_param_invisible_annotations(NULL)
    , attr_annotation_default(NULL)
{
    unsigned count = buffer.GetU2();
    while (count--)
    {
        AttributeInfo* attr = AttributeInfo::AllocateAttributeInfo(buffer);
        attributes.Next() = attr;
        switch (attr -> Tag())
        {
        case AttributeInfo::ATTRIBUTE_Synthetic:
            if (attr_synthetic)
                buffer.MarkInvalid("duplicate synthetic attribute");
            attr_synthetic = (SyntheticAttribute*) attr;
            SetACC_SYNTHETIC();
            break;
        case AttributeInfo::ATTRIBUTE_Deprecated:
            if (attr_deprecated)
                buffer.MarkInvalid("duplicate deprecated attribute");
            attr_deprecated = (DeprecatedAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Code:
            if (attr_code)
                buffer.MarkInvalid("duplicate code attribute");
            if (ACC_NATIVE() || ACC_ABSTRACT())
                buffer.MarkInvalid("code for native or abstract method");
            attr_code = (CodeAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Signature:
            if (attr_signature)
                buffer.MarkInvalid("duplicate signature attribute");
            attr_signature = (SignatureAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Bridge:
            if (attr_bridge)
                buffer.MarkInvalid("duplicate bridge attribute");
            attr_bridge = (BridgeAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Exceptions:
            if (attr_exceptions)
                buffer.MarkInvalid("duplicate exceptions attribute");
            attr_exceptions = (ExceptionsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
            if (attr_visible_annotations)
                buffer.MarkInvalid("duplicate visible attribute");
            attr_visible_annotations = (AnnotationsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
            if (attr_invisible_annotations)
                buffer.MarkInvalid("duplicate invisible attribute");
            attr_invisible_annotations = (AnnotationsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeVisibleParameterAnnotations:
            if (attr_param_visible_annotations)
                buffer.MarkInvalid("duplicate param visible attribute");
            attr_param_visible_annotations =
                (ParameterAnnotationsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeInvisibleParameterAnnotations:
            if (attr_param_invisible_annotations)
                buffer.MarkInvalid("duplicate param invisible attribute");
            attr_param_invisible_annotations =
                (ParameterAnnotationsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_AnnotationDefault:
            if (attr_annotation_default)
                buffer.MarkInvalid("duplicate annotation default attribute");
            if (! ACC_ABSTRACT() || ! ACC_PUBLIC())
                buffer.MarkInvalid("annotation default on non-abstract or non-public method");
            attr_annotation_default = (AnnotationDefaultAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Generic:
            // ignore
            break;
        default:
            // invalid method attribute
            buffer.MarkInvalid("invalid method attribute");
        }
    }
    if (! ACC_NATIVE() && ! ACC_ABSTRACT() && ! attr_code)
        buffer.MarkInvalid("no code for non-native, non-abstract method");
}

const char* MethodInfo::Signature(const ConstantPool& pool,
                                  const Option* /*option*/) const
{
    assert(pool[descriptor_index] -> Tag() == CPInfo::CONSTANT_Utf8);
    const CPUtf8Info* sig =
        /*(option->source >= JikesOption::SDK1_5 && attr_signature)
        ? attr_signature -> Signature(pool)
        :*/ (const CPUtf8Info*) pool[descriptor_index];
    return sig -> Bytes();
}

u2 MethodInfo::SignatureLength(const ConstantPool& pool,
                               const Option* /*option*/) const
{
    assert(pool[descriptor_index] -> Tag() == CPInfo::CONSTANT_Utf8);
    const CPUtf8Info* sig =
        /*(option->source >= JikesOption::SDK1_5 && attr_signature)
        ? attr_signature -> Signature(pool)
        :*/ (const CPUtf8Info*) pool[descriptor_index];
    return sig -> Length();
}


AttributeInfo::AttributeInfo(AttributeInfoTag _tag, ClassFile& buffer)
    : tag(_tag)
    , attribute_name_index(buffer.GetU2())
    , attribute_length(buffer.GetU4())
{}

AttributeInfo::AttributeInfoTag AttributeInfo::Tag(const CPUtf8Info* name)
{
    switch (name -> Length())
    {
    case 4:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_Code))
            return ATTRIBUTE_Code;
        break;
    case 8:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_StackMap))
            return ATTRIBUTE_StackMap;
        break;
    case 9:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_Signature))
            return ATTRIBUTE_Signature;
        if (! strcmp(name -> Bytes(), StringConstant::U8S_Synthetic))
            return ATTRIBUTE_Synthetic;
        break;
    case 10:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_Deprecated))
            return ATTRIBUTE_Deprecated;
        if (! strcmp(name -> Bytes(), StringConstant::U8S_Exceptions))
            return ATTRIBUTE_Exceptions;
        if (! strcmp(name -> Bytes(), StringConstant::U8S_SourceFile))
            return ATTRIBUTE_SourceFile;
        break;
    case 12:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_InnerClasses))
            return ATTRIBUTE_InnerClasses;
        break;
    case 13:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_ConstantValue))
            return ATTRIBUTE_ConstantValue;
        break;
    case 15:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_LineNumberTable))
            return ATTRIBUTE_LineNumberTable;
        if (! strcmp(name -> Bytes(), StringConstant::U8S_EnclosingMethod))
            return ATTRIBUTE_EnclosingMethod;
        break;
    case 17:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_AnnotationDefault))
            return ATTRIBUTE_AnnotationDefault;
        break;
    case 18:
        if (! strcmp(name -> Bytes(), StringConstant::U8S_LocalVariableTable))
            return ATTRIBUTE_LocalVariableTable;
        break;
    case 22:
        if (! strcmp(name -> Bytes(),
                     StringConstant::U8S_LocalVariableTypeTable))
            return ATTRIBUTE_LocalVariableTypeTable;
        break;
    case 25:
        if (! strcmp(name -> Bytes(),
                     StringConstant::U8S_RuntimeVisibleAnnotations))
            return ATTRIBUTE_RuntimeVisibleAnnotations;
        break;
    case 27:
        if (! strcmp(name -> Bytes(),
                     StringConstant::U8S_RuntimeInvisibleAnnotations))
            return ATTRIBUTE_RuntimeInvisibleAnnotations;
        break;
    case 34:
        if (! strcmp(name -> Bytes(),
                     StringConstant::U8S_RuntimeVisibleParameterAnnotations))
            return ATTRIBUTE_RuntimeVisibleParameterAnnotations;
        break;
    case 36:
        if (! strcmp(name -> Bytes(),
                     StringConstant::U8S_RuntimeInvisibleParameterAnnotations))
            return ATTRIBUTE_RuntimeInvisibleParameterAnnotations;
    }
    return ATTRIBUTE_Generic;
}

AttributeInfo* AttributeInfo::AllocateAttributeInfo(ClassFile& buffer)
{
    u2 index = buffer.PeekU2();
    if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_Utf8)
    {
        buffer.MarkInvalid("attribute name not utf8 constant");
        return new UnknownAttribute(buffer);
    }
    switch (Tag((CPUtf8Info*) buffer.Pool()[index]))
    {
    case ATTRIBUTE_ConstantValue:
        return new ConstantValueAttribute(buffer);
    case ATTRIBUTE_Code:
        return new CodeAttribute(buffer);
    case ATTRIBUTE_Exceptions:
        return new ExceptionsAttribute(buffer);
    case ATTRIBUTE_InnerClasses:
        return new InnerClassesAttribute(buffer);
    case ATTRIBUTE_SourceFile:
        return new SourceFileAttribute(buffer);
    case ATTRIBUTE_Synthetic:
        return new SyntheticAttribute(buffer);
    case ATTRIBUTE_LineNumberTable:
        return new LineNumberTableAttribute(buffer);
    case ATTRIBUTE_LocalVariableTable:
        return new LocalVariableTableAttribute(buffer, false);
    case ATTRIBUTE_Deprecated:
        return new DeprecatedAttribute(buffer);
    case ATTRIBUTE_Signature:
        return new SignatureAttribute(buffer);
    case ATTRIBUTE_Bridge:
        return new BridgeAttribute(buffer);
    case ATTRIBUTE_EnclosingMethod:
        return new EnclosingMethodAttribute(buffer);
    case ATTRIBUTE_LocalVariableTypeTable:
        return new LocalVariableTableAttribute(buffer, true);
    case ATTRIBUTE_StackMap:
        return new StackMapAttribute(buffer);
    case ATTRIBUTE_RuntimeVisibleAnnotations:
        return new AnnotationsAttribute(buffer, true);
    case ATTRIBUTE_RuntimeInvisibleAnnotations:
        return new AnnotationsAttribute(buffer, false);
    case ATTRIBUTE_RuntimeVisibleParameterAnnotations:
        return new ParameterAnnotationsAttribute(buffer, true);
    case ATTRIBUTE_RuntimeInvisibleParameterAnnotations:
        return new ParameterAnnotationsAttribute(buffer, false);
    case ATTRIBUTE_AnnotationDefault:
        return new AnnotationDefaultAttribute(buffer);
    default:
        return new UnknownAttribute(buffer);
    }
}


UnknownAttribute::UnknownAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_Generic, buffer)
{
    info_length = buffer.GetN(info, attribute_length);
}


ConstantValueAttribute::ConstantValueAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_ConstantValue, buffer)
    , constantvalue_index(buffer.GetU2())
{
    if (attribute_length != 2)
        buffer.MarkInvalid("bad constant value attribute length");
    if (! buffer.Pool()[constantvalue_index] -> Constant())
        buffer.MarkInvalid("bad constant value attribute type");
}


CodeAttribute::CodeAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_Code, buffer)
    , max_stack(buffer.GetU2())
    , max_locals(buffer.GetU2())
    , code(8, 4)
    , exception_table(6, 16)
    , attributes(6, 16)
    , attr_linenumbers(NULL)
    , attr_locals(NULL)
    , attr_stackmap(NULL)
{
    unsigned remaining = attribute_length - 12;
    // +2 for max_stack, +2 for max_locals, +4 for code_length,
    // +2 for exception_table_length, +2 for attributes_count
    u4 code_length = buffer.GetU4();
    remaining -= code_length;
    buffer.SkipN(code_length);

    u2 exception_table_length = buffer.GetU2();
    remaining -= exception_table_length * 8;
    while (exception_table_length--)
    {
        ExceptionElement& entry = exception_table.Next();
        entry.start_pc = buffer.GetU2();
        entry.end_pc = buffer.GetU2();
        entry.handler_pc = buffer.GetU2();
        entry.catch_type = buffer.GetU2();
        if (entry.catch_type &&
            buffer.Pool()[entry.catch_type] -> Tag() != CPInfo::CONSTANT_Class)
        {
            buffer.MarkInvalid("bad type for exception table catch type");
        }
    }

    u2 attributes_count = buffer.GetU2();
    while (attributes_count--)
    {
        AttributeInfo* attr = AllocateAttributeInfo(buffer);
        remaining -= 6 + attr -> AttributeLength();
        attributes.Next() = attr;
        switch (attr -> Tag())
        {
        case AttributeInfo::ATTRIBUTE_LineNumberTable:
            if (attr_linenumbers)
                buffer.MarkInvalid("duplicate line number table");
            attr_linenumbers = (LineNumberTableAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_LocalVariableTable:
            if (attr_locals)
                buffer.MarkInvalid("duplicate local variable table");
            attr_locals = (LocalVariableTableAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_LocalVariableTypeTable:
            if (attr_local_types)
                buffer.MarkInvalid("duplicate local variable type table");
            attr_local_types = (LocalVariableTableAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_StackMap:
            if (attr_stackmap)
                buffer.MarkInvalid("duplicate stack map");
            attr_stackmap = (StackMapAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Generic:
            // ignore
            break;
        default:
            buffer.MarkInvalid("invalid code attribute");
        }
    }
    if (remaining)
        buffer.MarkInvalid("bytes remaining at end of code attribute");
}


#ifdef JIKES_DEBUG
void CodeAttribute::Print(Ostream& os,const ConstantPool& constant_pool, int fill) const
{
    assert(! fill);
    PrintPrefix("Code", constant_pool, 0);
    os << endl << " max_stack: " << (unsigned) max_stack
            << ", max_locals: " << (unsigned) max_locals
            << ", code_length: " << (unsigned) code.Length() << endl;
    Operators::OpDmp(constant_pool, code);
    os << " exception_table_length: "
            << (unsigned) exception_table.Length() << endl;
    for (unsigned i = 0; i < exception_table.Length(); i++)
    {
        os << "  start " << (unsigned) exception_table[i].start_pc
                << ", end " << (unsigned) exception_table[i].end_pc
                << ", handler " << (unsigned) exception_table[i].handler_pc
                << ", catch_type " << (unsigned) exception_table[i].catch_type;
        if (! exception_table[i].catch_type)
            os << " (all)";
        else if (constant_pool[exception_table[i].catch_type] -> Tag() ==
                 CPInfo::CONSTANT_Class)
        {
            os << '=';
            constant_pool[exception_table[i].catch_type] ->
                Describe(constant_pool);
        }
        else os << "(invalid)";
        os << endl;
    }
    os << " attribute_count: " << (unsigned) attributes.Length() << endl;
    for (unsigned j = 0; j < attributes.Length(); j++)
        attributes[j] -> Print(os,constant_pool, 2);
}
#endif // JIKES_DEBUG


ExceptionsAttribute::ExceptionsAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_Exceptions, buffer)
    , exception_index_table(6, 16)
{
    unsigned count = buffer.GetU2();
    if (attribute_length != count * 2 + 2)
        buffer.MarkInvalid("bad exceptions attribute length");
    while (count--)
    {
        u2 index = buffer.GetU2();
        exception_index_table.Next() = index;
        if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_Class)
            buffer.MarkInvalid("bad type for exceptions attribute entry");
    }
}


InnerClassesAttribute::InnerClassesAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_InnerClasses, buffer)
    , classes(6, 16)
{
    unsigned count = buffer.GetU2();
    if (attribute_length != count * 8 + 2)
        buffer.MarkInvalid("bad inner classes attribute length");
    while (count--)
    {
        InnerClassesElement& entry = classes.Next();
        entry.inner_class_info_index = buffer.GetU2();
        entry.outer_class_info_index = buffer.GetU2();
        entry.inner_name_index = buffer.GetU2();
        entry.inner_class_access_flags.SetFlags(buffer.GetU2());
        if ((buffer.Pool()[entry.inner_class_info_index] -> Tag() !=
             CPInfo::CONSTANT_Class) ||
            (entry.outer_class_info_index &&
             (buffer.Pool()[entry.outer_class_info_index] -> Tag() !=
              CPInfo::CONSTANT_Class)) ||
            (entry.inner_name_index &&
             (buffer.Pool()[entry.inner_name_index] -> Tag() !=
              CPInfo::CONSTANT_Utf8)) ||
            ! entry.inner_class_access_flags.LegalAccess())
        {
            buffer.MarkInvalid("bad type in inner classes attribute");
        }
    }
}


SyntheticAttribute::SyntheticAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_Synthetic, buffer)
{
    if (attribute_length)
        buffer.MarkInvalid("bad synthetic attribute length");
}


SourceFileAttribute::SourceFileAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_SourceFile, buffer)
    , sourcefile_index(buffer.GetU2())
{
    if (attribute_length != 2)
        buffer.MarkInvalid("bad source file attribute length");
    if (buffer.Pool()[sourcefile_index] -> Tag() != CPInfo::CONSTANT_Utf8)
        buffer.MarkInvalid("bad type for source file attribute");
}


LineNumberTableAttribute::LineNumberTableAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_LineNumberTable, buffer)
    , line_number_table(6, 16)
{
    unsigned count = buffer.GetU2();
    if(attribute_length != count * 4 + 2)
        buffer.MarkInvalid("bad line number table length");
    while (count--)
    {
        LineNumberElement& entry = line_number_table.Next();
        entry.start_pc = buffer.GetU2();
        entry.line_number = buffer.GetU2();
    }
}


LocalVariableTableAttribute::LocalVariableTableAttribute(ClassFile& buffer,
                                                         bool generic)
    : AttributeInfo(generic ? ATTRIBUTE_LocalVariableTypeTable
                    : ATTRIBUTE_LocalVariableTable, buffer)
    , local_variable_table(6, 16)
{
    unsigned count = buffer.GetU2();
    if (attribute_length != count * 10 + 2)
        buffer.MarkInvalid("bad local variable table length");
    while (count--)
    {
        LocalVariableElement& entry = local_variable_table.Next();
        entry.start_pc = buffer.GetU2();
        entry.length = buffer.GetU2();
        entry.name_index = buffer.GetU2();
        entry.descriptor_index = buffer.GetU2();
        entry.index = buffer.GetU2();
        if ((buffer.Pool()[entry.name_index] -> Tag() !=
             CPInfo::CONSTANT_Utf8) ||
            (buffer.Pool()[entry.descriptor_index] -> Tag() !=
             CPInfo::CONSTANT_Utf8))
        {
            buffer.MarkInvalid("bad type for local variable table entry");
        }
    }
}


DeprecatedAttribute::DeprecatedAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_Deprecated, buffer)
{
    if (attribute_length)
        buffer.MarkInvalid("bad deprecated attribute length");
}


SignatureAttribute::SignatureAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_Signature, buffer)
    , signature_index(buffer.GetU2())
{
    if (attribute_length != 2)
        buffer.MarkInvalid("bad signature attribute length");
    if (buffer.Pool()[signature_index] -> Tag() != CPInfo::CONSTANT_Utf8)
        buffer.MarkInvalid("bad type for signature attribute");
}


BridgeAttribute::BridgeAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_Bridge, buffer)
{
    if (attribute_length)
        buffer.MarkInvalid("bad bridge attribute length");
}


void StackMapAttribute::StackMapFrame::VerificationTypeInfo::Read(ClassFile& buffer)
{
    int type = buffer.GetU1();
    tag = (VerificationTypeInfoTag) type;
    if (type < TYPE_Top || type > TYPE_Uninitialized)
    {
        type = TYPE_Top;
        buffer.MarkInvalid("bad stack map type");
    }
    if (type >= TYPE_Object)
    {
        info = buffer.GetU2();
        if (type == TYPE_Object &&
            buffer.Pool()[info] -> Tag() != CPInfo::CONSTANT_Class)
        {
            buffer.MarkInvalid("bad stack map info");
        }
    }
}

StackMapAttribute::StackMapFrame::StackMapFrame(ClassFile& buffer)
    : offset(buffer.GetU2())
    , locals(6, 16)
    , stack(6, 16)
    , frame_size(6)
    // +2 for offset, +2 for locals_size, +2 for stack_size
{
    unsigned count = buffer.GetU2();
    while (count--)
    {
        unsigned index = locals.NextIndex();
        locals[index].Read(buffer);
        frame_size += locals[index].Size();
    }
    count = buffer.GetU2();
    while (count--)
    {
        unsigned index = stack.NextIndex();
        stack[index].Read(buffer);
        frame_size += stack[index].Size();
    }
}

StackMapAttribute::StackMapAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_StackMap, buffer)
    , frames(6, 16)
{
    unsigned remaining = attribute_length - 2; // -2 for frame_count
    unsigned count = buffer.GetU2();
    while (count--)
    {
        unsigned index = frames.NextIndex();
        frames[index] = new StackMapFrame(buffer);
        remaining -= frames[index] -> FrameSize();
    }
    if (remaining)
        buffer.MarkInvalid("bytes remaining at end of stack map attribute");
}


AnnotationComponentValue* AnnotationComponentValue::AllocateAnnotationComponentValue(ClassFile& buffer)
{
    AnnotationComponentValueTag tag =
        (AnnotationComponentValueTag) buffer.GetU1();
    switch (tag)
    {
    case COMPONENT_boolean: case COMPONENT_byte: case COMPONENT_char:
    case COMPONENT_short: case COMPONENT_int: case COMPONENT_long:
    case COMPONENT_float: case COMPONENT_double: case COMPONENT_string:
    case COMPONENT_class:
        return new AnnotationComponentConstant(buffer, tag);
    case COMPONENT_enum:
        return new AnnotationComponentEnum(buffer);
    case COMPONENT_annotation:
        return new AnnotationComponentAnnotation(buffer);
    case COMPONENT_array:
        return new AnnotationComponentArray(buffer);
    default:
        buffer.MarkInvalid("unknown annotation component value");
        return new AnnotationComponentValue(tag);
    }
}


AnnotationComponentConstant::AnnotationComponentConstant(ClassFile& buffer,
                                                         AnnotationComponentValueTag _tag)
    : AnnotationComponentValue(_tag)
    , index(buffer.GetU2())
{
    switch (tag)
    {
    case COMPONENT_boolean: case COMPONENT_byte: case COMPONENT_char:
    case COMPONENT_short: case COMPONENT_int:
        if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_Integer)
            buffer.MarkInvalid("bad int-like annotation constant");
        break;
    case COMPONENT_long:
        if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_Long)
            buffer.MarkInvalid("bad long annotation constant");
        break;
    case COMPONENT_float:
        if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_Float)
            buffer.MarkInvalid("bad float annotation constant");
        break;
    case COMPONENT_double:
        if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_Double)
            buffer.MarkInvalid("bad double annotation constant");
        break;
    case COMPONENT_string:
        if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_String)
            buffer.MarkInvalid("bad string annotation constant");
        break;
    case COMPONENT_class:
        if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_Class)
            buffer.MarkInvalid("bad class annotation constant");
        break;
    default:
        assert(false && "invalid annotation constant");
    }
}


AnnotationComponentEnum::AnnotationComponentEnum(ClassFile& buffer)
    : AnnotationComponentValue(COMPONENT_enum)
    , type_name_index(buffer.GetU2())
    , const_name_index(buffer.GetU2())
{
    if (buffer.Pool()[type_name_index] -> Tag() != CPInfo::CONSTANT_Class ||
        buffer.Pool()[const_name_index] -> Tag() != CPInfo::CONSTANT_Utf8)
    {
        buffer.MarkInvalid("bad type for annotation component enum");
    }
}


AnnotationComponentAnnotation::AnnotationComponentAnnotation(ClassFile& buffer)
    : AnnotationComponentValue(COMPONENT_annotation)
{
    attr_value = new Annotation(buffer);
}

AnnotationComponentAnnotation::~AnnotationComponentAnnotation()
{
    delete attr_value;
}

u2 AnnotationComponentAnnotation::Length() const
{
    return 1 + attr_value -> Length(); // +1 tag
}

void AnnotationComponentAnnotation::Put(OutputBuffer& out) const
{
    AnnotationComponentValue::Put(out);
    attr_value -> Put(out);
}

#ifdef JIKES_DEBUG
void AnnotationComponentAnnotation::Print(Ostream& os,const ConstantPool& pool) const
{
    attr_value -> Print(os,pool);
}
#endif // JIKES_DEBUG


AnnotationComponentArray::AnnotationComponentArray(ClassFile& buffer)
    : AnnotationComponentValue(COMPONENT_array)
    , values(6, 16)
    , len(3) // +1 tag, +2 num_values
{
    unsigned count = buffer.GetU2();
    while (count--)
        AddValue(AllocateAnnotationComponentValue(buffer));
}


Annotation::Annotation(ClassFile& buffer)
    : type_index(buffer.GetU2())
    , components(6, 16)
{
    if (buffer.Pool()[type_index] -> Tag() != CPInfo::CONSTANT_Utf8)
        buffer.MarkInvalid("bad type for annotation");
    unsigned i = buffer.GetU2();
    while (i--)
    {
        Component& component = components.Next();
        u2 index = buffer.GetU2();
        component.component_name_index = index;
        if (buffer.Pool()[index] -> Tag() != CPInfo::CONSTANT_Utf8)
            buffer.MarkInvalid("bad type for annotation component name");
        component.component_value =
            AnnotationComponentValue::AllocateAnnotationComponentValue(buffer);
    }
}


AnnotationsAttribute::AnnotationsAttribute(ClassFile& buffer, bool visible)
    : AttributeInfo((visible ? ATTRIBUTE_RuntimeVisibleAnnotations
                     : ATTRIBUTE_RuntimeInvisibleAnnotations), buffer)
    , annotations(6, 16)
{
    unsigned count = buffer.GetU2();
    unsigned length = 2; // +2 num_annotations
    while (count--)
    {
        Annotation* value = new Annotation(buffer);
        annotations.Next() = value;
        length += value -> Length();
    }
    if (length != attribute_length)
        buffer.MarkInvalid("bad annotations attribute length");
}


ParameterAnnotationsAttribute::ParameterAnnotationsAttribute(ClassFile& buffer,
                                                             bool visible)
    : AttributeInfo((visible ? ATTRIBUTE_RuntimeVisibleParameterAnnotations
                     : ATTRIBUTE_RuntimeInvisibleParameterAnnotations), buffer)
    , num_parameters(buffer.GetU1())
    , parameters(NULL)
{
    unsigned length = 1 + 2 * num_parameters;
    // +1 num_parameters, +2 num_annotations * num_parameters
    if (num_parameters)
        parameters = new Tuple<Annotation*>[num_parameters];
    for (unsigned i = 0; i < num_parameters; i++)
    {
        unsigned count = buffer.GetU2();
        while (count--)
        {
            Annotation* value = new Annotation(buffer);
            parameters[i].Next() = value;
            length += value -> Length();
        }
    }
    if (length != attribute_length)
        buffer.MarkInvalid("bad parameter annotations attribute length");
}


AnnotationDefaultAttribute::AnnotationDefaultAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_AnnotationDefault, buffer)
{
    default_value =
        AnnotationComponentValue::AllocateAnnotationComponentValue(buffer);
    if (default_value -> Length() != attribute_length)
        buffer.MarkInvalid("bad annotation default attribute length");
}


EnclosingMethodAttribute::EnclosingMethodAttribute(ClassFile& buffer)
    : AttributeInfo(ATTRIBUTE_EnclosingMethod, buffer)
    , class_index(buffer.GetU2())
    , name_and_type_index(buffer.GetU2())
{
    if (attribute_length != 4)
        buffer.MarkInvalid("bad enclosing method attribute length");
    if (buffer.Pool()[class_index] -> Tag() != CPInfo::CONSTANT_Class ||
        (name_and_type_index &&
         (buffer.Pool()[name_and_type_index] -> Tag() !=
          CPInfo::CONSTANT_NameAndType)))
    {
        buffer.MarkInvalid("bad type for enclosing method attribute");
    }
}


ClassFile::ClassFile(const char* buf, unsigned buf_size)
    : problem(NULL)
    , buffer(buf)
    , buffer_tail(buf + buf_size)
    , magic(GetU4())
    , minor_version(GetU2())
    , major_version(GetU2())
    , constant_pool(8, 4)
    , interfaces(6, 16)
    , fields(6, 16)
    , methods(6, 16)
    , attributes(6, 16)
    , attr_synthetic(NULL)
    , attr_deprecated(NULL)
    , attr_signature(NULL)
    , attr_sourcefile(NULL)
    , attr_innerclasses(NULL)
    , attr_visible_annotations(NULL)
    , attr_invisible_annotations(NULL)
    , attr_enclosing_method(NULL)
{
    if (magic != MAGIC || major_version < 45)
        MarkInvalid("unknown class format");
    u2 count = GetU2();
    if (! count)
        MarkInvalid("empty constant pool");
    else
    {
        while (--count) // skip entry 0
        {
            CPInfo* entry = CPInfo::AllocateCPInfo(*this);
            if (entry -> Large())
                count--; // skip next entry for eight-byte constants
            constant_pool.SetNext(entry);
        }
    }
    if (! constant_pool.Check())
        MarkInvalid("invalid constant pool");
    access_flags = GetU2();
    if (! LegalAccess())
        MarkInvalid("illegal access");
    this_class = GetU2();
    if (constant_pool[this_class] -> Tag() != CPInfo::CONSTANT_Class)
        MarkInvalid("illegal this class");
    super_class = GetU2();
    if (super_class &&
        constant_pool[super_class] -> Tag() != CPInfo::CONSTANT_Class)
    {
        MarkInvalid("illegal super class");
    }
    count = GetU2();
    while (count--)
    {
        u2 inter = GetU2();
        if (constant_pool[inter] -> Tag() != CPInfo::CONSTANT_Class)
            MarkInvalid("illegal interface");
        interfaces.Next() = inter;
    }

    count = GetU2();
    while (count--)
        fields.Next() = new FieldInfo(*this);
    count = GetU2();
    while (count--)
        methods.Next() = new MethodInfo(*this);
    count = GetU2();
    while (count--)
    {
        AttributeInfo* attr = AttributeInfo::AllocateAttributeInfo(*this);
        attributes.Next() = attr;
        switch (attr -> Tag())
        {
        case AttributeInfo::ATTRIBUTE_Synthetic:
            if (attr_synthetic)
                MarkInvalid("duplicate synthetic attribute");
            attr_synthetic = (SyntheticAttribute*) attr;
            SetACC_SYNTHETIC();
            break;
        case AttributeInfo::ATTRIBUTE_Deprecated:
            if (attr_deprecated)
                MarkInvalid("duplicate deprecated attribute");
            attr_deprecated = (DeprecatedAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Signature:
            if (attr_signature)
                MarkInvalid("duplicate signature attribute");
            attr_signature = (SignatureAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_SourceFile:
            if (attr_sourcefile)
                MarkInvalid("duplicate source file attribute");
            attr_sourcefile = (SourceFileAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_InnerClasses:
            if (attr_innerclasses)
                MarkInvalid("duplicate inner classes attribute");
            attr_innerclasses = (InnerClassesAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
            if (attr_visible_annotations)
                MarkInvalid("duplicate visible annotations attribute");
            attr_visible_annotations = (AnnotationsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
            if (attr_invisible_annotations)
                MarkInvalid("duplicate invisible annotations attribute");
            attr_invisible_annotations = (AnnotationsAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_EnclosingMethod:
            if (attr_enclosing_method)
                MarkInvalid("duplicate enclosing method attribute");
            attr_enclosing_method = (EnclosingMethodAttribute*) attr;
            break;
        case AttributeInfo::ATTRIBUTE_Generic:
            // ignore
            break;
        default:
            MarkInvalid("invalid method attribute");
        }
    }
}

void ClassFile::Write(TypeSymbol* unit_type) const
{
    Semantic* sem = unit_type -> semantic_environment -> sem;
    LookupEnvironment& control = sem -> lookup_env;
    OutputBuffer output_buffer;

    const char* class_file_name = unit_type -> ClassName();
    if (control.option->verbose)
        os << "[write " << class_file_name << "]" << endl;
    assert (Valid());
    if (control.option->nowrite)
        return;

    unsigned i;
    output_buffer.PutU4(MAGIC);
    output_buffer.PutU2(minor_version);
    output_buffer.PutU2(major_version);

    output_buffer.PutU2(constant_pool.Length());
    assert(constant_pool.Check());
    for (i = 1; i < constant_pool.Length(); i++)
    {
        constant_pool[i] -> Put(output_buffer);
        if (constant_pool[i] -> Large())
            i++; // skip the next entry for eight-byte constants
    }

    output_buffer.PutU2(access_flags);
    output_buffer.PutU2(this_class);
    output_buffer.PutU2(super_class);
    output_buffer.PutU2(interfaces.Length());
    for (i = 0; i < interfaces.Length(); i++)
        output_buffer.PutU2(interfaces[i]);
    output_buffer.PutU2(fields.Length());
    for (i = 0; i < fields.Length(); i++)
        fields[i] -> Put(output_buffer);
    output_buffer.PutU2(methods.Length());
    for (i = 0; i < methods.Length(); i++)
        methods[i] -> Put(output_buffer);
    output_buffer.PutU2(attributes.Length());
    for (i = 0; i < attributes.Length(); i++)
        attributes[i] -> Put(output_buffer);

    // Now output to file
    if (! output_buffer.WriteToFile(class_file_name))
    {
        int length = strlen(class_file_name);
        wchar_t* name = new wchar_t[length + 1];
        for (int j = 0; j < length; j++)
            name[j] = class_file_name[j];
        name[length] = U_NULL;

        sem -> ReportSemErrorWithAst(SemanticError::CANNOT_WRITE_FILE,
                              unit_type -> declaration, name);
        delete [] name;
    }
}




} // Close namespace Jikes block

