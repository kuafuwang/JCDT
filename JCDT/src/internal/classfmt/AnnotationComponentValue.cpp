
#include <JCDT_Lib/internal/classfmt/AnnotationComponentValue.inl>
#include <JCDT_Lib/internal/classfmt/AnnotationComponentConstant.inl>
#include <JCDT_Lib/internal/classfmt/AnnotationComponentEnum.inl>
#include <JCDT_Lib/internal/classfmt/AnnotationComponentAnnotation.inl>
#include <JCDT_Lib/internal/classfmt/AnnotationComponentArray.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	AnnotationComponentValue* AnnotationComponentValue::AllocateAnnotationComponentValue(ClassFile& buffer)
	{
		AnnotationComponentValueTag tag =
			(AnnotationComponentValueTag)buffer.GetU1();
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

	

} // Close namespace Jikes block



