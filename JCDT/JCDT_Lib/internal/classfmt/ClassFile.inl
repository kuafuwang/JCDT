
#ifndef jcdt_class_ClassFile_INCLUDED
#define jcdt_class_ClassFile_INCLUDED
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>
#include "SignatureAttribute.inl"
#include "InnerClassesAttribute.inl"
#include "MethodInfo.inl"
#include "SourceFileAttribute.inl"
#include "FieldInfo.inl"

namespace Jikes { // Open namespace Jikes block

	class EnclosingMethodAttribute;


//
// Here we have the complete definition of the .class file format. JVMS 4.1.
//
class ClassFile : public AccessFlags
{
    enum
    {
        MAGIC = 0xcafebabe
    };
    const char* problem; // why this file is invalid (NULL if it's okay)
    const char* buffer; // current location during input (NULL for output)
    const char* const buffer_tail; // last location during input

protected:
    u4 magic;
    u2 minor_version;
    u2 major_version;

    // u2 constant_pool_count; // computed as constant_pool.Length()
    // cp_info constant_pool[constant_pool_count - 1]
    ConstantPool constant_pool; // note that constant_pool[0] is invalid

    // u2 access_flags; // inherited from AccessFlags
    u2 this_class;
    u2 super_class;

    // u2 interfaces_count // computed as interfaces.Length()
    Tuple<u2> interfaces; // u2 interfaces[interfaces_count]

    // u2 fields_count // computed as fields.Length()
    Tuple<FieldInfo*> fields; // field_info fields[fields_count]

    // u2 methods_count // computed as methods.Length()
    Tuple<MethodInfo*> methods; // method_info methods[methods_count]

    // u2 attributes_count // computed as attributes.Length()
    // u2 attribute_info attributes[attributes_count]
    Tuple<AttributeInfo*> attributes;

    // Remember location of known attributes.
    SyntheticAttribute* attr_synthetic;
    DeprecatedAttribute* attr_deprecated;
    SignatureAttribute* attr_signature;
    SourceFileAttribute* attr_sourcefile;
    InnerClassesAttribute* attr_innerclasses;
    AnnotationsAttribute* attr_visible_annotations;
    AnnotationsAttribute* attr_invisible_annotations;
    EnclosingMethodAttribute* attr_enclosing_method;

public:
	bool  m_for_compiler{};
    //
    // Construct a class file for output, given the finished type.
    //
	ClassFile();

	//
    // Read a class from input. Note that we try to parse to the end of the
    // file, even after encountering an error, for purposes of decompiling.
    //
    ClassFile(const char*, unsigned,bool for_compiler = true);

   virtual ~ClassFile()
    {
        unsigned i;
        for (i = 1; i < constant_pool.Length(); i++)
            if (constant_pool.Valid(i))
                delete constant_pool[i];
        for (i = 0; i < fields.Length(); i++)
            delete fields[i];
        for (i = 0; i < methods.Length(); i++)
            delete methods[i];
        for (i = 0; i < attributes.Length(); i++)
            delete attributes[i];
    }

    u2 ConstantPoolCount() const { return constant_pool.Length(); }
    const ConstantPool& Pool() const { return constant_pool; }
    CPClassInfo* ThisClass()
    {
        return constant_pool[this_class] -> Tag() == CPInfo::CONSTANT_Class
            ? (CPClassInfo*) constant_pool[this_class]
            : (CPClassInfo*) NULL;
    }
    CPClassInfo* SuperClass()
    {
        return constant_pool[super_class] -> Tag() == CPInfo::CONSTANT_Class
            ? (CPClassInfo*) constant_pool[super_class]
            : (CPClassInfo*) NULL;
    }
    u2 InterfacesCount() const { return interfaces.Length(); }
    CPClassInfo* Interface(u2 i)
    {
        return constant_pool[interfaces[i]] -> Tag() == CPInfo::CONSTANT_Class
            ? (CPClassInfo*) constant_pool[interfaces[i]]
            : (CPClassInfo*) NULL;
    }
    u2 FieldsCount() const { return fields.Length(); }
    const FieldInfo* Field(u2 i) const { return fields[i]; }
    u2 MethodsCount() const { return methods.Length();}
    const MethodInfo* Method(u2 i) const { return methods[i]; }

    u2 AttributesCount() const { return attributes.Length(); }
    const AttributeInfo* Attribute(u2 i) const { return attributes[i]; }
    inline void AddAttribute(AttributeInfo* attribute)
    {
        attributes.Next() = attribute;
        switch (attribute -> Tag())
        {
        case AttributeInfo::ATTRIBUTE_Synthetic:
            //
            // We must be adding the attribute because we are targetting an
            // older VM that doesn't recognize the access flag.
            //
            assert(! attr_synthetic && ACC_SYNTHETIC());
            ResetACC_SYNTHETIC();
            attr_synthetic = (SyntheticAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_Deprecated:
            assert(! attr_deprecated);
            attr_deprecated = (DeprecatedAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_Signature:
            assert(! attr_signature);
            attr_signature = (SignatureAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_SourceFile:
            assert(! attr_sourcefile);
            attr_sourcefile = (SourceFileAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_InnerClasses:
            assert(! attr_innerclasses);
            attr_innerclasses = (InnerClassesAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
            assert(! attr_visible_annotations);
            attr_visible_annotations = (AnnotationsAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
            assert(! attr_invisible_annotations);
            attr_invisible_annotations = (AnnotationsAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_EnclosingMethod:
            assert(! attr_enclosing_method);
            attr_enclosing_method = (EnclosingMethodAttribute*) attribute;
            break;
        default:
            assert(false && "adding unexpected class attribute");
        }
    }
    bool Synthetic() const { return attr_synthetic || ACC_SYNTHETIC(); }
    bool Deprecated() const { return attr_deprecated != NULL; }
    const char* SourceFile() const
    {
        return attr_sourcefile ? attr_sourcefile -> SourceFile(constant_pool)
            : (const char*) NULL;
    }
    u2 SourceFileLength() const
    {
        return attr_sourcefile
            ? attr_sourcefile -> SourceFileLength(constant_pool) : 0;
    }
    const InnerClassesAttribute* InnerClasses() const
    {
        return attr_innerclasses;
    }
    const SignatureAttribute* Signature() const { return attr_signature; }

    void Write(Ostream& os, TypeSymbol* unit_type) const;

    bool Valid() const { return (problem == NULL); }
    void MarkInvalid(const char* reason) { problem = reason; }
    const char* DescribeProblem() const { return problem; }

    inline u1 GetU1()
    {
        if (buffer == buffer_tail)
        {
            MarkInvalid("couldn't read u1");
            return 0;
        }
        return (u1) *buffer++;
    }
    inline u2 GetU2()
    {
        if (buffer + 1 >= buffer_tail)
        {
            MarkInvalid("couldn't read u2");
            buffer = buffer_tail;
            return 0;
        }
        u2 i = (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        return i;
    }
    inline u2 PeekU2()
    {
        if (buffer + 1 >= buffer_tail)
        {
            MarkInvalid("couldn't peek u2");
            return 0;
        }
        u2 i = (u1) *buffer;
        i = (i << 8) | (u1) buffer[1];
        return i;
    }
    inline u4 GetU4()
    {
        if (buffer + 3 >= buffer_tail)
        {
            MarkInvalid("couldn't get u4");
            buffer = buffer_tail;
            return 0;
        }
        u4 i = (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        return i;
    }
    inline BaseLong GetU8()
    {
        if (buffer + 7 >= buffer_tail)
        {
            MarkInvalid("couldn't get u8");
            buffer = buffer_tail;
            return 0;
        }
        BaseLong i = (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        i = (i << 8) | (u1) *buffer++;
        return i;
    }
    //
    // Gets max(len, rest of buffer) bytes, and stores it in a new u1[len + 1]
    // in the previously uninitialized bytes.  The extra byte is set to U_NULL,
    // so if bytes had no embedded NULLs, it can be cast to (const char*) and
    // used in methods like strlen(). Returns the size of bytes, and caller
    // is responsible for calling delete[] on bytes.
    //
    inline u4 GetN(u1*& bytes, u4 len)
    {
        if (buffer + len > buffer_tail)
        {
            MarkInvalid("couldn't get n bytes");
            len = buffer_tail - buffer;
        }
        bytes = new u1[len + 1];
        if (len)
            memcpy(bytes, buffer, len);
        bytes[len] = U_NULL;
        buffer += len;
        return len;
    }
    inline void SkipN(u4 len)
    {
        if (buffer + len > buffer_tail)
        {
            MarkInvalid("couldn't skip n bytes");
            buffer = buffer_tail;
        }
        else buffer += len;
    }

#ifdef JIKES_DEBUG
public:
    void Print(Ostream& os);
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

