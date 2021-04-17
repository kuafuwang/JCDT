#ifndef AstMemberValue_INCLUDED
#define AstMemberValue_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AbstractStatement.h"
namespace Jikes { // Open namespace Jikes block


	//
	// This is the superclass of constructs which can appear in an array
	// initializer, including annotations added by JSR 175.
	//
	class AstMemberValue : public AbstractStatement
	{
	public:
		// The field or method this expression resolves to, or the annotation type
		// that the annotation resolves to.
		Symbol* symbol;

		static AstMemberValue* MemberValueCast(Ast *node)
		{
			return DYNAMIC_CAST<AstMemberValue*>
				((node->class_tag == EXPRESSION || node->kind == ANNOTATION ||
					node->kind == ARRAY_INITIALIZER) ? node : NULL);
		}

		  AstMemberValue(AstKind k, AstTag t = NO_TAG)
			: AbstractStatement(k, t), symbol(nullptr)
		{}
		~AstMemberValue() {}

		TypeSymbol* Type();
	};



} // Close namespace Jikes block


#endif // AstMemberValue_INCLUDED
