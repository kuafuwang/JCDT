
#ifndef RecoveredInitializer_def_INCLUDED
#define RecoveredInitializer_def_INCLUDED


namespace Jikes { // Open namespace Jikes block



	RecoveredInitializer::RecoveredInitializer(FieldDeclaration* fieldDeclaration, 
		RecoveredElement* parent, int bracketBalance): RecoveredField(fieldDeclaration,parent, bracketBalance,nullptr)
	{
	
}
	RecoveredInitializer::RecoveredInitializer(FieldDeclaration* fieldDeclaration, 
		RecoveredElement* parent, int bracketBalance, Parser* parser): RecoveredField(fieldDeclaration, parent, bracketBalance)
	{
	
}
/*
 * Record a nested block declaration
 */
 RecoveredElement* RecoveredInitializer::add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue){
	 return NULL;
 }
/*
 * Record a field declaration (act like inside method body)
 */
 RecoveredElement* RecoveredInitializer::add(FieldDeclaration* newFieldDeclaration, int bracketBalanceValue){
	 return NULL;
 }
/*
 * Record a local declaration - regular method should have been created a block body
 */
 RecoveredElement* RecoveredInitializer::add(LocalDeclaration* localDeclaration, int bracketBalanceValue){
	 return NULL;
 }
/*
 * Record a statement - regular method should have been created a block body
 */
 RecoveredElement* RecoveredInitializer::add(AbstractStatement* statement, int bracketBalanceValue){
	 return NULL;
 }
 RecoveredElement* RecoveredInitializer::add(TypeDeclaration* typeDeclaration, int bracketBalanceValue){
	 return NULL;
 }

 wstring RecoveredInitializer::toString(int tab) {
	return wstring();
}
 FieldDeclaration* RecoveredInitializer::updatedFieldDeclaration(){
	 return nullptr;
	
}
/*
 * A closing brace got consumed, might have closed the current element,
 * in which case both the currentElement is exited
 */
 RecoveredElement* RecoveredInitializer::updateOnClosingBrace(int braceStart, int braceEnd){
	return NULL;
}
/*
 * An opening brace got consumed, might be the expected opening one of the current element,
 * in which case the bodyStart is updated.
 */
 RecoveredElement* RecoveredInitializer::updateOnOpeningBrace(int braceStart, int braceEnd){
	return NULL;
}
/*
 * Update the declarationSourceEnd of the corresponding parse node
 */
 void RecoveredInitializer::updateSourceEndIfNecessary(int braceStart, int braceEnd){
	 
 }




} // Close namespace Jikes block


#endif // _INCLUDED
