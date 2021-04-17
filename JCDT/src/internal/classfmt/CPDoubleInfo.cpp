
#include <JCDT_Lib/internal/classfmt/CPDoubleInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	CPDoubleInfo::CPDoubleInfo(ClassFile& buffer)
		: CPInfo(CONSTANT_Double)
		, value(buffer.GetU8())
	{}


	

} // Close namespace Jikes block



