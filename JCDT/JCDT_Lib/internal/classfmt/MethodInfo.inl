
#ifndef jcdt_class_MethodInfo_INCLUDED
#define jcdt_class_MethodInfo_INCLUDED
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>
#include "CodeAttribute.inl"
#include "BridgeAttribute.inl"
#include "SyntheticAttribute.inl"
#include "DeprecatedAttribute.inl"
#include "CPUtf8Info.inl"
#include "AnnotationDefaultAttribute.inl"
#include "SignatureAttribute.inl"
#include "AnnotationsAttribute.inl"
#include "ParameterAnnotationsAttribute.inl"



namespace Jikes {
	class ClassFile;
	class ExceptionsAttribute;
	// Open namespace Jikes block

//
// Describes a member method, including constructors and the static
// initializer. JVMS 4.6.
//
class MethodInfo : public AccessFlags
{
public:
    // u2 access_flags; // inherited from AccessFlags
    u2 name_index;
    u2 descriptor_index;
    // u2 attributes_count; // computed as attributes.Length();
    Tuple<AttributeInfo*> attributes; // attributes[attributes_count]

    // Remember location of known attributes.
    SyntheticAttribute* attr_synthetic;
    DeprecatedAttribute* attr_deprecated;
    SignatureAttribute* attr_signature;
    BridgeAttribute* attr_bridge;
    CodeAttribute* attr_code;
    ExceptionsAttribute* attr_exceptions;
    AnnotationsAttribute* attr_visible_annotations;
    AnnotationsAttribute* attr_invisible_annotations;
    ParameterAnnotationsAttribute* attr_param_visible_annotations;
    ParameterAnnotationsAttribute* attr_param_invisible_annotations;
    AnnotationDefaultAttribute* attr_annotation_default;


    MethodInfo()
        : 
		 name_index(0)
        , descriptor_index(0)
        , attr_synthetic(NULL)
        , attr_deprecated(NULL)
        , attr_signature(NULL)
        , attr_bridge(NULL)
        , attr_code(NULL)
        , attr_exceptions(NULL)
        , attr_visible_annotations(NULL)
        , attr_invisible_annotations(NULL)
        , attr_param_visible_annotations(NULL)
        , attr_param_invisible_annotations(NULL)
        , attr_annotation_default(NULL)
    {}
    MethodInfo(ClassFile&);
    ~MethodInfo();

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
        case AttributeInfo::ATTRIBUTE_Bridge:
            assert(! attr_bridge);
            attr_bridge = (BridgeAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_Code:
            assert(! attr_code);
            attr_code = (CodeAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_Exceptions:
            assert(! attr_exceptions);
            attr_exceptions = (ExceptionsAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
            assert(! attr_visible_annotations);
            attr_visible_annotations = (AnnotationsAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
            assert(! attr_invisible_annotations);
            attr_invisible_annotations = (AnnotationsAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeVisibleParameterAnnotations:
            assert(! attr_param_visible_annotations);
            attr_param_visible_annotations =
                (ParameterAnnotationsAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_RuntimeInvisibleParameterAnnotations:
            assert(! attr_param_invisible_annotations);
            attr_param_invisible_annotations =
                (ParameterAnnotationsAttribute*) attribute;
            break;
        case AttributeInfo::ATTRIBUTE_AnnotationDefault:
            assert(! attr_annotation_default);
            attr_annotation_default = (AnnotationDefaultAttribute*) attribute;
            break;
        default:
            assert(false && "adding unexpected method attribute");
        }
    }
    bool Synthetic() const { return attr_synthetic || ACC_SYNTHETIC(); }
    bool Deprecated() const { return attr_deprecated != NULL; }
    bool Bridge() const { return attr_bridge != NULL; }
    const CodeAttribute* Code() const { return attr_code; }
    const ExceptionsAttribute* Exceptions() const { return attr_exceptions; }

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

