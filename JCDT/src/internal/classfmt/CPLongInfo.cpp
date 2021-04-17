
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>
#include <JCDT_Lib/internal/classfmt/CPLongInfo.inl>

namespace Jikes { // Open namespace Jikes block



	CPLongInfo::CPLongInfo(ClassFile& buffer)
		: CPInfo(CONSTANT_Long)
		, value(buffer.GetU8())
	{
		
	}


} // Close namespace Jikes block



