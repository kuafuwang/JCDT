
#include <JCDT_Lib/internal/classfmt/InnerClassesAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


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
			if ((buffer.Pool()[entry.inner_class_info_index]->Tag() !=
				CPInfo::CONSTANT_Class) ||
				(entry.outer_class_info_index &&
				(buffer.Pool()[entry.outer_class_info_index]->Tag() !=
					CPInfo::CONSTANT_Class)) ||
					(entry.inner_name_index &&
				(buffer.Pool()[entry.inner_name_index]->Tag() !=
					CPInfo::CONSTANT_Utf8)) ||
				!entry.inner_class_access_flags.LegalAccess())
			{
				buffer.MarkInvalid("bad type in inner classes attribute");
			}
		}
	}


	

} // Close namespace Jikes block



