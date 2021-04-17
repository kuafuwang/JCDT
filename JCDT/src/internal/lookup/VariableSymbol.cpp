
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/impl/FileLocation.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/ast/ScopeUtil.h>

namespace Jikes { // Open namespace Jikes block
				  // Is variable a local variable?


	bool   VariableSymbol::canBeSeenBy(TypeSymbol* receiverType, InvocationSite* invocationSite, AstNodeScope* scope) {
		if (ACC_PUBLIC()) return true;


		if (IsLocal())
			return  false;


		TypeSymbol* invocationType;
		 invocationType = ScopeUtil::enclosingSourceType(scope);
		
		if (!invocationType)
		{
			return false;
		}

		auto declaringClass = (TypeSymbol*)(owner);

		if (invocationType == declaringClass && invocationType == receiverType) return true;

		if (ACC_PROTECTED()) {
			// answer true if the invocationType is the declaringClass or they are in the same package
			// OR the invocationType is a subclass of the declaringClass
			//    AND the receiverType is the invocationType or its subclass
			//    OR the method is a static method accessed directly through a type
			//    OR previous assertions are true for one of the enclosing type
			if (invocationType == declaringClass) return true;
			if (invocationType->package == declaringClass->package) 
				return true;

			auto currentType = invocationType;
			do {
				if ( currentType->IsSubclass(declaringClass)) {
					if (invocationSite->isSuperAccess()) {
						return true;
					}
					// receiverType can be an array binding in one case... see if you can change it
					if (receiverType->IsArray()) {
						return false;
					}
					if (ACC_STATIC()) {
						return true; // see 1FMEPDL - return invocationSite.isTypeAccess();
					}
					if (currentType == receiverType || receiverType->IsSubclass(currentType) ) {
					
						return true;
					}
				}
				currentType = currentType->EnclosingType();
			} while (currentType != nullptr);
			return false;
		}

		if (ACC_PRIVATE()) {
			// answer true if the receiverType is the declaringClass
			// AND the invocationType and the declaringClass have a common enclosingType
			if (receiverType != declaringClass) return false;

			if (invocationType != declaringClass) {
				auto outerInvocationType = invocationType;
				auto temp = outerInvocationType->EnclosingType();
				while (temp != nullptr) {
					outerInvocationType = temp;
					temp = temp->EnclosingType();
				}

				auto outerDeclaringClass = declaringClass;
				temp = outerDeclaringClass->EnclosingType();
				while (temp != nullptr) {
					outerDeclaringClass = temp;
					temp = temp->EnclosingType();
				}
				if (outerInvocationType != outerDeclaringClass)
					return false;
			}
			return true;
		}

		// isDefault()
		if (invocationType->package != declaringClass->package) return false;

		// receiverType can be an array binding in one case... see if you can change it
		if (receiverType->IsArray())
			return false;
		auto type = receiverType;
		auto declaringPackage = declaringClass->package;
		do {
			if (declaringClass == type) return true;
			if (declaringPackage != type->package) return false;
		} while ((type = type->super) != nullptr);
		return false;
	}


	bool VariableSymbol::IsLocal()
	{
		if (!owner)
			return true;
		return MethodSymbol::MethodCast(owner) != NULL;
	}
	// Is variable local to a particular method ?
	bool VariableSymbol::IsLocal(MethodSymbol* method)
	{

		return owner == method;
	}

	bool VariableSymbol::IsFinal(TypeSymbol* type)
	{
		return owner == type && ACC_FINAL();
	}
	
	void VariableSymbol::SetOwner(Symbol* owner_)
	{
		owner = owner_;
		assert(TypeSymbol::TypeCast(owner) || MethodSymbol::MethodCast(owner));
	}
	 TypeSymbol* VariableSymbol::ContainingType()
	{
		if (!owner)
			return nullptr;

		MethodSymbol* method_owner = MethodSymbol::MethodCast(owner);
		return method_owner ? method_owner->containing_type
			: TypeSymbol::TypeCast(owner);
	}
	 const TypeSymbol* VariableSymbol::ContainingType() const
	{
		if (!owner)
			return nullptr;

		MethodSymbol* method_owner = MethodSymbol::MethodCast(owner);
		return method_owner ? method_owner->containing_type
			: TypeSymbol::TypeCast(owner);
	}
	 bool VariableSymbol::IsField() const
	 {
		if (!owner)
			return false;
		return TypeSymbol::TypeCast(owner) != nullptr ;
	}
	 void VariableSymbol::SetType(TypeSymbol* _type)
	{
		type_ = _type;
		signature = type_->signature;
	}

	VariableSymbol::VariableSymbol(const NameSymbol* name_symbol_)
		: declarator(NULL)
		, file_location(NULL)
		, name_symbol(name_symbol_)
		, owner(NULL)
		, initial_value(NULL)
		, signature(NULL), pool_index(0)
		  , accessed_local(NULL)
		, external_name_symbol(NULL)
		, status(0)
		, local_variable_index(-1)
		, type_(NULL)
		, signature_string(NULL),
		field_declaration(nullptr)
	{
		Symbol::_kind = VARIABLE;
	}

	VariableSymbol::~VariableSymbol()
	{
		delete [] signature_string;
		delete file_location;

	}

	int VariableSymbol::LocalVariableIndex(Semantic* sem)
	{
		if (IsLocal(sem->ThisMethod()))
		{
			assert(sem->FinalFields());
			return local_variable_index + sem->FinalFields()->Length();
		}
		return local_variable_index;
	}

	void VariableSymbol::SetLocation()
	{
		if (!declarator)
		{
			file_location = new FileLocation(ContainingType()->file_symbol);
		}
		else
		{
			file_location =
				new FileLocation((ContainingType()->semantic_environment ->
					sem->lex_stream),
					declarator->LeftToken());
		}
	}

	void VariableSymbol::ProcessVariableSignature(Semantic* sem,
		Token* token_location)
	{
		if (!type_)
		{
			assert(sem);
			const char* signature = signature_string;

			SetType(sem->ProcessSignature((TypeSymbol*)owner, signature,
				token_location));
			assert(!*signature);
		}
	}

	

} // Close namespace Jikes block





