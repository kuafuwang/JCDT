
#include <JCDT_Lib/internal/classfmt/Annotation.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	Annotation::Annotation(ClassFile& buffer)
		: type_index(buffer.GetU2())
		, components(6, 16)
	{
		if (buffer.Pool()[type_index]->Tag() != CPInfo::CONSTANT_Utf8)
			buffer.MarkInvalid("bad type for annotation");
		unsigned i = buffer.GetU2();
		while (i--)
		{
			Component& component = components.Next();
			u2 index = buffer.GetU2();
			component.component_name_index = index;
			if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_Utf8)
				buffer.MarkInvalid("bad type for annotation component name");
			component.component_value =
				AnnotationComponentValue::AllocateAnnotationComponentValue(buffer);
		}
	}

	

} // Close namespace Jikes block



