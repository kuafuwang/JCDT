#include "stdafx.h"
#include <JavaModelLib/internal/core/BinaryType.h>
#include <JavaModelLib/internal/core/PackageFragment.h>
#include "JCDT_Lib/internal/util/zip/zip.h"
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/classfmt/class.h>
#include <JavaModelLib/internal/core/BinaryField.h>
#include <JavaModelLib/internal/core/BinaryMethod.h>
#include <JavaModelLib/internal/core/ClassFile.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include  <boost/algorithm/string.hpp>
#include <JavaModelLib/internal/lookup/SourceMapper.h>
#include <JCDT_Lib/internal/codegen/Signature.h>
#include <JavaModelLib/internal/info/SourceTypeElementInfo.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	

	void ProcessNestedTypeSignatures(BinaryType* _type, vector<char*>& nested_type_signatures);

	// Searches for a nested type with the simple name name_symbol located within
	// base_type, when base_type was loaded from a .class file.
	//
	
	BinaryType* ProcessNestedType(BinaryType* contain_type,
		const wstring&  inner_type_name)
	{

		BinaryType* inner_type = new BinaryType(contain_type,inner_type_name);
		auto type_info = new SourceTypeElementInfo();
		inner_type->info = type_info;
		
		type_info->InsertNestedTypeSymbol(inner_type);


		auto contain_type_info = contain_type->getElementInfo();
		if(contain_type_info)
		{
			type_info->qualifiedTypeNames.assign(contain_type_info->qualifiedTypeNames.begin(),
				contain_type_info->qualifiedTypeNames.end());
			type_info->qualifiedTypeNames.emplace_back(contain_type_info->name);
		}


		type_info->name = (inner_type_name);


		auto fullyQualiifiedName = contain_type->getFullyQualifiedName('$');
		fullyQualiifiedName += (StringConstant::US_DS);
		fullyQualiifiedName.append(inner_type_name);
		auto pkg = contain_type->getPackageFragment();
		if(pkg)
		{
			auto* file_symbol = pkg->getClassFile(fullyQualiifiedName);
			if (file_symbol) {
				inner_type->file_symbol = reinterpret_cast<ClassFile*>(file_symbol);
			}
		}

		return inner_type;
	}

	 void CopyNestedTypeSignature(const char* signature_, unsigned length, vector<char*>&  nested_type_signatures)
	{
		char* signature = new char[length + 1];
		strncpy(signature, signature_, length);
		signature[length] = U_NULL;
		nested_type_signatures.push_back(signature);
	}
	 void CopyThrowsSignature(const char* signature_, unsigned length, Tuple<char*>& throws_signatures)
	{
		 	char* signature = new char[length + 1];
		 	strncpy(signature, signature_, length);
		 	signature[length] = U_NULL;
		 	throws_signatures.Next() = signature;
	 }

	 void ProcessNestedTypeSignatures(BinaryType* _type, vector<char*>& nested_type_signatures)
	 {
		 for (unsigned i = 0, size = nested_type_signatures.size(); i < size; i++)
		 {
			 auto sig = nested_type_signatures[i];
			 string temp(sig, strlen(sig));
			 delete[] sig;
			 ProcessNestedType(_type, CharOperation::ConvertUtf8ToUnicode(temp));
		 }
	 }


	// Attempts to read in a type from a buffer representing a .class file.
	 void BinaryType::ProcessClassFile(BinaryType* type, SourceTypeElementInfo* type_info , Jikes::ClassFile* class_data)
	{
		if (class_data &&!class_data->Valid()){
			return;
		}
		if(!type_info || !type)
		{
			return;
		}
		// See if we read the expected type.
		const ConstantPool& pool = class_data->Pool();

		CPClassInfo* class_info = class_data->ThisClass();
		/*auto name_ = class_info->TypeName(pool);
		if (strcmp(type->fully_qualified_name.c_str(), name_))
		{
			delete class_data;
			return;
		}*/

		// We now have enough information to make a good estimate for the
		// size of the symbol table we need for this class.
		int i;
		const InnerClassesAttribute* inner_classes = class_data->InnerClasses();

		// Read the attributes. We do this now to learn about inner classes and
		// type parameterization, since this information may be needed when
		// processing supertypes, field signatures, and method signatures. Also,
		// we learn the access flags of this type.
	
		type_info->access.SetFlags(class_data->Flags());
		type_info->access.ResetACC_SUPER();
		if (class_data->Deprecated())
			type->MarkDeprecated();

		if (class_data->ACC_INTERFACE())
			type->MarkInterface();
		else 
			type->MarkClass();

		if (class_data->ACC_ENUM())
			type->MarkEnum();

	

		vector<char*>  nested_type_signatures;
		if (inner_classes)
		{
			for (i = inner_classes->InnerClassesLength() - 1; i >= 0; i--)
			{
				const CPClassInfo* inner = inner_classes->Inner(i, pool);
				const CPClassInfo* outer = inner_classes->Outer(i, pool);
				if (inner == class_info)
				{
					type_info->access.SetFlags(inner_classes->Flags(i));
					if (!outer)
					{
						// This type is local or anonymous, so we shouldn't be
						// reading it from the .class file. Marking it anonymous
						// is our clue to report an error for trying to use the
						// type by qualified name.
						type->MarkAnonymous();
					}
				}
				else if (outer == class_info && inner_classes->Name(i, pool) &&
					inner_classes->NameLength(i, pool))
				{
					// Some idiot compilers give anonymous classes the name "" and
					// an outer class, rather than obeying JVMS 4.7.5.  For
					// example, mail.jar in Sun's javamail 1.3.1 includes
					// javax.mail.Service with this property. The check for
					// NameLength above skips those invalid entries.
					CopyNestedTypeSignature( inner_classes->Name(i, pool),inner_classes->NameLength(i, pool),
						nested_type_signatures);
				}
			}
		}

		
		ProcessNestedTypeSignatures(type, nested_type_signatures);

		vector<wstring> qualifiedTypeNames(type_info->qualifiedTypeNames);
		qualifiedTypeNames.emplace_back(type_info->name);
		// Read the fields.
		for (i = class_data->FieldsCount() - 1; i >= 0; i--)
		{
			const FieldInfo* field = class_data->Field(i);
			if (field->ACC_SYNTHETIC())
				continue; // No point reading these - the user can't access them.

			string temp(field->Name(pool), field->Name(pool) + field->NameLength(pool));
			wstring field_name = CharOperation::ConvertUtf8ToUnicode(temp);



			BinaryField* symbol = new BinaryField(type, field_name,FIELD);
			auto field_element_info = reinterpret_cast<SourceFieldElementInfo*>(symbol->getElementInfo());
			field_element_info->qualifiedTypeNames.assign(qualifiedTypeNames.begin(), qualifiedTypeNames.end());
			field_element_info->name = (field_name);


			field_element_info->access.SetFlags(field->Flags());
			string sig(field->Signature(pool, nullptr),
				field->SignatureLength(pool, nullptr));

			int idx = 0;
			field_element_info->_type = Signature::parseType(sig, idx);

			const CPInfo* value = field->ConstantValue(pool);
			if (value)
			{
				switch (value->Tag())
				{
				case CPInfo::CONSTANT_Integer:
				{
					auto initial_value = new IntLiteralValue();
					initial_value->Initialize(((CPIntegerInfo*)value)->Value(), 0);
					field_element_info->initial_value = initial_value;
				}
				break;
				case CPInfo::CONSTANT_Long:
				{
					auto long_value = new LongLiteralValue();
					long_value->Initialize(((CPLongInfo*)value)->Value(), 0);
					field_element_info->initial_value = long_value;
				}
				break;
				case CPInfo::CONSTANT_Float:
				{
					auto float_value = new FloatLiteralValue();
					float_value->Initialize(((CPFloatInfo*)value)->Value(), 0);
					field_element_info->initial_value = float_value;

				}
				break;
				case CPInfo::CONSTANT_Double: {
					auto double_value = new DoubleLiteralValue();
					double_value->Initialize(((CPDoubleInfo*)value)->Value(), 0);
					field_element_info->initial_value = double_value;

				}
				break;
				case CPInfo::CONSTANT_String:
				{


					CPStringInfo* str_value = (CPStringInfo*)value;

					auto string_value = new Utf8LiteralValue();
					string_value->Initialize(str_value->Bytes(pool),
						str_value->Length(pool), 0, 0);
					field_element_info->initial_value = string_value;
				}
				break;
				default:
					assert(false && "unexpected constant pool entry");
				}
			}
			
			type_info->InsertField(symbol);
		}

		// Read the methods.
		for (i = class_data->MethodsCount() - 1; i >= 0; i--)
		{
			const MethodInfo* method = class_data->Method(i);

			if (!method)
				continue;

			string temp(method->Name(pool), method->Name(pool) + method->NameLength(pool));
			wstring method_name = CharOperation::ConvertUtf8ToUnicode(temp);

			if (method->ACC_SYNTHETIC()){
				continue;
			}
			else{
				if (method_name == L"<clinit>"){
					continue;
				}
				else if (method_name == L"<init>") {
					method_name = type->getElementName();
				}
			}
			const char* str = method->Signature(pool, nullptr);
			string sig(str,
				str + method->SignatureLength(pool, nullptr));

			auto  _ret = Signature::parseSignature(sig);

			BinaryMethod* symbol = new BinaryMethod(type, method_name, _ret.first);
			auto  method_info = reinterpret_cast<SourceMethodElementInfo*>(symbol->getElementInfo());
			method_info->access.SetFlags(method->Flags());

			method_info->return_type.swap(_ret.second);
			method_info->qualifiedTypeNames.assign(qualifiedTypeNames.begin(), qualifiedTypeNames.end());
			method_info->name = (method_name);

			
			/*const ExceptionsAttribute* throws_clause = method->Exceptions();
			if (throws_clause) {
				Tuple<char*> throws_signatures(8);
				for (int j = throws_clause->NumberOfExceptions() - 1; j >= 0; j--) {
					const CPClassInfo* exception =
						throws_clause->Exception(j, pool);
					CopyThrowsSignature(exception->TypeName(pool),
						exception->TypeNameLength(pool), throws_signatures);
				}
			}*/
			type_info->InsertMethodSymbol(symbol);
		}
	}

	// This is called when a type needs to be read from a .class file. It reads
	// the file and fills in the symbol table of type.

	void BinaryType::ReadClassFile(BinaryType* type, SourceTypeElementInfo* type_info)
	{
		if (!type)
			return;

		auto file_symbol = reinterpret_cast<ClassFile*>( type->file_symbol);
		if (!file_symbol) {
			return;
		}
		if (file_symbol->IsZip()){
			// zip包中的class文件
			auto _class_file = file_symbol->getClassFileInJar();
			if(_class_file)
				ProcessClassFile(type, type_info,_class_file);
			
		}
		else
		{
			// 不是zip包中的class文件

			// Get a ReadObject from the API that contains the file's data.

			FileReader* classFile =
				FileHelper::getInstance()->read(file_symbol->getPath());
			if (classFile != NULL)
			{
				// Process the file data.
				unsigned size = classFile->getBufferSize();

#if defined(WIN32_FILE_SYSTEM)
				size = ((0xFFFFFFFF && GetLastError()) != NO_ERROR) ? 0 : size;
#endif
				Jikes::ClassFile* class_data = new Jikes::ClassFile(classFile->getBuffer(), size);
				ProcessClassFile(type, type_info, class_data);
				delete class_data;
				delete classFile;
			}
		}
	}

	MemberElementInfo* BinaryType::getElementInfo(){
		if (!info){
			info = new  SourceTypeElementInfo();
			
		}
		if(!info->fIsStructureKnown){
			info->fIsStructureKnown = true;
			BinaryType::ReadClassFile(this, info);
		}
			
		return info;
	}

	//IMethod* BinaryType::createIndepentMethod(const wstring& name, const vector<wstring>& parameterTypes)
	//{
	//		auto temp = new IndependentBinaryMethod(this, name, parameterTypes);
	//		temp->getMosesPath();
	//		return temp;
	//}

	//IField* BinaryType::creatIndependtField(const wstring& name)
	//{
	//	
	//	auto temp = new  IndependentBinaryField(this, name, FIELD);
	//	temp->getMosesPath();
	//	return temp;
	//}

	//IType* BinaryType::createIndependentType(const wstring& name)
	//{
	//	
	//	auto temp = new  IndependentBinaryType(this, name);
	//	temp->getMosesPath();
	//	return temp;
	//}

	bool BinaryType::isClass()
	{
		auto _info = getElementInfo();
		return BaseJavaType::isClass();
	}

	void BinaryType::figureRange()
	{
		auto _info = getElementInfo();
		if (!_info)
			return;

		auto _rng = _info->getSourceRange();
		if (!_rng)
			return;

		if(_rng->m_nStartLine == ISourceRange::UNKNOWN_FLAGE){
			SourceMapper* mapper = new SourceMapper();
			mapper->MapperSource(this);
			delete mapper;
		}
	}

	IJavaElement* BinaryType::ShallowCopy(IJavaElement* parent)
	{

		auto clone = new BinaryType(*this);

		clone->fParent = parent;
		auto _class_file = dynamic_cast<ClassFile*>(parent);
		if(_class_file){
		
			clone->file_symbol = _class_file;
		}
		else
		{
			auto _type = dynamic_cast<BinaryType*>(parent);	
			assert(_type);
		}
		return clone;
	}



	BinaryType::BinaryType(IJavaElement* parent, const wstring& name)
		: BaseJavaType(parent, name)
	{

	}

	BinaryType::BinaryType(BinaryType&o)
		: BaseJavaType(o)
	{
		HelpToDeepCopy(o);
	}

	BinaryType::~BinaryType()
	{
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




