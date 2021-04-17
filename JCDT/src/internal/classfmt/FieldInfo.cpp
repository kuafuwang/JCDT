#include <JCDT_Lib/internal/classfmt/FieldInfo.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	FieldInfo::FieldInfo(ClassFile& buffer)
		: AccessFlags(buffer.GetU2())
		, name_index(buffer.GetU2())
		, descriptor_index(buffer.GetU2())
		, attr_synthetic(NULL)
		, attr_deprecated(NULL)
		, attr_signature(NULL)
		, attr_constantvalue(NULL)
		, attr_visible_annotations(NULL)
		, attr_invisible_annotations(NULL)
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
			case AttributeInfo::ATTRIBUTE_ConstantValue:
				if (attr_constantvalue)
					buffer.MarkInvalid("duplicate ConstantValue attribute");
				if (!ACC_FINAL())
					buffer.MarkInvalid("ConstantValue attribute without final");
				attr_constantvalue = (ConstantValueAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Signature:
				if (attr_signature)
					buffer.MarkInvalid("duplicate signature attribute");
				attr_signature = (SignatureAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
				if (attr_visible_annotations)
					buffer.MarkInvalid("duplicate signature attribute");
				attr_visible_annotations = (AnnotationsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
				if (attr_invisible_annotations)
					buffer.MarkInvalid("duplicate invisible attribute");
				attr_invisible_annotations = (AnnotationsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Generic:
				// ignore
				break;
			default:
				// invalid field attribute
				buffer.MarkInvalid("invalid field attribute");
			}
		}
	}

	const char* FieldInfo::Signature(const ConstantPool& pool,
		const Option* /*option*/) const
	{
		assert(pool[descriptor_index]->Tag() == CPInfo::CONSTANT_Utf8);
		const CPUtf8Info* sig =
			/*(option->source >= JikesOption::SDK1_5 && attr_signature)
			? attr_signature -> Signature(pool)
			:*/ (const CPUtf8Info*)pool[descriptor_index];
		return sig->Bytes();
	}

	u2 FieldInfo::SignatureLength(const ConstantPool& pool,
		const Option* /*option*/) const
	{
		assert(pool[descriptor_index]->Tag() == CPInfo::CONSTANT_Utf8);
		const CPUtf8Info* sig =
			/*(option->source >= JikesOption::SDK1_5 && attr_signature)
			? attr_signature -> Signature(pool)
			:*/ (const CPUtf8Info*)pool[descriptor_index];
		return sig->Length();
	}

	void FieldInfo::Print(Ostream& os, const ConstantPool& constant_pool) const
	{
		os << "field: ";
		AccessFlags::Print(os, ACCESS_VARIABLE);
		os << ", name: " << (unsigned)name_index;
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
} // Close namespace Jikes block



