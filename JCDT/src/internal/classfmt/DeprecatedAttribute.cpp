
#include <JCDT_Lib/internal/classfmt/DeprecatedAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block

	DeprecatedAttribute::DeprecatedAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_Deprecated, buffer)
	{
		if (attribute_length)
			buffer.MarkInvalid("bad deprecated attribute length");
	}


} // Close namespace Jikes block



