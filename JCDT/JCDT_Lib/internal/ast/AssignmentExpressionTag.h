
#ifndef AssignmentExpressionTag_Jikes_INCLUDED
#define AssignmentExpressionTag_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block
	class AssignmentExpressionEnum {
	public:
		enum AssignmentExpressionTag
		{
			NONE,
			SIMPLE_EQUAL,
			STAR_EQUAL,
			SLASH_EQUAL,
			MOD_EQUAL,
			PLUS_EQUAL,
			MINUS_EQUAL,
			LEFT_SHIFT_EQUAL,
			RIGHT_SHIFT_EQUAL,
			UNSIGNED_RIGHT_SHIFT_EQUAL,

			AND_EQUAL,
			XOR_EQUAL,
			IOR_EQUAL,

			_num_kinds
		};
	};

} // Close namespace Jikes block


#endif // _INCLUDED

