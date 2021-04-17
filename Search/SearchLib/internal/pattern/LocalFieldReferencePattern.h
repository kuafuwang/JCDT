#ifndef  LocalFieldReferencePattern_JIKES_SERACH_INCLUDE
#define LocalFieldReferencePattern_JIKES_SERACH_INCLUDE
#include "MultipleSearchPattern.h"
#include <JCDT_Lib/internal/ast/AstFieldAccess.h>
#include <boost/unordered/unordered_set.hpp>
namespace Jikes {namespace JavaModel {
	class ISourceRange;
}
}

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class LocalFieldReferencePattern:public MultipleSearchPattern
	{
	public:
	
		LocalFieldReferencePattern(
			const string& _doc_path,
			JavaModel::ISourceRange* _rng,
			 std::wstring _name,
			int _matchMode,
			bool _isCaseSensitive,
			std::wstring _typeQualification,
			std::wstring _typeSimpleName,
			bool _readAccess,
			bool _writeAccess);
		void ReportReference(Ast* reference, JavaModel::IJavaElement* element, int accuracy, MatchSet* locator, int extra_info);
		void matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
			int accuracy, MatchSet* locator) override;
		bool IsMayInTheFile(BaseLexStream*) override;

		boost::unordered_set<Ast*> writesNodes;
		string   path;
		JavaModel::ISourceRange* rng;
		unsigned start_loc;
		unsigned int end_loc;
		// selector	
		 std::wstring name;

	
		// type
		 std::wstring typeQualification;
		 std::wstring typeSimpleName;

		// read/write access
		 bool readAccess = true;
		 bool writeAccess = true;

		 /**
		 * Returns whether a field reference or name reference will need to be resolved to
		 * find out if this method pattern matches it.
		 */
		  bool needsResolve() {
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
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif