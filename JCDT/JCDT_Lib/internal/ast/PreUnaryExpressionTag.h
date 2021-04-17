
#ifndef PreUnaryExpressionTag_Jikes_INCLUDED
#define PreUnaryExpressionTag_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block

	class  PreUnaryExpressionEnum {
	public:
		enum PreUnaryExpressionTag
		{
			NONE,
			PLUSPLUS,
			MINUSMINUS,
			PLUS,
			MINUS,
			TWIDDLE,
			NOT,

			_num_kinds
		};
	};

} // Close namespace Jikes block


#endif // _INCLUDED

