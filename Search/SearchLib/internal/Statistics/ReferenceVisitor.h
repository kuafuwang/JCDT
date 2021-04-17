
#ifndef JCDT_ReferenceVisitor__Search_H_FLAG_
#define JCDT_ReferenceVisitor_Search_H_FLAG_

#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <map>


using std::set;
using std::wstring;

namespace Jikes {


namespace Search {
	
	/*class ScopeSet
	{
	public:
		Ast* original;
		vector<  std::pair<Ast*, AstNodeScope*> * > matchs;
		ScopeSet(Ast* o);
		~ScopeSet();
	};
	class ReferenceVisitorResult
	{
	public:

		
		map<FileSymbol*,ScopeSet*> all_sets;
		unsigned int number;
		ReferenceVisitorResult();
		~ReferenceVisitorResult();
	};*/
	class FindReferenceOption;

	class ReferenceVisitorBase:public ASTVisitor
	{
	public:
		ReferenceVisitorBase(FindReferenceOption* _option);
		
		bool visit(AstMethodBody * node, AstNodeScope* scope);

		void endVisit(AstMethodBody * node, AstNodeScope* scope);
		bool visit(AstVariableDeclarator* node, AstNodeScope* scope);

		map<Symbol*, AstNodeScope* >  localFieldScopeDatas;


		map<Symbol*, unsigned int>  losers;
		map<Symbol*, unsigned int  >  candidate;

		map<Symbol*, int >  templocalFieldCandidate;

		map<Symbol*, int >  templocalFieldLosers;

		map<Symbol*, int > theOnes;

		Ast* topMethodBody;

		virtual void clear();

		FindReferenceOption* option;



		set<FileSymbol*> all_files;
	};

	class ZeroReferenceVisitor :public ReferenceVisitorBase
	{
	public:
		ZeroReferenceVisitor(FindReferenceOption* _option);

		

		virtual bool visit(FieldReference* node, AstNodeScope* scope);
		virtual bool visit(AstName* node, AstNodeScope* scope);


		virtual bool visit(AstMethodInvocation* node, AstNodeScope* scope);
		virtual bool visit(AstClassCreationExpression* node, AstNodeScope* scope);

		virtual bool visit(AstThisCall* node, AstNodeScope* scope);

		virtual bool visit(AstSuperCall* node, AstNodeScope* scope);

		bool ProcessMethod(Symbol* symbol);
		bool PrcoessVariable(Symbol* symbol);
		void clear();

	};
	class ReferenceVisitor :public ReferenceVisitorBase
	{
	public:
		bool PrcoessVariable(Symbol* node_symol);
		void Filter(Symbol* node_symol);
		ReferenceVisitor(FindReferenceOption* _option);
	
		virtual bool visit(FieldReference* node, AstNodeScope* scope);
	
		virtual bool visit(AstName* node, AstNodeScope* scope);
	

		virtual bool visit(AstMethodInvocation* node, AstNodeScope* scope);
		virtual bool visit(AstClassCreationExpression* node, AstNodeScope* scope);

		virtual bool visit(AstThisCall* node, AstNodeScope* scope);

		virtual bool visit(AstSuperCall* node, AstNodeScope* scope);
		bool ProcessMethod(Symbol* symbol);
	};
	
}//namespace Search

} // Close namespace Jikes block


#endif // JCDT_ReferenceVisitor_Search_H_FLAG_
