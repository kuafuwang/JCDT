
#ifndef jcdt_class_BridgeAttribute_INCLUDED
#define jcdt_class_BridgeAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block


	//
// Valid for methods, this marks the method as a bridge for covariant returns.
// JSR 14? (mentioned in JSR 200).
//
class BridgeAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo

public:
    BridgeAttribute(u2 name_index, u2 /*index*/)
        : AttributeInfo(ATTRIBUTE_Bridge, name_index)
    {}
    BridgeAttribute(ClassFile&);
    virtual ~BridgeAttribute() {}

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,"Bridge", constant_pool, 0);
        os << endl;
    }
#endif // JIKES_DEBUG
};



} // Close namespace Jikes block


#endif // _INCLUDED

