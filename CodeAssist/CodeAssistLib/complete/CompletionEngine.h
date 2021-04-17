
#ifndef CompletionEngine_Jikes_CodeAssist_INCLUDED
#define CompletionEngine_Jikes_CodeAssist_INCLUDED

#include "CodeAssistLib/impl/Engine.h"
#include <JavaModelLib/internal/lookup/ISearchRequestor.h>
#include <boost/smart_ptr/shared_ptr.hpp>


//#include "CompleteLex.h"



namespace Jikes{
	class PackageSymbol;
	class IProgressMonitor;
	namespace JavaModel
	{
		class LayerPosInJavaModel;
		class NameLookup;
		class SearchableEnvironmentRequestor;
		class SelectionRequestor;
		class OriginalJavaProject;
		class ISourceRange;
		enum FileTypeInJavaProject;
	}
}

namespace Jikes { // Open namespace Jikes block
	

namespace CodeAssist{
	class CompleteLex;
	class AssistInfo;
	class ResultCollector;
	class ISearchableNameEnvironment;
	class ICompletionRequestor;
	class CompleteAssistASTVisitor;
	struct IAccessRule
	{
	/**
	* Constant indicating that files matching the rule's pattern are accessible.
	*/
	static const 	int K_ACCESSIBLE = 0;

	/**
	* Constant indicating that files matching the rule's pattern are non accessible.
	*/
	static const int K_NON_ACCESSIBLE = 1;

	/**
	* Constant indicating that access to the files matching the rule's pattern is discouraged.
	*/
	static const 	int K_DISCOURAGED = 2;

	};




	class CompletionEngine:public Engine,public JavaModel::ISearchRequestor
	{
		class __InvocationSite :public InvocationSite
		{
		public:
			virtual ~__InvocationSite()
			{
			}

		private:
			bool isSuperAccess() override { return false; }
			bool isTypeAccess()override { return false; }

		};
		
	public:
		static  TypeSymbol* NO_TYPE_SYMBOL;
		static MethodSymbol* NO_EXCLUDE_CONSTRUCTOR;
		static wstring EMPTY_TOKEN;
		static wstring NoChar;
		enum StaticAccess {
			ONLY_STATIC,
			NO_STATIC,
			DO_NOT_CARE_STATIC
		};
		enum ConstAccess {
			ONLY_CONST,
			NO_CONST,
			DO_NOT_CARE_CONST
		};
		enum FindVariableAndMethodOption
		{
			VarialeOnly,
			MethodOnly,
		    VarialeAndMethodBoth,
		};
		CompleteAssistASTVisitor* visitor;
		bool assistNodeIsClass;
		bool assistNodeIsEnum;
		bool assistNodeIsException;
		bool assistNodeIsInterface;
		bool insideQualifiedReference;
		bool assistNodeIsExtendedType;
		bool assistNodeIsInsideCase;
	




		int startPosition, endPosition, offset;
		unsigned int actualCompletionPosition;

		wstring pakcageAssistWhenDot;
		int lenEntered;
		void setSourceRange(int start, int end) {

			startPosition = start;
			endPosition = end + 1;
		}
	    unsigned	int tokenKind;
		static vector<TypeSymbol*> NoSuperInterfaces;
		static vector<wstring> baseTypes;
		vector<TypeSymbol*> expectedTypes;

		static wstring classField;
		static wstring lengthField;
		static wstring _THIS;
		static wstring THROWS;
		static __InvocationSite  FakeInvocationSite;
		JavaModel::NameLookup* GetNameLookUp();



		JavaModel::NameLookup* namel_lookup_;
		JavaModel::SelectionRequestor*  selecttonRequestor;
		bool hasImportWord;
		boost::shared_ptr<JavaModel::OriginalJavaProject> project;

		JavaModel::SearchableEnvironmentRequestor* nameLookupRequestor;

		JavaModel::ISourceRange*  completionRang;
		ResultCollector* transitRequstor;
		bool isForCodeComplete;
	protected:
		ICompletionRequestor* requestor;
		vector<pair< vector<wstring >, bool> >* importPackageNames;
	private:

		set<wstring> error_files;

		JavaModel::FileTypeInJavaProject  file_type;

		AssistInfo& _info;
		JavaModel::LayerPosInJavaModel* layerPos;
		bool keyWordWeightIncrease;
		
	public:



		CompletionEngine(
			boost::shared_ptr<JavaModel::OriginalJavaProject>,
			INameEnvironment*, Option*,
			AssistInfo&, JavaModel::LayerPosInJavaModel* layerPos, 
			ICompletionRequestor* _requestor
		);
		virtual ~CompletionEngine();
		void findFieldsLoop(const wstring& fieldName, 
			TypeSymbol* receiverType,
			TypeSymbol* currentType,
			AstNodeScope* scope, 
			vector<pair<VariableSymbol*, TypeSymbol*>>& fieldsFound, 
			vector<VariableSymbol*>& localsFound, 
			StaticAccess onlyStaticFields, 
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope,
			bool implicitCall,
			set<TypeSymbol*>& visistedType ,
			TypeSymbol* variableType,
			ConstAccess constAccess);

		
		void findFields(const wstring& fieldName,
			vector<VariableSymbol*>& fields,
			AstNodeScope* scope,
			vector<pair<VariableSymbol*, TypeSymbol*>>& fieldsFound,
			vector<VariableSymbol*>& localsFound, 
			StaticAccess onlyStaticFields, 
			TypeSymbol* receiverType,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope,
			bool implicitCall, 
			TypeSymbol* variableType, ConstAccess constAccess
		);

		void findFields(const wstring& fieldName, 
			TypeSymbol* receiverType,
			AstNodeScope* scope, 
			vector<pair<VariableSymbol*, TypeSymbol*>>& fieldsFound, 
			vector<VariableSymbol*>& localsFound,
			StaticAccess onlyStaticFields,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope,
			bool implicitCall ,
			TypeSymbol* variableType,
			ConstAccess constAccess)
		{
				set<TypeSymbol*> visistedType;
				findFieldsLoop(fieldName,
					receiverType, 
					receiverType,
					scope, 
					fieldsFound,
					localsFound,
					onlyStaticFields,
					invocationSite,
					invocationScope, 
					implicitCall,
					visistedType , 
					variableType,
					constAccess
				);
		}
	
	
		void findFieldsAndMethods(
			const wstring& token, 
			TypeSymbol* receiverType, 
			AstNodeScope* scope,
			InvocationSite* invocationSite, 
			AstNodeScope* invocationScope,
			bool implicitCall,
			TypeSymbol* fieldTypeOrMethodReturnType,
			vector<   pair< VariableSymbol*, TypeSymbol*> >& fieldsFound,
			vector<  VariableSymbol* >& localsFound
		);
		
		void ProcessCompletionOnMemberAccess(FieldReference* field_reference, AstNodeScope* scope);
		
		void findImplicitMessageSends(const wstring& token, vector<TypeSymbol*>& argTypes, AstNodeScope* scope,
			InvocationSite* invocationSite, AstNodeScope* invocationScope);
		
		void findIntefacesMethodsLoop(const wstring& selector, vector<TypeSymbol*>& argTypes, TypeSymbol* currentType,
			TypeSymbol* receiverType, AstNodeScope* scope, vector<pair<MethodSymbol*, TypeSymbol*>>& methodsFound, 
			StaticAccess onlyStaticMethods, bool exactMatch, bool isCompletingDeclaration, InvocationSite* invocationSite,
			AstNodeScope* invocationScope, bool implicitCall, set<TypeSymbol*>& visitedTypes, TypeSymbol* returnType
		);

		void findIntefacesMethods(const wstring& selector, vector<TypeSymbol*>& argTypes, TypeSymbol* receiverType,
			vector<TypeSymbol*>* itsInterfaces, AstNodeScope* scope, vector<pair<MethodSymbol*, TypeSymbol*>>& methodsFound, 
			StaticAccess onlyStaticMethods, bool exactMatch, bool isCompletingDeclaration, InvocationSite* invocationSite,
			AstNodeScope* invocationScope, bool implicitCall, TypeSymbol* returnType);
		
	
		void findLocalMethodDeclarations(const wstring& methodName, vector<MethodSymbol*>& methods,
			AstNodeScope* scope, vector<pair<MethodSymbol*, TypeSymbol*>>& methodsFound, StaticAccess onlyStaticMethods,
			bool exactMatch, TypeSymbol* receiverType, TypeSymbol* returnType);
	

		void findLocalMethods(const wstring& methodName, vector<TypeSymbol*>& argTypes,
			vector< MethodSymbol* >& methods, AstNodeScope* scope,
			vector<  pair< MethodSymbol*, TypeSymbol*> >& methodsFound, StaticAccess onlyStaticMethods, bool exactMatch,
			TypeSymbol* receiverType, InvocationSite* invocationSite, AstNodeScope* invocationScope, bool implicitCall, TypeSymbol* returnType);

		void findMethods(const wstring& selector, 
			vector<TypeSymbol*>& argTypes,
			TypeSymbol* receiverType, 
			AstNodeScope* scope,
			vector<pair<MethodSymbol*, TypeSymbol*>>& methodsFound, 
			StaticAccess onlyStaticMethods,
			bool exactMatch, 
			bool isCompletingDeclaration,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope,
			bool implicitCall, TypeSymbol* returnType
		);
	
		void findConstructors(TypeSymbol* currentType, 
			vector<TypeSymbol*>& argTypes,
			AstNodeScope* scope,
			InvocationSite* invocationSite, 
			bool forAnonymousType, MethodSymbol* excludeConstructor
		);
		
		void findAnonymousType(TypeSymbol* currentType, vector<TypeSymbol*>& argTypes,
			AstNodeScope* scope, InvocationSite* invocationSite);
		
		void findVariableName(const wstring& token, const wstring& qualifiedPackageName,
			const wstring& qualifiedSourceName, const wstring& sourceName, vector<wstring>& excludeNames, int dim);
		
		void findVariableNames(const wstring& name, AstType* type, vector<wstring>& excludeNames);
		
		void ProcessCompletionOnMessageSend(AstMethodInvocation* ast_node, AstNodeScope* scope);
		
		void ProcessCompletionOnExplicitConstructorCall(Ast* ast, AstNodeScope* scope);
		
		void ProcessCompletionOnExpression(Expression* expression, AstNodeScope* scope);
		
		void ProcessCompletionOnAstTypeName(AstTypeName* ast_node, AstNodeScope* scope);
		
		void ProcessCompletionOnClassCreationExpressionOnTypeName(AstClassCreationExpression* ast_node, AstNodeScope* scope);
		
		void ProcessCompletionOnMethodName(MethodDeclaration* ast_node, AstNodeScope* scope);
		
		void ProcessCompletionOnMethodReturnType(MethodDeclaration* method_declaration,  AstNodeScope* scope);
		
		int computeRelevanceForExpectingType(TypeSymbol* proposalType);
		
		int computeRelevanceForExpectingType( wstring packageName, const wstring& typeName);
		
		void findMemberTypes(const wstring& typeName, vector<TypeSymbol*>& memberTypes, vector<TypeSymbol*>& typesFound, TypeSymbol* receiverType, TypeSymbol* invocationType);
		
		void findMemberTypesLoop(const wstring& typeName, TypeSymbol* receiverType, 
			TypeSymbol* currentType, AstNodeScope* scope, TypeSymbol* typeInvocation, vector<TypeSymbol*>& typesFound, set<TypeSymbol*>& visistedType);
		
		void findMemberTypes(const wstring& typeName, TypeSymbol* receiverType, AstNodeScope* scope, TypeSymbol* typeInvocation);
		
		void findTypesAndSubpackages(const wstring& token, PackageSymbol* packageBinding);
		
		void ProcessCompletionOnTypeReference(AstTypeName* ast_node,  AstNodeScope* scope);
		
		void ProcessCompletionOnFieldName(AstVariableDeclarator* ast_node, AstNodeScope* scope);
		
		void ProcessCompletionOnLocalFieldName(AstVariableDeclarator* ast_node, AstNodeScope* scope);
		
		void findVariablesAndMethods(const wstring& token,
			AstNodeScope* scope,
			InvocationSite* invocationSite, 
			AstNodeScope* invocationScope,
			FindVariableAndMethodOption _findOption, 
			TypeSymbol* variableTypeOrMethodReturnType,
			vector<pair< MethodSymbol*, TypeSymbol*>  >& methodsFound,
			vector<pair< VariableSymbol*, TypeSymbol*>  >& fieldsFound,
			vector<VariableSymbol*>  localsFound ,
			ConstAccess constAccess
		);
		

		
		void findClassField(const wstring& token, TypeSymbol* receiverType, AstNodeScope* scope);
		
		void ProcessCompletionOnNameReference(AstName* ast_name, AstNodeScope* scope);

		Expression* BuildExpressAstNode(AstNodeScope* scope, CompleteLex* lex, int pos, INameEnvironment* nameEnv, CompleteAssistASTVisitor* visitor, Parser* parser, CompilationUnitDeclaration* compilation_unit);
	
		vector<Ast*> GetHomeMakeAst(CompleteAssistASTVisitor* visitor, unsigned pos, INameEnvironment* nameEnv, CompilationUnitDeclaration* compilation_unit, CompleteLex* lex, Parser* parser);

		void ProcessCompletionOnReturnStatement();
		void ProcessCompletionOnCaseStatement();
		void ProccessKeyWord(const wstring identiferForKeyWord, CompleteAssistASTVisitor* visitor, unsigned int preTokenKind);
		void GetCallTips(CompleteAssistASTVisitor* visitor);
		void GetResult();
		void ProcessCompletionOnAssignmentExpression(AstAssignmentExpression* expression, AstNodeScope* scope);

		void complete();
		void findMethodParameterNames(MethodSymbol* method, vector<wstring>& parameterTypeNames, vector<wstring>& parameterNames);
		public:
		bool mustQualifyType(const wstring& packageName, const wstring& typeName);
		
		void findPackages(const wstring& prefex);
		void findTypes(const wstring& prefex);

		void findPackages(AstPackageDeclaration* packageStatement, AstNodeScope* scope);
		void findImports(AstImportDeclaration* importReference, AstNodeScope* scope);
		void findNestedTypes(const wstring& typeName, TypeSymbol* currentType, AstNodeScope* scope);
		void findTypesAndPackages(const wstring& token, AstNodeScope* scope);
		void findKeywords(const wstring& keyword, vector<wstring>& choices, AstNodeScope* scope);
		void ProcessCompletionOnFieldType(AstType* type, AstNodeScope* scope);
		double computeRelevanceForCaseMatching(const wstring& token, const wstring& proposalName);
		int computeRelevanceForClass();
		double computeBaseRelevance();
		int computeBaseRelevancePackageRelevance();
		int computeRelevanceForResolution();
		int computeRelevanceForResolution(bool isResolved);
		int computeRelevanceForRestrictions(int accessRuleKind);
		int computeRelevanceForEnum();
		int computeRelevanceForFinal(bool onlyFinal, bool isFinal);
		int computeRelevanceForEnumConstant(TypeSymbol* proposalType);
		int omputeRelevanceForException();
		int computeRelevanceForInterface();
		int computeRelevanceForQualification(bool prefixRequired);
		int computeRelevanceForStatic(bool onlyStatic, bool isStatic);
		int computeRelevanceForInheritance(TypeSymbol* receiverType, TypeSymbol* declaringClass);
	public:
		void acceptClass(const std::wstring& packageName, const std::wstring& typeName, int modifiers) override;
		void acceptInterface(const std::wstring& packageName, const std::wstring& typeName, int modifiers) override;
		void acceptPackage(const std::wstring& packageName) override;
		void acceptType(const std::wstring& packageName, const std::wstring& typeName, int modifiers) override;

		void resolveImportNames();

	

	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

