#ifndef jcdt_class_AttributeInfo_INCLUDED
#define jcdt_class_AttributeInfo_INCLUDED
#include <JCDT_Lib/internal/util/tuple.h>
#include "ConstantPool.inl"


namespace Jikes {
	class CPUtf8Info;
	class ClassFile;

	// Open namespace Jikes block


//
// Describes an arbitrary attribute. We subclass this for known attributes.
//
class AttributeInfo
{
public:
    enum AttributeInfoTag
    {
        ATTRIBUTE_ConstantValue,
        ATTRIBUTE_Code,
        ATTRIBUTE_Exceptions,
        ATTRIBUTE_InnerClasses,
        ATTRIBUTE_Synthetic,
        ATTRIBUTE_SourceFile,
        ATTRIBUTE_LineNumberTable,
        ATTRIBUTE_LocalVariableTable,
        ATTRIBUTE_Deprecated,
        ATTRIBUTE_Signature, // defined in JSR 14
        ATTRIBUTE_Bridge, // defined in JSR 14?
        ATTRIBUTE_EnclosingMethod, // defined in JSR 14
        ATTRIBUTE_LocalVariableTypeTable, // defined in JSR 14
        ATTRIBUTE_StackMap, // defined in JSR 139
        ATTRIBUTE_RuntimeVisibleAnnotations, // defined in JSR 175
        ATTRIBUTE_RuntimeInvisibleAnnotations, // defined in JSR 175
        ATTRIBUTE_RuntimeVisibleParameterAnnotations, // defined in JSR 175
        ATTRIBUTE_RuntimeInvisibleParameterAnnotations, // defined in JSR 175
        ATTRIBUTE_AnnotationDefault, // defined in JSR 175
        // all others are currently unknown to jikes
        ATTRIBUTE_Generic

    };

protected:
    const AttributeInfoTag tag;
    const u2 attribute_name_index;
    u4 attribute_length;

    AttributeInfo(AttributeInfoTag _tag, u2 _name_index, u4 length = 0)
        : tag(_tag)
        , attribute_name_index(_name_index)
        , attribute_length(length)
    {
        assert(tag != ATTRIBUTE_Generic);
    }
    AttributeInfo(AttributeInfoTag, ClassFile&);
public:
    virtual ~AttributeInfo() {}

    AttributeInfoTag Tag() const { return tag; }
    u2 AttributeNameIndex() const { return attribute_name_index; }
    u4 AttributeLength() const { return attribute_length; }

    static AttributeInfoTag Tag(const CPUtf8Info* name);

    //
    // This reads the next attribute from the buffer, advancing it to the
    // end of the attribute. The user is responsible for deleting the result.
    //
    static AttributeInfo* AllocateAttributeInfo(ClassFile&);

    // Subclasses must override if attribute_length != 0.
    virtual void Put(OutputBuffer& out) const
    {
        assert(tag != ATTRIBUTE_Generic);
        assert(attribute_name_index);
        out.PutU2(attribute_name_index);
        out.PutU4(attribute_length);
    }

#ifdef JIKES_DEBUG
    void PrintPrefix(Ostream& os, const char* category, const ConstantPool& constant_pool,
                     int fill) const
    {
        os.width(fill);
        os << "" << category << ": name: "
                << (unsigned) attribute_name_index;
        if (constant_pool[attribute_name_index] -> Tag() ==
            CPInfo::CONSTANT_Utf8)
        {
            os << "=\"";
            constant_pool[attribute_name_index] ->  Describe(os,constant_pool);
            os << '"';
        }
        else os << "(invalid)";
        os << ", length: " << (unsigned) attribute_length;
    }

    virtual void Print(Ostream& os,const ConstantPool& constant_pool,
                       int fill = 0) = 0;
#endif // JIKES_DEBUG
};


} // Close namespace Jikes block


#endif // _INCLUDED
