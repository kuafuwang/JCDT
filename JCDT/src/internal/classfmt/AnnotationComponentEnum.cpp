
#include <JCDT_Lib/internal/classfmt/AnnotationComponentEnum.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	AnnotationComponentEnum::AnnotationComponentEnum(ClassFile& buffer)
		: AnnotationComponentValue(COMPONENT_enum)
		, type_name_index(buffer.GetU2())
		, const_name_index(buffer.GetU2())
	{
		if (buffer.Pool()[type_name_index]->Tag() != CPInfo::CONSTANT_Class ||
			buffer.Pool()[const_name_index]->Tag() != CPInfo::CONSTANT_Utf8)
		{
			buffer.MarkInvalid("bad type for annotation component enum");
		}
	}


} // Close namespace Jikes block



