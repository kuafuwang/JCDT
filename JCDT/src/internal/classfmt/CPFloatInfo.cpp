
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>
#include <JCDT_Lib/internal/classfmt/CPFloatInfo.inl>

namespace Jikes { // Open namespace Jikes block



	CPFloatInfo::CPFloatInfo(ClassFile& buffer)
		: CPInfo(CONSTANT_Float)
		, value(buffer.GetU4())
	{}


	

} // Close namespace Jikes block



