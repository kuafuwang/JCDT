
#ifndef jcdt_class_ExceptionsAttribute_INCLUDED
#define jcdt_class_ExceptionsAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block

	class CPClassInfo;
//
// Valid only on methods, this is the throws clause. JVMS 4.7.4.
//
class ExceptionsAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    // u2 number_of_exceptions; // computed as exception_index_table.Length() 
    // exception_index_table[number_of_exceptions]
    Tuple<u2> exception_index_table;

public:
    ExceptionsAttribute(u2 _name_index)
        : AttributeInfo(ATTRIBUTE_Exceptions, _name_index, 2)
        // +2 for number_of_exceptions
        , exception_index_table(6, 16)
    {}
    ExceptionsAttribute(ClassFile&);
    virtual ~ExceptionsAttribute() {}

    u2 NumberOfExceptions() const { return exception_index_table.Length(); }
    const CPClassInfo* Exception(u2 i, const ConstantPool& constant_pool) const
    {
        assert(constant_pool[exception_index_table[i]] -> Tag() ==
               CPInfo::CONSTANT_Class);
        return (const CPClassInfo*) constant_pool[exception_index_table[i]];
    }

    void AddExceptionIndex(u2 index)
    {
        attribute_length += 2;
        exception_index_table.Next() = index;
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(exception_index_table.Length());
        for (unsigned i = 0; i < exception_index_table.Length(); i++)
            out.PutU2(exception_index_table[i]);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,"Exceptions", constant_pool, 0);
        os << ", count: " << (unsigned) exception_index_table.Length()
                << endl;
        for (unsigned i = 0; i < exception_index_table.Length(); i++)
        {
            os << "  " << (unsigned) exception_index_table[i];
            if (constant_pool[exception_index_table[i]] -> Tag() ==
                CPInfo::CONSTANT_Class)
            {
                os << '=';
                constant_pool[exception_index_table[i]] ->
                     Describe(os,constant_pool);
            }
            else os << "(invalid)";                    
            os << endl;
        }
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

