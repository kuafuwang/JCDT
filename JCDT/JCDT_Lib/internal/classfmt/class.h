// $Id: JCDT_Lib/internal/classfmt/class.inl,v 1.45 2004/12/12 04:35:52 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#ifndef class_INCLUDED
#define class_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/impl/access.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/impl/long.h"
#include "JCDT_Lib/internal/impl/double.h"

#include "class_Pre_declaration.h"




//
// This header defines the .class file format, for both reading in and writing
// out valid classfiles.
//



//
// The prefix CP stands for Constant Pool. This is the base class of all
// constant pool information; subclasses give more detail. JVMS 4.4.
//
#include "CPInfo.inl"

//
// Specialization of Tuple to more gracefully handle invalid indices. This
// is because slot 0 and slots following a long or double entry are invalid.
//
#include "ConstantPool.inl"



//
// Describes a UTF8 sequence. JVMS 4.4.7.
//
#include "CPUtf8Info.inl"

//
// Describes a class or interface name. JVMS 4.4.1.
//
#include "CPClassInfo.inl"

//
// Describes a field or method's declaring class, name, and type. JVMS 4.4.2.
//
#include "CPMemberInfo.inl"

//
// Describes a String literal. JVMS 4.4.3.
//
#include "CPStringInfo.inl"

//
// Describes an int literal. JVMS 4.4.4.
//
#include "CPIntegerInfo.inl"

//
// Describes a float literal. JVMS 4.4.4.
//
#include "CPFloatInfo.inl"

//
// Describes a long literal. JVMS 4.4.5.
//
#include "CPLongInfo.inl"

//
// Describes a double literal. JVMS 4.4.5.
//
#include "CPDoubleInfo.inl"

//
// Describes a member name and its signature. JVMS 4.4.6.
//
#include "CPNameAndTypeInfo.inl"

//
// FieldInfo and MethodInfo come next in the JVMS, but must appear after
// we know about attributes.
//


//
// Describes an arbitrary attribute. We subclass this for known attributes.
//
#include "AttributeInfo.inl"

//
// An unknown attribute - we are allowed to read them from existing .class
// files, but should never generate or write one.
//
#include "UnknownAttribute.inl"

//
// Valid only on final fields, this marks the initial value of a constant
// variable. JVMS 4.7.2.
//
#include "ConstantValueAttribute.inl"


//
// Valid only on methods, this is the throws clause. JVMS 4.7.4.
//
#include "ExceptionsAttribute.inl"

  
//
// Valid only on classes, this lists information about all nested classes
// mentioned in the constant pool. JVMS 4.7.5.
//
#include "InnerClassesAttribute.inl" 


//
// Valid for classes, methods, and fields, this marks a compiler-generated
// entity that has no counterpart in source. JVMS 4.7.6.
//
#include "SyntheticAttribute.inl"

//
// Valid only for classes, this names the source file. JVMS 4.7.7.
//
#include "SourceFileAttribute.inl"

//
// Valid only for code, this correlates bytecode offsets to source file line
// numbers. JVMS 4.7.8.
//
#include "LineNumberTableAttribute.inl"

//
// Valid only for code, LocalVariableTable gives local variable name, scope,
// and type information relative to bytecode offsets. JVMS 4.7.10.
// Also, the LocalVariableTypeTable gives generic type information.
//
#include "LocalVariableTableAttribute.inl"


//
// Valid for classes, methods, and fields, this marks an entity as
// deprecated. JVMS 4.7.10.
//
#include "DeprecatedAttribute.inl"

//
// Valid for classes, methods, and fields, this gives the parameterized type
// signature for a generic type. JSR 14 section 7.
//
#include "SignatureAttribute.inl"

//
// Valid for methods, this marks the method as a bridge for covariant returns.
// JSR 14? (mentioned in JSR 200).
//
#include "BridgeAttribute.inl"


//
// Valid for code, this speeds up verification by listing the types of all
// locals and stack elements at the start of each basic block. JSR 139
// appendix 1 section 2.1 (or JSR 30 section 5.3.1.2).
//
#include "StackMapAttribute.inl"


//
// Valid only on non-abstract non-native methods, this is the method
// implementation. JVMS 4.7.3.
//
#include "CodeAttribute.inl"


//
// Forward declaration, since AnnotationComponentValue and Annotation are
// mutually referential.
//


//
// Describes an annotation value, as used inside AnnotationsAttribute,
// ParameterAnnotationsAttribute, and AnnotationDefaultAttribute. JSR 175
// section IX.
//
#include "AnnotationComponentValue.inl"

//
// Represents boolean, byte, short, char, int, long, float, double, String,
// and Class annotation values.
//
#include "AnnotationComponentConstant.inl"

//
// Represents enum constant annotation values.
//
#include "AnnotationComponentEnum.inl"

//
// Represents nested annotation values.
//
#include "AnnotationComponentAnnotation.inl"

//
// Represents array annotation values.
//
#include "AnnotationComponentArray.inl"

//
// Describes an annotation, as used inside AnnotationComponentValue,
// ParameterAnnotationsAttribute, and AnnotationDefaultAttribute. JSR 175
// section IX.
//
#include "Annotation.inl"



//
// Valid on all classes, fields, and methods, this lists the RuntimeVisible
// and RuntimeInvisible annotations tied to the item. JSR 175, section IX.
//
#include "AnnotationsAttribute.inl"

//
// Valid on all methods, this lists the RuntimeVisible and RuntimeInvisible
// annotations tied to each parameter. JSR 175, section IX.
//
#include "ParameterAnnotationsAttribute.inl"

//
// Valid only on method members of annotation types, this lists the default
// return value for the annotation method. JSR 175, section IX.
//
#include "AnnotationDefaultAttribute.inl"


//
// Valid only on local and anonymous types, this lists the enclosing method
// of the type. JSR 202? (mentioned in JSR 200).
//
#include "EnclosingMethodAttribute.inl"

//
// Describes a member field. JVMS 4.5.
//
#include "FieldInfo.inl"

//
// Describes a member method, including constructors and the static
// initializer. JVMS 4.6.
//
#include "MethodInfo.inl"

//
// Here we have the complete definition of the .class file format. JVMS 4.1.
//
#include "ClassFile.inl"
 



#endif // class_INCLUDED

