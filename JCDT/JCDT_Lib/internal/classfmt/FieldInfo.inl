
#ifndef jcdt_class_FieldInfo_INCLUDED
#define jcdt_class_FieldInfo_INCLUDED
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>
#include "AttributeInfo.inl"
#include "SyntheticAttribute.inl"
#include "DeprecatedAttribute.inl"
#include "SignatureAttribute.inl"
#include "ConstantValueAttribute.inl"
#include "AnnotationsAttribute.inl"
#include "CPUtf8Info.inl"



namespace Jikes {
	class Option;

	// Open namespace Jikes block



//
// Describes a member field. JVMS 4.5.
//
class FieldInfo : public AccessFlags
{
    // u2 access_flags; // inherited from AccessFlags
    u2 name_index;
    u2 descriptor_index;
    // u2 attributes_count; // computed as attributes.Length();
    Tuple<AttributeInfo*> attributes; // attributes[attributes_count]

    // Remember location of known attributes.
    SyntheticAttribute* attr_synthetic;
    DeprecatedAttribute* attr_deprecated;
    SignatureAttribute* attr_signature;
    ConstantValueAttribute* attr_constantvalue;
    AnnotationsAttribute* attr_visible_annotations;
    AnnotationsAttribute* attr_invisible_annotations;

public:
    FieldInfo()
        :name_index(0)
        , descriptor_index(0)
        , attr_synthetic(NULL)
        , attr_deprecated(NULL)
        , attr_signature(NULL)
        , attr_constantvalue(NULL)
        , attr_visible_annotations(NULL)
        , attr_invisible_annotations(NULL)
    {}
    FieldInfo(ClassFile&);
    ~FieldInfo()
    {
        for (unsigned i = 0; i < attributes.Length(); i++)
            delete attributes[i];
    }

    inline void SetNameIndex(u2 _name_index) { name_index = _name_index; }
    const char* Name(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[name_index] -> Tag() == CPInfo::CONSTANT_Utf8);
        return ((const CPUtf8Info*) constant_pool[name_index]) -> Bytes();
    }
    u2 NameLength(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[name_index] -> Tag() == CPInfo::CONSTANT_Utf8);
        return ((const CPUtf8Info*) constant_pool[name_index]) -> Length();
    }

    inline void SetDescriptorIndex(u2 index) { descriptor_index = index; }
    const char* Signature(const ConstantPool&, const Option*) const;
    u2 SignatureLength(const ConstantPool&, const Option*) const;

    inline u2 AttributesCount() const { return attributes.Length(); }
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
        case AttributeInfo::ATTRIBUTE_ConstantValue:
            assert(! attr_constantvalue);
            attr_constantvalue = (ConstantValueAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
            assert(! attr_visible_annotations);
            attr_visible_annotations = (AnnotationsAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
            assert(! attr_invisible_annotations);
            attr_invisible_annotations = (AnnotationsAttribute*) attribute;
            break;
        default:
            assert(false && "adding unexpected field attribute");
        }
    }
    bool Synthetic() const { return attr_synthetic || ACC_SYNTHETIC(); }
    bool Deprecated() const { return attr_deprecated != NULL; }
    const CPInfo* ConstantValue(const ConstantPool& constant_pool) const
    {
        return attr_constantvalue ? attr_constantvalue -> Value(constant_pool)
            : (const CPInfo*) NULL;
    }
    

    inline void Put(OutputBuffer& out) const
    {
        out.PutU2(access_flags);
        out.PutU2(name_index);
        out.PutU2(descriptor_index);
        out.PutU2(attributes.Length());
        for (unsigned i = 0; i < attributes.Length(); i++)
            attributes[i] -> Put(out);
    }

#ifdef JIKES_DEBUG
    void Print(Ostream& os, const ConstantPool& constant_pool) const;
#endif // JIKES_DEBUG
};

	

} // Close namespace Jikes block


#endif // _INCLUDED

