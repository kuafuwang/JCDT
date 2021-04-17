#ifndef AstAnnotation_INCLUDED
#define AstAnnotation_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstMemberValue.h"
namespace Jikes { // Open namespace Jikes block



	//
	// Annotation is added by JSR 175. This covers NormalAnnotation,
	// MarkerAnnotation, and SingleMemberAnnotation in the grammar.
	//
	class AstAnnotation : public AstMemberValue
	{
		AstStoragePool* pool;
		AstArray<AstMemberValuePair*>* member_value_pairs;

	public:
		Token* at_token;
		AstName* name;
		Token* right_paren_token_opt;
		static AstAnnotation* AnnotationCast(Ast* node)
		{
			return DYNAMIC_CAST<AstAnnotation*>(node->kind == ANNOTATION ? node : NULL);
		}

		AstAnnotation(AstStoragePool* p);

		~AstAnnotation() {}

		  AstMemberValuePair*& MemberValuePair(unsigned i);

		  unsigned NumMemberValuePairs();
		  void AllocateMemberValuePairs(unsigned estimate = 1);
		  void AddMemberValuePair(AstMemberValuePair*);

#ifdef JIKES_DEBUG
		 void Print(Ostream&,LexStream&) override;
		 void Unparse(Ostream&, LexStream*) override;
#endif // JIKES_DEBUG

		Ast* Clone(AstStoragePool*) override;

		 void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		Token* LeftToken() override;

		Token* RightToken() override;
	};



} // Close namespace Jikes block


#endif // AstAnnotation_INCLUDED
