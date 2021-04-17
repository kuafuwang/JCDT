#include "stdafx.h"
#include "ResultCollector.h"
#include "Proposal/AnomymousCompletionProposal.h"
#include "Proposal/TypeCompletionProposal.h"
#include "Proposal/FieldCompletionProposal.h"
#include "Proposal/LocalVariableCompletionProposal.h"
#include "Proposal/MethodCompletionProposal.h"
#include "Proposal/PackageCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		ResultCollector::ResultCollector()
		{
			
		}

		ResultCollector::~ResultCollector()
		{
			for (auto it : items)
			{
				delete it;
			}
			for (auto it : tips)
			{
				delete it;
			}

		}

		void ResultCollector::ClearAndDelete()
		{
			for (auto it : items)
			{
				delete it;
			}
			items.clear();
			for (auto it : tips)
			{
				delete it;
			}
			tips.clear();
			allDisplayString.clear();
		}

		void ResultCollector::acceptAnonymousType(const wstring& superTypePackageName, const wstring& superTypeName, 
			const	vector< wstring>& parameterPackageNames, const vector< wstring>& parameterTypeNames, const vector< wstring>& parameterNames,
			const wstring& completionName, int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
		   auto item = 	new  AnomymousCompletionProposal(modifiers, relevance, completionRang, superTypePackageName,
				superTypeName, parameterPackageNames, parameterTypeNames,
				parameterNames, completionName);
		   item->needToCheckMatch = needMatch;
		   auto display = item->GetDisplayString();
		   if (!allDisplayString.insert(display).second) {
			   delete item;
		   }
		   else {
			   items.emplace_back(item);
		   }
		}

		void ResultCollector::acceptClass(const wstring& packageName, const wstring& className, const wstring& completionName, 
			int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  TypeCompletionProposal(IJavaCompletionProposal::ClassProposal, modifiers, relevance,completionRang, packageName, completionName
				));
			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
		}

		void ResultCollector::acceptField(const wstring& declaringTypePackageName, const wstring& declaringTypeName, const wstring& name,
			const wstring& typePackageName, const wstring& typeName, const wstring& completionName, int modifiers,
			JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  FieldCompletionProposal(modifiers, relevance, completionRang, declaringTypePackageName, declaringTypeName,
				name, typePackageName, typeName, completionName));
			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
		}

		void ResultCollector::acceptInterface(const wstring& packageName, const wstring& interfaceName, 
			const wstring& completionName, int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  TypeCompletionProposal(IJavaCompletionProposal::InterfaceProposal,
				modifiers, relevance, completionRang, packageName, completionName
			));
			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
		}

		void ResultCollector::acceptLabel(const wstring& labelName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = new  JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal,
				0, relevance, completionRang);
			
			item->fDisplayString = (labelName);

			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}

		}

		void ResultCollector::acceptLocalVariable(const wstring& name, const wstring& typePackageName, const wstring& typeName, 
			int modifiers, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  LocalVariableCompletionProposal(
				modifiers, relevance, completionRang , name, typePackageName, typeName));
			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
		}

		void ResultCollector::acceptMethod(
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
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  MethodCompletionProposal(
				modifiers, relevance, completionRang, declaringTypePackageName, declaringTypeName, selector,
				parameterPackageNames, parameterTypeNames , parameterNames, returnTypePackageName, returnTypeName,completionName));
			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
		}

		void ResultCollector::acceptMethodDeclaration(
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
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  MethodCompletionProposal(
				modifiers, relevance, completionRang, declaringTypePackageName, declaringTypeName, selector,
				parameterPackageNames, parameterTypeNames, parameterNames, returnTypePackageName, returnTypeName, completionName));
			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
		}

		void ResultCollector::acceptModifier(const wstring& modifierName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = new  JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal,
				0, relevance, completionRang);
			
			item->fDisplayString = (modifierName);
			item->needToCheckMatch = needMatch;

			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
			
		}

		void ResultCollector::acceptPackage(const wstring& packageName, const wstring& completionName,
			JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  PackageCompletionProposal(completionName,
				 0, relevance, completionRang, packageName));

			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second){
				delete item;
			}
			else{
				items.emplace_back(item);
			}
				
		}

		void ResultCollector::acceptType(const wstring& packageName, const wstring& typeName,
			const wstring& completionName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  TypeCompletionProposal(IJavaCompletionProposal::ClassProposal,
				0, relevance, completionRang, packageName, typeName
			));

			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
		}

		void ResultCollector::acceptVariableName(const wstring& typePackageName, const wstring& typeName, const wstring& name,
			const wstring& completionName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = (new  LocalVariableCompletionProposal(
				0, relevance, completionRang, name, typePackageName, typeName));

			item->needToCheckMatch = needMatch;
			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
		}

		void ResultCollector::JustClearNoDelete()
		{
			items.clear();
			tips.clear();
			allDisplayString.clear();
		}

		void ResultCollector::acceptKeyword(const wstring& keywordName, JavaModel::ISourceRange* completionRang, double  relevance, bool needMatch )
		{
			if (!completionRang)
				return;
			completionRang = completionRang->Clone();
			auto item = new  JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal,
				0, relevance, completionRang);		
			item->fDisplayString =(keywordName);
			item->needToCheckMatch = needMatch;


			auto display = item->GetDisplayString();
			if (!allDisplayString.insert(display).second) {
				delete item;
			}
			else {
				items.emplace_back(item);
			}
			
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

