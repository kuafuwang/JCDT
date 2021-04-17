
#include <JCDT_Lib/internal/classfmt/ExceptionsAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block




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
			if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_Class)
				buffer.MarkInvalid("bad type for exceptions attribute entry");
		}
	}


} // Close namespace Jikes block



