
#include <JCDT_Lib/internal/classfmt/UnknownAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	UnknownAttribute::UnknownAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_Generic, buffer)
	{
		info_length = buffer.GetN(info, attribute_length);
	}


} // Close namespace Jikes block



