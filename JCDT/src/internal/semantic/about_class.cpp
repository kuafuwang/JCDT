#include "JCDT_Lib/internal/classfmt/class.h"
#include "JCDT_Lib/internal/codegen/code.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/impl/option.h"

#include  "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/internal/util/zip/zip.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"




namespace Jikes { // Open namespace Jikes block




	//
	// This processes a descriptor, and returns the associated type, or else
	// control.no_type if the descriptor is bad. Signature is assumed to be null
	// terminated, and this adjusts the pointer to the spot just after the type
	// parsed.
	//
	TypeSymbol* Semantic::ProcessSignature(TypeSymbol* base_type,
		const char*& signature, Token* tok)
	{
		TypeSymbol* type;
		int num_dimensions = 0;
		for (; *signature == U_LEFT_BRACKET; signature++)
			num_dimensions++;
		switch (*signature++)
		{
		case U_B:
			type = lookup_env.byte_type;
			break;
		case U_C:
			type = lookup_env.char_type;
			break;
		case U_D:
			type = lookup_env.double_type;
			break;
		case U_F:
			type = lookup_env.float_type;
			break;
		case U_I:
			type = lookup_env.int_type;
			break;
		case U_J:
			type = lookup_env.long_type;
			break;
		case U_S:
			type = lookup_env.short_type;
			break;
		case U_Z:
			type = lookup_env.boolean_type;
			break;
		case U_V:
			if (num_dimensions)
				return lookup_env.no_type;
			type = lookup_env.void_type;
			break;
		case U_L:
		{
			const char* str = signature;
			while (*str && *str != U_SEMICOLON)
				str++;
			if (!*str)
			{
				signature = str;
				return lookup_env.no_type;
			}
			type = ReadTypeFromSignature(base_type, signature,
				str - signature, tok);
			signature = str + 1;
		}
		break;
		case U_T:
			assert(false && "generics not implemented yet");
		case U_NULL: // oops, already exceeded string
			signature--;
			// fallthrough
		default:
			return lookup_env.no_type;
		}
		return type->GetArrayType(this, num_dimensions);
	}

	//
	// This returns the type associated with a CONSTANT_Class entry, or
	// control.no_type if there was an error. Unless the type is bad, it is
	// necessarily a subtype of java.lang.Object.
	//
	TypeSymbol* Semantic::GetType(TypeSymbol* base_type, CPClassInfo* class_info,
		const ConstantPool& constant_pool,
		Token* tok)
	{
		if (!class_info->Type())
		{
			const char* str = class_info->TypeName(constant_pool);
			if (*str == U_LEFT_BRACKET)
				class_info->SetType(ProcessSignature(base_type, str, tok));
			else
			{
				u2 length = class_info->TypeNameLength(constant_pool);
				class_info->SetType(ReadTypeFromSignature(base_type, str,
					length, tok));
			}
		}
		return class_info->Type();
	}

	//
	// Searches for a nested type with the simple name name_symbol located within
	// base_type, when base_type was loaded from a .class file.
	//
	TypeSymbol* Semantic::ProcessNestedType(TypeSymbol* base_type,
		NameSymbol* name_symbol,
		Token* tok)
	{
		TypeSymbol* inner_type = base_type->FindTypeSymbol(name_symbol);
		if (!inner_type)
		{
			int length = base_type->ExternalNameLength() + 1 +
				name_symbol->NameLength(); // +1 for $,... +1 for $
			wchar_t* external_name = new wchar_t[length + 1]; // +1 for '\0';
			wcscpy(external_name, base_type->ExternalName());
			wcscat(external_name, StringConstant::US_DS);
			wcscat(external_name, name_symbol->Name());
			NameSymbol* external_name_symbol =
				lookup_env.nameEnv->FindOrInsertName(external_name, length);

			delete[] external_name;

			inner_type = base_type->InsertNestedTypeSymbol(name_symbol);
			inner_type->outermost_type = base_type->outermost_type;
			inner_type->supertypes_closure = new SymbolSet;
			inner_type->SetExternalIdentity(external_name_symbol);
			inner_type->SetOwner(base_type);
			inner_type->SetSignature(lookup_env);

			FileSymbol* file_symbol =
				lookup_env.GetFile(lookup_env.option, base_type->ContainingPackage(),
					external_name_symbol);
			if (file_symbol)
			{
				inner_type->file_symbol = file_symbol;
				inner_type->SetLocation();

				ReadClassFile(inner_type, tok);
			}
			else
			{
				// this symbol table will only contain a default constructor
				inner_type->SetSymbolTable(1);
				inner_type->super = lookup_env.Object();
				inner_type->MarkBad();
				AddDefaultConstructor(inner_type);
				ReportSemErrorSimple(SemanticError::TYPE_NOT_FOUND, tok,
					inner_type->ContainingPackageName(),
					inner_type->ExternalName());
			}
		}

		return inner_type;
	}

	//
	// Retrieves the innermost nested type from a signature containing '$' as
	// the type nesting separator.
	//
	TypeSymbol* Semantic::RetrieveNestedTypes(TypeSymbol* base_type,
		wchar_t* signature, Token* tok)
	{
		int len;
		for (len = 0;
		signature[len] != U_NULL && signature[len] != U_DOLLAR; len++)
			;
		NameSymbol* name_symbol = lookup_env.nameEnv->FindOrInsertName(signature, len);
		TypeSymbol* inner_type = ProcessNestedType(base_type, name_symbol, tok);

		return (signature[len] == U_DOLLAR
			? RetrieveNestedTypes(inner_type, &signature[len + 1], tok)
			: inner_type);
	}

	//
	// Given a UTF8 signature, this finds the type that it refers to, loading
	// the type into memory or causing a type not found error.
	//
	TypeSymbol* Semantic::ReadTypeFromSignature(TypeSymbol* base_type,
		const char* utf8_signature,
		int length, Token* tok)
	{
		TypeSymbol* type = lookup_env.type_table.FindType(utf8_signature, length);

		if (type)
		{
			if (type->SourcePending())
				lookup_env.ProcessFileHeaders(type->file_symbol);
		}
		else
		{
			wchar_t* signature = new wchar_t[length + 1];
			CharOperation::ConvertUtf8ToUnicode(signature, utf8_signature, length);

			int total_length;
			for (total_length = 0;
			signature[total_length] != U_NULL &&
				signature[total_length] != U_DOLLAR; total_length++)
				;

			if (signature[total_length] != U_NULL &&
				Code::IsDecimalDigit(signature[total_length + 1]))
			{
				// an anonymous or a local type?
				for (total_length += 2;
				Code::IsDecimalDigit(signature[total_length]); total_length++)
					// will stop at next '$' or '\0' !!!
					;
				if (signature[total_length] != U_NULL)
				{
					// not an anonymous type? then scan local type name
					for (total_length++;
					signature[total_length] != U_NULL &&
						signature[total_length] != U_DOLLAR; total_length++)
						;
				}
			}

			int len;
			for (len = total_length - 1;
			len >= 0 && signature[len] != U_SLASH; len--)
				;

			wchar_t* name = &(signature[len + 1]);

			//
			// When a package name is specified in the signature, we look for the
			// type in question in that package, i.e., we redefine package.
			// Otherwise, we search for the type in the unnamed package.
			//
			PackageSymbol* package = NULL;

			//
			// Which package?
			//
			if (len >= 0)
			{
				wchar_t* package_name = new wchar_t[len + 1];
				for (int i = 0; i < len; i++)
					package_name[i] = signature[i];
				package_name[len] = U_NULL;
				package = lookup_env.ProcessPackage(package_name);

				if (package->directory.Length() == 0)
				{
					ReportSemErrorSimple(SemanticError::PACKAGE_NOT_FOUND, tok,
						package->PackageName());
				}
				delete[] package_name;
			}
			else package = lookup_env.UnnamedPackage();

			//
			// Process type
			//
			NameSymbol* name_symbol =
				lookup_env.nameEnv->FindOrInsertName(name, total_length - (len + 1));
			type = package->FindTypeSymbol(name_symbol);
			if (type)
			{
				if (type->SourcePending())
					lookup_env.ProcessFileHeaders(type->file_symbol);
			}
			else
			{
				FileSymbol* file_symbol = lookup_env.GetFile(lookup_env.option, package,
					name_symbol);
				//
				// If a file_symbol was not found, ReadType will issue an error
				// message
				//
				type = ReadType(file_symbol, package, name_symbol, tok);

				//
				// If we have to do a full check and we have a case where a
				// ".class" file depends on a ".java" file then we should signal
				// that the ".java" file associated with the ".class" file should
				// be recompiled.
				//
				if (lookup_env.option->full_check && !lookup_env.option->depend &&
					file_symbol && file_symbol->IsJava() &&
					!file_symbol->IsZip())
				{
					lookup_env.recompilation_file_set.AddElement(file_symbol);
					if (!lookup_env.option->incremental && lookup_env.option->pedantic)
					{
						ReportSemErrorSimple(SemanticError::RECOMPILATION, tok,
							base_type->ContainingPackageName(),
							base_type->ExternalName(),
							type->ContainingPackageName(),
							type->ExternalName());
					}
				}
			}

			if (signature[total_length] == U_DOLLAR)
				type = RetrieveNestedTypes(type, &signature[total_length + 1],
					tok);

			delete[] signature;
		}

		//
		// Establish a dependence from base_type (read from a class file) to type.
		//
		AddDependence(base_type, type);

		return type;
	}

	//
	// This is called when a type needs to be read from a .class file. It reads
	// the file and fills in the symbol table of type.
	//
	void Semantic::ReadClassFile(TypeSymbol* type, Token* tok)
	{
#ifdef JIKES_DEBUG
		lookup_env.class_files_read++;
#endif // JIKES_DEBUG

		FileSymbol* file_symbol = type->file_symbol;

		if (lookup_env.option->verbose) {
			Coutput << "[read "
				<< file_symbol->FileName()
				<< "]" << endl;
		}

		if (file_symbol->IsZip())
		{
			Zip *zip = file_symbol->Zipfile();
			u4 offset = file_symbol->offset;
			u4 uncompressed_size = file_symbol->uncompressed_size;

			ZipFile* zipfile = new ZipFile(zip, uncompressed_size,offset);

		
			if (zipfile->Buffer() == NULL)
			{
				// this symbol table will only contain a default constructor
				type->SetSymbolTable(1);
				if (type != lookup_env.Object())
					type->super = (type == lookup_env.Throwable()
						? lookup_env.Object() : lookup_env.Throwable());
				type->MarkBad();
				AddDefaultConstructor(type);

				ReportSemErrorSimple(SemanticError::COMPRESSED_ZIP_FILE, tok,
					file_symbol->PathSym()->Name(),
					type->ContainingPackageName(),
					type->ExternalName());
			}
			else ProcessClassFile(type, zipfile->Buffer(),
				file_symbol->uncompressed_size, tok);
			delete zipfile;
		}
		else
		{
			// Get a ReadObject from the API that contains the file's data.
			FileReader* classFile =
				FileHelper::getInstance()->read(file_symbol->FileName());
			if (classFile == NULL)
			{
				// this symbol table will only contain a default constructor
				type->SetSymbolTable(1);
				if (type != lookup_env.Object())
					type->super = (type == lookup_env.Throwable()
						? lookup_env.Object() : lookup_env.Throwable());
				type->MarkBad();
				AddDefaultConstructor(type);

				ReportSemErrorSimple(SemanticError::CANNOT_OPEN_CLASS_FILE, tok,
					type->ContainingPackageName(),
					type->ExternalName());
			}

			else
			{
				// Process the file data.
				unsigned size = classFile->getBufferSize();

#if defined(WIN32_FILE_SYSTEM)
				size = ((0xFFFFFFFF && GetLastError()) != NO_ERROR) ? 0 : size;
#endif
				ProcessClassFile(type, classFile->getBuffer(), size, tok);
				delete classFile;
			}
		}
	}

	//
	// Attempts to read in a type from a buffer representing a .class file.
	//
	void Semantic::ProcessClassFile(TypeSymbol* type, const char* buffer,
		unsigned buffer_size, Token* tok)
	{
		assert(!type->HeaderProcessed());
		ClassFile* class_data = new ClassFile(buffer, buffer_size);
		if (!class_data->Valid())
		{
			const char* problem = class_data->DescribeProblem();
			int length = strlen(problem);
			wchar_t* unicode_problem = new wchar_t[length + 1];
			CharOperation::ConvertUtf8ToUnicode(unicode_problem, problem, length);

			ReportSemErrorSimple(SemanticError::INVALID_CLASS_FILE, tok,
				type->ExternalName(),
				type->file_symbol->PathSym()->Name(),
				type->ContainingPackageName(),
				unicode_problem);
			type->MarkBad();
			delete class_data;
			delete unicode_problem;
			return;
		}

		//
		// See if we read the expected type.
		//
		const ConstantPool& pool = class_data->Pool();
		CPClassInfo* class_info = class_data->ThisClass();
		if (strcmp(type->fully_qualified_name->value,
			class_info->TypeName(pool)))
		{
			wchar_t* str = new wchar_t[class_info->TypeNameLength(pool) + 1];
			CharOperation::ConvertUtf8ToUnicode(str, class_info->TypeName(pool),
				class_info->TypeNameLength(pool));
			// Passing NULL as the filename allows the error format to use the
			// platform dependent directory delimiter.
			ReportSemErrorSimple(SemanticError::WRONG_TYPE_IN_CLASSFILE, tok,
				type->ExternalName(),
				type->file_symbol->PathSym()->Name(),
				NULL, str);
			type->MarkBad();
			delete[] str;
			delete class_data;
			return;
		}

		type->MarkHeaderProcessed();
		type->MarkConstructorMembersProcessed();
		type->MarkMethodMembersProcessed();
		type->MarkFieldMembersProcessed();
		type->MarkLocalClassProcessingCompleted();
		type->MarkSourceNoLongerPending();

		//
		// Since the unnamed packages includes all directories on the CLASSPATH,
		// check that types are not duplicated between directories.
		//
		if (!type->IsNested() &&
			type->ContainingPackage() == lookup_env.UnnamedPackage())
		{
			TypeSymbol* old_type = (TypeSymbol*)
				lookup_env.unnamed_package_types.Image(type->Identity());
			if (!old_type)
				lookup_env.unnamed_package_types.AddElement(type);
			else
			{
				ReportSemErrorSimple(SemanticError::DUPLICATE_TYPE_DECLARATION,
					tok, type->Name(), old_type->FileLoc());
			}
		}

		//
		// On systems such as NT and Win-95 that are not case-sensitive,
		// we need to confirm that the type name specified matches the name
		// in the class file.
		// TODO: Is this necessary, or did the previous check filter this?
		//      assert(type_name_length - (n + 1) == type -> ExternalNameLength());
		//      int i;
		//      for (i = 0; i < type -> ExternalNameLength(); i++)
		//      {
		//          if (type_name[n + 1 + i] != type -> ExternalName()[i])
		//              break;
		//      }
		//      if (i < type -> ExternalNameLength())
		//      {
		//          wchar_t* name = new wchar_t[type_name_length + 1];
		//          for (int k = 0; k < type_name_length; k++)
		//              name[k] = type_name[k];
		//          name[type_name_length] = U_NULL;
		//          ReportSemErrorSimple(SemanticError::TYPE_NAME_MISMATCH, tok,
		//                         type -> ContainingPackageName(),
		//                         type -> ExternalName(), name);
		//          delete [] name;
		//      }

		//
		// We now have enough information to make a good estimate for the
		// size of the symbol table we need for this class.
		//
		int i;
		const InnerClassesAttribute* inner_classes = class_data->InnerClasses();
		type->SetSymbolTable(class_data->FieldsCount() +
			class_data->MethodsCount() +
			(inner_classes
				? inner_classes->InnerClassesLength() : 0));

		//
		// Read the attributes. We do this now to learn about inner classes and
		// type parameterization, since this information may be needed when
		// processing supertypes, field signatures, and method signatures. Also,
		// we learn the access flags of this type.
		//
		type->SetFlags(class_data->Flags());
		type->ResetACC_SUPER();
		if (class_data->Deprecated())
			type->MarkDeprecated();
		if (inner_classes)
		{
			for (i = inner_classes->InnerClassesLength() - 1; i >= 0; i--)
			{
				const CPClassInfo* inner = inner_classes->Inner(i, pool);
				const CPClassInfo* outer = inner_classes->Outer(i, pool);
				if (inner == class_info)
				{
					type->SetFlags(inner_classes->Flags(i));
					if (!outer)
					{
						//
						// This type is local or anonymous, so we shouldn't be
						// reading it from the .class file. Marking it anonymous
						// is our clue to report an error for trying to use the
						// type by qualified name.
						//
						type->MarkAnonymous();
					}
				}
				else if (outer == class_info && inner_classes->Name(i, pool) &&
					inner_classes->NameLength(i, pool))
				{
					//
					// Some idiot compilers give anonymous classes the name "" and
					// an outer class, rather than obeying JVMS 4.7.5.  For
					// example, mail.jar in Sun's javamail 1.3.1 includes
					// javax.mail.Service with this property. The check for
					// NameLength above skips those invalid entries.
					//
					type->AddNestedTypeSignature((inner_classes ->
						Name(i, pool)),
						(inner_classes ->
							NameLength(i, pool)));
				}
			}
		}
		
		type->ProcessNestedTypeSignatures(this, tok);
		

		//
		// Tie this type to its supertypes.
		// FIXME: for sdk1_5, read attr_signature
		//
		class_info = class_data->SuperClass();
		if ((type == lookup_env.Object()) != (class_info == NULL))
			type->MarkBad();
		if (class_info)
		{
			type->super = GetType(type, class_info, pool, tok);
			if (type->super->ACC_INTERFACE() || type->super->IsArray() ||
				type->super->Bad())
			{
				type->MarkBad();
			}
			else
			{
				type->supertypes_closure->AddElement(type->super);
				type->supertypes_closure->Union(*type->super ->
					supertypes_closure);
			}
		}
		for (i = class_data->InterfacesCount() - 1; i >= 0; i--)
		{
			class_info = class_data->Interface(i);
			assert(class_info);
			TypeSymbol* interf = GetType(type, class_info, pool, tok);
			type->AddInterface(interf);
			type->supertypes_closure->AddElement(interf);
			type->supertypes_closure->Union(*interf->supertypes_closure);
			if (!interf->ACC_INTERFACE())
				type->MarkBad();
		}

		//
		// Read the fields.
		//
		for (i = class_data->FieldsCount() - 1; i >= 0; i--)
		{
			const FieldInfo* field = class_data->Field(i);
			if (field->ACC_SYNTHETIC())
				continue; // No point reading these - the user can't access them.
			NameSymbol* name_symbol =
				lookup_env.nameEnv->ConvertUtf8ToUnicode(field->Name(pool),
					field->NameLength(pool));
			VariableSymbol* symbol = new VariableSymbol(name_symbol);
			symbol->SetOwner(type);
			symbol->MarkComplete();
			symbol->MarkInitialized();
			symbol->SetFlags(field->Flags());
			symbol->SetSignatureString(field->Signature(pool, lookup_env.option),
				field->SignatureLength(pool, lookup_env.option));
			if (field->Deprecated())
				symbol->MarkDeprecated();
			const CPInfo* value = field->ConstantValue(pool);
			if (value)
			{
				switch (value->Tag())
				{
				case CPInfo::CONSTANT_Integer:
					symbol->initial_value = lookup_env.nameEnv->int_pool.
						FindOrInsert(((CPIntegerInfo*)value)->Value());
					break;
				case CPInfo::CONSTANT_Long:
					symbol->initial_value = lookup_env.nameEnv->long_pool.
						FindOrInsert(((CPLongInfo*)value)->Value());
					break;
				case CPInfo::CONSTANT_Float:
					symbol->initial_value = lookup_env.nameEnv->float_pool.
						FindOrInsert(((CPFloatInfo*)value)->Value());
					break;
				case CPInfo::CONSTANT_Double:
					symbol->initial_value = lookup_env.nameEnv->double_pool.
						FindOrInsert(((CPDoubleInfo*)value)->Value());
					break;
				case CPInfo::CONSTANT_String:
				{
					CPStringInfo* str_value = (CPStringInfo*)value;
					symbol->initial_value = lookup_env.nameEnv->Utf8_pool.
						FindOrInsert(str_value->Bytes(pool),
							str_value->Length(pool));
				}
				break;
				default:
					assert(false && "unexpected constant pool entry");
				}
			}
			type->InsertVariableSymbol(symbol);
			
			symbol->ProcessVariableSignature(this, tok);
			
		}

		// Read the methods.
		//
		auto nameEnv = lookup_env.nameEnv;
		for (i = class_data->MethodsCount() - 1; i >= 0; i--)
		{
			const MethodInfo* method = class_data->Method(i);
			NameSymbol* name_symbol =
				nameEnv->ConvertUtf8ToUnicode(method->Name(pool),
					method->NameLength(pool));
			if (method->ACC_SYNTHETIC() ||
				name_symbol == nameEnv->clinit_name_symbol)
			{
				continue; // No point reading these - the user can't access them.
			}
			MethodSymbol* symbol = new MethodSymbol(name_symbol);
			if(name_symbol == nameEnv->init_name_symbol){
				symbol->MarkConstructor();
			}
			symbol->SetContainingType(type);
			symbol->SetFlags(method->Flags());
			Utf8LiteralValue* sig = nameEnv->Utf8_pool.
				FindOrInsert(method->Signature(pool, lookup_env.option),
					method->SignatureLength(pool, lookup_env.option));
			symbol->SetSignature(sig);
			if (method->Deprecated())
				symbol->MarkDeprecated();
			const ExceptionsAttribute* throws_clause = method->Exceptions();
			if (throws_clause)
			{
				for (int j = throws_clause->NumberOfExceptions() - 1;
				j >= 0; j--)
				{
					const CPClassInfo* exception =
						throws_clause->Exception(j, pool);
					symbol ->
						AddThrowsSignature(exception->TypeName(pool),
							exception->TypeNameLength(pool));
				}
			}

			type->InsertMethodSymbol(symbol);
			
			symbol->ProcessMethodSignature(this, tok);
			
		}

		//
		// If requested by +F, suck in all types referred to in the constant pool
		// (both in CONSTANT_Class and in descriptors of CONSTANT_NameAndType).
		//
		if (!type->file_symbol->IsZip())
		{
			for (i = pool.Length() - 1; i > 0; i--)
			{
				if (pool[i]->Tag() == CPInfo::CONSTANT_Class)
					GetType(type, (CPClassInfo*)pool[i], pool, tok);
				else if (pool[i]->Tag() == CPInfo::CONSTANT_NameAndType)
				{
					const char* signature =
						((CPNameAndTypeInfo*)pool[i])->Signature(pool);
					if (*signature != U_LEFT_PARENTHESIS)
						// no '(' indicates a field descriptor
						ProcessSignature(type, signature, tok);
					else // a method descriptor
					{
						while (*signature && *signature++ != U_RIGHT_PARENTHESIS);
						ProcessSignature(type, signature, tok);
					}
				}
			}
		}

		delete class_data;
		type->CompressSpace();
	}



} // Close namespace Jikes block



