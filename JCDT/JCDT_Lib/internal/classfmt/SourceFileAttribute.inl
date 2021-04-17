
#ifndef jcdt_class_SourceFileAttribute_INCLUDED
#define jcdt_class_SourceFileAttribute_INCLUDED
#include "AttributeInfo.inl"
#include "CPUtf8Info.inl"


namespace Jikes { // Open namespace Jikes block

//
// Valid only for classes, this names the source file. JVMS 4.7.7.
//
class SourceFileAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    const u2 sourcefile_index;

public:
    SourceFileAttribute(u2 _name_index, u2 _sourcefile_index)
        : AttributeInfo(ATTRIBUTE_SourceFile, _name_index, 2)
        , sourcefile_index(_sourcefile_index)
    {}
    SourceFileAttribute(ClassFile&);
    virtual ~SourceFileAttribute() {}

    const char* SourceFile(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[sourcefile_index] -> Tag() ==
               CPInfo::CONSTANT_Utf8);
        return ((CPUtf8Info*) constant_pool[sourcefile_index]) -> Bytes();
    }
    u2 SourceFileLength(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[sourcefile_index] -> Tag() ==
               CPInfo::CONSTANT_Utf8);
        return ((CPUtf8Info*) constant_pool[sourcefile_index]) -> Length();
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(sourcefile_index);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,"SourceFile", constant_pool, 0);
        os << ", name: " << (unsigned) sourcefile_index;
        if (constant_pool[sourcefile_index] -> Tag() == CPInfo::CONSTANT_Utf8)
        {
            os << "=\"";
            constant_pool[sourcefile_index] ->  Describe(os,constant_pool);
            os << '"';
        }
        else os << "(invalid)";
        os << endl;
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

