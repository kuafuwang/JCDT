
#include <JCDT_Lib/internal/classfmt/CPIntegerInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	CPIntegerInfo::CPIntegerInfo(ClassFile& buffer)
		: CPInfo(CONSTANT_Integer)
		, bytes(buffer.GetU4())
	{}


} // Close namespace Jikes block



