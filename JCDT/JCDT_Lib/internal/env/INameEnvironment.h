
#ifndef INameEnvironment_INCLUDED
#define INameEnvironment_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/util/set.h"
#include <vector>
#include <set>
#include "JCDT_Lib/internal/util/CharOperation.h"
#include <map>
#include <JCDT_Lib/internal/lookup/LiteralLookupTable.h>
#include <JCDT_Lib/internal/lookup/NameLookupTable.h>
#include <JCDT_Lib/internal/lookup/IntLiteralTable.h>
#include <JCDT_Lib/internal/lookup/LongLiteralTable.h>
#include <JCDT_Lib/internal/lookup/FloatLiteralTable.h>
#include <JCDT_Lib/internal/lookup/DoubleLiteralTable.h>
#include <JCDT_Lib/internal/lookup/Utf8LiteralTable.h>
#include <JCDT_Lib/internal/lookup/BindingSymbolTable.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>
#include <JCDT_Lib/internal/lookup/PathSymbol.h>
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>

namespace Jikes {
	class DirectoryEntry;
	// Open namespace Jikes block

class SystemTable;
class Option;
class Semantic;
class JikesClassPaths;
//
// This class represents the control information common across all compilation
// units.  It provides a cache for essential classes and objects, as well as
// the command line options in force.
//
class INameEnvironment : public StringConstant
{
public:
   
    Option* option;
    BindingSymbolTable classpath_table;
    unsigned dot_classpath_index;

    Tuple<PathSymbol*> classpath;
    Tuple<wchar_t*> bad_dirnames;
    Tuple<wchar_t*> bad_zip_filenames;
    vector<wstring> bad_input_filenames;
	vector<wstring> unreadable_input_filenames;
    Tuple<const wchar_t*> general_io_errors;
    Tuple<const wchar_t*> general_io_warnings;

    SystemTable* system_table;
    Tuple<DirectorySymbol*> system_directories;

	// Tables for hashing everything we've seen so far.
	LiteralLookupTable string_table;
	LiteralLookupTable int_table;
	LiteralLookupTable long_table;
	LiteralLookupTable char_table;
	LiteralLookupTable float_table;
	LiteralLookupTable double_table;

	NameLookupTable name_table;

    // This cache of name symbols is initialized in system.cpp.
    NameSymbol* access_name_symbol;
    NameSymbol* array_name_symbol;
    NameSymbol* assert_name_symbol;
    NameSymbol* block_init_name_symbol;
    NameSymbol* class_name_symbol;
    NameSymbol* clinit_name_symbol;
    NameSymbol* clone_name_symbol;
    NameSymbol* dot_name_symbol;
    NameSymbol* dot_dot_name_symbol;
    NameSymbol* Enum_name_symbol;
    NameSymbol* equals_name_symbol;
    NameSymbol* false_name_symbol;
    NameSymbol* hashCode_name_symbol;
    NameSymbol* init_name_symbol;
    NameSymbol* length_name_symbol;
    NameSymbol* null_name_symbol;
    NameSymbol* Object_name_symbol;
    NameSymbol* package_info_name_symbol;
    NameSymbol* question_name_symbol;
    NameSymbol* serialPersistentFields_name_symbol;
    NameSymbol* serialVersionUID_name_symbol;
    NameSymbol* this_name_symbol;
    NameSymbol* true_name_symbol;
    NameSymbol* val_name_symbol;

    Utf8LiteralValue* ConstantValue_literal;
    Utf8LiteralValue* Exceptions_literal;
    Utf8LiteralValue* InnerClasses_literal;
    Utf8LiteralValue* Synthetic_literal;
    Utf8LiteralValue* Deprecated_literal;
    Utf8LiteralValue* LineNumberTable_literal;
    Utf8LiteralValue* LocalVariableTable_literal;
    Utf8LiteralValue* Code_literal;
    Utf8LiteralValue* SourceFile_literal;
    Utf8LiteralValue* EnclosingMethod_literal;

	
	Tuple<DirectorySymbol*> unnamed_package_dirs;

    IntLiteralTable int_pool;
    LongLiteralTable long_pool;
    FloatLiteralTable float_pool;
    DoubleLiteralTable double_pool;
    Utf8LiteralTable Utf8_pool;
	
    INameEnvironment(Option*, JikesClassPaths&);
    ~INameEnvironment();

	Utf8LiteralValue* ConvertUnicodeToUtf8(const wchar_t* source);
	NameSymbol* ConvertUtf8ToUnicode(const char* source, int length);

    void FindPathsToDirectory(PackageSymbol*);
    DirectoryEntry* FindInputFile(FileSymbol*);
    void RereadDirectory(DirectorySymbol*);
    void RereadDirectories();
   
    //
    // The one and only bad value constant.
    //
    LiteralValue* BadValue() { return &bad_value; }

    //
    // Note that only names are converted here and not literals, since
    // no error can occur in a name.
    // A literal is converted during the semantic pass so that an
    // accurate diagnostic can be issued in case it is invalid.
    //
	NameSymbol* FindOrInsertName(const wchar_t* name, int len);

    //
    // Make up a parameter name of the form $(num) and return its name symbol.
    //
	NameSymbol* MakeParameter(int num);

	FileSymbol* GetVirtualFile();

    DirectorySymbol* FindSubdirectory(PathSymbol*,const wchar_t*, int);
    DirectorySymbol* ProcessSubdirectories(const wchar_t*, int, bool);

public:
    LiteralValue bad_value;
	
    NameSymbol* FindOrInsertSystemName(const char* name);

    void ProcessGlobals();
  
    void ProcessPath(JikesClassPaths&);
	void AddClassPath(const std::wstring& _pathName, JikesClassPaths::DataElement* dataElement);
	void AddClassPaths(JikesClassPaths&);

	void ProcessNewInputFiles(SymbolSet&, set<wstring>&);
	void ProcessNewInputFiles(SymbolSet&, set<FileSymbol*>&);
    FileSymbol* FindOrInsertJavaInputFile(DirectorySymbol*, NameSymbol*);
	bool InsertJavaInputFile(DirectorySymbol*, FileSymbol*, NameSymbol*);

    FileSymbol* FindOrInsertJavaInputFile(const wchar_t*, int);

	bool  InsertJavaInputFile(const wchar_t*, int, FileSymbol*);
};



inline Utf8LiteralValue* INameEnvironment::ConvertUnicodeToUtf8(const wchar_t* source)
{
	// Should be big enough for the worst case.
	char* target = new char[wcslen(source) * 3 + 1];
	int length = CharOperation::ConvertUnicodeToUtf8(source, target);
	Utf8LiteralValue* literal = Utf8_pool.FindOrInsert(target, length);
	delete[] target;
	return literal;
}


inline NameSymbol* INameEnvironment::ConvertUtf8ToUnicode(const char* source, int length)
{
	wchar_t* name = new wchar_t[length + 1];
	int name_length = CharOperation::ConvertUtf8ToUnicode(name, source, length);
	NameSymbol* name_symbol = FindOrInsertName(name, name_length);
	delete[] name;
	return name_symbol;
}

inline NameSymbol* INameEnvironment::FindOrInsertName(const wchar_t* name, int len)
{
	NameSymbol* name_symbol = name_table.FindOrInsertName(name, len);
	if (!name_symbol->Utf8_literal)
		name_symbol->Utf8_literal =
		ConvertUnicodeToUtf8(name_symbol->Name());
	return name_symbol;
}

inline NameSymbol* INameEnvironment::MakeParameter(int num)
{
	IntToWstring value(num);
	wchar_t str[13];
	str[0] = U_DOLLAR; // '$'
	wcscpy(&str[1], value.String());
	return FindOrInsertName(str, value.Length() + 1);
}


} // Close namespace Jikes block


#endif // control_INCLUDED

