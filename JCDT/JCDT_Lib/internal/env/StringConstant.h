
#ifndef StringConstant_Jikes_INCLUDED
#define StringConstant_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block

//
	// Constant strings used in the program. Those prefixed with US_ are
	// Unicode strings in wchar_t, and those with U8S are UTF8 strings in char.
	//
	class StringConstant
	{
	public:
		//
		// Symbols and operators.
		//
		static const wchar_t US_AND[]; // L"&"
		static const wchar_t US_AND_AND[]; // L"&&"
		static const wchar_t US_AND_EQUAL[]; // L"&="
		static const wchar_t US_AT[]; // L"@"
		static const wchar_t US_COLON[]; // L":"
		static const wchar_t US_COMMA[]; // L";"
		static const wchar_t US_DIVIDE[]; // L"/"
		static const wchar_t US_DIVIDE_EQUAL[]; // L"/="
		static const wchar_t US_DOT[]; // L"."
		static const wchar_t US_DOT_DOT_DOT[]; // L"..."
		static const wchar_t US_EMPTY[]; // L""
		static const wchar_t US_EOF[]; // L"EOF"
		static const wchar_t US_EQUAL[]; // L"="
		static const wchar_t US_EQUAL_EQUAL[]; // L"=="
		static const wchar_t US_GREATER[]; // L">"
		static const wchar_t US_GREATER_EQUAL[]; // L">="
		static const wchar_t US_LBRACE[]; // L"{"
		static const wchar_t US_LBRACKET[]; // L"["
		static const wchar_t US_LEFT_SHIFT[]; // L"<<"
		static const wchar_t US_LEFT_SHIFT_EQUAL[]; // L"<<="
		static const wchar_t US_LESS[]; // L"<"
		static const wchar_t US_LESS_EQUAL[]; // L"<="
		static const wchar_t US_LPAREN[]; // L"("
		static const wchar_t US_MINUS[]; // L"-"
		static const wchar_t US_MINUS_EQUAL[]; // L"-="
		static const wchar_t US_MINUS_MINUS[]; // L"--"
		static const wchar_t US_MULTIPLY[]; // L"*"
		static const wchar_t US_MULTIPLY_EQUAL[]; // L"*="
		static const wchar_t US_NOT[]; // L"!"
		static const wchar_t US_NOT_EQUAL[]; // L"!="
		static const wchar_t US_OR[]; // L"|"
		static const wchar_t US_OR_EQUAL[]; // L"|="
		static const wchar_t US_OR_OR[]; // L"||"
		static const wchar_t US_PLUS[]; // L"+"
		static const wchar_t US_PLUS_EQUAL[]; // L"+="
		static const wchar_t US_PLUS_PLUS[]; // L"++"
		static const wchar_t US_QUESTION[]; // L"?"
		static const wchar_t US_RBRACE[]; // L"}"
		static const wchar_t US_RBRACKET[]; // L"]"
		static const wchar_t US_REMAINDER[]; // L"%"
		static const wchar_t US_REMAINDER_EQUAL[]; // L"%="
		static const wchar_t US_RIGHT_SHIFT[]; // L">>"
		static const wchar_t US_RIGHT_SHIFT_EQUAL[]; // L">>="
		static const wchar_t US_RPAREN[]; // L")"
		static const wchar_t US_SEMICOLON[]; // L";"
		static const wchar_t US_TWIDDLE[]; // L"~"
		static const wchar_t US_UNSIGNED_RIGHT_SHIFT[]; // L">>>"
		static const wchar_t US_UNSIGNED_RIGHT_SHIFT_EQUAL[]; // L">>>="
		static const wchar_t US_XOR[]; // L"^"
		static const wchar_t US_XOR_EQUAL[]; // L"^="


		//
		// Unicode strings for files and packages.
		//
		static const wchar_t US_DS[]; // L"$"
		static const wchar_t US_LB_RB[]; // L"[]"
		static const wchar_t US_MI[]; // L"-"
		static const wchar_t US_SC[]; // L";"
		static const wchar_t US_SL[]; // L"/"
		static const wchar_t US_jar[]; // L"jar"
		static const wchar_t US_java_SL_io[]; // L"java/io"
		static const wchar_t US_java_SL_lang[]; // L"java/lang"
												// L"java/lang/annotation"
		static const wchar_t US_java_SL_lang_SL_annotation[];
		static const wchar_t US_java_SL_util[]; // L"java/util"
	
		static const wchar_t US_java_SL_Object[]; // L"java/lang/Object"

		
		static const wchar_t US_zip[]; // L"zip"


		//
		// Java keywords.
		//
		static const wchar_t US_abstract[]; // L"abstract"
		static const wchar_t US_assert[]; // L"assert"
		static const wchar_t US_boolean[]; // L"boolean"
		static const wchar_t US_break[]; // L"break"
		static const wchar_t US_byte[]; // L"byte"
		static const wchar_t US_case[]; // L"case"
		static const wchar_t US_catch[]; // L"catch"
		static const wchar_t US_char[]; // L"char"
		static const wchar_t US_class[]; // L"class"
		static const wchar_t US_const[]; // L"const"
		static const wchar_t US_continue[]; // L"continue"
		static const wchar_t US_default[]; // L"default"
		static const wchar_t US_do[]; // L"do"
		static const wchar_t US_double[]; // L"double"
		static const wchar_t US_else[]; // L"else"
		static const wchar_t US_enum[]; // L"enum"
		static const wchar_t US_extends[]; // L"extends"
		static const wchar_t US_false[]; // L"false"
		static const wchar_t US_final[]; // L"final"
		static const wchar_t US_finally[]; // L"finally"
		static const wchar_t US_float[]; // L"float"
		static const wchar_t US_for[]; // L"for"
		static const wchar_t US_goto[]; // L"goto"
		static const wchar_t US_if[]; // L"if"
		static const wchar_t US_implements[]; // L"implements"
		static const wchar_t US_import[]; // L"import"
		static const wchar_t US_instanceof[]; // L"instanceof"
		static const wchar_t US_int[]; // L"int"
		static const wchar_t US_interface[]; // L"interface"
		static const wchar_t US_long[]; // L"long"
		static const wchar_t US_native[]; // L"native"
		static const wchar_t US_new[]; // L"new"
		static const wchar_t US_null[]; // L"null"
		static const wchar_t US_package[]; // L"package"
		static const wchar_t US_private[]; // L"private"
		static const wchar_t US_protected[]; // L"protected"
		static const wchar_t US_public[]; // L"public"
		static const wchar_t US_return[]; // L"return"
		static const wchar_t US_short[]; // L"short"
		static const wchar_t US_static[]; // L"static"
		static const wchar_t US_strictfp[]; // L"strictfp"
		static const wchar_t US_super[]; // L"super"
		static const wchar_t US_switch[]; // L"switch"
		static const wchar_t US_synchronized[]; // L"synchronized"
		static const wchar_t US_this[]; // L"this"
		static const wchar_t US_throw[]; // L"throw"
		static const wchar_t US_throws[]; // L"throws"
		static const wchar_t US_transient[]; // L"transient"
		static const wchar_t US_true[]; // L"true"
		static const wchar_t US_try[]; // L"try"
		static const wchar_t US_void[]; // L"void"
		static const wchar_t US_volatile[]; // L"volatile"
		static const wchar_t US_while[]; // L"while"

		//
		// Miscellaneous strings.
		//
		static const char U8S_help_header[];
		static const char U8S_command_format[];

		//
		// Constant pool entries.
		//
		static const char U8S_AnnotationDefault[]; // "AnnotationDefault"
		static const char U8S_Code[]; // "Code"
		static const char U8S_ConstantValue[]; // "ConstantValue"
		static const char U8S_Deprecated[]; // "Deprecated"
		static const char U8S_EnclosingMethod[]; // "EnclosingMethod"
		static const char U8S_Exceptions[]; // "Exceptions"
		static const char U8S_InnerClasses[]; // "InnerClasses"
		static const char U8S_LineNumberTable[]; // "LineNumberTable"
		static const char U8S_LocalVariableTable[]; // "LocalVariableTable"
		static const char U8S_LocalVariableTypeTable[]; // "LocalVariableTypeTable"
														// "RuntimeInvisibleAnnotations"
		static const char U8S_RuntimeInvisibleAnnotations[];
		// "RuntimeInvisibleParameterAnnotations"
		static const char U8S_RuntimeInvisibleParameterAnnotations[];
		// "RuntimeVisibleAnnotations"
		static const char U8S_RuntimeVisibleAnnotations[];
		// "RuntimeVisibleParameterAnnotations"
		static const char U8S_RuntimeVisibleParameterAnnotations[];
		static const char U8S_Signature[]; // "Signature"
		static const char U8S_SourceFile[]; // "SourceFile"
		static const char U8S_StackMap[]; // "StackMap"
		static const char U8S_Synthetic[]; // "Synthetic"

										   //
										   // ASCII file names.
										   //
		static const char U8S_DO_class[]; // ".class"
		static const char U8S_DO_java[]; // ".java"
		static const char U8S_DO_tok[]; // ".tok"
		static const char U8S_DO_u[]; // ".u"
		static const char U8S_LP[]; // "("
		static const char U8S_RP[]; // ")"
		static const char U8S_SL[]; // "/"

									//
									// Convert number to string.
									//
		static const char U8S_NaN[]; // "NaN"
		static const char U8S_pos_Infinity[]; // "Infinity"
		static const char U8S_neg_Infinity[]; // "-Infinity"
		static const char U8S_pos_Zero[]; // "0.0"
		static const char U8S_neg_Zero[]; // "-0.0"
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

