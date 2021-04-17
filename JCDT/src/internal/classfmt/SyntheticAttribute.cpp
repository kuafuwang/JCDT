
#include <JCDT_Lib/internal/classfmt/SyntheticAttribute.inl>
#include <JCDT_Lib/internal/classfmt/SourceFileAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	SyntheticAttribute::SyntheticAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_Synthetic, buffer)
	{
		if (attribute_length)
			buffer.MarkInvalid("bad synthetic attribute length");
	}


	SourceFileAttribute::SourceFileAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_SourceFile, buffer)
		, sourcefile_index(buffer.GetU2())
	{
		if (attribute_length != 2)
			buffer.MarkInvalid("bad source file attribute length");
		if (buffer.Pool()[sourcefile_index]->Tag() != CPInfo::CONSTANT_Utf8)
			buffer.MarkInvalid("bad type for source file attribute");
	}


} // Close namespace Jikes block



