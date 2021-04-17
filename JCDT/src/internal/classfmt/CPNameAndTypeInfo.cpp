
#include <JCDT_Lib/internal/classfmt/CPNameAndTypeInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	CPNameAndTypeInfo::CPNameAndTypeInfo(ClassFile& buffer)
		: CPInfo(CONSTANT_NameAndType)
		, name_index(buffer.GetU2())
		, descriptor_index(buffer.GetU2())
	{}

	void CPNameAndTypeInfo::Print(Ostream& os, const ConstantPool& constant_pool) const
	{
		os << "NameAndType: name: "
			<< (unsigned)name_index;
		if (constant_pool[name_index]->Tag() == CONSTANT_Utf8)
		{
			os << "=\"";
			constant_pool[name_index]->Describe(os, constant_pool);
			os << '"';
		}
		else os << "(invalid)";
		os << endl << "        type: " << (unsigned)descriptor_index;
		if (constant_pool[descriptor_index]->Tag() == CONSTANT_Utf8)
		{
			os << "=\"";
			constant_pool[descriptor_index]->Describe(os, constant_pool);
			os << '"';
		}
		else os << "(invalid)";
		os << endl;
	}

	void CPNameAndTypeInfo::Describe(Ostream& os, const ConstantPool& constant_pool) const
	{
		if (constant_pool[name_index]->Tag() == CONSTANT_Utf8)
			constant_pool[name_index]->Describe(os, constant_pool);
		else os << "(invalid)";
		os << " ";
		if (constant_pool[descriptor_index]->Tag() == CONSTANT_Utf8)
			constant_pool[descriptor_index]->Describe(os, constant_pool);
		else os << "(invalid)";
	}
} // Close namespace Jikes block



