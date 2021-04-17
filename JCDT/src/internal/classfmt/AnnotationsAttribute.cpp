
#include <JCDT_Lib/internal/classfmt/AnnotationsAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



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
			length += value->Length();
		}
		if (length != attribute_length)
			buffer.MarkInvalid("bad annotations attribute length");
	}


	

} // Close namespace Jikes block



