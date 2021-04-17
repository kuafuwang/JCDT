#ifndef RecoveredField_def_INCLUDED
#define RecoveredField_def_INCLUDED

#include <string>
using std::wstring;

namespace Jikes
{ // Open namespace Jikes block


	RecoveredField::RecoveredField(FieldDeclaration* fieldDeclaration, RecoveredElement* parent, 
		int bracketBalance): RecoveredElement(parent, bracketBalance,nullptr)
	{
	}

	RecoveredField::RecoveredField(FieldDeclaration* fieldDeclaration, RecoveredElement* parent,
	                               int bracketBalance, Parser* parser): RecoveredElement(parent, bracketBalance, parser)
	{
	}

	

	/*
	 * Record an expression statement if field is expecting an initialization expression,
	 * used for completion inside field initializers.
	 */
	RecoveredElement* RecoveredField::add(AbstractStatement* statement, int bracketBalanceValue)
	{
		if (alreadyCompletedFieldInitialization || !( dynamic_cast< Expression*>(statement))) {
			return RecoveredElement::add(statement, bracketBalanceValue);
		}
		else {
			alreadyCompletedFieldInitialization = true;
			//fieldDeclaration->initialization = (Expression*)statement;
			//fieldDeclaration->RightToken()->EndLocation() = statement->RightToken()->EndLocation();
			//fieldDeclaration->declarationEnd = statement->RightToken()->EndLocation();
			return this;
		}
	}

	/*
	 * Record a type declaration if this field is expecting an initialization expression
	 * and the type is an anonymous type.
	 * Used for completion inside field initializers.
	 */
	RecoveredElement* RecoveredField::add(TypeDeclaration* typeDeclaration, int bracketBalanceValue)
	{
		//if (alreadyCompletedFieldInitialization
		//	|| ((typeDeclaration->bits & ASTNode->IsAnonymousType) == 0)
		//	|| (fieldDeclaration->RightToken()->EndLocation() != 0 && typeDeclaration->sourceStart > fieldDeclaration->RightToken()->EndLocation())) {
		//	return RecoveredElement::add(typeDeclaration, bracketBalanceValue);
		//}
		//else {
		//	// Prepare anonymous type list
		//	if (anonymousTypes == nullptr) {
		//		anonymousTypes = new RecoveredType[5];
		//		anonymousTypeCount = 0;
		//	}
		//	else {
		//		if (anonymousTypeCount == anonymousTypes->length) {
		//			System->arraycopy(
		//				anonymousTypes,
		//				0,
		//				(anonymousTypes = new RecoveredType[2 * anonymousTypeCount]),
		//				0,
		//				anonymousTypeCount);
		//		}
		//	}
		//	// Store type declaration as an anonymous type
		//	RecoveredType element = new RecoveredType(typeDeclaration, this, bracketBalanceValue);
		//	anonymousTypes[anonymousTypeCount++] = element;
		//	return element;
		//}
	}

	
	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredField::parseTree()
	{
		return fieldDeclaration;
	}

	/*
	 * Answer the very source end of the corresponding parse node
	 */
	int RecoveredField::sourceEnd()
	{
		return fieldDeclaration->RightToken()->EndLocation();
	}

	wstring RecoveredField::toString(int tab)
	{
		return wstring();
	}

	FieldDeclaration* RecoveredField::updatedFieldDeclaration()
	{
		//if (anonymousTypes != nullptr) {
		//	if (fieldDeclaration->initialization == nullptr) {
		//		for (int i = 0; i < anonymousTypeCount; i++) {
		//			RecoveredType recoveredType = anonymousTypes[i];
		//			TypeDeclaration typeDeclaration = recoveredType->typeDeclaration;
		//			if (typeDeclaration->RightToken()->EndLocation() == 0) {
		//				typeDeclaration->RightToken()->EndLocation() = fieldDeclaration->RightToken()->EndLocation();
		//				typeDeclaration->bodyEnd = fieldDeclaration->RightToken()->EndLocation();
		//			}
		//			if (recoveredType->preserveContent) {
		//				TypeDeclaration anonymousType = recoveredType->updatedTypeDeclaration();
		//				fieldDeclaration->initialization = anonymousType->allocation;
		//				if (fieldDeclaration->RightToken()->EndLocation() == 0) {
		//					int end = anonymousType->RightToken()->EndLocation();
		//					fieldDeclaration->RightToken()->EndLocation() = end;
		//					fieldDeclaration->declarationEnd = end;
		//				}
		//			}
		//		}
		//		if (anonymousTypeCount > 0) fieldDeclaration->bits |= ASTNode->HasLocalType;
		//	}
		//	else if (fieldDeclaration->getKind() == AbstractVariableDeclaration->ENUM_CONSTANT) {
		//		// fieldDeclaration is an enum constant
		//		for (int i = 0; i < anonymousTypeCount; i++) {
		//			RecoveredType recoveredType = anonymousTypes[i];
		//			TypeDeclaration typeDeclaration = recoveredType->typeDeclaration;
		//			if (typeDeclaration->RightToken()->EndLocation() == 0) {
		//				typeDeclaration->RightToken()->EndLocation() = fieldDeclaration->RightToken()->EndLocation();
		//				typeDeclaration->bodyEnd = fieldDeclaration->RightToken()->EndLocation();
		//			}
		//			recoveredType->updatedTypeDeclaration();
		//		}
		//	}
		//}
		return fieldDeclaration;
	}

	/*
	 * A closing brace got consumed, might have closed the current element,
	 * in which case both the currentElement is exited.
	 *
	 * Fields have no associated braces, thus if matches, then update parent.
	 */
	RecoveredElement* RecoveredField::updateOnClosingBrace(int braceStart, int braceEnd)
	{
		//if (bracketBalance > 0) { // was an array initializer
		//	bracketBalance--;
		//	if (bracketBalance == 0) {
		//		if (fieldDeclaration->getKind() == AbstractVariableDeclaration->ENUM_CONSTANT) {
		//			updateSourceEndIfNecessary(braceEnd - 1);
		//			return parent;
		//		}
		//		else {
		//			alreadyCompletedFieldInitialization = true;
		//		}
		//	}
		//	return this;
		//}
		//else if (bracketBalance == 0) {
		//	alreadyCompletedFieldInitialization = true;
		//	updateSourceEndIfNecessary(braceEnd - 1);
		//}
		//if (parent != nullptr) {
		//	return parent->updateOnClosingBrace(braceStart, braceEnd);
		//}
		//return this;
	}

	/*
	 * An opening brace got consumed, might be the expected opening one of the current element,
	 * in which case the bodyStart is updated.
	 */
	RecoveredElement* RecoveredField::updateOnOpeningBrace(int braceStart, int braceEnd)
	{
		//if (fieldDeclaration->RightToken()->EndLocation() == 0
		//	&& (fieldDeclaration->type instanceof ArrayTypeReference || fieldDeclaration->type instanceof ArrayQualifiedTypeReference)
		//	&& !alreadyCompletedFieldInitialization) {
		//	bracketBalance++;
		//	return nullptr; // no update is necessary	(array initializer)
		//}
		//if (fieldDeclaration->RightToken()->EndLocation() == 0
		//	&& fieldDeclaration->getKind() == AbstractVariableDeclaration->ENUM_CONSTANT) {
		//	bracketBalance++;
		//	return nullptr; // no update is necessary	(enum constant)
		//}
		//// might be an array initializer
		//updateSourceEndIfNecessary(braceStart - 1, braceEnd - 1);
		//return parent->updateOnOpeningBrace(braceStart, braceEnd);
	}

	void RecoveredField::updateParseTree()
	{
		updatedFieldDeclaration();
	}

	/*
	 * Update the declarationSourceEnd of the corresponding parse node
	 */
	void RecoveredField::updateSourceEndIfNecessary(int bodyStart, int bodyEnd)
	{
		if (fieldDeclaration->RightToken()->EndLocation() == 0) {
			//fieldDeclaration->RightToken()->EndLocation() = bodyEnd;
			//fieldDeclaration->RightToken() = bodyEnd;
		}
	}
}; // Close namespace Jikes block


#endif // _INCLUDED


