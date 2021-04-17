
#ifndef PostUnaryExpressionTag_Jikes_INCLUDED
#define PostUnaryExpressionTag_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block

	class PostUnaryExpressionEnum {
	public:

		enum PostUnaryExpressionTag
		{
			NONE,
			PLUSPLUS,
			MINUSMINUS,

			_num_kinds
		};
	};
} // Close namespace Jikes block


#endif // _INCLUDED

