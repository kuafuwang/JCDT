
#include <JCDT_Lib/internal/classfmt/ConstantValueAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block




	ConstantValueAttribute::ConstantValueAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_ConstantValue, buffer)
		, constantvalue_index(buffer.GetU2())
	{
		if (attribute_length != 2)
			buffer.MarkInvalid("bad constant value attribute length");
		if (!buffer.Pool()[constantvalue_index]->Constant())
			buffer.MarkInvalid("bad constant value attribute type");
	}


} // Close namespace Jikes block



