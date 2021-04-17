
#ifndef BinaryExpressionTag_Jikes_INCLUDED
#define BinaryExpressionTag_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block

	class BinaryExpressionEnum{
	public:
		enum BinaryExpressionTag
		{
			NONE,
			STAR,
			SLASH,
			MOD,
			PLUS,
			MINUS,
			LEFT_SHIFT,
			RIGHT_SHIFT,
			UNSIGNED_RIGHT_SHIFT,
			LESS,
			GREATER,
			AND,
			XOR,
			IOR,
			AND_AND,
			OR_OR,

			LESS_EQUAL,
			GREATER_EQUAL,
			EQUAL_EQUAL,
			NOT_EQUAL,

			_num_kinds
		};
	};

} // Close namespace Jikes block


#endif // _INCLUDED

