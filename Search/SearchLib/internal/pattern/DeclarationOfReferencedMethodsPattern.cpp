#include "stdafx.h"
#include "DeclarationOfReferencedMethodsPattern.h"
#include <SearchLib/internal/core/IJavaSearchResultCollector.h>

#include <JavaModelLib/internal/core/IJavaElement.h>
#include <JCDT_Lib/internal/ast/AstMethodInvocation.h>
#include <JCDT_Lib/internal/lookup/Symbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <SearchLib/internal/matching/MatchLocator.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <SearchLib/internal/matching/MatchSet.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	void DeclarationOfReferencedMethodsPattern::matchReportReference(Ast* reference, 
		JavaModel::IJavaElement* element, int accuracy, MatchSet* locator)
	{
		// need accurate match to be able to open on type ref
		if (accuracy == IJavaSearchResultCollector::POTENTIAL_MATCH || !reference)
			return;

		// element that references the method must be included in the enclosing element
		while (element != nullptr && !enclosingElement->equals(element) ) {
			element = element->getParent();
		}
		if (element == nullptr) return;
		auto symbol = ((AstMethodInvocation*)reference)->symbol;
		if(symbol && symbol->_kind == Symbol::METHOD)
		{
			reportDeclaration(  reinterpret_cast<MethodSymbol*>(symbol)  , locator);
		}
		
	}

	void DeclarationOfReferencedMethodsPattern::reportDeclaration(MethodSymbol* methodBinding, 
		MatchSet* locator)
	{
		if (!methodBinding || !locator)
			return;

		auto  declaringClass = methodBinding->containing_type;

		if (!declaringClass)
			return;

		JavaModel::IType* type = locator->lookupType(declaringClass);
		if (type == nullptr)
			return; // case of a secondary type

		auto  selector = methodBinding->getNameString();

	
		vector<wstring> parameterTypes;
		methodBinding->GetParameterTypeQuilifiedNames(parameterTypes);
		auto method = type->getMethod(selector, parameterTypes);

		if (!method || knownMethods.find(method) != knownMethods.end())
			return;

		knownMethods.insert(method);

		
		bool isBinary = type->isBinary();
		
		if (isBinary) {
			locator->reportBinaryMatch(method, IJavaSearchResultCollector::EXACT_MATCH);
		}
		else {

			auto typeDecl = declaringClass->declaration;
			if (!typeDecl)
				return;
			AbstractMethodDeclaration* methodDecl = nullptr;
			
			for (int i = 0, length = typeDecl->NumConstructors(); i < length; i++) {
				methodDecl = typeDecl->Constructor(i);

				if (methodDecl && selector== methodDecl->selector()) {
					break;
				}
			}
			if (methodDecl != nullptr) {
				
				locator->report(methodDecl->sourceStart(), methodDecl->sourceEnd(), 
					method, IJavaSearchResultCollector::EXACT_MATCH,0);
				return;
			}

			for (int i = 0, length = typeDecl->NumMethods(); i < length; i++) {
				methodDecl = typeDecl->Method(i);

				if (methodDecl && selector == methodDecl->selector()) {
					break;
				}
			}
			if (methodDecl != nullptr) {
				locator->report(methodDecl->sourceStart(), methodDecl->sourceEnd(), 
					method, IJavaSearchResultCollector::EXACT_MATCH,0);
				return;
			}

		}
	}
}


} // Close namespace Jikes block


