
#include <JCDT_Lib/internal/classfmt/CPMemberInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	CPMemberInfo::CPMemberInfo(ConstantPoolTag _tag, ClassFile& buffer)
		: CPInfo(_tag)
		, class_index(buffer.GetU2())
		, name_and_type_index(buffer.GetU2())
	{}


} // Close namespace Jikes block



