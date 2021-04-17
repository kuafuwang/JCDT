#include <JCDT_Lib/internal/classfmt/MethodInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	MethodInfo::MethodInfo(ClassFile& buffer)
		: AccessFlags(buffer.GetU2())
		, name_index(buffer.GetU2())
		, descriptor_index(buffer.GetU2())
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
	{
		unsigned count = buffer.GetU2();
		while (count--)
		{
			AttributeInfo* attr = AttributeInfo::AllocateAttributeInfo(buffer);
			attributes.Next() = attr;
			switch (attr->Tag())
			{
			case AttributeInfo::ATTRIBUTE_Synthetic:
				if (attr_synthetic)
					buffer.MarkInvalid("duplicate synthetic attribute");
				attr_synthetic = (SyntheticAttribute*)attr;
				SetACC_SYNTHETIC();
				break;
			case AttributeInfo::ATTRIBUTE_Deprecated:
				if (attr_deprecated)
					buffer.MarkInvalid("duplicate deprecated attribute");
				attr_deprecated = (DeprecatedAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Code:
				if (attr_code)
					buffer.MarkInvalid("duplicate code attribute");
				if (ACC_NATIVE() || ACC_ABSTRACT())
					buffer.MarkInvalid("code for native or abstract method");
				attr_code = (CodeAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Signature:
				if (attr_signature)
					buffer.MarkInvalid("duplicate signature attribute");
				attr_signature = (SignatureAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Bridge:
				if (attr_bridge)
					buffer.MarkInvalid("duplicate bridge attribute");
				attr_bridge = (BridgeAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Exceptions:
				if (attr_exceptions)
					buffer.MarkInvalid("duplicate exceptions attribute");
				attr_exceptions = (ExceptionsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
				if (attr_visible_annotations)
					buffer.MarkInvalid("duplicate visible attribute");
				attr_visible_annotations = (AnnotationsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
				if (attr_invisible_annotations)
					buffer.MarkInvalid("duplicate invisible attribute");
				attr_invisible_annotations = (AnnotationsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_RuntimeVisibleParameterAnnotations:
				if (attr_param_visible_annotations)
					buffer.MarkInvalid("duplicate param visible attribute");
				attr_param_visible_annotations =
					(ParameterAnnotationsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_RuntimeInvisibleParameterAnnotations:
				if (attr_param_invisible_annotations)
					buffer.MarkInvalid("duplicate param invisible attribute");
				attr_param_invisible_annotations =
					(ParameterAnnotationsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_AnnotationDefault:
				if (attr_annotation_default)
					buffer.MarkInvalid("duplicate annotation default attribute");
				if (!ACC_ABSTRACT() || !ACC_PUBLIC())
					buffer.MarkInvalid("annotation default on non-abstract or non-public method");
				attr_annotation_default = (AnnotationDefaultAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Generic:
				// ignore
				break;
			default:
				// invalid method attribute
				buffer.MarkInvalid("invalid method attribute");
			}
		}
		if (!ACC_NATIVE() && !ACC_ABSTRACT() && !attr_code)
			buffer.MarkInvalid("no code for non-native, non-abstract method");
	}

	const char* MethodInfo::Signature(const ConstantPool& pool,
		const Option* /*option*/) const
	{
		assert(pool[descriptor_index]->Tag() == CPInfo::CONSTANT_Utf8);
		const CPUtf8Info* sig =
			/*(option->source >= JikesOption::SDK1_5 && attr_signature)
			? attr_signature -> Signature(pool)
			:*/ (const CPUtf8Info*)pool[descriptor_index];
		return sig->Bytes();
	}

	u2 MethodInfo::SignatureLength(const ConstantPool& pool,
		const Option* /*option*/) const
	{
		assert(pool[descriptor_index]->Tag() == CPInfo::CONSTANT_Utf8);
		const CPUtf8Info* sig =
			/*(option->source >= JikesOption::SDK1_5 && attr_signature)
			? attr_signature -> Signature(pool)
			:*/ (const CPUtf8Info*)pool[descriptor_index];
		return sig->Length();
	}

	void MethodInfo::Print(Ostream& os, const ConstantPool& constant_pool) const
	{
		os << "method: ";
		AccessFlags::Print(os,ACCESS_METHOD);
		os << " name: " << (unsigned)name_index;
		if (constant_pool[name_index]->Tag() == CPInfo::CONSTANT_Utf8)
		{
			os << '=';
			constant_pool[name_index]->Describe(os, constant_pool);
		}
		else os << "(invalid)";
		os << endl << " type: " << (unsigned)descriptor_index;
		if (constant_pool[descriptor_index]->Tag() == CPInfo::CONSTANT_Utf8)
		{
			os << '=';
			constant_pool[descriptor_index]->Describe(os, constant_pool);
		}
		else os << "(invalid)";
		os << ", attributes: " << (unsigned)attributes.Length() << endl;
		for (unsigned i = 0; i < attributes.Length(); i++)
			attributes[i]->Print(os, constant_pool);
		os << endl;
	}

	MethodInfo::~MethodInfo()
	{
		for (unsigned i = 0; i < attributes.Length(); i++)
			delete attributes[i];
	}

	
} // Close namespace Jikes block



