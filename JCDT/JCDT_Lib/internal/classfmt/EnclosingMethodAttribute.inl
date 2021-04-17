
#ifndef jct_class_EnclosingMethodAttribute_INCLUDED
#define jct_class_EnclosingMethodAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block


	
//
// Valid only on local and anonymous types, this lists the enclosing method
// of the type. JSR 202? (mentioned in JSR 200).
//
class EnclosingMethodAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    //
    // JSR 202 does not use a methodref, because classes defined in static or
    // instance initializers are given a name_and_type_index of 0.  You can
    // determine whether a class with an unlisted enclosing method was
    // declared in <clinit> or all <init>s by the presence of a this$0 member.
    //
    const u2 class_index;
    const u2 name_and_type_index;

public:
    EnclosingMethodAttribute(u2 _name_index, u2 type, u2 name)
        : AttributeInfo(ATTRIBUTE_EnclosingMethod, _name_index, 4)
        , class_index(type)
        , name_and_type_index(name)
    {}
    EnclosingMethodAttribute(ClassFile&);
    virtual ~EnclosingMethodAttribute() {}

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(class_index);
        out.PutU2(name_and_type_index);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os, const ConstantPool& constant_pool, int fill = 0);
#endif // JIKES_DEBUG
};



} // Close namespace Jikes block


#endif // _INCLUDED

