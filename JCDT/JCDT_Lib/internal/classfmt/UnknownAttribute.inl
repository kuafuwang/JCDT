
#ifndef jcdt_class_UnknownAttribute_INCLUDED
#define jcdt_class_UnknownAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// An unknown attribute - we are allowed to read them from existing .class
// files, but should never generate or write one.
//
class UnknownAttribute : public AttributeInfo
{
    u1* info; // u1 info[attribute_length];
    u4 info_length; // in corrupted file, info_length < attribute_length

public:
    UnknownAttribute(ClassFile&);
    virtual ~UnknownAttribute() { delete [] info; }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill) 
    {
        PrintPrefix(os,"Unrecognized attribute", constant_pool, fill);
        os << endl;
        os.width(fill);
        os << "" << " info: " << (unsigned) info_length << ' ';
        for (unsigned i = 0; i < info_length; i++)
            if (info[i] <= 0x20 || info[i] >= 0x7f)
                os << "\\x" << IntToString(info[i], 2).String();
            else os << (char) info[i];
        os << endl;
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

