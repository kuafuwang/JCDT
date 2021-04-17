
#ifndef jcdt_class_ConstantValueAttribute_INCLUDED
#define jcdt_class_ConstantValueAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// Valid only on final fields, this marks the initial value of a constant
// variable. JVMS 4.7.2.
//
class ConstantValueAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    const u2 constantvalue_index;

public:
    ConstantValueAttribute(u2 _name_index, u2 _constantvalue_index)
        : AttributeInfo(ATTRIBUTE_ConstantValue, _name_index, 2)
        , constantvalue_index(_constantvalue_index)
    {}
    ConstantValueAttribute(ClassFile&);
    virtual ~ConstantValueAttribute() {}

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(constantvalue_index);
    }

    const CPInfo* Value(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[constantvalue_index] -> Constant());
        return constant_pool[constantvalue_index];
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,"ConstantValue", constant_pool, 0);
        os << ", value: " << (unsigned) constantvalue_index << " (";
        if (constant_pool[constantvalue_index] -> Constant())
        {
            constant_pool[constantvalue_index] ->  Describe(os,constant_pool);
        }
        else os << "invalid";
        os << ')' << endl;
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

