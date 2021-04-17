
#include <JCDT_Lib/internal/classfmt/AnnotationComponentArray.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block




	AnnotationComponentArray::AnnotationComponentArray(ClassFile& buffer)
		: AnnotationComponentValue(COMPONENT_array)
		, values(6, 16)
		, len(3) // +1 tag, +2 num_values
	{
		unsigned count = buffer.GetU2();
		while (count--)
			AddValue(AllocateAnnotationComponentValue(buffer));
	}



} // Close namespace Jikes block



