
#ifndef jcdt_class_LocalVariableTableAttribute_INCLUDED
#define jcdt_class_LocalVariableTableAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// Valid only for code, LocalVariableTable gives local variable name, scope,
// and type information relative to bytecode offsets. JVMS 4.7.10.
// Also, the LocalVariableTypeTable gives generic type information.
//
class LocalVariableTableAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    // u2 local_variable_length; // computed as local_variable_table.Length()
    struct LocalVariableElement
    {
        u2 start_pc;
        u2 length;
        u2 name_index;
        u2 descriptor_index;
        u2 index;
    };
    // local_variable_table[local_variable_table_length]
    Tuple<LocalVariableElement> local_variable_table;

public:
    LocalVariableTableAttribute(u2 _name_index, bool generic = false)
        : AttributeInfo(generic ? ATTRIBUTE_LocalVariableTypeTable
                        : ATTRIBUTE_LocalVariableTable, _name_index, 2)
        // +2 for local_variable_table_length
        , local_variable_table(6, 16)
    {}
    LocalVariableTableAttribute(ClassFile&, bool generic);
    virtual ~LocalVariableTableAttribute() {}

    u2 LocalVariableTableLength() const
    {
        return local_variable_table.Length();
    }

    //
    // make entry in local variable table
    //
    void AddLocalVariable(u2 start, u2 end, u2 name, u2 descriptor, u2 index)
    {
        assert(end >= start);

        if (end > start)
        {
            attribute_length += 10;
            LocalVariableElement& entry = local_variable_table.Next();
            entry.start_pc = start;
            entry.length = end - start;
            entry.name_index = name;
            entry.descriptor_index = descriptor;
            entry.index = index;
        }
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(local_variable_table.Length());
        for (unsigned i = 0; i < local_variable_table.Length(); i++)
        {
            out.PutU2(local_variable_table[i].start_pc);
            out.PutU2(local_variable_table[i].length);
            out.PutU2(local_variable_table[i].name_index);
            out.PutU2(local_variable_table[i].descriptor_index);
            out.PutU2(local_variable_table[i].index);
        }
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os, const ConstantPool& constant_pool, int fill = 2);
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

