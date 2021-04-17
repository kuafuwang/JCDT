
#include <JCDT_Lib/internal/classfmt/BridgeAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	BridgeAttribute::BridgeAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_Bridge, buffer)
	{
		if (attribute_length)
			buffer.MarkInvalid("bad bridge attribute length");
	}


} // Close namespace Jikes block



