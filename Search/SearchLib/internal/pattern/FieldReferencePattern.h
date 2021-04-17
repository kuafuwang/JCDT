#ifndef  FieldReferencePattern_JIKES_SERACH_INCLUDE
#define FieldReferencePattern_JIKES_SERACH_INCLUDE
#include "MultipleSearchPattern.h"
#include <JCDT_Lib/internal/ast/AstFieldAccess.h>
#include <boost/unordered/unordered_set.hpp>
namespace Jikes { // Open namespace Jikes block

namespace Search{
	class FieldReferencePattern:public MultipleSearchPattern
	{
	public:
		FieldReferencePattern(
			 std::wstring _name,
			int _matchMode,
			bool _isCaseSensitive,
			std::wstring _declaringQualification,
			std::wstring _declaringSimpleName,
			std::wstring _typeQualification,
			std::wstring _typeSimpleName,
			bool _readAccess,
			bool _writeAccess);

		// selector	
		 std::wstring name;
		// declaring type
		 std::wstring declaringQualification;
		 std::wstring declaringSimpleName;

		// type
		 std::wstring typeQualification;
		 std::wstring typeSimpleName;

		// read/write access
		 bool readAccess = true;
		 bool writeAccess = true;

		 boost::unordered_set<Ast*> writesNodes;
		
		 /**
		 * Returns whether a field reference or name reference will need to be resolved to
		 * find out if this method pattern matches it.
		 */
		  bool needsResolve() {

			 // declaring type
			 if (declaringSimpleName.empty() != true || declaringQualification.empty() != true)
				 return true;

			 // return type
			 if (typeSimpleName.empty() != true || typeQualification.empty() != true)
				 return true;

			 return false;
		 }
		  /**
		  * @see SearchPattern#matchCheck(AstNode, MatchSet)
		  */
	
		/**
		* @see SearchPattern#matchContainer()
		*/
		int matchContainer();
		
		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);

		/**
		* Returns whether this field reference pattern matches the given field reference.
		* Look at resolved information only if specified.
		*/
		int matchLevel(FieldReference* fieldRef, bool resolve);
		/**
		* Returns whether this field reference pattern matches the given field declaration in
		* write access.
		* Look at resolved information only if specified.
		*/
		int matchLevel(AstVariableDeclarator* fieldDecl, bool resolve);

		/**
		* Returns whether this field reference pattern matches the given name reference.
		* Look at resolved information only if specified.
		*/
		int matchLevel(AstName* nameRef, bool resolve);

		/**
		* Returns whether this field reference pattern matches the given field binding.
		*/
		int matchLevel(Symbol* _symbol) override;
		std::wstring indexEntryPrefix() override{
			return name;
		};

		virtual void matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
			int accuracy, MatchSet* locator) override;

	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif