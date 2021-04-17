
#ifndef jcdt_class_DeprecatedAttribute_INCLUDED
#define jcdt_class_DeprecatedAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// Valid for classes, methods, and fields, this marks an entity as
// deprecated. JVMS 4.7.10.
//
class DeprecatedAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo

public:
    DeprecatedAttribute(u2 _name_index)
        : AttributeInfo(ATTRIBUTE_Deprecated, _name_index)
    {}
    DeprecatedAttribute(ClassFile&);
    virtual ~DeprecatedAttribute() {}

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,"Deprecated", constant_pool, 0);
        os << endl;
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

