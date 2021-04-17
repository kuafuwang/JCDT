
#include <JCDT_Lib/internal/classfmt/LocalVariableTableAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	LocalVariableTableAttribute::LocalVariableTableAttribute(ClassFile& buffer,
		bool generic)
		: AttributeInfo(generic ? ATTRIBUTE_LocalVariableTypeTable
			: ATTRIBUTE_LocalVariableTable, buffer)
		, local_variable_table(6, 16)
	{
		unsigned count = buffer.GetU2();
		if (attribute_length != count * 10 + 2)
			buffer.MarkInvalid("bad local variable table length");
		while (count--)
		{
			LocalVariableElement& entry = local_variable_table.Next();
			entry.start_pc = buffer.GetU2();
			entry.length = buffer.GetU2();
			entry.name_index = buffer.GetU2();
			entry.descriptor_index = buffer.GetU2();
			entry.index = buffer.GetU2();
			if ((buffer.Pool()[entry.name_index]->Tag() !=
				CPInfo::CONSTANT_Utf8) ||
				(buffer.Pool()[entry.descriptor_index]->Tag() !=
					CPInfo::CONSTANT_Utf8))
			{
				buffer.MarkInvalid("bad type for local variable table entry");
			}
		}
	}

	void LocalVariableTableAttribute::Print(Ostream& os, const ConstantPool& constant_pool, int fill)
	{
		assert(fill == 2);
		PrintPrefix(os, tag == ATTRIBUTE_LocalVariableTable
			                ? "LocalVariableTable"
			                : "LocalVariableTypeTable", constant_pool, 2);
		os << ", count: " << (unsigned)local_variable_table.Length()
			<< endl;
		for (unsigned i = 0; i < local_variable_table.Length(); i++)
		{
			unsigned index = local_variable_table[i].name_index;
			os << "    pc: "
				<< (unsigned)local_variable_table[i].start_pc
				<< ", length: "
				<< (unsigned)local_variable_table[i].length << "(pc: "
				<< (unsigned)(local_variable_table[i].start_pc +
					local_variable_table[i].length)
				<< "), name: " << index;
			if (constant_pool[index]->Tag() == CPInfo::CONSTANT_Utf8)
			{
				os << '=';
				constant_pool[index]->Describe(os, constant_pool);
			}
			else os << "(invalid)";

			index = local_variable_table[i].descriptor_index;
			os  << "  ,type: " << index;
			if (constant_pool[index]->Tag() == CPInfo::CONSTANT_Utf8)
			{
				os << '=';
				constant_pool[index]->Describe(os, constant_pool);
			}
			else os << "(invalid)";
			os << ", index: " << (unsigned)local_variable_table[i].index
				<< endl;
		}
	}
} // Close namespace Jikes block



