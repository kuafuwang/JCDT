
#include <JCDT_Lib/internal/classfmt/AnnotationDefaultAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	AnnotationDefaultAttribute::AnnotationDefaultAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_AnnotationDefault, buffer)
	{
		default_value =
			AnnotationComponentValue::AllocateAnnotationComponentValue(buffer);
		if (default_value->Length() != attribute_length)
			buffer.MarkInvalid("bad annotation default attribute length");
	}

} // Close namespace Jikes block



