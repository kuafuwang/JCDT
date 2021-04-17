#ifndef jcdt_class_CPUtf8Info_INCLUDED
#define jcdt_class_CPUtf8Info_INCLUDED
#include "CPInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// Describes a UTF8 sequence. JVMS 4.4.7.
//
class CPUtf8Info : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const u2 len;
    u1* bytes; // bytes[length]

    bool valid;

public:
    CPUtf8Info(const char* _bytes, unsigned length)
        : CPInfo(CONSTANT_Utf8)
        , len(length)
        , valid(true)
    {
        bytes = new u1[len + 1];
        for (unsigned i = 0; i < len; i++)
            bytes[i] = (u1) _bytes[i];
        bytes[len] = U_NULL;
        Init(len);
        assert(valid && length <= 0xffff);
    }
    CPUtf8Info(ClassFile&);
    virtual ~CPUtf8Info()
    {
        delete [] bytes;
    }

    //
    // If this UTF8 was valid, then Bytes returns a NULL-terminated string
    // that can be passed to strcmp() and friends, since Java's modified UTF8
    // encoding ensures there are no embedded \0. However, since we already
    // know Length(), calling strlen() on the result is inefficient.
    //
    const char* Bytes() const { return (const char*) bytes; }
    u2 Length() const { return len; }

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU2(len);
        out.PutN(bytes, len);
    }

    virtual bool Check(const ConstantPool&) const { return valid; }

private:
    void Init(u2);

#ifdef JIKES_DEBUG
    //
    // When debugging, contents represents the String literal (less the
    // enclosing "") made of printing ASCII characters that will regenerate
    // the same sequence of bytes, provided the UTF8 was valid.  Otherwise it
    // contains some hints as to where the UTF8 went wrong. It is a
    // NULL-terminated sequence, with no embedded NULLs.
    //
    string contents;

public:
    string GetContents();

    virtual void Print(Ostream& os, const ConstantPool&) const;

    virtual void Describe(Ostream& os, const ConstantPool&) const;
#endif // JIKES_DEBUG
};


} // Close namespace Jikes block


#endif // _INCLUDED
