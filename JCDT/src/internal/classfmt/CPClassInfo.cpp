
#include <JCDT_Lib/internal/classfmt/CPClassInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	CPClassInfo::CPClassInfo(ClassFile& buffer)
		: CPInfo(CONSTANT_Class)
		, name_index(buffer.GetU2())
		, type(NULL)
	{}

	std::string CPClassInfo::getName(ConstantPool& constant_pool)
	{
		if (constant_pool[name_index]->Tag() == CONSTANT_Utf8)
			return ((CPUtf8Info*)(constant_pool[name_index]))->GetContents().c_str();

		return "(invalid)";
	}
} // Close namespace Jikes block



