
#ifndef _java_model_INCLUDED_INCLUDED
#define _java_model_INCLUDED_INCLUDED

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IJavaSearchConstants 
	{
	public:

		/**
		* The nature of searched element or the nature
		* of match in unknown.
		*/
		static const int UNKNOWN = -1;

		/* Nature of searched element */

		/**
		* The searched element is a type.
		*/
		static const int TYPE = 0;

		/**
		* The searched element is a method.
		*/
		static const int METHOD = 1;

		/**
		* The searched element is a package.
		*/
		static const int PACKAGE = 2;

		/**
		* The searched element is a constructor.
		*/
		static const int CONSTRUCTOR = 3;

		/**
		* The searched element is a field.
		*/
		static const int FIELD = 4;

		/**
		* The searched element is a class.
		* More selective than using TYPE
		*/
		static const int CLASS = 5;

		/**
		* The searched element is an interface.
		* More selective than using TYPE
		*/
		static const int INTERFACE = 6;

		/* Nature of match */

		/**
		* The search result is a declaration.
		* Can be used in conjunction with any of the nature of searched elements
		* so as to better narrow down the search.
		*/
		static const int DECLARATIONS = 0;

		/**
		* The search result is a type that implements an interface.
		* Used in conjunction with either TYPE or CLASS or INTERFACE, it will
		* respectively search for any type implementing/extending an interface, or
		* rather exclusively search for classes implementing an interface, or interfaces
		* extending an interface.
		*/
		static const int IMPLEMENTORS = 1;

		/**
		* The search result is a reference.
		* Can be used in conjunction with any of the nature of searched elements
		* so as to better narrow down the search.
		* References can contain implementers since they are more generic kind
		* of matches.
		*/
		static const int REFERENCES = 2;

		/**
		* The search result is a declaration, a reference, or an implementer
		* of an interface.
		* Can be used in conjunction with any of the nature of searched elements
		* so as to better narrow down the search.
		*/
		static const int ALL_OCCURRENCES = 3;

		/**
		* When searching for field matches, it will exclusively find read accesses, as
		* opposed to write accesses. Note that some expressions are considered both
		* as field read/write accesses: e.g. x++; x+= 1;
		*
		* @since 2.0
		*/
		static const int READ_ACCESSES = 4;

		/**
		* When searching for field matches, it will exclusively find write accesses, as
		* opposed to read accesses. Note that some expressions are considered both
		* as field read/write accesses: e.g. x++; x+= 1;
		*
		* @since 2.0
		*/
		static const int WRITE_ACCESSES = 5;

		/* Syntactic match modes */

		/**
		* The search pattern matches exactly the search result,
		* i.e. the source of the search result equals the search pattern.
		*/
		static const int EXACT_MATCH = 0;
		/**
		* The search pattern is a prefix of the search result.
		*/
		static const int PREFIX_MATCH = 1;
		/**
		* The search pattern contains one or more wild cards ('*') where a
		* wild-card can replace 0 or more characters in the search result.
		*/
		static const int PATTERN_MATCH = 2;


		/* Case sensitivity */

		/**
		* The search pattern matches the search result only
		* if cases are the same.
		*/
	   static const	bool  CASE_SENSITIVE = true;
		/**
		* The search pattern ignores cases in the search result.
		*/
	   static const	bool CASE_INSENSITIVE = false;


		/* Waiting policies */

		/**
		* The search operation starts immediately, even if the underlying indexer
		* has not finished indexing the workspace. Results will more likely
		* not contain all the matches.
		*/
		//static const int FORCE_IMMEDIATE_SEARCH = IJob.ForceImmediate;
		/**
		* The search operation throws an <code>org.eclipse.core.runtime.OperationCanceledException</code>
		* if the underlying indexer has not finished indexing the workspace.
		*/
	//	static const int CANCEL_IF_NOT_READY_TO_SEARCH = IJob.CancelIfNotReady;
		/**
		* The search operation waits for the underlying indexer to finish indexing
		* the workspace before starting the search.
		*/
		//static const int WAIT_UNTIL_READY_TO_SEARCH = IJob.WaitUntilReady;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

