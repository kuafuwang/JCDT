#pragma once

#include <CodeAssistLib/complete/ICompletionRequestor.h>
#include <map>
#include <set>
namespace Jikes { // Open namespace Jikes block
	class AstName;

	namespace Correction {
		class SimilarElement;
		class ProblemPosition;

		class SimilarElementsRequestor :public CodeAssist::ICompletionRequestor
		{
		public:


			static const int CLASSES = 1 << 1;
			static const int INTERFACES = 1 << 2;
			static const int PRIMITIVETYPES = 1 << 3;
			static const int VOIDTYPE = 1 << 4;
			static const int REF_TYPES = CLASSES | INTERFACES;
			static const int ALL_TYPES = PRIMITIVETYPES | REF_TYPES;
			static const int METHODS = 1 << 5;
			static const int FIELDS = 1 << 6;
			static const int LOCALS = 1 << 7;
			static const int VARIABLES = FIELDS | LOCALS;

			static vector<wstring>  PRIM_TYPES;//$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$

			wstring fPreferredType;
			 int fNumberOfArguments;
			 int fKind;
			 wstring fName;

			 std::set<SimilarElement*> 	 fResult;

			 std::set<SimilarElement*> 	 fOthers;

			static void findSimilarElement(vector<SimilarElement*>&, ProblemPosition* problemPos, AstName* name, int kind);

			static void findSimilarVariableElement(vector<SimilarElement*>&, ProblemPosition* problemPos,
				const wstring& name, int kind);

			static void findSimilarTypeElement(vector<SimilarElement*>&,
				ProblemPosition* problemPos ,const wstring& typeName,
				int kind);

			static void findSimilarElement(vector<SimilarElement*>&, ProblemPosition* problemPos, const wstring& name, int kind, int nArguments, const wstring& preferredType);
			void process(vector<SimilarElement*>& result, ProblemPosition* problemPos);


	
			/**
			* Constructor for SimilarElementsRequestor.
			*/
			std::set<wstring>   records;

			SimilarElementsRequestor(const wstring& name, int kind, int nArguments, const wstring& preferredType);


			SimilarElementsRequestor();
			~SimilarElementsRequestor();
			void ClearAndDelete();
			void addResult(SimilarElement* elem);
			void processKeywords();
			void addOther(SimilarElement* elem);

			void addType(int kind, const std::wstring& packageName, 
				const std::wstring& typeName,
				const	std::wstring& completionName,
				double relevance,
				int _modifiers);
			void addVariable(
				int kind,
				const  std::wstring& name, 
				const	std::wstring& typePackageName,
				const	std::wstring& typeName,
				double relevance,
				int modifiers);
		

			 void acceptAnonymousType(
				const wstring& superTypePackageName,
				const wstring& superTypeName,
				const vector< wstring>& parameterPackageNames,
				const vector< wstring>& parameterTypeNames,
				const vector< wstring>& parameterNames,
				const wstring& completionName,
				int modifiers,
				JavaModel::ISourceRange*  completionRang,
				double  relevance,
				bool needMatch = true) override;
			/**
			* Code assist notification of a class completion.
			*
			* @param packageName Declaring package name of the class.
			* @param className Name of the class.
			* @param completionName The completion for the class.	Can include ';' for imported classes.
			* @param modifiers The modifiers of the class.
			* @param completionStart The start position of insertion of the name of the class.
			* @param completionEnd The end position of insertion of the name of the class.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*/
				void acceptClass(
				const wstring& packageName,
				const wstring& className,
				const wstring& completionName,
				int modifiers,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of a compilation error detected during completion.
			*  @param error Only problems which are categorized as non-syntax errors are notified to the
			*     requestor, warnings are silently ignored.
			*		In case an error got signalled, no other completions might be available,
			*		therefore the problem message should be presented to the user.
			*		The source positions of the problem are related to the source where it was
			*		detected (might be in another compilation unit, if it was indirectly requested
			*		during the code assist process).
			*      Note: the problem knows its originating file name.
			*/

			//void acceptError(IProblem error);

			/**
			* Code assist notification of a field completion.
			*
			* @param declaringTypePackageName Name of the package in which the type that contains this field is declared.
			* @param declaringTypeName Name of the type declaring this new field.
			* @param name Name of the field.
			* @param typePackageName Name of the package in which the type of this field is declared.
			* @param typeName Name of the type of this field.
			* @param completionName The completion for the field.
			* @param modifiers The modifiers of this field.
			* @param completionStart The start position of insertion of the name of this field.
			* @param completionEnd The end position of insertion of the name of this field.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Base types are in the form "int" or "boolean".
			*    Array types are in the qualified form "M[]" or "int[]".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*/
				void acceptField(
				const wstring& declaringTypePackageName,
				const wstring& declaringTypeName,
				const wstring& name,
				const wstring& typePackageName,
				const wstring& typeName,
				const wstring& completionName,
				int modifiers,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of an interface completion.
			*
			* @param packageName Declaring package name of the interface.
			* @param className Name of the interface.
			* @param completionName The completion for the interface.	Can include ';' for imported interfaces.
			* @param modifiers The modifiers of the interface.
			* @param completionStart The start position of insertion of the name of the interface.
			* @param completionEnd The end position of insertion of the name of the interface.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*/
			 void acceptInterface(
				const wstring& packageName,
				const wstring& interfaceName,
				const wstring& completionName,
				int modifiers,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of a keyword completion.
			* @param keywordName The keyword source.
			* @param completionStart The start position of insertion of the name of this keyword.
			* @param completionEnd The end position of insertion of the name of this keyword.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*/
			  void acceptKeyword(const wstring& keywordName,
				JavaModel::ISourceRange*  completionRang, double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of a label completion.
			*
			* @param labelName The label source.
			* @param completionStart The start position of insertion of the name of this label.
			* @param completionEnd The end position of insertion of the name of this label.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*/
				void acceptLabel(const wstring& labelName, JavaModel::ISourceRange*
				completionRang, double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of a local variable completion.
			*
			* @param name Name of the new local variable.
			* @param typePackageName Name of the package in which the type of this new local variable is declared.
			* @param typeName Name of the type of this new local variable.
			* @param modifiers The modifiers of this new local variable.
			* @param completionStart The start position of insertion of the name of this new local variable.
			* @param completionEnd The end position of insertion of the name of this new local variable.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Base types are in the form "int" or "boolean".
			*    Array types are in the qualified form "M[]" or "int[]".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*/
			 void acceptLocalVariable(
				const wstring& name,
				const wstring& typePackageName,
				const wstring& typeName,
				int modifiers,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of a method completion.
			*
			* @param declaringTypePackageName Name of the package in which the type that contains this new method is declared.
			* @param declaringTypeName Name of the type declaring this new method.
			* @param selector Name of the new method.
			* @param parameterPackageNames Names of the packages in which the parameter types are declared.
			*    	Should contain as many elements as parameterTypeNames.
			* @param parameterTypeNames Names of the parameters types.
			*    	Should contain as many elements as parameterPackageNames.
			* @param returnTypePackageName Name of the package in which the return type is declared.
			* @param returnTypeName Name of the return type of this new method, should be <code>null</code> for a constructor.
			* @param completionName The completion for the method. Can include zero, one or two brackets. If the closing bracket is included, then the cursor should be placed before it.
			* @param modifiers The modifiers of this new method.
			* @param completionStart The start position of insertion of the name of this new method.
			* @param completionEnd The end position of insertion of the name of this new method.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Base types are in the form "int" or "boolean".
			*    Array types are in the qualified form "M[]" or "int[]".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*
			* NOTE: parameter names can be retrieved from the source model after the user selects a specific method.
			*/
			 void acceptMethod(
				const wstring& declaringTypePackageName,
				const wstring& declaringTypeName,
				const wstring& selector,
				const vector< wstring>& parameterPackageNames,
				const vector< wstring>& parameterTypeNames,
				const  vector< wstring>& parameterNames,
				const wstring& returnTypePackageName,
				const wstring& returnTypeName,
				const wstring& completionName,
				int modifiers,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;

			/**
			* Code assist notification of a method completion.
			*
			* @param declaringTypePackageName Name of the package in which the type that contains this new method is declared.
			* @param declaringTypeName Name of the type declaring this new method.
			* @param selector Name of the new method.
			* @param parameterPackageNames Names of the packages in which the parameter types are declared.
			*    	Should contain as many elements as parameterTypeNames.
			* @param parameterTypeNames Names of the parameters types.
			*    	Should contain as many elements as parameterPackageNames.
			* @param returnTypePackageName Name of the package in which the return type is declared.
			* @param returnTypeName Name of the return type of this new method, should be <code>null</code> for a constructor.
			* @param completionName The completion for the method. Can include zero, one or two brackets. If the closing bracket is included, then the cursor should be placed before it.
			* @param modifiers The modifiers of this new method.
			* @param completionStart The start position of insertion of the name of this new method.
			* @param completionEnd The end position of insertion of the name of this new method.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Base types are in the form "int" or "boolean".
			*    Array types are in the qualified form "M[]" or "int[]".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*
			* NOTE: parameter names can be retrieved from the source model after the user selects a specific method.
			*/
			 void acceptMethodDeclaration(
				const wstring& declaringTypePackageName,
				const wstring& declaringTypeName,
				const wstring& selector,
				const vector< wstring>& parameterPackageNames,
				const vector< wstring>& parameterTypeNames,
				const vector< wstring>& parameterNames,
				const wstring& returnTypePackageName,
				const wstring& returnTypeName,
				const wstring& completionName,
				int modifiers,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of a modifier completion.
			*
			* @param modifierName The new modifier.
			* @param completionStart The start position of insertion of the name of this new modifier.
			* @param completionEnd The end position of insertion of the name of this new modifier.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*/
			 void acceptModifier(const wstring& modifierName,
				JavaModel::ISourceRange*  completionRang, double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of a package completion.
			*
			* @param packageName The package name.
			* @param completionName The completion for the package. Can include '.*;' for imports.
			* @param completionStart The start position of insertion of the name of this new package.
			* @param completionEnd The end position of insertion of the name of this new package.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    The default package is represented by an empty array.
			*/

			 void acceptPackage(
				const wstring& packageName,
				const wstring& completionName,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;
			/**
			* Code assist notification of a type completion.
			*
			* @param packageName Declaring package name of the type.
			* @param typeName Name of the type.
			* @param completionName The completion for the type. Can include ';' for imported types.
			* @param completionStart The start position of insertion of the name of the type.
			* @param completionEnd The end position of insertion of the name of the type.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*/
			 void acceptType(
				const wstring& packageName,
				const wstring& typeName,
				const wstring& completionName,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;

			/**
			* Code assist notification of a variable name completion.
			*
			* @param typePackageName Name of the package in which the type of this variable is declared.
			* @param typeName Name of the type of this variable.
			* @param name Name of the variable.
			* @param completionName The completion for the variable.
			* @param completionStart The start position of insertion of the name of this variable.
			* @param completionEnd The end position of insertion of the name of this variable.
			* @param relevance The relevance of the completion proposal
			* 		It is a positive integer which are used for determine if this proposal is more relevant than another proposal.
			* 		This value can only be used for compare relevance. A proposal is more relevant than another if his relevance
			* 		value is higher.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Base types are in the form "int" or "boolean".
			*    Array types are in the qualified form "M[]" or "int[]".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*/
			 void acceptVariableName(
				const wstring& typePackageName,
				const wstring& typeName,
				const wstring& name,
				const wstring& completionName,
				JavaModel::ISourceRange*  completionRang,
				double  relevance, bool needMatch = true) override;

			void SetTokenForMatch(const wstring& token) override{
				tokenForMatch = token;
			};

			int ResultSize() override {
				return 0;
			}
			void JustClearNoDelete();
			wstring tokenForMatch;
		};
	}
}
