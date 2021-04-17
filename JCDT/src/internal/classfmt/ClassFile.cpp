
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>

namespace Jikes { // Open namespace Jikes block


	ClassFile::ClassFile(): problem(NULL)
	                        , buffer(NULL)
	                        , buffer_tail(NULL)
	                        , magic(MAGIC)
	                        , minor_version(0), major_version(0), constant_pool(8, 4)
	                        , this_class(0), super_class(0), interfaces(6, 16)
	                        , fields(6, 16)
	                        , methods(6, 16)
	                        , attributes(6, 16)
	                        , attr_synthetic(NULL)
	                        , attr_deprecated(NULL)
	                        , attr_signature(NULL)
	                        , attr_sourcefile(NULL)
	                        , attr_innerclasses(NULL)
	                        , attr_visible_annotations(NULL)
	                        , attr_invisible_annotations(NULL)
	                        , attr_enclosing_method(NULL), m_for_compiler(true)
	{
	}

	ClassFile::ClassFile(const char* buf, unsigned buf_size, bool for_compiler)
		: problem(NULL)
		, buffer(buf)
		, buffer_tail(buf + buf_size)
		, magic(GetU4())
		, minor_version(GetU2())
		, major_version(GetU2())
		, constant_pool(8, 4)
		, interfaces(6, 16)
		, fields(6, 16)
		, methods(6, 16)
		, attributes(6, 16)
		, attr_synthetic(NULL)
		, attr_deprecated(NULL)
		, attr_signature(NULL)
		, attr_sourcefile(NULL)
		, attr_innerclasses(NULL)
		, attr_visible_annotations(NULL)
		, attr_invisible_annotations(NULL)
		, attr_enclosing_method(NULL), m_for_compiler(for_compiler)
	{
		if (magic != MAGIC || major_version < 45)
			MarkInvalid("unknown class format");
		u2 count = GetU2();
		if (!count)
			MarkInvalid("empty constant pool");
		else
		{
			while (--count) // skip entry 0
			{
				CPInfo* entry = CPInfo::AllocateCPInfo(*this);
				if (entry->Large())
					count--; // skip next entry for eight-byte constants
				constant_pool.SetNext(entry);
			}
		}
		if (!constant_pool.Check())
			MarkInvalid("invalid constant pool");
		access_flags = GetU2();
		if (!LegalAccess())
			MarkInvalid("illegal access");
		this_class = GetU2();
		if (constant_pool[this_class]->Tag() != CPInfo::CONSTANT_Class)
			MarkInvalid("illegal this class");
		super_class = GetU2();
		if (super_class &&
			constant_pool[super_class]->Tag() != CPInfo::CONSTANT_Class)
		{
			MarkInvalid("illegal super class");
		}
		count = GetU2();
		while (count--)
		{
			u2 inter = GetU2();
			if (constant_pool[inter]->Tag() != CPInfo::CONSTANT_Class)
				MarkInvalid("illegal interface");
			interfaces.Next() = inter;
		}

		count = GetU2();
		while (count--)
			fields.Next() = new FieldInfo(*this);
		count = GetU2();
		while (count--)
			methods.Next() = new MethodInfo(*this);
		count = GetU2();
		while (count--)
		{
			AttributeInfo* attr = AttributeInfo::AllocateAttributeInfo(*this);
			attributes.Next() = attr;
			switch (attr->Tag())
			{
			case AttributeInfo::ATTRIBUTE_Synthetic:
				if (attr_synthetic)
					MarkInvalid("duplicate synthetic attribute");
				attr_synthetic = (SyntheticAttribute*)attr;
				SetACC_SYNTHETIC();
				break;
			case AttributeInfo::ATTRIBUTE_Deprecated:
				if (attr_deprecated)
					MarkInvalid("duplicate deprecated attribute");
				attr_deprecated = (DeprecatedAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Signature:
				if (attr_signature)
					MarkInvalid("duplicate signature attribute");
				attr_signature = (SignatureAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_SourceFile:
				if (attr_sourcefile)
					MarkInvalid("duplicate source file attribute");
				attr_sourcefile = (SourceFileAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_InnerClasses:
				if (attr_innerclasses)
					MarkInvalid("duplicate inner classes attribute");
				attr_innerclasses = (InnerClassesAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_RuntimeVisibleAnnotations:
				if (attr_visible_annotations)
					MarkInvalid("duplicate visible annotations attribute");
				attr_visible_annotations = (AnnotationsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_RuntimeInvisibleAnnotations:
				if (attr_invisible_annotations)
					MarkInvalid("duplicate invisible annotations attribute");
				attr_invisible_annotations = (AnnotationsAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_EnclosingMethod:
				if (attr_enclosing_method)
					MarkInvalid("duplicate enclosing method attribute");
				attr_enclosing_method = (EnclosingMethodAttribute*)attr;
				break;
			case AttributeInfo::ATTRIBUTE_Generic:
				// ignore
				break;
			default:
				MarkInvalid("invalid method attribute");
			}
		}
	}

	void ClassFile::Write(Ostream& os, TypeSymbol* unit_type) const
	{
		Semantic* sem = unit_type->semantic_environment->sem;
		LookupEnvironment& control = sem->lookup_env;
		OutputBuffer output_buffer;

		const char* class_file_name = unit_type->ClassName();
		if (control.option->verbose)
			os << "[write " << class_file_name << "]" << endl;
		assert(Valid());
		if (control.option->nowrite)
			return;

		unsigned i;
		output_buffer.PutU4(MAGIC);
		output_buffer.PutU2(minor_version);
		output_buffer.PutU2(major_version);

		output_buffer.PutU2(constant_pool.Length());
		assert(constant_pool.Check());
		for (i = 1; i < constant_pool.Length(); i++)
		{
			constant_pool[i]->Put(output_buffer);
			if (constant_pool[i]->Large())
				i++; // skip the next entry for eight-byte constants
		}

		output_buffer.PutU2(access_flags);
		output_buffer.PutU2(this_class);
		output_buffer.PutU2(super_class);
		output_buffer.PutU2(interfaces.Length());
		for (i = 0; i < interfaces.Length(); i++)
			output_buffer.PutU2(interfaces[i]);
		output_buffer.PutU2(fields.Length());
		for (i = 0; i < fields.Length(); i++)
			fields[i]->Put(output_buffer);
		output_buffer.PutU2(methods.Length());
		for (i = 0; i < methods.Length(); i++)
			methods[i]->Put(output_buffer);
		output_buffer.PutU2(attributes.Length());
		for (i = 0; i < attributes.Length(); i++)
			attributes[i]->Put(output_buffer);

		// Now output to file
		if (!output_buffer.WriteToFile(class_file_name))
		{
			int length = strlen(class_file_name);
			wchar_t* name = new wchar_t[length + 1];
			for (int j = 0; j < length; j++)
				name[j] = class_file_name[j];
			name[length] = U_NULL;

			sem->ReportSemErrorWithAst(SemanticError::CANNOT_WRITE_FILE,
				unit_type->declaration, name);
			delete[] name;
		}
	}

	void ClassFile::Print(Ostream& os)
	{
		unsigned i;
		if (problem)
			os << " *** Not a valid Java .class file (" << problem
				<< ")! ***" << endl;

		os << "*** Magic number: 0x";
		for (i = 32; i; i -= 4)
		{
			char c = (magic >> (i - 4)) & 0xf;
			os << (char)(c < 10 ? c + '0' : c + 'a' - 10);
		}
		os << ", version: " << major_version << '.' << minor_version
			<< endl << endl << "### Constant pool: constant_pool_count: "
			<< (unsigned)constant_pool.Length() << endl;
		for (i = 1; i < constant_pool.Length(); i++)
		{
			if (! constant_pool.Valid(i))
				continue;
			os.width(5);
			os << i << ": ";
			constant_pool[i]->Print(os, constant_pool);
		}
		os << endl << "### ";
		AccessFlags::Print(os,ACCESS_TYPE);
		os << "this_class: " << (unsigned)this_class;
		if (constant_pool[this_class]->Tag() == CPInfo::CONSTANT_Class)
		{
			os << '=';
			constant_pool[this_class]->Describe(os, constant_pool);
		}
		else os << "(invalid)";
		os << endl << "super_class: " << (unsigned)super_class;
		if (! super_class)
			os << "(none)";
		else if (constant_pool[super_class]->Tag() == CPInfo::CONSTANT_Class)
		{
			os << '=';
			constant_pool[super_class]->Describe(os, constant_pool);
		}
		else os << "(invalid)";
		os << endl << "interfaces_count: "
			<< (unsigned)interfaces.Length() << endl;
		for (i = 0; i < interfaces.Length(); i++)
		{
			os << "  " << (unsigned)interfaces[i];
			if (constant_pool[interfaces[i]]->Tag() ==
				CPInfo::CONSTANT_Class)
			{
				os << '=';
				constant_pool[interfaces[i]]->Describe(os, constant_pool);
			}
			else os << "(invalid)";
			os << endl;
		}
		os << endl << "### Fields: fields_count: "
			<< (unsigned)fields.Length() << endl;
		for (i = 0; i < fields.Length(); i++)
			fields[i]->Print(os, constant_pool);
		os << endl << "### Methods: methods_count: "
			<< (unsigned)methods.Length() << endl;
		for (i = 0; i < methods.Length(); i++)
			methods[i]->Print(os, constant_pool);
		os << endl << "### Attributes: attributes_count: "
			<< (unsigned)attributes.Length() << endl;
		for (i = 0; i < attributes.Length(); i++)
			attributes[i]->Print(os, constant_pool);
	}
} // Close namespace Jikes block



