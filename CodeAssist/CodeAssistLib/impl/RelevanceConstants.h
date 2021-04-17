
#ifndef RelevanceConstants_Jikes_CodeAssist_INCLUDED
#define RelevanceConstants_Jikes_CodeAssist_INCLUDED

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{

	enum  RelevanceConstants {
		R_INCREASE_WEIGHT = 2,
		R_DEFAULT =   5,
		R_INTERESTING =   5,
		
		R_CASE =   10,
		R_CAMEL_CASE =   5,
		R_EXACT_NAME =   4,

		R_VOID =   -5,
		R_EXPECTED_TYPE =   20,
		R_EXACT_EXPECTED_TYPE =   30,
		R_INTERFACE =   20,
		R_CLASS =   20,
		R_ENUM =   20,
		R_ANNOTATION =   20,
		R_EXCEPTION =   20,
		R_ENUM_CONSTANT =   5,
		R_ABSTRACT_METHOD =   20,
		R_NON_STATIC =   11,
		R_UNQUALIFIED =   3,
		R_QUALIFIED =   2,
		R_PACKAGE =   4,
		R_NAME_FIRST_PREFIX =   6,
		R_NAME_PREFIX =   5,
		R_NAME_FIRST_SUFFIX =   4,
		R_NAME_SUFFIX =   3,
		R_NAME_LESS_NEW_CHARACTERS =   15,
		R_METHOD_OVERIDE =   3,
		R_NON_RESTRICTED =   3,
		R_TRUE_OR_FALSE =   1,
		R_INLINE_TAG =   31,
		R_VALUE_TAG =   31,
		R_NON_INHERITED =   2,
		R_NO_PROBLEMS =   1,
		R_RESOLVED =   1,
		R_TARGET =   5,
		R_FINAL =   3,
		R_CASE_IN_SWITCH =   30,
	};


}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

