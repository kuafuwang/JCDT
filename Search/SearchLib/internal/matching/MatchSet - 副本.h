#ifndef  MatchSet_JIKES_SERACH_INCLUDE
#define MatchSet_JIKES_SERACH_INCLUDE
#include <vector>
#include <boost/unordered/unordered_map.hpp>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <SearchLib/internal/pattern/SearchPattern.h>
using namespace std;
namespace Jikes
{
	class Ast;
	namespace JavaModel
	{
		class IJavaElement;
		class IMethod;
		class IInitializer;
		class ISourceRange;
		class IType;
		class IField;
		class Member;
	}
}
namespace Jikes { // Open namespace Jikes block

namespace Search{

	class FileResultElement;
	class MatchingOpenable;
	class SearchPattern;
	class MemberInFileElemnt;
	class SearchMatch;
	class MemberInFileElemnt;
	
	class MatchSet
	{
	public:
		
		void getResult(FileResultElement* copyer, bool _copy_children);
		boost::unordered_map<JavaModel::IJavaElement*, vector< SearchMatch* > > result;

		MatchingOpenable* opnable;

		SearchPattern* pattern;

		int matchContainer;
		
		BaseLexStream* lex;

		void setLex(BaseLexStream* _lex){
			lex = _lex;
		}
		
		class LocalDeclarationVisitor :public ASTVisitor
		{
		public:
			MatchSet* _set;
			JavaModel::IJavaElement* enclosingElement;
			bool visit(TypeDeclaration * node, AstNodeScope* scope)override;
		};

		bool cuHasBeenResolved = false;

		/**
		* Set of matching ast nodes that don't need to be resolved.
		*/
		boost::unordered_map<Ast*, int>  _matchingNodes;

		/**
		* Set of potential matching ast nodes. They need to be resolved
		* to determine if they really match the search pattern.
		*/
		boost::unordered_map<Ast*, int> _potentialMatchingNodes;


		MatchSet(MatchingOpenable* _opnable);
		~MatchSet();
		void addPossibleMatch(Ast* node) {
			_potentialMatchingNodes.insert(
				make_pair(node,
					SearchPattern::POSSIBLE_MATCH)
			);
		}
		void addTrustedMatch(Ast* node) {
			_matchingNodes.insert(
				make_pair(node,
					SearchPattern::ACCURATE_MATCH)
			);
		}
		int checkMatching(Ast* node) {
			return  pattern->matchCheck(node, this);
		}
		bool isEmpty() const {
			return
				_potentialMatchingNodes.empty() == true
				&& _matchingNodes.empty() == true;
		}
		/**
		* Returns the matching nodes that are in the given range.
		*/
		std::vector<std::pair< Ast*, int> >* matchingNodes(int start, int end) {
			return nodesInRange(start, end, _matchingNodes);
		}
		bool needsResolve() const{
			return _potentialMatchingNodes.empty() != true;
		}

		/**
		* Returns the matching nodes that are in the given range in the source order.
		*/
		std::vector<std::pair< Ast*, int> >* nodesInRange(int start, int end, boost::unordered_map<Ast*, int>& set);

		/**
		* Returns the potential matching nodes that are in the given range.
		*/
		std::vector<std::pair< Ast*, int> >* potentialMatchingNodes(int start, int end) {
			return nodesInRange(start, end, _potentialMatchingNodes);
		}

		bool HasNodeInpotentialMatchingNodes(unsigned int start, unsigned int end) {
			for (auto it : _potentialMatchingNodes) {
				auto node = it.first;
				if (node &&  node->LeftToken() && node->LeftToken()->start_location >= start)
				{
					if (node->RightToken() && node->RightToken()->EndLocation() <= end)
					{
						return  true;
					}
				}
			}
			return false;
		}
		int removePossibleMatch(Ast* node) {
			auto find_it = _potentialMatchingNodes.find(node);
			if (find_it == _potentialMatchingNodes.end())
			{
				return 0;
			}
			else
			{
				auto _ret = find_it->second;
				_potentialMatchingNodes.erase(find_it);
				return _ret;
			}
			return 0;
		}
		int removeTrustedMatch(Ast* node) {
			auto find_it = _matchingNodes.find(node);
			if (find_it == _matchingNodes.end())
			{
				return 0;
			}
			else
			{
				auto _ret = find_it->second;
				_matchingNodes.erase(find_it);
				return _ret;
			}
			return 0;
		}
		/**
		* Visit the given method declaration and report the nodes that match exactly the
		* search pattern (ie. the ones in the matching nodes set)
		* Note that the method declaration has already been checked.
		*/
		void reportMatching(AbstractMethodDeclaration* method,
			JavaModel::IJavaElement* parent);
		/**
		* Visit the given parse tree and report the nodes that match exactly the
		* search pattern.
		*/
		void reportMatching(CompilationUnitDeclaration* unit);
		/**
		* Visit the given field declaration and report the nodes that match exactly the
		* search pattern (ie. the ones in the matching nodes set)
		* Note that the field declaration has already been checked.
		*/
		void reportMatching(AstVariableDeclarator* field, JavaModel::IJavaElement* parent,
			TypeDeclaration* type);

		void reportMatching(AstType* fieldType, JavaModel::IJavaElement* parent,
			TypeDeclaration* type);

		void reportMatching(Initializer* initializer, JavaModel::IJavaElement* parent,
			TypeDeclaration* type);
		/**
		* Visit the given type declaration and report the nodes that match exactly the
		* search pattern (ie. the ones in the matching nodes set)
		* Note that the type declaration has already been checked.
		*/
		void reportMatching(TypeDeclaration* type, JavaModel::IJavaElement* parent);

		JavaModel::IType* lookupType(TypeSymbol* typeBinding);   

		void report(
			Ast* reference,
			JavaModel::IJavaElement* element,
			int accuracy,int extra_info);

		void report(int sourceStart, int sourceEnd, JavaModel::IJavaElement* element, int accuracy, int extra_info);


		/**
		* Reports the given field declaration to the search requestor.
		*/
		void reportFieldDeclaration(
			AstVariableDeclarator* fieldDeclaration,
			JavaModel::IJavaElement* parent,
			int accuracy);

		/**
		* Reports the given import to the search requestor.
		*/
		void reportImport(ImportReference* reference, int accuracy);

		/**
		* Reports the given method declaration to the search requestor.
		*/
		void reportMethodDeclaration(
			AbstractMethodDeclaration* methodDeclaration,
			JavaModel::IJavaElement* parent,
			int accuracy);

		/**
		* Reports the given package declaration to the search requestor.
		*/
		void reportPackageDeclaration(ImportReference* node) {
			// TBD
		}

		/**
		* Reports the given package reference to the search requestor.
		*/
		void reportPackageReference(ImportReference* node) {
			// TBD
		}

		/**
		* Reports the given reference to the search requestor.
		* It is done in the given method and the method's defining types
		* have the given simple names.
		*/
		void reportReference(
			Ast* reference,
			AbstractMethodDeclaration* methodDeclaration,
			JavaModel::IJavaElement* parent,
			int accuracy);

		/**
		* Reports the given reference to the search requestor.
		* It is done in the given field and given type.
		* The field's defining types have the given simple names.
		*/
		void reportReference(
			Ast* reference,
			TypeDeclaration* typeDeclaration,
			AstVariableDeclarator* fieldDeclaration,
			JavaModel::IJavaElement* parent,
			int accuracy);
		JavaModel::IJavaElement* createImportHandle(ImportReference* importRef);

		void reportReference(
			Ast* reference,
			TypeDeclaration* typeDeclaration,
			AstType* fieldDeclaration,
			JavaModel::IJavaElement* parent,
			int accuracy);

		void reportReference(
			Ast* reference,
			TypeDeclaration* typeDeclaration,
			Initializer* fieldDeclaration,
			JavaModel::IJavaElement* parent,
			int accuracy);

		/**
		* Reports the given super type reference to the search requestor.
		* It is done in the given defining type (with the given simple names).
		*/
		void reportSuperTypeReference(
			AstType* typeRef,
			JavaModel::IJavaElement* type,
			int accuracy);

		void reportBinaryMatch(JavaModel::Member* binaryMember, int accuracy);;




		/**
		* Reports the given type declaration to the search requestor.
		*/
		void reportTypeDeclaration(
			TypeDeclaration* typeDeclaration,
			JavaModel::IJavaElement* parent,
			int accuracy);

		/**
		* Creates an IType from the given simple top level type name.
		*/
		JavaModel::IType* createTypeHandle(const wstring& simpleTypeName);



		/**
		* Creates an IType from the given simple inner type name and parent type.
		*/
		JavaModel::IType* createTypeHandle(JavaModel::IType* parent, const wstring& simpleTypeName);
		JavaModel::IField* createFieldHandle(AstVariableDeclarator* field, JavaModel::IType* type);
		JavaModel::IMethod* createMethodHandle(AbstractMethodDeclaration* method, JavaModel::IType* type);
		JavaModel::IInitializer* createInitializerHandle(Initializer* initializer,JavaModel::IType* type);
	};

	


}// Close namespace Search block
	

} // Close namespace Jikes block

#endif