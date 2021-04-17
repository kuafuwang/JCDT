#include "stdafx.h"
#include "DeclarationOfReferencedTypesPattern.h"
#include <SearchLib/internal/core/IJavaSearchResultCollector.h>
#include <JavaModelLib/internal/core/IJavaElement.h>
#include <JavaModelLib/internal/core/IType.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	DeclarationOfReferencedTypesPattern::DeclarationOfReferencedTypesPattern(
		JavaModel::IJavaElement* _enclosingElement)
	:TypeReferencePattern(L"", L"", PATTERN_MATCH, false), enclosingElement(_enclosingElement)
	{

	}

	void DeclarationOfReferencedTypesPattern::matchReportReference(Ast* reference, 
		JavaModel::IJavaElement* element, int accuracy, MatchSet* locator)
	{
		//// need accurate match to be able to open on type ref
		//if (accuracy == IJavaSearchResultCollector::POTENTIAL_MATCH) return;

		//// element that references the type must be included in the enclosing element
		//while (element != nullptr && enclosingElement != (element)) {
		//	element = element->getParent();
		//}
		//if (element == nullptr) return;

		//int maxType = -1;
		//TypeBinding typeBinding = nullptr;
		//if (reference instanceof TypeReference) {
		//	typeBinding = ((TypeReference)reference).binding;
		//	maxType = Integer.MAX_VALUE;
		//}
		//else if (reference instanceof QualifiedNameReference) {
		//	QualifiedNameReference qNameRef = (QualifiedNameReference)reference;
		//	Binding binding = qNameRef.binding;
		//	maxType = qNameRef.tokens.length - 1;
		//	switch (qNameRef.bits & AstNode.RestrictiveFlagMASK) {
		//	case BindingIds.FIELD: // reading a field
		//		typeBinding = qNameRef.actualReceiverType;
		//		int otherBindingsCount = qNameRef.otherBindings == nullptr ? 0 : qNameRef.otherBindings.length;
		//		maxType -= otherBindingsCount + 1;
		//		break;
		//	case BindingIds.TYPE: //=============only type ==============
		//		typeBinding = (TypeBinding)binding;
		//		break;
		//	case BindingIds.VARIABLE: //============unbound cases===========
		//	case BindingIds.TYPE | BindingIds.VARIABLE:
		//		if (binding instanceof ProblemBinding) {
		//			ProblemBinding pbBinding = (ProblemBinding)binding;
		//			typeBinding = pbBinding.searchType; // second chance with recorded type so far
		//			char[] partialQualifiedName = pbBinding.name;
		//			maxType = CharOperation.occurencesOf('.', partialQualifiedName) - 1; // index of last bound token is one before the pb token
		//			if (typeBinding == nullptr || maxType < 0) return;
		//		}
		//		break;
		//	}
		//}
		//else if (reference instanceof SingleNameReference) {
		//	typeBinding = (TypeBinding)((SingleNameReference)reference).binding;
		//	maxType = 1;
		//}

		//if (typeBinding == nullptr || typeBinding instanceof BaseTypeBinding) return;
		//if (typeBinding instanceof ArrayBinding) {
		//	typeBinding = ((ArrayBinding)typeBinding).leafComponentType;
		//}
		//reportDeclaration(typeBinding, maxType, locator);
	}

	void DeclarationOfReferencedTypesPattern::reportDeclaration(TypeSymbol* typeBinding, int maxType,
		MatchSet* locator)
	{
		//JavaModel::IType * type = locator->lookupType(typeBinding);
		//if (type == nullptr) return; // case of a secondary type
	
		//bool isBinary = type->isBinary();
	
		//if (isBinary) {
		//	IBinaryType info = nullptr;
		//	info = locator.getBinaryInfo((org.eclipse.jdt.internal.core.ClassFile)type.getClassFile(), resource);
		//}

		//while (maxType >= 0 && type != nullptr) {
		//	if (knownTypes.find(type) != knownTypes.end()) {
		//		if (isBinary) {
		//			locator->reportBinaryMatch(type,IJavaSearchResultCollector::EXACT_MATCH);
		//		}
		//		else {
		//			TypeDeclaration typeDecl = ((SourceTypeBinding)typeBinding).scope.referenceContext;
		//			locator->report(typeDecl.sourceStart, typeDecl.sourceEnd, type, IJavaSearchResultCollector::EXACT_MATCH);
		//		}
		//		knownTypes.add(type);
		//	}
		//	if (typeBinding instanceof BinaryTypeBinding) {
		//		typeBinding = ((BinaryTypeBinding)typeBinding).enclosingType();
		//	}
		//	else {
		//		typeBinding = ((SourceTypeBinding)typeBinding).enclosingType();
		//	}
		//	IJavaElement parent = type.getParent();
		//	if (parent instanceof IType) {
		//		type = (IType)parent;
		//	}
		//	else {
		//		type = nullptr;
		//	}
		//	maxType--;
		//}
	}
}


} // Close namespace Jikes block


