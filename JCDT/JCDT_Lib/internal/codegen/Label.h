
#ifndef jcdt_codegen_Label_INCLUDED
#define jcdt_codegen_Label_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"



namespace Jikes { // Open namespace Jikes block



	class Label
	{
	public:

		class LabelUse
		{
		public:
			int use_length; // length of use (2 or 4 bytes)
			int op_offset; // length of use from opcode starting instruction
			int use_offset; // offset in code stream of use

			LabelUse() : use_length(0), op_offset(0), use_offset(0) {}

			LabelUse(int _length, int _op_offset, int _use)
				: use_length(_length),
				op_offset(_op_offset),
				use_offset(_use)
			{}
		};

		bool defined; // boolean, set when value is known
		u2 definition; // offset of definition point of label
		Tuple<LabelUse> uses;

		Label() : defined(false), definition(0) {}

		//
		// All used labels should have been completed and reset, otherwise a goto
		// will cause an infinite loop because it was emitted with an offset of 0.
		//
		~Label() { assert(!uses.Length()); }

		void Reset()
		{
			uses.Reset();
			defined = false;
			definition = 0;
		}
	};


} // Close namespace Jikes block


#endif // _INCLUDED

