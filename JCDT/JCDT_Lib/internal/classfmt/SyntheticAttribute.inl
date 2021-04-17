
#ifndef jcdt_class_SyntheticAttribute_INCLUDED
#define jcdt_class_SyntheticAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// Valid for classes, methods, and fields, this marks a compiler-generated
// entity that has no counterpart in source. JVMS 4.7.6.
//
class SyntheticAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo

public:
    SyntheticAttribute(u2 _name_index)
        : AttributeInfo(ATTRIBUTE_Synthetic, _name_index)
    {}
    SyntheticAttribute(ClassFile&);
    virtual ~SyntheticAttribute() {}

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,"Synthetic", constant_pool, 0);
        os << endl;
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

