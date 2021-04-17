#include "stdafx.h"
#include "SimilarElementsRequestor.h"
#include "ProblemPosition.h"
#include <JCDT_Lib/compiler/Compiler.h>
#include <CodeAssistLib/select/SelectionEngine.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/compiler/BuildMsgInfo.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <CodeAssistLib/select/SelecctResult.h>
#include <JavaModelLib/internal/core/Member.h>
#include <JCDT_Lib/internal/impl/option.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <JavaModelLib/internal/core/IType.h>
#include <boost/algorithm/string.hpp>
#include <tchar.h>
#include <windows.h>
#include <CodeAssistLib/complete/CompletionEngine.h>
#include <CodeAssistLib/complete/ResultCollector.h>
#include "NameMatcher.h"
#include "SimilarElement.h"

using namespace Jikes::CodeAssist;
namespace Jikes { // Open namespace Jikes block


	namespace Correction {

		vector<wstring>	 SimilarElementsRequestor::PRIM_TYPES = 
		{
			L"boolean", L"byte", L"char", L"short", L"int", L"long", L"float", L"double"
		}; //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$

		void SimilarElementsRequestor::findSimilarElement(vector<SimilarElement*>&result, ProblemPosition* problemPos, AstName* name, int kind)
		{
			//int pos = name.getStartPosition();
			//int nArguments = -1;

			//if (name.getParent().getNodeType() == ASTNode.METHOD_INVOCATION)
			//{
			//	MethodInvocation invocation = (MethodInvocation)name.getParent();
			//	if (name.equals(invocation.getName()))
			//	{
			//		if ((kind & METHODS) != 0)
			//		{
			//			nArguments = ((MethodInvocation)name.getParent()).arguments().size();
			//		}
			//	}
			//	else
			//	{
			//		pos = invocation.getStartPosition(); // workaround for code assist
			//		// foo(| code assist here returns only method declaration
			//	}
			//}
			//ITypeBinding binding = ASTResolving.getTypeBinding(name);

			//String returnType = (binding != null) ? binding.getName() : null;
			//return findSimilarElement(cu, pos, name.getIdentifier(), kind, nArguments, returnType);
		}

		void SimilarElementsRequestor::findSimilarVariableElement(vector<SimilarElement*>& result, ProblemPosition* problemPos, const wstring& name, int kind)
		{
			return findSimilarElement(result, problemPos, name, kind, 0, L"");
		}

		void SimilarElementsRequestor::findSimilarTypeElement(vector<SimilarElement*>& result, ProblemPosition* problemPos, const wstring& typeName, int kind)
		{
			return findSimilarElement(result, problemPos, typeName, kind, 0, L"");
		}

		void SimilarElementsRequestor::findSimilarElement(vector<SimilarElement*>& result, 
			ProblemPosition* problemPos, const wstring& name,
			int kind, int nArguments, const wstring& preferredType)
		{
			auto requestor = new SimilarElementsRequestor(name, kind, nArguments, preferredType);
			 requestor->process(result, problemPos);
			 delete requestor;
		}


		void  SimilarElementsRequestor::process(vector<SimilarElement*>& result, ProblemPosition* problemPos)
		{
			if (!problemPos)
				return;

			auto pJM =  problemPos->pJm;
			auto cur_info = problemPos->cur_info;

			if (!pJM || !cur_info)
				return;

			auto _pos = pJM->getLayerPos(cur_info->doc_path);
			auto java_project = pJM->getJavaProject(_pos->proj);
			if (!java_project) {
				delete _pos;
				return;
			}

			CompletionEngine* engine = nullptr;
			Option* option = new Option();
			option->bytecode = false;
			JikesClassPaths collector;
			pJM->getExpandClasspath(collector, _pos->proj);

			INameEnvironment* nameEnv = new INameEnvironment(option, collector);
		
			engine = new CompletionEngine(
				java_project,
				nameEnv,
				option,
				*cur_info,
				_pos,
				this
			);
			engine->isForCodeComplete = false;

			engine->complete();
			
		
			delete engine;
		
			processKeywords();

			for(auto it : fResult)
			{
				result.push_back(it);
			}
			fResult.clear();
		}
		
		SimilarElementsRequestor::SimilarElementsRequestor(const wstring& name, int kind, int nArguments, const wstring& preferredType)
		{
	
			fName = name;
			fKind = kind;
			fNumberOfArguments = nArguments;
			fPreferredType = preferredType;

		}

		SimilarElementsRequestor::SimilarElementsRequestor(): fNumberOfArguments(0), fKind(0)
		{
			
		}

		SimilarElementsRequestor::~SimilarElementsRequestor()
		{
		
		}

		void SimilarElementsRequestor::ClearAndDelete()
		{
			
		}
		/**
		* Method addPrimitiveTypes.
		*/
		 void SimilarElementsRequestor::addResult(SimilarElement* elem) {

			 assert(elem);
			 if (!elem)
				 return;
			for(auto it : fResult)
			{
				if(it->eaqual(elem))
				{
					return;
				}
			}
			fResult.insert(elem);
		}

		 void SimilarElementsRequestor::processKeywords()
		 {
			if ((fKind & PRIMITIVETYPES) != 0) {
				auto data = PRIM_TYPES.data();
				for (int i = 0; i < PRIM_TYPES.size(); i++) {
					if (NameMatcher::isSimilarName(fName, data[i])) {
						addResult(new SimilarElement(PRIMITIVETYPES, data[i], 50,0));
					}
				}
			}
			if ((fKind & VOIDTYPE) != 0) {
				wstring voidType = L"void"; //$NON-NLS-1$
				if (NameMatcher::isSimilarName(fName, voidType)) {
					addResult(new SimilarElement(PRIMITIVETYPES, voidType, 50,0));
				}
			}
		}
		  void  SimilarElementsRequestor::addOther(SimilarElement* elem) {
			 fOthers.insert(elem);
		 }

		  void SimilarElementsRequestor::addType(int kind, const std::wstring& packageName, const std::wstring& typeName,
			  const  std::wstring& completionName, double relevance ,int _modifiers)
		  {
			  wstring buf;
			 if (packageName.length ()> 0) {
				 buf.append(packageName);
				 buf.push_back('.');
			 }
			 buf.append(typeName);
			 if(records.find(buf) == records.end())
			 {
				 records.insert(buf);
			 }
			 else
			 {
				 return;
			 }
			 SimilarElement* elem = new SimilarElement(kind, buf, relevance, _modifiers);

			 if (NameMatcher::isSimilarName(fName,  typeName)) {
				 addResult(elem);
			 }
			 addOther(elem);
		 }

		  void SimilarElementsRequestor::addVariable(int kind, const std::wstring& name, const std::wstring& typePackageName,
			  const std::wstring& typeName, double relevance , int modifiers) {
			 auto  variableName = name;
			 if (NameMatcher::isSimilarName(fName, variableName)) {
				 addResult(new SimilarElement(kind, variableName, relevance, modifiers));
			 }
			 else if (fPreferredType.empty() != true) {
				 wstring buf;
				 if (typePackageName.length() > 0) {
					 buf.append(typePackageName);
					 buf.push_back('.');
				 }
				 buf.append(typeName);
				 if (fPreferredType== buf) {
					 if (records.find(buf) == records.end())
					 {
						 records.insert(buf);
					 }
					 else
					 {
						 return;
					 }
					 addResult(new SimilarElement(kind, variableName, relevance, modifiers));
				 }
			 }

		 }


		void SimilarElementsRequestor::acceptAnonymousType(const wstring& superTypePackageName, const wstring& superTypeName, 
			const	vector< wstring>& parameterPackageNames, const vector< wstring>& parameterTypeNames, const vector< wstring>& parameterNames,
			const wstring& completionName, int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
		
		}

		void SimilarElementsRequestor::acceptClass(const wstring& packageName, const wstring& className, const wstring& completionName, 
			int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if ((fKind & CLASSES) != 0) {
				addType(CLASSES, packageName, className, completionName, relevance , modifiers);
			}
		
		}

		void SimilarElementsRequestor::acceptField(const wstring& declaringTypePackageName, const wstring& declaringTypeName, const wstring& name,
			const wstring& typePackageName, const wstring& typeName, const wstring& completionName, int modifiers,
			JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if ((fKind & FIELDS) != 0) {
				addVariable(FIELDS, name, typePackageName, typeName, relevance, modifiers);
			}
		}

		void SimilarElementsRequestor::acceptInterface(const wstring& packageName, const wstring& interfaceName, 
			const wstring& completionName, int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if ((fKind & INTERFACES) != 0) {
				addType(INTERFACES, packageName, interfaceName, completionName, relevance, modifiers);
			}
		}

		void SimilarElementsRequestor::acceptLabel(const wstring& labelName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			

		}

		void SimilarElementsRequestor::acceptLocalVariable(const wstring& name, const wstring& typePackageName, const wstring& typeName, 
			int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if ((fKind & VARIABLES) != 0) {
				addVariable(VARIABLES, name, typePackageName, typeName, relevance, modifiers);
			}
		}

		void SimilarElementsRequestor::acceptMethod(
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
			JavaModel::ISourceRange* completionRang, 
			double  relevance
			, bool needMatch )
		{
			if ((fKind & METHODS) != 0) {
				wstring methodName = (selector);
				if (fNumberOfArguments == -1 || fNumberOfArguments == parameterTypeNames.size()) {
					int similarity = NameMatcher::getSimilarity(fName, methodName);
					if (similarity >= 0) {
						if (records.find(methodName) == records.end())
						{
							records.insert(methodName);
						}
						else
						{
							return;
						}
						auto elem = new SimilarElement(METHODS, methodName, relevance + similarity, modifiers);
						addResult(elem);
					}
				}
			}
		}

		void SimilarElementsRequestor::acceptMethodDeclaration(
			const wstring& declaringTypePackageName, 
			const wstring& declaringTypeName,
			const wstring& selector,
			const  vector< wstring>& parameterPackageNames,
			const vector< wstring>& parameterTypeNames, 
			const  vector< wstring>& parameterNames,
			const wstring& returnTypePackageName, 
			const wstring& returnTypeName, const wstring& completionName,
			int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			
		}

		void SimilarElementsRequestor::acceptModifier(const wstring& modifierName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			
		}

		void SimilarElementsRequestor::acceptPackage(const wstring& packageName, const wstring& completionName,
			JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			
		}

		void SimilarElementsRequestor::acceptType(const wstring& packageName, const wstring& typeName,
			const wstring& completionName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			
		}

		void SimilarElementsRequestor::acceptVariableName(const wstring& typePackageName, const wstring& typeName, const wstring& name,
			const wstring& completionName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			
		}

		void SimilarElementsRequestor::JustClearNoDelete()
		{
			
		}

		void SimilarElementsRequestor::acceptKeyword(const wstring& keywordName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			
			
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

