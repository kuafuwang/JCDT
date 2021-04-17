
#include <JCDT_Lib/internal/classfmt/LineNumberTableAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	LineNumberTableAttribute::LineNumberTableAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_LineNumberTable, buffer)
		, line_number_table(6, 16)
	{
		unsigned count = buffer.GetU2();
		if (attribute_length != count * 4 + 2)
			buffer.MarkInvalid("bad line number table length");
		while (count--)
		{
			LineNumberElement& entry = line_number_table.Next();
			entry.start_pc = buffer.GetU2();
			entry.line_number = buffer.GetU2();
		}
	}

	int LineNumberTableAttribute::getLineNumber(int pc)
	{
		int line_number = -1;
		int distance = -1;
		auto length = line_number_table.Length();
		for (unsigned i = 0; i < length; i++)
		{
			LineNumberElement& record = line_number_table[i];
			if(record.start_pc <= pc)
			{
				// the first matching record
				if (distance == -1) {
					distance = pc - record.start_pc;
					line_number = record.line_number;
				}
				else {
					if ((pc - record.start_pc) < distance) {
						distance = pc - record.start_pc;
						line_number = record.line_number;
					}
				}
			}
		}
	

		return line_number;
	}

	void LineNumberTableAttribute::Print(Ostream& os, const ConstantPool& constant_pool, int fill)
	{
		assert(fill == 2);
		PrintPrefix(os, "LineNumberTable", constant_pool, 2);
		os << ", count: " << (unsigned)line_number_table.Length()
			<< endl;
		for (unsigned i = 0; i < line_number_table.Length(); i++)
		{
			os << "    line: "
				<< (unsigned)line_number_table[i].line_number
				<< ",  pc: "
				<< (unsigned)line_number_table[i].start_pc
			<< endl;
		}
	}
} // Close namespace Jikes block



