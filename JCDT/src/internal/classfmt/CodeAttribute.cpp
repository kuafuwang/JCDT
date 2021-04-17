
#include <JCDT_Lib/internal/classfmt/CodeAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>
#include <JCDT_Lib/internal/classfmt/LineNumberTableAttribute.inl>
#include <JCDT_Lib/internal/classfmt/LocalVariableTableAttribute.inl>
#include <JCDT_Lib/internal/classfmt/Operators.h>
#include <JCDT_Lib/internal/classfmt/StackMapAttribute.inl>

namespace Jikes { // Open namespace Jikes block



	CodeAttribute::CodeAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_Code, buffer)
		, max_stack(buffer.GetU2())
		, max_locals(buffer.GetU2())
		, code(8, 4)
		, exception_table(6, 16)
		, attributes(6, 16)
		, attr_linenumbers(NULL)
		, attr_locals(NULL)
		, attr_stackmap(NULL)
	{
		unsigned remaining = attribute_length - 12;
		// +2 for max_stack, +2 for max_locals, +4 for code_length,
		// +2 for exception_table_length, +2 for attributes_count
		u4 code_length = buffer.GetU4();
		remaining -= code_length;
		if(buffer.m_for_compiler)
		{
			buffer.SkipN(code_length);
		}
		else
		{
			while (code_length--)
			{
				code.Next() = buffer.GetU1();
			}
		}

		

		u2 exception_table_length = buffer.GetU2();
		remaining -= exception_table_length * 8;
		while (exception_table_length--)
		{
			ExceptionElement& entry = exception_table.Next();
			entry.start_pc = buffer.GetU2();
			entry.end_pc = buffer.GetU2();
			entry.handler_pc = buffer.GetU2();
			entry.catch_type = buffer.GetU2();
			if (entry.catch_type &&
				buffer.Pool()[entry.catch_type]->Tag() != CPInfo::CONSTANT_Class)
			{
				buffer.MarkInvalid("bad type for exception table catch type");
			}
		}

		u2 attributes_count = buffer.GetU2();
		while (attributes_count--)
		{
			AttributeInfo* attr = AllocateAttributeInfo(buffer);
			remaining -= 6 + attr->AttributeLength();
			attributes.Next() = attr;
			switch (attr->Tag())
			{
			case AttributeInfo::ATTRIBUTE_LineNumberTable:
				if (attr_linenumbers)
					buffer.MarkInvalid("duplicate line number table");
				attr_linenumbers = (LineNumberTableAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_LocalVariableTable:
				if (attr_locals)
					buffer.MarkInvalid("duplicate local variable table");
				attr_locals = (LocalVariableTableAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_LocalVariableTypeTable:
				if (attr_local_types)
					buffer.MarkInvalid("duplicate local variable type table");
				attr_local_types = (LocalVariableTableAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_StackMap:
				if (attr_stackmap)
					buffer.MarkInvalid("duplicate stack map");
				attr_stackmap = (StackMapAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Generic:
				// ignore
				break;
			default:
				buffer.MarkInvalid("invalid code attribute");
			}
		}
		if (remaining)
			buffer.MarkInvalid("bytes remaining at end of code attribute");
	}

	extern void OperatorsEx_OpDmp(Ostream& os, const ConstantPool& constant_pool, const Tuple<u1>& code, std::vector<std::pair<int, int>>& codePostion);
#ifdef JIKES_DEBUG
	void CodeAttribute::Print(Ostream& os,const ConstantPool& constant_pool, int fill) 
	{
		assert(!fill);
		PrintPrefix(os,"Code", constant_pool, 0);
		os << endl << " max_stack: " << (unsigned)max_stack
			<< ", max_locals: " << (unsigned)max_locals
			<< ", code_length: " << (unsigned)code.Length() << endl;
		std::vector<std::pair<int,int>> codePositons;
		OperatorsEx_OpDmp(os,constant_pool, code, codePositons);
		os << " exception_table_length: "
			<< (unsigned)exception_table.Length() << endl;
		for (unsigned i = 0; i < exception_table.Length(); i++)
		{
			os << "  start " << (unsigned)exception_table[i].start_pc
				<< ", end " << (unsigned)exception_table[i].end_pc
				<< ", handler " << (unsigned)exception_table[i].handler_pc
				<< ", catch_type " << (unsigned)exception_table[i].catch_type;
			if (!exception_table[i].catch_type)
				os << " (all)";
			else if (constant_pool[exception_table[i].catch_type]->Tag() ==
				CPInfo::CONSTANT_Class)
			{
				os << '=';
				constant_pool[exception_table[i].catch_type] ->
					Describe(os,constant_pool);
			}
			else os << "(invalid)";
			os << endl;
		}
		os << " attribute_count: " << (unsigned)attributes.Length() << endl;
		LineNumberTableAttribute* line_attr = nullptr;
		for (unsigned j = 0; j < attributes.Length(); j++)
		{
			auto attr = attributes[j];
			if(attr->Tag( ) == AttributeInfo::ATTRIBUTE_LineNumberTable)
			{
				line_attr = (LineNumberTableAttribute*)(attr);
			}
			attr->Print(os, constant_pool, 2);
		}
			


		if (!line_attr)
		{
			return;
		}
		
		auto& line_number_table = line_attr->line_number_table;
	
		// resolve line number
		auto data = codePositons.data();
		for(size_t i = 0 ; i < codePositons.size(); ++i)
		{
			int line_number = line_attr->getLineNumber(data[i].first);
			
			m_Positons[line_number].emplace_back(data[i].second);
		}
		
	}
#endif // JIKES_DEBUG



} // Close namespace Jikes block



