#include <JCDT_Lib/internal/env/StringConstant.h>
#include <JCDT_Lib/internal/env/U_chars.h>
#include <JCDT_Lib/internal/env/config.h>
namespace Jikes { // Open namespace Jikes block



	//
	// Punctuation and operators
	//
	const wchar_t StringConstant::US_AND[] = { U_AM, U_NU }; // L"&"
	const wchar_t StringConstant::US_AND_AND[] = { U_AM, U_AM, U_NU }; // L"&&"
	const wchar_t StringConstant::US_AND_EQUAL[] = { U_AM, U_EQ, U_NU }; // L"&="
	const wchar_t StringConstant::US_AT[] = { U_AT, U_NU }; // L"@"
	const wchar_t StringConstant::US_COLON[] = { U_CO, U_NU }; // L":"
	const wchar_t StringConstant::US_COMMA[] = { U_CM, U_NU }; // L","
	const wchar_t StringConstant::US_DIVIDE[] = { U_SL, U_NU }; // L"/"
	const wchar_t StringConstant::US_DIVIDE_EQUAL[] = { U_SL, U_EQ, U_NU }; // L"/="
	const wchar_t StringConstant::US_DOT[] = { U_DO, U_NU }; // L"."
	const wchar_t StringConstant::US_DOT_DOT_DOT[] = {
		U_DO, U_DO, U_DO, U_NU }; // L"..."
	const wchar_t StringConstant::US_EMPTY[] = { U_NU }; // L""
	const wchar_t StringConstant::US_EOF[] = { U_E, U_O, U_F, U_NU }; // L"EOF"
	const wchar_t StringConstant::US_EQUAL[] = { U_EQ, U_NU }; // L"="
	const wchar_t StringConstant::US_EQUAL_EQUAL[] = { U_EQ, U_EQ, U_NU }; // L"=="
	const wchar_t StringConstant::US_GREATER[] = { U_GT, U_NU }; // L">"
	const wchar_t StringConstant::US_GREATER_EQUAL[] = { U_GT, U_EQ, U_NU }; // L">="
	const wchar_t StringConstant::US_LBRACE[] = { U_OS, U_NU }; // L"{"
	const wchar_t StringConstant::US_LBRACKET[] = { U_LB, U_NU }; // L"["
	const wchar_t StringConstant::US_LEFT_SHIFT[] = { U_LT, U_LT, U_NU }; // L"<<"
	const wchar_t StringConstant::US_LEFT_SHIFT_EQUAL[] = {
		U_LT, U_LT, U_EQ, U_NU }; // L"<<="
	const wchar_t StringConstant::US_LESS[] = { U_LT, U_NU }; // L"<"
	const wchar_t StringConstant::US_LESS_EQUAL[] = { U_LT, U_EQ, U_NU }; // L"<="
	const wchar_t StringConstant::US_LPAREN[] = { U_LP, U_NU }; // L"("
	const wchar_t StringConstant::US_MINUS[] = { U_MI, U_NU }; // L"-"
	const wchar_t StringConstant::US_MINUS_EQUAL[] = { U_MI, U_EQ, U_NU }; // L"-="
	const wchar_t StringConstant::US_MINUS_MINUS[] = { U_MI, U_MI, U_NU }; // L"--"
	const wchar_t StringConstant::US_MULTIPLY[] = { U_ST, U_NU }; // L"*"
	const wchar_t StringConstant::US_MULTIPLY_EQUAL[] = {
		U_ST, U_EQ, U_NU }; // L"*="
	const wchar_t StringConstant::US_NOT[] = { U_EX, U_NU }; // L"!"
	const wchar_t StringConstant::US_NOT_EQUAL[] = { U_EX, U_EQ, U_NU }; // L"!="
	const wchar_t StringConstant::US_OR[] = { U_BA, U_NU }; // L"|"
	const wchar_t StringConstant::US_OR_EQUAL[] = { U_BA, U_EQ, U_NU }; // L"|="
	const wchar_t StringConstant::US_OR_OR[] = { U_BA, U_BA, U_NU }; // L"||"
	const wchar_t StringConstant::US_PLUS[] = { U_PL, U_NU }; // L"+"
	const wchar_t StringConstant::US_PLUS_EQUAL[] = { U_PL, U_EQ, U_NU }; // L"+="
	const wchar_t StringConstant::US_PLUS_PLUS[] = { U_PL, U_PL, U_NU }; // L"++"
	const wchar_t StringConstant::US_QUESTION[] = { U_QU, U_NU }; // L"?"
	const wchar_t StringConstant::US_RBRACE[] = { U_CS, U_NU }; // L"}"
	const wchar_t StringConstant::US_RBRACKET[] = { U_RB, U_NU }; // L"]"
	const wchar_t StringConstant::US_REMAINDER[] = { U_PE, U_NU }; // L"%"
	const wchar_t StringConstant::US_REMAINDER_EQUAL[] = {
		U_PE, U_EQ, U_NU }; // L"%="
	const wchar_t StringConstant::US_RIGHT_SHIFT[] = { U_GT, U_GT, U_NU }; // L">>"
	const wchar_t StringConstant::US_RIGHT_SHIFT_EQUAL[] = {
		U_GT, U_GT, U_EQ, U_NU }; // L">>="
	const wchar_t StringConstant::US_RPAREN[] = { U_RP, U_NU }; // L")"
	const wchar_t StringConstant::US_SEMICOLON[] = { U_SC, U_NU }; // L";"
	const wchar_t StringConstant::US_TWIDDLE[] = { U_TI, U_NU }; // L"~"
	const wchar_t StringConstant::US_UNSIGNED_RIGHT_SHIFT[] = {
		U_GT, U_GT, U_GT, U_NU }; // L">>>"
	const wchar_t StringConstant::US_UNSIGNED_RIGHT_SHIFT_EQUAL[] = {
		U_GT, U_GT, U_GT, U_EQ, U_NU }; // L">>>="
	const wchar_t StringConstant::US_XOR[] = { U_CA, U_NU }; // L"^"
	const wchar_t StringConstant::US_XOR_EQUAL[] = { U_CA, U_EQ, U_NU }; // L"^="

																		 //
																		 // Common constant pool entries
																		 //
	const wchar_t StringConstant::US_DS[] = { U_DS, U_NU }; // L"$"
	const wchar_t StringConstant::US_LB_RB[] = { U_LB, U_RB, U_NU }; // L"[]"
	const wchar_t StringConstant::US_MI[] = { U_MI, U_NU }; // L"-"
	const wchar_t StringConstant::US_SC[] = { U_SC, U_NU }; // L";"
	const wchar_t StringConstant::US_SL[] = { U_SL, U_NU }; // L"/"
	const wchar_t StringConstant::US_jar[] = { U_j, U_a, U_r, U_NU }; // L"jar"
	const wchar_t StringConstant::US_java_SL_io[] = {
		U_j, U_a, U_v, U_a, U_SL, U_i, U_o, U_NU }; // L"java/io"
	const wchar_t StringConstant::US_java_SL_lang[] = {
		U_j, U_a, U_v, U_a, U_SL, U_l, U_a, U_n, U_g, U_NU }; // L"java/lang"
	const wchar_t StringConstant::US_java_SL_lang_SL_annotation[] = {
		U_j, U_a, U_v, U_a, U_SL, U_l, U_a, U_n, U_g, U_SL,
		U_a, U_n, U_n, U_o, U_t, U_a, U_t, U_i, U_o, U_n,
		U_NU }; // L"java/lang/annotation"
	const wchar_t StringConstant::US_java_SL_util[] = {
		U_j, U_a, U_v, U_a, U_SL, U_u, U_t, U_i, U_l, U_NU }; // L"java/util"

	const wchar_t StringConstant::US_java_SL_Object[] = {
		U_j, U_a, U_v, U_a, U_SL,
		U_l, U_a, U_n, U_g,U_SL,
		U_O,U_b,U_j, U_e,U_c,U_t,U_NU };// L"java/lang/object"

	const wchar_t StringConstant::US_zip[] = { U_z, U_i, U_p, U_NU }; // L"zip"

																	  //
																	  // Java keywords.
																	  //
	const wchar_t StringConstant::US_abstract[] = {
		U_a, U_b, U_s, U_t, U_r, U_a, U_c, U_t, U_NU }; // L"abstract"
	const wchar_t StringConstant::US_assert[] = {
		U_a, U_s, U_s, U_e, U_r, U_t, U_NU }; // L"assert"
	const wchar_t StringConstant::US_boolean[] = {
		U_b, U_o, U_o, U_l, U_e, U_a, U_n, U_NU }; // L"boolean"
	const wchar_t StringConstant::US_break[] = {
		U_b, U_r, U_e, U_a, U_k, U_NU }; // L"break"
	const wchar_t StringConstant::US_byte[] = {
		U_b, U_y, U_t, U_e, U_NU }; // L"byte"
	const wchar_t StringConstant::US_case[] = {
		U_c, U_a, U_s, U_e, U_NU }; // L"case"
	const wchar_t StringConstant::US_catch[] = {
		U_c, U_a, U_t, U_c, U_h, U_NU }; // L"catch"
	const wchar_t StringConstant::US_char[] = {
		U_c, U_h, U_a, U_r, U_NU }; // L"char"
	const wchar_t StringConstant::US_class[] = {
		U_c, U_l, U_a, U_s, U_s, U_NU }; // L"class"
	const wchar_t StringConstant::US_const[] = {
		U_c, U_o, U_n, U_s, U_t, U_NU }; // L"const"
	const wchar_t StringConstant::US_continue[] = {
		U_c, U_o, U_n, U_t, U_i, U_n, U_u, U_e, U_NU }; // L"continue"
	const wchar_t StringConstant::US_default[] = {
		U_d, U_e, U_f, U_a, U_u, U_l, U_t, U_NU }; // L"default"
	const wchar_t StringConstant::US_do[] = { U_d, U_o, U_NU }; // L"do"
	const wchar_t StringConstant::US_double[] = {
		U_d, U_o, U_u, U_b, U_l, U_e, U_NU }; // L"double"
	const wchar_t StringConstant::US_else[] = {
		U_e, U_l, U_s, U_e, U_NU }; // L"else"
	const wchar_t StringConstant::US_enum[] = {
		U_e, U_n, U_u, U_m, U_NU }; // L"enum"
	const wchar_t StringConstant::US_extends[] = {
		U_e, U_x, U_t, U_e, U_n, U_d, U_s, U_NU }; // L"extends"
	const wchar_t StringConstant::US_false[] = {
		U_f, U_a, U_l, U_s, U_e, U_NU }; // L"false"
	const wchar_t StringConstant::US_final[] = {
		U_f, U_i, U_n, U_a, U_l, U_NU }; // L"final"
	const wchar_t StringConstant::US_finally[] = {
		U_f, U_i, U_n, U_a, U_l, U_l, U_y, U_NU }; // L"finally"
	const wchar_t StringConstant::US_float[] = {
		U_f, U_l, U_o, U_a, U_t, U_NU }; // L"float"
	const wchar_t StringConstant::US_for[] = { U_f, U_o, U_r, U_NU }; // L"for"
	const wchar_t StringConstant::US_goto[] = {
		U_g, U_o, U_t, U_o, U_NU }; // L"goto"
	const wchar_t StringConstant::US_if[] = { U_i, U_f, U_NU }; // L"if"
	const wchar_t StringConstant::US_implements[] = {
		U_i, U_m, U_p, U_l, U_e, U_m, U_e, U_n, U_t, U_s, U_NU }; // L"implements"
	const wchar_t StringConstant::US_import[] = {
		U_i, U_m, U_p, U_o, U_r, U_t, U_NU }; // L"import"
	const wchar_t StringConstant::US_instanceof[] = {
		U_i, U_n, U_s, U_t, U_a, U_n, U_c, U_e, U_o, U_f, U_NU }; // L"instanceof"
	const wchar_t StringConstant::US_int[] = { U_i, U_n, U_t, U_NU }; // L"int"
	const wchar_t StringConstant::US_interface[] = {
		U_i, U_n, U_t, U_e, U_r, U_f, U_a, U_c, U_e, U_NU }; // L"interface"
	const wchar_t StringConstant::US_long[] = {
		U_l, U_o, U_n, U_g, U_NU }; // L"long"
	const wchar_t StringConstant::US_native[] = {
		U_n, U_a, U_t, U_i, U_v, U_e, U_NU }; // L"native"
	const wchar_t StringConstant::US_new[] = { U_n, U_e, U_w, U_NU }; // L"new"
	const wchar_t StringConstant::US_null[] = {
		U_n, U_u, U_l, U_l, U_NU }; // L"null"
	const wchar_t StringConstant::US_package[] = {
		U_p, U_a, U_c, U_k, U_a, U_g, U_e, U_NU }; // L"package"
	const wchar_t StringConstant::US_private[] = {
		U_p, U_r, U_i, U_v, U_a, U_t, U_e, U_NU }; // L"private"
	const wchar_t StringConstant::US_protected[] = {
		U_p, U_r, U_o, U_t, U_e, U_c, U_t, U_e, U_d, U_NU }; // L"protected"
	const wchar_t StringConstant::US_public[] = {
		U_p, U_u, U_b, U_l, U_i, U_c, U_NU }; // L"public"
	const wchar_t StringConstant::US_return[] = {
		U_r, U_e, U_t, U_u, U_r, U_n, U_NU }; // L"return"
	const wchar_t StringConstant::US_short[] = {
		U_s, U_h, U_o, U_r, U_t, U_NU }; // L"short"
	const wchar_t StringConstant::US_static[] = {
		U_s, U_t, U_a, U_t, U_i, U_c, U_NU }; // L"static"
	const wchar_t StringConstant::US_strictfp[] = {
		U_s, U_t, U_r, U_i, U_c, U_t, U_f, U_p, U_NU }; // L"strictfp"
	const wchar_t StringConstant::US_super[] = {
		U_s, U_u, U_p, U_e, U_r, U_NU }; // L"super"
	const wchar_t StringConstant::US_switch[] = {
		U_s, U_w, U_i, U_t, U_c, U_h, U_NU }; // L"switch"
	const wchar_t StringConstant::US_synchronized[] = {
		U_s, U_y, U_n, U_c, U_h, U_r, U_o, U_n, U_i, U_z, U_e, U_d,
		U_NU }; // L"synchronized"
	const wchar_t StringConstant::US_this[] = {
		U_t, U_h, U_i, U_s, U_NU }; // L"this"
	const wchar_t StringConstant::US_throw[] = {
		U_t, U_h, U_r, U_o, U_w, U_NU }; // L"throw"
	const wchar_t StringConstant::US_throws[] = {
		U_t, U_h, U_r, U_o, U_w, U_s, U_NU }; // L"throws"
	const wchar_t StringConstant::US_transient[] = {
		U_t, U_r, U_a, U_n, U_s, U_i, U_e, U_n, U_t, U_NU }; // L"transient"
	const wchar_t StringConstant::US_true[] = {
		U_t, U_r, U_u, U_e, U_NU }; // L"true"
	const wchar_t StringConstant::US_try[] = { U_t, U_r, U_y, U_NU }; // L"try"
	const wchar_t StringConstant::US_void[] = {
		U_v, U_o, U_i, U_d, U_NU }; // L"void"
	const wchar_t StringConstant::US_volatile[] = {
		U_v, U_o, U_l, U_a, U_t, U_i, U_l, U_e, U_NU }; // L"volatile"
	const wchar_t StringConstant::US_while[] = {
		U_w, U_h, U_i, U_l, U_e, U_NU }; // L"while"

										 //
										 // Miscellaneous strings.
										 //
	const char StringConstant::U8S_help_header[] =
		"Jikes Compiler - " JIKES_VERSION_STRING
		"\nCopyright (C) IBM Corporation 1997-2003, 2004.\n"
		"- Licensed Materials - Program Property of IBM - All Rights Reserved.\n";
	const char StringConstant::U8S_command_format[] =
		"use: jikes [options] [@files] file.java...\n";

	//
	// Constant pool entries.
	//
	const char StringConstant::U8S_AnnotationDefault[] = {
		U_A, U_n, U_n, U_o, U_t, U_a, U_t, U_i, U_o, U_n,
		U_D, U_e, U_f, U_a, U_u, U_l, U_t, U_NU }; // "AnnotationDefault
	const char StringConstant::U8S_Code[] = { U_C, U_o, U_d, U_e, U_NU }; // "Code"
	const char StringConstant::U8S_ConstantValue[] = {
		U_C, U_o, U_n, U_s, U_t, U_a, U_n, U_t,
		U_V, U_a, U_l, U_u, U_e, U_NU }; // "ConstantValue"
	const char StringConstant::U8S_Deprecated[] = {
		U_D, U_e, U_p, U_r, U_e, U_c, U_a, U_t, U_e, U_d, U_NU }; // "Deprecated"
	const char StringConstant::U8S_EnclosingMethod[] = {
		U_E, U_n, U_c, U_l, U_o, U_s, U_i, U_n, U_g,
		U_M, U_e, U_t, U_h, U_o, U_d, U_NU }; // "EnclosingMethod"
	const char StringConstant::U8S_Exceptions[] = {
		U_E, U_x, U_c, U_e, U_p, U_t, U_i, U_o, U_n, U_s, U_NU }; // "Exceptions"
	const char StringConstant::U8S_InnerClasses[] = {
		U_I, U_n, U_n, U_e, U_r,
		U_C, U_l, U_a, U_s, U_s, U_e, U_s, U_NU }; // "InnerClasses"
	const char StringConstant::U8S_LineNumberTable[] = {
		U_L, U_i, U_n, U_e, U_N, U_u, U_m, U_b, U_e, U_r,
		U_T, U_a, U_b, U_l, U_e, U_NU }; // "LineNumberTable"
	const char StringConstant::U8S_LocalVariableTable[] = {
		U_L, U_o, U_c, U_a, U_l, U_V, U_a, U_r, U_i, U_a, U_b, U_l, U_e,
		U_T, U_a, U_b, U_l, U_e, U_NU }; // "LocalVariableTable"
	const char StringConstant::U8S_LocalVariableTypeTable[] = {
		U_L, U_o, U_c, U_a, U_l, U_V, U_a, U_r, U_i, U_a, U_b, U_l, U_e,
		U_T, U_y, U_p, U_e, U_T, U_a, U_b, U_l, U_e,
		U_NU }; // "LocalVariableTypeTable"
	const char StringConstant::U8S_RuntimeInvisibleAnnotations[] = {
		U_R, U_u, U_n, U_t, U_i, U_m, U_e,
		U_I, U_n, U_v, U_i, U_s, U_i, U_b, U_l, U_e,
		U_A, U_n, U_n, U_o, U_t, U_a, U_t, U_i, U_o, U_n, U_s,
		U_NU }; // 'RuntimeInvisibleAnnotations"
	const char StringConstant::U8S_RuntimeVisibleAnnotations[] = {
		U_R, U_u, U_n, U_t, U_i, U_m, U_e, U_V, U_i, U_s, U_i, U_b, U_l, U_e,
		U_A, U_n, U_n, U_o, U_t, U_a, U_t, U_i, U_o, U_n, U_s,
		U_NU }; // 'RuntimeVisibleAnnotations"
	const char StringConstant::U8S_RuntimeInvisibleParameterAnnotations[] = {
		U_R, U_u, U_n, U_t, U_i, U_m, U_e,
		U_I, U_n, U_v, U_i, U_s, U_i, U_b, U_l, U_e,
		U_P, U_a, U_r, U_a, U_m, U_e, U_t, U_e, U_r,
		U_A, U_n, U_n, U_o, U_t, U_a, U_t, U_i, U_o, U_n, U_s,
		U_NU }; // 'RuntimeInvisibleParameterAnnotations"
	const char StringConstant::U8S_RuntimeVisibleParameterAnnotations[] = {
		U_R, U_u, U_n, U_t, U_i, U_m, U_e, U_V, U_i, U_s, U_i, U_b, U_l, U_e,
		U_P, U_a, U_r, U_a, U_m, U_e, U_t, U_e, U_r,
		U_A, U_n, U_n, U_o, U_t, U_a, U_t, U_i, U_o, U_n, U_s,
		U_NU }; // 'RuntimeVisibleParameterAnnotations"
	const char StringConstant::U8S_Signature[] = {
		U_S, U_i, U_g, U_n, U_a, U_t, U_u, U_r, U_e, U_NU }; // "Signature"
	const char StringConstant::U8S_SourceFile[] = {
		U_S, U_o, U_u, U_r, U_c, U_e, U_F, U_i, U_l, U_e, U_NU }; // "SourceFile"
	const char StringConstant::U8S_StackMap[] = {
		U_S, U_t, U_a, U_c, U_k, U_M, U_a, U_p, U_NU }; // "StackMap"
	const char StringConstant::U8S_Synthetic[] = {
		U_S, U_y, U_n, U_t, U_h, U_e, U_t, U_i, U_c, U_NU }; // "Synthetic"

															 //
															 // ASCII file names.
															 //
	const char StringConstant::U8S_DO_class[] = {
		U_DO, U_c, U_l, U_a, U_s, U_s, U_NU }; // ".class"
	const char StringConstant::U8S_DO_java[] = {
		U_DO, U_j, U_a, U_v, U_a, U_NU }; // ".java"
	const char StringConstant::U8S_DO_tok[] = {
		U_DO, U_t, U_o, U_k, U_NU }; // ".tok"
	const char StringConstant::U8S_DO_u[] = { U_DO, U_u, U_NU }; // ".u"
	const char StringConstant::U8S_LP[] = { U_LP, U_NU }; // "("
	const char StringConstant::U8S_RP[] = { U_RP, U_NU }; // ")"
	const char StringConstant::U8S_SL[] = { U_SL, U_NU }; // "/"

														  //
														  // Convert number to string.
														  //
	const char StringConstant::U8S_NaN[] = { U_N, U_a, U_N, U_NU }; // "NaN"
	const char StringConstant::U8S_pos_Infinity[] = {
		U_I, U_n, U_f, U_i, U_n, U_i, U_t, U_y, U_NU }; // "Infinity"
	const char StringConstant::U8S_neg_Infinity[] = {
		U_MINUS, U_I, U_n, U_f, U_i, U_n, U_i, U_t, U_y, U_NU }; // "-Infinity"
	const char StringConstant::U8S_pos_Zero[] = { U_0, U_DOT, U_0, U_NU }; // "0.0"
	const char StringConstant::U8S_neg_Zero[] = {
		U_MINUS, U_0, U_DOT, U_0, U_NU }; // "-0.0"


} // Close namespace Jikes block



