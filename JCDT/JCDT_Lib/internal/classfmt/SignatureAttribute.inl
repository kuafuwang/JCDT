
#ifndef jcdt_class_SignatureAttribute_INCLUDED
#define jcdt_class_SignatureAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block

//
// Valid for classes, methods, and fields, this gives the parameterized type
// signature for a generic type. JSR 14 section 7.
//
class SignatureAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    const u2 signature_index;

public:
    SignatureAttribute(u2 name_index, u2 index)
        : AttributeInfo(ATTRIBUTE_Signature, name_index, 2)
        , signature_index(index)
    {}
    SignatureAttribute(ClassFile&);
    virtual ~SignatureAttribute() {}

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(signature_index);
    }

    const CPUtf8Info* Signature(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[signature_index] -> Tag() ==
               CPInfo::CONSTANT_Utf8);
        return (const CPUtf8Info*) constant_pool[signature_index];
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os, const ConstantPool& constant_pool, int fill = 0);
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

