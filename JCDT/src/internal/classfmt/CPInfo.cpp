
#include <JCDT_Lib/internal/classfmt/CPInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPUtf8Info.inl>
#include <JCDT_Lib/internal/classfmt/CPIntegerInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPFloatInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPLongInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPMemberInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPDoubleInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPClassInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPNameAndTypeInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPStringInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	CPInfo* CPInfo::AllocateCPInfo(ClassFile& buffer)
	{
		u1 tag = buffer.GetU1();
		switch (tag)
		{
		case CONSTANT_Utf8:
			return new CPUtf8Info(buffer);
		case CONSTANT_Integer:
			return new CPIntegerInfo(buffer);
		case CONSTANT_Float:
			return new CPFloatInfo(buffer);
		case CONSTANT_Long:
			return new CPLongInfo(buffer);
		case CONSTANT_Double:
			return new CPDoubleInfo(buffer);
		case CONSTANT_Class:
			return new CPClassInfo(buffer);
		case CONSTANT_String:
			return new CPStringInfo(buffer);
		case CONSTANT_Fieldref:
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			return new CPMemberInfo((ConstantPoolTag)tag, buffer);
		case CONSTANT_NameAndType:
			return new CPNameAndTypeInfo(buffer);
		default:
			return new CPInfo((ConstantPoolTag)tag);
		}
	}

	

} // Close namespace Jikes block



