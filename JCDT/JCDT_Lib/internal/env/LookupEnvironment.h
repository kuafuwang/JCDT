
#ifndef LookupEnvironment_INCLUDED
#define LookupEnvironment_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/util/set.h"
#include "JCDT_Lib/internal/lookup/TypeLookupTable.h"
#include <vector>
#include <set>
#include <JCDT_Lib/internal/lookup/BindingSymbolTable.h>
#include <JCDT_Lib/internal/lookup/DirectoryEntry.h>
#include <JCDT_Lib/internal/impl/option.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/classfmt/AnnotationComponentAnnotation.inl>


namespace Jikes { // Open namespace Jikes block

	class BaseLexStream;
class AstStoragePool;
class Option;
class Scanner;
class Parser;
class Semantic;
class LexStream;
class AstPackageDeclaration;
class AstName;
class TypeDependenceChecker;
class INameEnvironment;
class ITypeRequestor;
class ParseError;
class CompilationUnitDeclaration;
//
// This class represents the control information common across all compilation
// units.  It provides a cache for essential classes and objects, as well as
// the command line options in force.
//
class LookupEnvironment : public StringConstant
{
public:
	INameEnvironment* nameEnv;
	ITypeRequestor*  typeRequestor;
    int return_code;
    Option* option;
  
    BindingSymbolTable external_table;

   
  
   
    Semantic* system_semantic;
	vector<Semantic*> semantic;
	vector<TypeSymbol*> needs_body_work;
	vector<TypeSymbol*> type_trash_bin;

    NameSymbolMap unnamed_package_types;

	bool record_problem = false;

	SymbolSet input_java_file_set;
    SymbolSet input_class_file_set;
    SymbolSet expired_file_set;
    SymbolSet recompilation_file_set;

	vector<FileSymbol*> input_files;

    
	FileSymbol* main_file_clone;
	AstStoragePool* ast_pool_for_pkg;
	
	//
	// Tables for hashing everything we've seen so far.
	//
    TypeLookupTable type_table;


	bool IsBaseType(Symbol* _type) const
	{
		if(_type == byte_type){
			return true;
		}
		if (_type == short_type) {
			return true;
		}
		if (_type == int_type) {
			return true;
		}
		if (_type == long_type) {
			return true;
		}
		if (_type == float_type) {
			return true;
		}
		if (_type == double_type) {
			return true;
		}
		if (_type == boolean_type) {
			return true;
		}
		return false;
	}
    //
    // The primitive types.
    //
    TypeSymbol* byte_type;
    TypeSymbol* short_type;
    TypeSymbol* int_type;
    TypeSymbol* long_type;
    TypeSymbol* char_type;
    TypeSymbol* float_type;
    TypeSymbol* double_type;
    TypeSymbol* boolean_type;
    TypeSymbol* void_type;
    TypeSymbol* null_type;
    TypeSymbol* no_type;

    //
    // System package accessors.
    //
	PackageSymbol* AnnotationPackage();
	PackageSymbol* IoPackage();
	PackageSymbol* LangPackage();
	PackageSymbol* UtilPackage();
	PackageSymbol* UnnamedPackage();

	void Makefile();
	
	void ReportSymstemErrorMsg() const;
	void cleanUp();

public:                                                        
	inline TypeSymbol* Annotation()
	{                                                          
		return  ProcessSystemType(AnnotationPackage(), "Annotation");
	}

	inline TypeSymbol* AssertionError()
	{
		return  ProcessSystemType(lang_package, "AssertionError");
	}
	inline TypeSymbol* Boolean(){
		return  ProcessSystemType(lang_package, "Boolean");
	}
	inline TypeSymbol* Byte() {
		return  ProcessSystemType(lang_package, "Byte");
	}
	inline TypeSymbol* Character() {
		return  ProcessSystemType(lang_package, "Character");
	}
	inline TypeSymbol* Class() {
		return  ProcessSystemType(lang_package, "Class");
	}
	inline TypeSymbol* ClassNotFoundException() {
		return  ProcessSystemType(lang_package, "ClassNotFoundException");
	}
	inline TypeSymbol* Double() {
		return  ProcessSystemType(lang_package, "Double");
	}
	inline TypeSymbol* ElementType() {
		return  ProcessSystemType(AnnotationPackage(), "ElementType");
	}
	inline TypeSymbol* Error() {
		return  ProcessSystemType(lang_package, "Error");
	}
	inline TypeSymbol* Exception() {
		return  ProcessSystemType(lang_package, "Exception");
	}
	inline TypeSymbol* Float() {
		return  ProcessSystemType(lang_package, "Float");
	}
	inline TypeSymbol* Iterable() {
		return  ProcessSystemType(lang_package, "Iterable");
	}
	inline TypeSymbol* Iterator() {
		return  ProcessSystemType(lang_package, "Iterator");
	}
	inline TypeSymbol* Long() {
		return  ProcessSystemType(lang_package, "Long");
	}
	inline TypeSymbol* NoClassDefFoundError() {
		return  ProcessSystemType(lang_package, "NoClassDefFoundError");
	}
	inline TypeSymbol* Object() {
		return  ProcessSystemType(lang_package, "Object");
	}
	inline TypeSymbol* Overrides() {
		return  ProcessSystemType(lang_package, "Overrides");
	}
	inline TypeSymbol* Retention() {
		return  ProcessSystemType(AnnotationPackage(), "Retention");
	}
	inline TypeSymbol* RetentionPolicy() {
		return  ProcessSystemType(AnnotationPackage(), "RetentionPolicy");
	}
	inline TypeSymbol* RuntimeException() {
		return  ProcessSystemType(lang_package, "RuntimeException");
	}
	inline TypeSymbol* Serializable() {
		return  ProcessSystemType(IoPackage(), "Serializable");
	}
	inline TypeSymbol* Short() {
		return  ProcessSystemType(lang_package, "Short");
	}
	inline TypeSymbol* String() {
		if (option->jcdk_version == Option::JCDK_3_X){
			return ProcessSystemType(lang_package, "String");
		}
		else{
			return no_type;
		}
	}
	inline bool IsString(const Symbol* type) {
		if( type->getNameString() == L"String"){
			if(type == ProcessSystemType(lang_package, "String")){
				return true;
			}
		}
		return false;
	}
	inline TypeSymbol* StringBuffer() {
		if (option->jcdk_version == Option::JCDK_3_X) {
			return ProcessSystemType(lang_package, "StringBuffer");
		}
		else {
			return no_type;
		}
	}
	inline TypeSymbol* StringBuilder() {
		if (option->jcdk_version == Option::JCDK_3_X) {
			return ProcessSystemType(lang_package, "StringBuilder");
		}
		else {
			return no_type;
		}
		
	}
	inline TypeSymbol* Target() {
		return  ProcessSystemType(AnnotationPackage(), "Target");
	}
	inline TypeSymbol* Throwable() {
		return  ProcessSystemType(lang_package, "Throwable");
	}
	inline TypeSymbol* Void() {
		return  ProcessSystemType(lang_package, "Void");
	}
	inline TypeSymbol* Enum() {
		return  ProcessSystemType(lang_package, "Enum");
	}
	inline TypeSymbol* Integer() {
		return  ProcessSystemType(lang_package, "Integer");
	}
    //
    // System type, method, and field accessors. Useful boilerplate macros
    // reduce the chance for typos, but be sure to update Compiler::LookupEnvironment to
    // initialize the field created by the macros to NULL.
    //
#define METHOD_ACCESSOR(method, type, name, descriptor)                      \
public:                                                                      \
    inline MethodSymbol* method ## Method()                                  \
    {                                                                        \
        return ProcessSystemMethod(type, name, descriptor); \
    }

    // FIELD_ACCESSOR(classname, fieldname, "descriptor");
#define FIELD_ACCESSOR(type, name, descriptor)                  \
public:                                                         \
    inline VariableSymbol* type ## _ ## name ## _Field()        \
    {                                                           \
        return   ProcessSystemField(type (), #name, descriptor); \
    }

    
    METHOD_ACCESSOR(AssertionError_Init, AssertionError(), "<init>", "()V");
    METHOD_ACCESSOR(AssertionError_InitWithChar, AssertionError(),
                    "<init>", "(C)V");
    METHOD_ACCESSOR(AssertionError_InitWithBoolean, AssertionError(),
                    "<init>", "(Z)V");
    METHOD_ACCESSOR(AssertionError_InitWithInt, AssertionError(),
                    "<init>", "(I)V");
    METHOD_ACCESSOR(AssertionError_InitWithLong, AssertionError(),
                    "<init>", "(J)V");
    METHOD_ACCESSOR(AssertionError_InitWithFloat, AssertionError(),
                    "<init>", "(F)V");
    METHOD_ACCESSOR(AssertionError_InitWithDouble, AssertionError(),
                    "<init>", "(D)V");
    METHOD_ACCESSOR(AssertionError_InitWithObject, AssertionError(),
                    "<init>", "(Ljava/lang/Object;)V");
	
    FIELD_ACCESSOR(Boolean, TYPE, "java/lang/Class");
   
    FIELD_ACCESSOR(Byte, TYPE, "java/lang/Class");
   
    FIELD_ACCESSOR(Character, TYPE, "java/lang/Class");
   
    METHOD_ACCESSOR(Class_forName, Class(),
                    "forName", "(Ljava/lang/String;)Ljava/lang/Class;");
    METHOD_ACCESSOR(Class_getComponentType, Class(),
                    "getComponentType", "()Ljava/lang/Class;");
    METHOD_ACCESSOR(Class_desiredAssertionStatus, Class(),
                    "desiredAssertionStatus", "()Z");

  
    //TYPE_ACCESSOR(Cloneable, lang_package);
    //TYPE_ACCESSOR(Comparable, lang_package);
 
    FIELD_ACCESSOR(Double, TYPE, "java/lang/Class");
   

    FIELD_ACCESSOR(ElementType, TYPE, "java/lang/annotation/ElementType");
    FIELD_ACCESSOR(ElementType, FIELD, "java/lang/annotation/ElementType");
    FIELD_ACCESSOR(ElementType, METHOD, "java/lang/annotation/ElementType");
    FIELD_ACCESSOR(ElementType, PARAMETER, "java/lang/annotation/ElementType");
    FIELD_ACCESSOR(ElementType, CONSTRUCTOR,
                   "java/lang/annotation/ElementType");
    FIELD_ACCESSOR(ElementType, LOCAL_VARIABLE,
                   "java/lang/annotation/ElementType");
    FIELD_ACCESSOR(ElementType, ANNOTATION_TYPE,
                   "java/lang/annotation/ElementType");
    FIELD_ACCESSOR(ElementType, PACKAGE, "java/lang/annotation/ElementType");

 


    METHOD_ACCESSOR(Enum_Init, Enum(), "<init>", "(Ljava/lang/String;I)V");
    METHOD_ACCESSOR(Enum_ordinal, Enum(), "ordinal", "()I");
    METHOD_ACCESSOR(Enum_valueOf, Enum(), "valueOf",
                    "(Ljava/lang/Class;Ljava/lang/String;)Ljava/lang/Enum;");

    
    FIELD_ACCESSOR(Float, TYPE, "java/lang/Class");

    FIELD_ACCESSOR(Integer, TYPE, "java/lang/Class");
   
    METHOD_ACCESSOR(Iterable_iterator, Iterable(),
                    "iterator", "()Ljava/util/Iterator;");


    METHOD_ACCESSOR(Iterator_hasNext, Iterator(), "hasNext", "()Z");
    METHOD_ACCESSOR(Iterator_next, Iterator(), "next", "()Ljava/lang/Object;");
   
    FIELD_ACCESSOR(Long, TYPE, "java/lang/Class");

    METHOD_ACCESSOR(NoClassDefFoundError_Init, NoClassDefFoundError(),
                    "<init>", "()V");
    METHOD_ACCESSOR(NoClassDefFoundError_InitString, NoClassDefFoundError(),
                    "<init>", "(Ljava/lang/String;)V");
  
    METHOD_ACCESSOR(Object_getClass, Object(),
                    "getClass", "()Ljava/lang/Class;");

    FIELD_ACCESSOR(RetentionPolicy, SOURCE,
                   "java/lang/annotation/RetentionPolicy");
    FIELD_ACCESSOR(RetentionPolicy, CLASS,
                   "java/lang/annotation/RetentionPolicy");
    FIELD_ACCESSOR(RetentionPolicy, RUNTIME,
                   "java/lang/annotation/RetentionPolicy");
  
  
    FIELD_ACCESSOR(Short, TYPE, "java/lang/Class");
  
    METHOD_ACCESSOR(StringBuffer_Init, StringBuffer(), "<init>", "()V");
    METHOD_ACCESSOR(StringBuffer_InitWithString, StringBuffer(),
                    "<init>", "(Ljava/lang/String;)V");
    METHOD_ACCESSOR(StringBuffer_toString, StringBuffer(),
                    "toString", "()Ljava/lang/String;");
    METHOD_ACCESSOR(StringBuffer_append_char, StringBuffer(),
                    "append", "(C)Ljava/lang/StringBuffer;");
    METHOD_ACCESSOR(StringBuffer_append_boolean, StringBuffer(),
                    "append", "(Z)Ljava/lang/StringBuffer;");
    METHOD_ACCESSOR(StringBuffer_append_int, StringBuffer(),
                    "append", "(I)Ljava/lang/StringBuffer;");
    METHOD_ACCESSOR(StringBuffer_append_long, StringBuffer(),
                    "append", "(J)Ljava/lang/StringBuffer;");
    METHOD_ACCESSOR(StringBuffer_append_float, StringBuffer(),
                    "append", "(F)Ljava/lang/StringBuffer;");
    METHOD_ACCESSOR(StringBuffer_append_double, StringBuffer(),
                    "append", "(D)Ljava/lang/StringBuffer;");
    METHOD_ACCESSOR(StringBuffer_append_string, StringBuffer(),
                    "append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;");
    METHOD_ACCESSOR(StringBuffer_append_object, StringBuffer(),
                    "append", "(Ljava/lang/Object;)Ljava/lang/StringBuffer;");

  

    METHOD_ACCESSOR(StringBuilder_Init, StringBuilder(), "<init>", "()V");
    METHOD_ACCESSOR(StringBuilder_InitWithString, StringBuilder(),
                    "<init>", "(Ljava/lang/String;)V");
    METHOD_ACCESSOR(StringBuilder_toString, StringBuilder(),
                    "toString", "()Ljava/lang/String;");
    METHOD_ACCESSOR(StringBuilder_append_char, StringBuilder(),
                    "append", "(C)Ljava/lang/StringBuilder;");
    METHOD_ACCESSOR(StringBuilder_append_boolean, StringBuilder(),
                    "append", "(Z)Ljava/lang/StringBuilder;");
    METHOD_ACCESSOR(StringBuilder_append_int, StringBuilder(),
                    "append", "(I)Ljava/lang/StringBuilder;");
    METHOD_ACCESSOR(StringBuilder_append_long, StringBuilder(),
                    "append", "(J)Ljava/lang/StringBuilder;");
    METHOD_ACCESSOR(StringBuilder_append_float, StringBuilder(),
                    "append", "(F)Ljava/lang/StringBuilder;");
    METHOD_ACCESSOR(StringBuilder_append_double, StringBuilder(),
                    "append", "(D)Ljava/lang/StringBuilder;");
    METHOD_ACCESSOR(StringBuilder_append_string, StringBuilder(),
                    "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
    METHOD_ACCESSOR(StringBuilder_append_object, StringBuilder(),
                    "append", "(Ljava/lang/Object;)Ljava/lang/StringBuilder;");

    METHOD_ACCESSOR(Throwable_getMessage, Throwable(),
                    "getMessage", "()Ljava/lang/String;");
    METHOD_ACCESSOR(Throwable_initCause, Throwable(), "initCause",
                    "(Ljava/lang/Throwable;)Ljava/lang/Throwable;");

 
    FIELD_ACCESSOR(Void, TYPE, "java/lang/Class");


#undef METHOD_ACCESSOR
#undef FIELD_ACCESSOR

	
    LookupEnvironment(Option*, INameEnvironment*, ITypeRequestor*);
    ~LookupEnvironment();
	TypeSymbol* getType( string compoundTypeName);
	

	void FindMoreRecentInputFiles(SymbolSet&);
    void RemoveTrashedTypes(SymbolSet&);
    void ComputeRecompilationSet(TypeDependenceChecker&);
	bool RecordIncrementalRecompilationUnit(set<wstring>& ,set<wstring>& ,set<wstring>&, set<wstring>&);
	bool RecordcompilationUnit(set<wstring>&);
	bool RecordcompilationUnit(set<FileSymbol*>&);

	set<wstring> CalculateRecompilationSet(set<wstring>&, set<wstring>&);
	void HelpToComputeRecompilationSet(TypeDependenceChecker& dependence_checker);


	FileSymbol* RecordIncrementalRecompilationUnit(const wstring&, set<wstring> &error_files);
    //
    // The one and only bad value constant.
    //
    LiteralValue* BadValue() { return bad_value; }
	NameSymbol* MakeParameter(int num);
  
    static DirectorySymbol* GetOutputDirectory(FileSymbol*);
    static FileSymbol* GetJavaFile(PackageSymbol*, const NameSymbol*);
    
	FileSymbol* (*GetFile) (Option*, PackageSymbol*, const NameSymbol*);

    static FileSymbol* GetFileFirst(Option*, PackageSymbol*,
                                    const NameSymbol*);
    static FileSymbol* GetFileBoth(Option*, PackageSymbol*,
                                   const NameSymbol*);

    PackageSymbol* FindOrInsertPackage(LexStream*, AstName*);
    void ProcessPackageDeclaration(FileSymbol*, CompilationUnitDeclaration*);
	

    void CleanUp(FileSymbol*);
	void ProcessFile(FileSymbol*);
	void ProcessFileBodys();


	bool IsSimpleIntegerValueType(const TypeSymbol* type);
	bool IsIntegral(const TypeSymbol* type);
	bool IsFloatingPoint(const TypeSymbol* type);
	bool IsNumeric(const TypeSymbol* type);
	bool IsDoubleWordType(const TypeSymbol* type);
	bool IsPrimitive(const TypeSymbol* type);

	void ProcessBadType(TypeSymbol* type_symbol);
    void ProcessFileHeaders(FileSymbol*);

#ifdef JIKES_DEBUG
	static int input_files_processed,
        class_files_read,
        class_files_written,
        line_count;
#endif // JIKES_DEBUG


	PackageSymbol* ProcessPackage(const wchar_t*);

 
	

private:
    LiteralValue* bad_value;

    //
    // Cache of system packages. lang and unnamed are always valid, because of
    // ProcessUnnamedPackage and ProcessSystemInformation in system.cpp, the
    // constructor initializes the rest to NULL in control.cpp; see accessor
    // methods above for assignment.
    //
    PackageSymbol* annotation_package;
    PackageSymbol* io_package;
    PackageSymbol* lang_package;
    PackageSymbol* util_package;
    PackageSymbol* unnamed_package;

  
    
    void ProcessUnnamedPackage();
 
   
    TypeSymbol* GetPrimitiveType(const char*, char);
    void ProcessSystemInformation();
    TypeSymbol* ProcessSystemType(PackageSymbol*, const char*);
    MethodSymbol* ProcessSystemMethod(TypeSymbol*, const char*, const char*);
    VariableSymbol* ProcessSystemField(TypeSymbol*, const char*, const char*);

    void ProcessMembers(FileSymbol*);
    void CollectTypes(TypeSymbol*, vector<TypeSymbol*>&);
    void ProcessBodies(TypeSymbol*);
    void CheckForUnusedImports(Semantic *);


   
};



//
// System package accessors.
//
inline PackageSymbol* LookupEnvironment::AnnotationPackage()
{
	if (!annotation_package)
		annotation_package = ProcessPackage(US_java_SL_lang_SL_annotation);
	return annotation_package;
}
inline PackageSymbol* LookupEnvironment::IoPackage()
{
	if (!io_package)
		io_package = ProcessPackage(US_java_SL_io);
	return io_package;
}
inline PackageSymbol* LookupEnvironment::LangPackage()
{
	assert(lang_package);
	return lang_package;
}
inline PackageSymbol* LookupEnvironment::UtilPackage()
{
	if (!util_package)
		util_package = ProcessPackage(US_java_SL_util);
	return util_package;
}
inline PackageSymbol* LookupEnvironment::UnnamedPackage()
{
	assert(unnamed_package);
	return unnamed_package;
}

inline bool LookupEnvironment::IsSimpleIntegerValueType(const TypeSymbol* type)
{
	return type == byte_type || type == short_type ||
		type == int_type || type == char_type;
}

inline bool LookupEnvironment::IsIntegral(const TypeSymbol* type)
{
	return IsSimpleIntegerValueType(type) || type == long_type;
}

inline bool LookupEnvironment::IsFloatingPoint(const TypeSymbol* type)
{
	return type == float_type || type == double_type;
}

inline bool LookupEnvironment::IsNumeric(const TypeSymbol* type)
{
	return IsIntegral(type) || IsFloatingPoint(type);
}

inline bool LookupEnvironment::IsDoubleWordType(const TypeSymbol* type)
{
	return type == long_type || type == double_type;
}

inline bool LookupEnvironment::IsPrimitive(const TypeSymbol* type)
{
	return IsNumeric(type) || type == boolean_type;
}

inline void LookupEnvironment::ProcessBadType(TypeSymbol* type_symbol)
{
	type_trash_bin.push_back(type_symbol);
}



} // Close namespace Jikes block


#endif // control_INCLUDED

