#ifndef RecoveredBlock__def_INCLUDED
#define RecoveredBlock_def_INCLUDED

#include <vector>


namespace Jikes
{ // Open namespace Jikes block


	RecoveredBlock::RecoveredBlock(AstBlock* block, RecoveredElement* parent, int bracketBalance)
	:RecoveredStatement(block, parent, bracketBalance){
		blockDeclaration = block;
		foundOpeningBrace = true;
		preserveContent = parser()->methodRecoveryActivated || parser()->statementRecoveryActivated;
	}


	

	/*
	 * Record a nested block declaration
	 */
	RecoveredElement* RecoveredBlock::add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue)
	{
		/* do not consider a nested block starting passed the block end (if set)
		it must be belonging to an enclosing block */
		if (blockDeclaration->LeftToken()->StartLocation() != 0
			&& nestedBlockDeclaration->LeftToken()->StartLocation() > blockDeclaration->RightToken()->EndLocation()) {
			return parent->add(nestedBlockDeclaration, bracketBalanceValue);
		}

		RecoveredBlock* element = new RecoveredBlock(nestedBlockDeclaration, this, bracketBalanceValue);

		// if we have a pending Argument, promote it into the new block
		if (pendingArgument != nullptr) {
			element->attach(pendingArgument);
			pendingArgument = nullptr;
		}
		if (parser()->statementRecoveryActivated) {
			addBlockStatement(element);
		}
		attach(element);
		if ( nestedBlockDeclaration->RightToken()->EndLocation() == 0) return element;
		return this;
	}

	/*
	 * Record a local declaration
	 */
	// done
	RecoveredElement* RecoveredBlock::add(LocalDeclaration* localDeclaration, int bracketBalanceValue)
	{
		return this->add(localDeclaration, bracketBalanceValue, false);
	}

	/*
	 * Record a local declaration
	 */
	RecoveredElement* RecoveredBlock::add(LocalDeclaration* localDeclaration, int bracketBalanceValue, bool delegatedByParent)
	{
		/* do not consider a local variable starting passed the block end (if set)
		it must be belonging to an enclosing block */
		if (blockDeclaration->RightToken()->EndLocation() != 0
			&& localDeclaration->LeftToken()->StartLocation() > blockDeclaration->RightToken()->EndLocation()) {
			if (delegatedByParent) return this; //ignore
			return parent->add(localDeclaration, bracketBalanceValue);
		}

		RecoveredLocalVariable* element = new RecoveredLocalVariable(localDeclaration, this, bracketBalanceValue);
		
		/*if (dynamic_cast<Argument*>(localDeclaration)) {
			pendingArgument = element;
			return this;
		}*/

		attach(element);
		if (localDeclaration->RightToken()->EndLocation() == 0) return element;
		return this;
	}

	/*
	 * Record a statement declaration
	 */
	RecoveredElement* RecoveredBlock::add(AbstractStatement* stmt, int bracketBalanceValue)
	{
		return add(stmt, bracketBalanceValue, false);
	}

	/*
	 * Record a statement declaration
	 */
	RecoveredElement* RecoveredBlock::add(AbstractStatement* stmt, int bracketBalanceValue, bool delegatedByParent)
	{
		if (blockDeclaration->RightToken()->EndLocation() != 0
			&& stmt->LeftToken()->StartLocation() > blockDeclaration->RightToken()->EndLocation()) {
			if (delegatedByParent) return this; //ignore
			return parent->add(stmt, bracketBalanceValue);
		}

		RecoveredStatement* element = new RecoveredStatement(stmt, this, bracketBalanceValue);
		attach(element);
		if (stmt->RightToken()->EndLocation() == 0) return element;
		return this;
	}

	/*
	 * Addition of a type to an initializer (act like inside method body)
	 */
	RecoveredElement* RecoveredBlock::add(TypeDeclaration* typeDeclaration, int bracketBalanceValue)
	{
		return add(typeDeclaration, bracketBalanceValue, false);
	}

	/*
	 * Addition of a type to an initializer (act like inside method body)
	 */
	RecoveredElement* RecoveredBlock::add(TypeDeclaration* typeDeclaration, int bracketBalanceValue, bool delegatedByParent)
	{
		/* do not consider a type starting passed the block end (if set)
		it must be belonging to an enclosing block */
		if (blockDeclaration->RightToken()->EndLocation() != 0
			&& typeDeclaration->LeftToken()->StartLocation() > blockDeclaration->RightToken()->EndLocation()) {
			if (delegatedByParent) return this; //ignore
			return parent->add(typeDeclaration, bracketBalanceValue);
		}

		RecoveredStatement* element = new RecoveredType(typeDeclaration, this, bracketBalanceValue);
		attach(element);
		if (typeDeclaration->RightToken()->EndLocation() == 0) return element;
		return this;
	}

	
	/*
	 * Attach a recovered statement
	 */
	void RecoveredBlock::attach(RecoveredStatement* recoveredStatement)
	{
		statements.push_back(recoveredStatement);
	}

	

	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredBlock::parseTree()
	{
		return blockDeclaration;
	}

	
	
	/*
	 * Rebuild a block from the nested structure which is in scope
	 */
	AstBlock* RecoveredBlock::updatedBlock()
	{

		// if block was not marked to be preserved or empty, then ignore it
		statementCount = statements.size();
		if (!preserveContent || statementCount == 0) return nullptr;

		vector<AbstractStatement*>  updatedStatements(statementCount);
		int updatedCount = 0;


		// may need to update the end of the last statement
		RecoveredStatement* lastStatement = statements[statementCount - 1];

		RecoveredMethod* enclosingMethod_ = enclosingMethod();
		RecoveredInitializer* enclosingIntializer = enclosingInitializer();
		int bodyEndValue = 0;
		if (enclosingMethod_ != nullptr) {
			bodyEndValue = enclosingMethod_->methodDeclaration->BodyEnd();
			if (enclosingIntializer != nullptr &&
				enclosingMethod_->methodDeclaration->sourceStart()
				< enclosingIntializer->fieldDeclaration->LeftToken()->StartLocation()
			) {
				bodyEndValue = enclosingIntializer->fieldDeclaration->RightToken()->EndLocation();
			}
		}
		else if (enclosingIntializer != nullptr) {
			bodyEndValue = enclosingIntializer->fieldDeclaration->RightToken()->EndLocation();
		}
		else {
			bodyEndValue = blockDeclaration->RightToken()->EndLocation() - 1;
		}

		if ( dynamic_cast< RecoveredLocalVariable*>(lastStatement)) {
			RecoveredLocalVariable* lastLocalVariable = (RecoveredLocalVariable*)lastStatement;
			if (lastLocalVariable->localDeclaration->RightToken()->EndLocation() == 0) {

				//lastLocalVariable->localDeclaration->RightToken()->EndLocation() = bodyEndValue;
				//lastLocalVariable->localDeclaration->declarationEnd = bodyEndValue;
			}
		}
		else if (dynamic_cast< RecoveredBlock*>(lastStatement)) {
			RecoveredBlock* lastBlock = (RecoveredBlock*)lastStatement;
			if (lastBlock->blockDeclaration->RightToken()->EndLocation() == 0) {
				//lastBlock->blockDeclaration->RightToken()->EndLocation() = bodyEndValue;
			}
		}
		else if (!(dynamic_cast< RecoveredType*>(lastStatement) )) {
			if (lastStatement->statement->RightToken()->EndLocation() == 0) {
				//lastStatement->statement->RightToken()->EndLocation() = bodyEndValue;
			}
		}

		int lastEnd = blockDeclaration->LeftToken()->StartLocation();

		// only collect the non-nullptr updated statements
		for (int i = 0; i < statementCount; i++) {
			auto updatedStatement = statements[i]->updatedStatement();
			if (updatedStatement != nullptr) {
				updatedStatements[updatedCount++] = updatedStatement;

				if (dynamic_cast< LocalDeclaration*>(updatedStatement)) {
					LocalDeclaration* localDeclaration = (LocalDeclaration*)updatedStatement;
					if (localDeclaration->RightToken()->EndLocation() > lastEnd) {
						lastEnd = localDeclaration->RightToken()->EndLocation();
					}
				}
				else if (
					dynamic_cast< TypeDeclaration*>(updatedStatement)) {
					TypeDeclaration* typeDeclaration = (TypeDeclaration*)updatedStatement;
					if (typeDeclaration->RightToken()->EndLocation() > lastEnd) {
						lastEnd = typeDeclaration->RightToken()->EndLocation();
					}
				}
				else {
					if (updatedStatement->RightToken()->EndLocation() > lastEnd) {
						lastEnd = updatedStatement->RightToken()->EndLocation();
					}
				}
			}
		}
		if (updatedCount == 0) return nullptr; // not interesting block

											   // resize statement collection if necessary
		if (updatedCount != statementCount) {
			//blockDeclaration->statements = new Statement[updatedCount];
			//System->arraycopy(updatedStatements, 0, blockDeclaration->statements, 0, updatedCount);
		}
		else {
			//blockDeclaration->statements = updatedStatements;
		}

		if (blockDeclaration->RightToken()->EndLocation() == 0) {
			if (lastEnd < bodyEndValue) {
			//	blockDeclaration->RightToken()->EndLocation() = bodyEndValue;
			}
			else {
				//blockDeclaration->RightToken()->EndLocation() = lastEnd;
			}
		}

		return blockDeclaration;
	}

	/*
	 * Rebuild a statement from the nested structure which is in scope
	 */
	AbstractStatement* RecoveredBlock::updatedStatement()
	{
		return updatedBlock();
	}

	/*
	 * A closing brace got consumed, might have closed the current element,
	 * in which case both the currentElement is exited
	 */
	RecoveredElement* RecoveredBlock::updateOnClosingBrace(int braceStart, int braceEnd)
	{
		if ((--bracketBalance <= 0) && (parent != nullptr)) {
			updateSourceEndIfNecessary(braceStart, braceEnd);

			/* if the block is the method body, then it closes the method too */
			RecoveredMethod* method = enclosingMethod();
			if (method != nullptr && method->methodBody == this) {
				return parent->updateOnClosingBrace(braceStart, braceEnd);
			}
			RecoveredInitializer* initializer = enclosingInitializer();
			if (initializer != nullptr && initializer->initializerBody == this) {
				return parent->updateOnClosingBrace(braceStart, braceEnd);
			}
			return parent;
		}
		return this;
	}

	/*
	 * An opening brace got consumed, might be the expected opening one of the current element,
	 * in which case the bodyStart is updated.
	 */
	RecoveredElement* RecoveredBlock::updateOnOpeningBrace(int braceStart, int braceEnd)
	{
		// create a nested block
		//AstBlock* block = new AstBlock(nullptr);
		//block->sourceStart = parser()->scanner->startPosition;
		///return add(block, 1);
		return nullptr;
	}

	/*
	 * Final update the corresponding parse node
	 */
	void RecoveredBlock::updateParseTree()
	{
		updatedBlock();
	}

	/*
	 * Rebuild a flattened block from the nested structure which is in scope
	 */
	AbstractStatement* RecoveredBlock::updateStatement()
	{

		// if block was closed or empty, then ignore it
		if (blockDeclaration->RightToken()->EndLocation() != 0 || statementCount == 0) return nullptr;

		vector<AbstractStatement*> updatedStatements(statementCount);
		int updatedCount = 0;

		// only collect the non-nullptr updated statements
		for (int i = 0; i < statementCount; i++) {
			auto updatedStatement = statements[i]->updatedStatement();
			if (updatedStatement != nullptr) {
				updatedStatements[updatedCount++] = updatedStatement;
			}
		}
		if (updatedCount == 0) return nullptr; // not interesting block

											   // resize statement collection if necessary
		if (updatedCount != statementCount) {
		//	blockDeclaration->statements = new Statement[updatedCount];
		//	System->arraycopy(updatedStatements, 0, blockDeclaration->statements, 0, updatedCount);
		}
		else {
		//	blockDeclaration->statements = updatedStatements;
		}

		return blockDeclaration;
	}

	/*
	 * Record a field declaration
	 */
	RecoveredElement* RecoveredBlock::add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue)
	{
		/* local variables inside method can only be final and non void */
		vector<wstring> fieldTypeName;
		//if ((fieldDeclaration->modifiers_opt->HasModifier(TK_final)) != 0 // local var can only be final 
		//	|| (fieldDeclaration->type == nullptr) // initializer
		//	|| ((fieldTypeName = fieldDeclaration->type->getTypeName())->length == 1 // non void
		//		&& CharOperation->equals(fieldTypeName[0], TypeBinding->VOID->sourceName()))) {
		//	updateSourceEndIfNecessary(previousAvailableLineEnd(fieldDeclaration->LeftToken()->StartLocation() - 1));
		//	return parent->add(fieldDeclaration, bracketBalanceValue);
		//}

		/* do not consider a local variable starting passed the block end (if set)
		it must be belonging to an enclosing block */
		if (blockDeclaration->RightToken()->EndLocation() != 0
			&& fieldDeclaration->LeftToken()->StartLocation() > blockDeclaration->RightToken()->EndLocation()) {
			return parent->add(fieldDeclaration, bracketBalanceValue);
		}

		// ignore the added field, since indicates a local variable behind recovery point
		// which thus got parsed as a field reference-> This can happen if restarting after
		// having reduced an assistNode to get the following context (see 1GEK7SG)
		return this;
	}
} // Close namespace Jikes block


#endif // _INCLUDED


