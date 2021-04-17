#ifndef RecoveredElement_def_INCLUDED
#define RecoveredElement_def_INCLUDED
#include <string>
using std::wstring;

namespace Jikes
{ // Open namespace Jikes block


	RecoveredElement::RecoveredElement(RecoveredElement* parent, int bracketBalance)
	{
		parent = parent;
		bracketBalance = bracketBalance;
		recoveringParser = nullptr;
	}

	RecoveredElement::RecoveredElement(RecoveredElement* parent, int bracketBalance, Parser* parser)
	{
		parent = parent;
		bracketBalance = bracketBalance;
		recoveringParser = parser;
	
	}

	

	

	/*
	 *	Record a method declaration
	 */
	RecoveredElement* RecoveredElement::add(AbstractMethodDeclaration* methodDeclaration, int bracketBalanceValue)
	{
		/* default behavior is to delegate recording to parent if any */
		if (parent == nullptr) return this; // ignore
		updateSourceEndIfNecessary(previousAvailableLineEnd(methodDeclaration->LeftToken()->StartLocation() - 1));
		return parent->add(methodDeclaration, bracketBalanceValue);
	}

	

	/*
	 * Record a nested block declaration
	 */
	RecoveredElement* RecoveredElement::add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue)
	{
		/* default behavior is to delegate recording to parent if any */
		if (parent == nullptr) return this; // ignore
		updateSourceEndIfNecessary(previousAvailableLineEnd(nestedBlockDeclaration->LeftToken()->StartLocation() - 1));
		return parent->add(nestedBlockDeclaration, bracketBalanceValue);
	}

	/*
	 * Record a field declaration
	 */
	RecoveredElement* RecoveredElement::add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue)
	{
		/* default behavior is to delegate recording to parent if any */
		if (parent == nullptr) return this; // ignore
		updateSourceEndIfNecessary(previousAvailableLineEnd(fieldDeclaration->LeftToken()->StartLocation() - 1));
		return parent->add(fieldDeclaration, bracketBalanceValue);
	}

	
	/*
	 * Record a local declaration
	 */
	RecoveredElement* RecoveredElement::add(LocalDeclaration* localDeclaration, int bracketBalanceValue)
	{
		/* default behavior is to delegate recording to parent if any */
		if (parent == nullptr) return this; // ignore
		updateSourceEndIfNecessary(previousAvailableLineEnd(localDeclaration->LeftToken()->StartLocation() - 1));
		return parent->add(localDeclaration, bracketBalanceValue);
	}

	/*
	 * Record a statement
	 */
	RecoveredElement* RecoveredElement::add(AbstractStatement* statement, int bracketBalanceValue)
	{
		/* default behavior is to delegate recording to parent if any */
		if (parent == nullptr) return this; // ignore
		updateSourceEndIfNecessary(previousAvailableLineEnd(statement->LeftToken()->StartLocation() - 1));
		return parent->add(statement, bracketBalanceValue);
	}

	/*
	 *	Record a type declaration
	 */
	RecoveredElement* RecoveredElement::add(TypeDeclaration* typeDeclaration, int bracketBalanceValue)
	{
		/* default behavior is to delegate recording to parent if any */
		if (parent == nullptr) return this; // ignore
		updateSourceEndIfNecessary(previousAvailableLineEnd(typeDeclaration->LeftToken()->StartLocation() - 1));
		return parent->add(typeDeclaration, bracketBalanceValue);
	}

	void RecoveredElement::addBlockStatement(RecoveredBlock* recoveredBlock)
	{
	/*	AstBlock block = recoveredBlock->blockDeclaration;
		if (block->statements != nullptr) {
			Statement[] statements = block->statements;
			for (int i = 0; i < statements->length; i++) {
				recoveredBlock->add(statements[i], 0);
			}
		}*/
	}

	/*
	 * Answer the depth of this element, considering the parent link.
	 */
	int RecoveredElement::depth()
	{
		int depth = 0;
		RecoveredElement* current = this;
		while ((current = current->parent) != nullptr) depth++;
		return depth;
	}

	/*
	 * Answer the enclosing method node, or null if none
	 */
	RecoveredInitializer* RecoveredElement::enclosingInitializer()
	{
		RecoveredElement* current = this;
		while (current != nullptr) {
			if (  dynamic_cast<RecoveredInitializer*>(current) ) {
				return (RecoveredInitializer*)current;
			}
			current = current->parent;
		}
		return nullptr;
	}

	/*
	 * Answer the enclosing method node, or null if none
	 */
	RecoveredMethod* RecoveredElement::enclosingMethod()
	{
		RecoveredElement* current = this;
		while (current != nullptr) {
			if ( dynamic_cast<RecoveredMethod*>(current)) {
				return (RecoveredMethod*)current;
			}
			current = current->parent;
		}
		return nullptr;
	}

	/*
	 * Answer the enclosing type node, or null if none
	 */
	RecoveredType* RecoveredElement::enclosingType()
	{
		RecoveredElement* current = this;
		while (current != nullptr) {
			if (dynamic_cast<RecoveredType*>(current) ) {
				return (RecoveredType*)current;
			}
			current = current->parent;
		}
		return nullptr;
	}

	/*
	 * Answer the closest specified parser
	 */
	Parser* RecoveredElement::parser()
	{
		RecoveredElement* current = this;
		while (current != nullptr) {
			if (current->recoveringParser != nullptr) {
				return current->recoveringParser;
			}
			current = current->parent;
		}
		return nullptr;
	}

	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredElement::parseTree()
	{
		return NULL;
	}

	

	/*
	 * Iterate the enclosing blocks and tag them so as to preserve their content
	 */
	void RecoveredElement::preserveEnclosingBlocks()
	{
		auto current = this;
		while (current != nullptr) {
			if (dynamic_cast<RecoveredBlock*>(current) ) {
				((RecoveredBlock*)current)->preserveContent = true;
			}
			if (dynamic_cast<RecoveredType*>(current) ) { // for anonymous types
				((RecoveredType*)current)->preserveContent = true;
			}
			current = current->parent;
		}
	}

	/*
	 * Answer the position of the previous line end if
	 * there is nothing but spaces in between it and the
	 * line end. Used to trim spaces on unclosed elements.
	 */
	int RecoveredElement::previousAvailableLineEnd(int position)
	{
	/*	Parser* parser_ = parser();
		if (parser_ == nullptr) return position;

		Scanner scanner = parser->scanner;
		if (scanner->lineEnds == nullptr) return position;

		int index = scanner->getLineNumber(position);
		if (index < 2) return position;
		int previousLineEnd = scanner->lineEnds[index - 2];

		char[] source = scanner->source;
		for (int i = previousLineEnd + 1; i < position; i++) {
			if (!(source[i] == ' ' || source[i] == '\t')) return position;
		}
		return previousLineEnd;*/
		return 0;
	}

	/*
	 * Answer the very source end of the corresponding parse node
	 */
	int RecoveredElement::sourceEnd()
	{
		return 0;
	}

	wstring RecoveredElement::tabString(int tab)
	{
		return wstring();
	}

	/*
	 * Answer the top node
	 */
	RecoveredElement* RecoveredElement::topElement()
	{
		auto current = this;
		while (current->parent != nullptr) {
			current = current->parent;
		}
		return current;
	}

	wstring RecoveredElement::toString()
	{
		return wstring();
	}

	wstring RecoveredElement::toString(int tab)
	{
		return wstring();
	}

	/*
	 * Answer the enclosing type node, or null if none
	 */
	RecoveredType* RecoveredElement::type()
	{
		auto current = this;
		while (current != nullptr) {
			if (  dynamic_cast<RecoveredType*>(current)) {
				return (RecoveredType*)current;
			}
			current = current->parent;
		}
		return nullptr;
	}

	/*
	 * Update the bodyStart of the corresponding parse node
	 */
	void RecoveredElement::updateBodyStart(int bodyStart)
	{
		foundOpeningBrace = true;
	}

	/*
	 * Update the corresponding parse node from parser state which
	 * is about to disappear because of restarting recovery
	 */
	void RecoveredElement::updateFromParserState()
	{
		// default implementation: do nothing

	}

	/*
	 * A closing brace got consumed, might have closed the current element,
	 * in which case both the currentElement is exited
	 */
	RecoveredElement* RecoveredElement::updateOnClosingBrace(int braceStart, int braceEnd)
	{
		if ((--bracketBalance <= 0) && (parent != nullptr)) {
			updateSourceEndIfNecessary(braceStart, braceEnd);
			return parent;
		}
		return this;
	}

	/*
	 * An opening brace got consumed, might be the expected opening one of the current element,
	 * in which case the bodyStart is updated.
	 */
	/* RecoveredElement updateOnOpeningBrace(int braceEnd){return null;}*/
	RecoveredElement* RecoveredElement::updateOnOpeningBrace(int braceStart, int braceEnd)
	{
		if (bracketBalance++ == 0) {
			updateBodyStart(braceEnd + 1);
			return this;
		}
		return nullptr; // no update is necessary
	}

	/*
	 * Final update the corresponding parse node
	 */
	void RecoveredElement::updateParseTree()
	{
		// default implementation: do nothing
	}

	/*
	 * Update the declarationSourceEnd of the corresponding parse node
	 */
	void RecoveredElement::updateSourceEndIfNecessary(int braceStart, int braceEnd)
	{
		// default implementation: do nothing
	}

	void RecoveredElement::updateSourceEndIfNecessary(int sourceEnd)
	{
		updateSourceEndIfNecessary(sourceEnd + 1, sourceEnd);
	}
} // Close namespace Jikes block


#endif // _INCLUDED


