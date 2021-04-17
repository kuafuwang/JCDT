
#ifndef jcdt_class_LineNumberTableAttribute_INCLUDED
#define jcdt_class_LineNumberTableAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block

//
// Valid only for code, this correlates bytecode offsets to source file line
// numbers. JVMS 4.7.8.
//
class LineNumberTableAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    // u2 line_number_table_length; // computed as line_number_table.Length()
    struct LineNumberElement
    {
        u2 start_pc;
        u2 line_number;
    };


public:
	// line_number_table[line_number_table_length]
	Tuple<LineNumberElement> line_number_table;

    LineNumberTableAttribute(u2 _name_index)
        : AttributeInfo(ATTRIBUTE_LineNumberTable, _name_index, 2)
        // +2 for line_number_table_length
        , line_number_table(6, 16)
    {}
    LineNumberTableAttribute(ClassFile&);
    virtual ~LineNumberTableAttribute() {}

    u2 LineNumberTableLength() const
    {
        return line_number_table.Length();
    }

    void AddLineNumber(u2 start_pc, u2 line_number)
    {
        if (line_number_table.Length() > 1)
        {
            LineNumberElement& other = line_number_table.Top();
            if (start_pc == other.start_pc)
            {
                other.line_number = line_number;
                return;
            }
        }
        attribute_length += 4;
        LineNumberElement& entry = line_number_table.Next();
        entry.start_pc = start_pc;
        entry.line_number = line_number;
    }

    void SetMax(u2 max_pc)
    {
        unsigned i = line_number_table.Length();
        while(--i > 0 && line_number_table[i].start_pc > max_pc)
            line_number_table[i].start_pc = max_pc;
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(line_number_table.Length());
        for (unsigned i = 0; i < line_number_table.Length(); i++)
        {
            out.PutU2(line_number_table[i].start_pc);
            out.PutU2(line_number_table[i].line_number);
        }
    }
	int getLineNumber(int pc);
#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os, const ConstantPool& constant_pool, int fill = 2);
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

