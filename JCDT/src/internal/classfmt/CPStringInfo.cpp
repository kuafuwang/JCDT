
#include <JCDT_Lib/internal/classfmt/CPInfo.inl>
#include <JCDT_Lib/internal/classfmt/CPStringInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block

	CPStringInfo::CPStringInfo(ClassFile& buffer)
		: CPInfo(CPInfo::CONSTANT_String)
		, string_index(buffer.GetU2())
	{
		
	}

	void CPStringInfo::Print(Ostream& os, const ConstantPool& constant_pool) const
	{
		os << "String: string: " << (unsigned)string_index;
		if (constant_pool[string_index]->Tag() == CONSTANT_Utf8)
		{
			os << "=\"";
			constant_pool[string_index]->Describe(os, constant_pool);
			os << '"';
		}
		else os << "(invalid)";
		os << endl;
	}

	void CPStringInfo::Describe(Ostream& os, const ConstantPool& constant_pool) const
	{
		if (constant_pool[string_index]->Tag() == CONSTANT_Utf8)
		{
			os << '"';
			constant_pool[string_index]->Describe(os, constant_pool);
			os << '"';
		}
		else os << "(invalid)";
	}
} // Close namespace Jikes block



