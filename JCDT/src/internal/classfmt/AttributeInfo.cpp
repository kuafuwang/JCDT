
#include <JCDT_Lib/internal/classfmt/AttributeInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPUtf8Info.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>
#include <JCDT_Lib/internal/classfmt/UnknownAttribute.inl>
#include <JCDT_Lib/internal/classfmt/LineNumberTableAttribute.inl>
#include <JCDT_Lib/internal/classfmt/LocalVariableTableAttribute.inl>
#include <JCDT_Lib/internal/classfmt/StackMapAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ExceptionsAttribute.inl>
#include <JCDT_Lib/internal/classfmt/EnclosingMethodAttribute.inl>

namespace Jikes { // Open namespace Jikes block




	AttributeInfo::AttributeInfo(AttributeInfoTag _tag, ClassFile& buffer)
		: tag(_tag)
		, attribute_name_index(buffer.GetU2())
		, attribute_length(buffer.GetU4())
	{}

	AttributeInfo::AttributeInfoTag AttributeInfo::Tag(const CPUtf8Info* name)
	{
		switch (name->Length())
		{
		case 4:
			if (!strcmp(name->Bytes(), StringConstant::U8S_Code))
				return ATTRIBUTE_Code;
			break;
		case 8:
			if (!strcmp(name->Bytes(), StringConstant::U8S_StackMap))
				return ATTRIBUTE_StackMap;
			break;
		case 9:
			if (!strcmp(name->Bytes(), StringConstant::U8S_Signature))
				return ATTRIBUTE_Signature;
			if (!strcmp(name->Bytes(), StringConstant::U8S_Synthetic))
				return ATTRIBUTE_Synthetic;
			break;
		case 10:
			if (!strcmp(name->Bytes(), StringConstant::U8S_Deprecated))
				return ATTRIBUTE_Deprecated;
			if (!strcmp(name->Bytes(), StringConstant::U8S_Exceptions))
				return ATTRIBUTE_Exceptions;
			if (!strcmp(name->Bytes(), StringConstant::U8S_SourceFile))
				return ATTRIBUTE_SourceFile;
			break;
		case 12:
			if (!strcmp(name->Bytes(), StringConstant::U8S_InnerClasses))
				return ATTRIBUTE_InnerClasses;
			break;
		case 13:
			if (!strcmp(name->Bytes(), StringConstant::U8S_ConstantValue))
				return ATTRIBUTE_ConstantValue;
			break;
		case 15:
			if (!strcmp(name->Bytes(), StringConstant::U8S_LineNumberTable))
				return ATTRIBUTE_LineNumberTable;
			if (!strcmp(name->Bytes(), StringConstant::U8S_EnclosingMethod))
				return ATTRIBUTE_EnclosingMethod;
			break;
		case 17:
			if (!strcmp(name->Bytes(), StringConstant::U8S_AnnotationDefault))
				return ATTRIBUTE_AnnotationDefault;
			break;
		case 18:
			if (!strcmp(name->Bytes(), StringConstant::U8S_LocalVariableTable))
				return ATTRIBUTE_LocalVariableTable;
			break;
		case 22:
			if (!strcmp(name->Bytes(),
				StringConstant::U8S_LocalVariableTypeTable))
				return ATTRIBUTE_LocalVariableTypeTable;
			break;
		case 25:
			if (!strcmp(name->Bytes(),
				StringConstant::U8S_RuntimeVisibleAnnotations))
				return ATTRIBUTE_RuntimeVisibleAnnotations;
			break;
		case 27:
			if (!strcmp(name->Bytes(),
				StringConstant::U8S_RuntimeInvisibleAnnotations))
				return ATTRIBUTE_RuntimeInvisibleAnnotations;
			break;
		case 34:
			if (!strcmp(name->Bytes(),
				StringConstant::U8S_RuntimeVisibleParameterAnnotations))
				return ATTRIBUTE_RuntimeVisibleParameterAnnotations;
			break;
		case 36:
			if (!strcmp(name->Bytes(),
				StringConstant::U8S_RuntimeInvisibleParameterAnnotations))
				return ATTRIBUTE_RuntimeInvisibleParameterAnnotations;
		}
		return ATTRIBUTE_Generic;
	}

	AttributeInfo* AttributeInfo::AllocateAttributeInfo(ClassFile& buffer)
	{
		u2 index = buffer.PeekU2();
		if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_Utf8)
		{
			buffer.MarkInvalid("attribute name not utf8 constant");
			return new UnknownAttribute(buffer);
		}
		switch (Tag((CPUtf8Info*)buffer.Pool()[index]))
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



} // Close namespace Jikes block



