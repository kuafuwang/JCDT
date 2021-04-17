
#ifndef jcdt_class_CodeAttribute_INCLUDED
#define jcdt_class_CodeAttribute_INCLUDED
#include "AttributeInfo.inl"
#include "LineNumberTableAttribute.inl"
#include "LocalVariableTableAttribute.inl"
#include "StackMapAttribute.inl"
#include <set>
#include <unordered_map>
namespace Jikes { // Open namespace Jikes block


//
// Valid only on non-abstract non-native methods, this is the method
// implementation. JVMS 4.7.3.
//
class CodeAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    u2 max_stack;
    u2 max_locals;
    // u4 code_length; // computed as code.Length();
    Tuple<u1> code; // code[code_length]
    // u2 exception_table_length; // computed as exception_table.Length();
    struct ExceptionElement
    {
        u2 start_pc;
        u2 end_pc;
        u2 handler_pc;
        u2 catch_type;
    };
    // exception_table[exception_table_length]
    Tuple<ExceptionElement> exception_table;
    // u2 attribute_count; // computed as attributes.Length();
    Tuple<AttributeInfo*> attributes; // attributes[attributes_count]

    // Remember location of known attributes.
    LineNumberTableAttribute* attr_linenumbers;
    LocalVariableTableAttribute* attr_locals;
    LocalVariableTableAttribute* attr_local_types;
    StackMapAttribute* attr_stackmap;

public:
    CodeAttribute(u2 _name_index, u2 _max_locals)
        : AttributeInfo(ATTRIBUTE_Code, _name_index, 12)
        // +2 for max_stack, +2 for max_locals, +4 for code_length,
        // +2 for exception_table_length, +2 for attributes_count
        , max_stack(0) // filled in later
        , max_locals(_max_locals)
        , code(8, 4)
        , exception_table(6, 16)
        , attributes(6, 16)
        , attr_linenumbers(NULL)
        , attr_locals(NULL)
        , attr_stackmap(NULL)
    {}
    CodeAttribute(ClassFile&);
    virtual ~CodeAttribute()
    {
        for (unsigned i = 0; i < attributes.Length(); i++)
            delete attributes[i];
    }

    u2 MaxStack() const { return max_stack; }
    void SetMaxStack(u2 val) { max_stack = val; }

    u2 MaxLocals() const { return max_locals; }
    void SetMaxLocals(u2 val) { max_locals = val; }

    u2 CodeLength() const
    {
        return CodeLengthExceeded() ? 65535 : (u2) code.Length();
    }
    bool CodeLengthExceeded() const { return code.Length() > 65535; }

    void ResetCode(unsigned i, u1 byte) { code[i] = byte; }

    void AddCode(u1 byte)
    {
        code.Next() = byte;
        attribute_length++;
    }

    void DeleteCode(unsigned count)
    {
        assert(count <= code.Length());
        code.Reset(code.Length() - count);
        attribute_length -= count;
    }

    u2 ExceptionTableLength() const { return exception_table.Length(); }

    void AddException(u2 start_pc, u2 end_pc, u2 handler_pc, u2 catch_type)
    {
        if (start_pc == end_pc)
            return;
        if (exception_table.Length())
        {
            ExceptionElement& other = exception_table.Top();
            if (end_pc == other.start_pc && handler_pc == other.handler_pc &&
                catch_type == other.catch_type)
            {
                other.start_pc = start_pc;
                return;
            }
        }
        attribute_length += 8;
        ExceptionElement& entry = exception_table.Next();
        entry.start_pc = start_pc;
        entry.end_pc = end_pc;
        entry.handler_pc = handler_pc;
        entry.catch_type = catch_type;
    }

    u2 AttributesCount() const { return attributes.Length(); }

    void AddAttribute(AttributeInfo* attribute)
    {
        attributes.Next() = attribute;
        attribute_length += 6 + attribute -> AttributeLength();
        // +6 for name and length of attribute
        switch (attribute -> Tag())
        {
        case AttributeInfo::ATTRIBUTE_LineNumberTable:
            assert(! attr_linenumbers);
            attr_linenumbers = (LineNumberTableAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_LocalVariableTable:
            assert(! attr_locals);
            attr_locals = (LocalVariableTableAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_LocalVariableTypeTable:
            assert(! attr_local_types);
            attr_local_types = (LocalVariableTableAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_StackMap:
            assert(! attr_stackmap);
            attr_stackmap = (StackMapAttribute*) attribute;
            break;
        default:
            assert(false && "adding unexpected code attribute");
        }
    }

    virtual void Put(OutputBuffer& out) const
    {
        unsigned i;
        AttributeInfo::Put(out);
        out.PutU2(max_stack);
        out.PutU2(max_locals);
        out.PutU4(code.Length());
        for (i = 0; i < code.Length(); i++)
            out.PutU1(code[i]);
        out.PutU2(exception_table.Length());
        for (i = 0; i < exception_table.Length(); i++)
        {
            out.PutU2(exception_table[i].start_pc);
            out.PutU2(exception_table[i].end_pc);
            out.PutU2(exception_table[i].handler_pc);
            out.PutU2(exception_table[i].catch_type);
        }
        out.PutU2(attributes.Length());
        for (i = 0; i < attributes.Length(); i++)
            attributes[i] -> Put(out);
    }

#ifdef JIKES_DEBUG

	std::unordered_map <int, std::vector<int>> m_Positons;
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) ;
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

