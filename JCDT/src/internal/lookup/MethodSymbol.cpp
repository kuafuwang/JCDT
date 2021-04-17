
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/semantic/SemanticEnvironment.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/lookup/BlockSymbol.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include  <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/ast/ScopeUtil.h>

namespace Jikes { // Open namespace Jikes block


	MethodSymbol::MethodSymbol(const NameSymbol* name_symbol_)
		: declaration(NULL)
		, name_symbol(name_symbol_)
		, containing_type(NULL)
		, block_symbol(NULL)
		, next_method(NULL)
		, signature(NULL)
		, file_location(NULL), pool_index(0)
		  , max_block_depth(1), isConstructor(false)
		  // there must be at least one block in a method
							 // this default is useful for default constructors.
		, accessed_member(NULL)
		, external_name_symbol(NULL)
		, status(0)
		, header(NULL)
		, type_(NULL)
		, formal_parameters(NULL)
		, throws(NULL)
		, throws_signatures(NULL)
		
	{
		Symbol::_kind = METHOD;
	}
	

	wchar_t* MethodSymbol::Header()
	{
		assert(type_);

		if (!header)
		{
			bool is_constructor = Name()[0] == U_LESS && Name()[1] == U_i;
			unsigned num_parameters = NumFormalParameters();
			int length = (Type()->ContainingPackage()->PackageNameLength() +
				Type()->ExternalNameLength() +
				(is_constructor ? containing_type->NameLength()
					: NameLength())
				+ 5); // +5 for '.' after package_name, ' ' after type,
					  // '(' after name, ')' after all parameters,
					  // ';' to terminate
			for (unsigned i = 0; i < num_parameters; i++)
			{
				VariableSymbol* formal = FormalParameter(i);
				length += (formal->Type()->ContainingPackage()->PackageNameLength() +
					formal->Type()->ExternalNameLength() +
					formal->NameLength() + 4);
				// +4 for '.' after package_name, ' ' after type; ',' and ' ' to
				// separate this formal parameter from the next one. Last
				// parameter may need '...' instead of '[]', but doesn't need ', '.
			}

			if (throws_signatures && NumThrowsSignatures())
			{
				length += 7; // for " throws"
				for (unsigned j = 0; j < NumThrowsSignatures(); j++)
					length += strlen(ThrowsSignature(j)) + 2; // +2 for ", "
			}
			else if (NumThrows())
			{
				length += 7; // for " throws"
				for (unsigned j = 0; j < NumThrows(); j++)
				{
					TypeSymbol* exception = Throws(j);
					length += (exception->ContainingPackage() ->
						PackageNameLength() +
						exception->ExternalNameLength() + 3);
					// +3 for " throws", '.' after package_name, and ',' and
					// ' ' to separate this throws clause from the next one
				}
			}

			header = new wchar_t[length + 1]; // +1 for '\0'
			wchar_t* s = header;
			const wchar_t* s2;

			if (is_constructor)
			{
				for (s2 = containing_type->Name(); *s2; s2++)
					*s++ = *s2;
			}
			else
			{
				PackageSymbol* package = Type()->ContainingPackage();
				wchar_t* package_name = package->PackageName();
				if (package->PackageNameLength() > 0 &&
					wcscmp(package_name, StringConstant::US_DOT) != 0)
				{
					while (*package_name)
					{
						*s++ = (*package_name == U_SLASH ? (wchar_t)U_DOT
							: *package_name);
						package_name++;
					}
					*s++ = U_DOT;
				}

				for (s2 = Type()->ExternalName(); *s2; s2++)
					*s++ = *s2;
				*s++ = U_SPACE;
				for (s2 = Name(); *s2; s2++)
					*s++ = *s2;
			}
			*s++ = U_LEFT_PARENTHESIS;
			if (num_parameters > 0)
			{
				for (unsigned k = 0; k < num_parameters; k++)
				{
					VariableSymbol* formal = FormalParameter(k);

					PackageSymbol* package =
						formal->Type()->ContainingPackage();
					wchar_t* package_name = package->PackageName();
					if (package->PackageNameLength() > 0 &&
						wcscmp(package_name, StringConstant::US_DOT) != 0)
					{
						while (*package_name)
						{
							*s++ = (*package_name == U_SLASH ? (wchar_t)U_DOT
								: *package_name);
							package_name++;
						}
						*s++ = U_DOT;
					}

					for (s2 = formal->Type()->ExternalName(); *s2; s2++)
					{
						*s++ = *s2;
					}
					if (k == num_parameters - 1 && ACC_VARARGS())
					{
						assert(s[-2] == U_LB && s[-1] == U_RB);
						s[-2] = U_DOT;
						s[-1] = U_DOT;
						*s++ = U_DOT;
					}
					*s++ = U_SPACE;
					for (s2 = formal->Name(); *s2; s2++)
						*s++ = *s2;
					*s++ = U_COMMA;
					*s++ = U_SPACE;
				}

				s -= 2; // remove the last ',' and ' '
			}
			*s++ = U_RIGHT_PARENTHESIS;

			if (throws_signatures && NumThrowsSignatures())
			{
				*s++ = U_SPACE;
				*s++ = U_t;
				*s++ = U_h;
				*s++ = U_r;
				*s++ = U_o;
				*s++ = U_w;
				*s++ = U_s;
				for (unsigned k = 0; k < NumThrowsSignatures(); k++)
				{
					*s++ = U_SPACE;
					for (char* signature = ThrowsSignature(k);
					*signature; signature++)
					{
						*s++ = (*signature == U_SLASH ? (wchar_t)U_DOT
							: *signature);
					}
					*s++ = U_COMMA;
				}
				s--; // remove the last ','
			}
			else if (NumThrows() > 0)
			{
				*s++ = U_SPACE;
				*s++ = U_t;
				*s++ = U_h;
				*s++ = U_r;
				*s++ = U_o;
				*s++ = U_w;
				*s++ = U_s;
				for (unsigned k = 0; k < NumThrows(); k++)
				{
					TypeSymbol* exception = Throws(k);

					PackageSymbol* package = exception->ContainingPackage();
					wchar_t* package_name = package->PackageName();
					*s++ = U_SPACE;
					if (package->PackageNameLength() > 0 &&
						wcscmp(package_name, StringConstant::US_DOT) != 0)
					{
						while (*package_name)
						{
							*s++ = (*package_name == U_SLASH ? (wchar_t)U_DOT
								: *package_name);
							package_name++;
						}
						*s++ = U_DOT;
					}

					for (s2 = exception->ExternalName(); *s2; s2++)
						*s++ = *s2;
					*s++ = U_COMMA;
				}

				s--; // remove the last ','
			}

			*s++ = U_SEMICOLON;
			*s = U_NULL;

			assert((s - header) <= length);
		}
		return header;
	}

	void MethodSymbol::SetLocation()
	{
		//MethodDeclaration or ConstructorDeclaration
		if (file_location)
			return;
		if (!declaration)
			file_location = new FileLocation(containing_type->file_symbol);
		else
		{
			MethodDeclaration* method_declaration =
				MethodDeclaration::MethodDeclarationCast(declaration);
			ConstructorDeclaration* constructor_declaration =
				ConstructorDeclaration::ConstructorDeclarationCast(declaration);

			file_location =
				new FileLocation((containing_type->semantic_environment ->
					sem->lex_stream),
					(method_declaration
						? method_declaration->LeftToken()
						: constructor_declaration->LeftToken()));
		}
	}

	bool MethodSymbol::areParametersEqual(MethodSymbol* method) const
	{
		int length = method->NumFormalParameters();
		if (length != NumFormalParameters())
			return false;

		for (int i = 0; i < length; i++)
			if (FormalParameter(i)->Type() != method->FormalParameter(i)->Type() )
				return false;
		return true;
	}

	void MethodSymbol::CleanUp()
	{
		BlockSymbol* block = new BlockSymbol(NumFormalParameters());

		//
		// Make a copy of each parameter into the new pared-down symbol table and
		// fix the FormalParameter information to identify the new symbol.
		//
		for (unsigned k = 0; k < NumFormalParameters(); k++)
		{
			VariableSymbol* formal_parameter = (*formal_parameters)[k];
			VariableSymbol* symbol =
				block->InsertVariableSymbol(formal_parameter->Identity());
			symbol->SetType(formal_parameter->Type());
			symbol->MarkComplete();
			(*formal_parameters)[k] = symbol;
		}

		//
		// Destroy the old symbol and replace it by the new one.
		//
		delete block_symbol;
		block->CompressSpace(); // space optimization
		SetBlockSymbol(block);

		declaration = NULL; // remove reference to Ast structure
	}

	void MethodSymbol::GetParameterNames(vector<wstring>& parameterPackageNames, vector<wstring>& parameterTypeNames)
	{
		auto _num = NumFormalParameters();
		for (auto i = 0; i < _num; ++i)
		{
			auto parameter = FormalParameter(i);
			wstring parameterPackageName;
			wstring parameterTypeName;
			auto type_symbol_parameter = parameter->Type();
			if (type_symbol_parameter) {
				auto packae_symbol_parameter = type_symbol_parameter->ContainingPackage();
				const wchar_t* temp = packae_symbol_parameter->PackageName();
				if (temp) {
					parameterPackageName.assign(temp, temp + packae_symbol_parameter->PackageNameLength());
					boost::replace_all(parameterPackageName, L"/", L".");
				}
				temp = type_symbol_parameter->Name();
				if (temp)
					parameterTypeName.assign(temp, temp + type_symbol_parameter->NameLength());
			}
			parameterPackageNames.push_back(parameterPackageName);
			parameterTypeNames.push_back(parameterTypeName);

		}
	}


	void MethodSymbol::GetParameterTypeQuilifiedNames(vector<wstring>& parameterTypes)
	{
		auto _num = NumFormalParameters();
		if (!_num)
			return;
		vector<wstring> parameterPackageNames;
		vector<wstring> parameterTypeNames;
		for (auto i = 0; i < _num; ++i)
		{
			auto parameter = FormalParameter(i);
			wstring parameterPackageName;
			wstring parameterTypeName;
			auto type_symbol_parameter = parameter->Type();
			if (type_symbol_parameter) {
				auto packae_symbol_parameter = type_symbol_parameter->ContainingPackage();
				const wchar_t* temp = packae_symbol_parameter->PackageName();
				if (temp) {
					parameterPackageName.assign(temp, temp + packae_symbol_parameter->PackageNameLength());
					boost::replace_all(parameterPackageName, L"/", L".");
				}
				temp = type_symbol_parameter->Name();
				if (temp)
					parameterTypeName.assign(temp, temp + type_symbol_parameter->NameLength());
			}
			parameterPackageNames.push_back(parameterPackageName);
			parameterTypeNames.push_back(parameterTypeName);

		}
		if (parameterTypeNames.size() != 0) {
			for (int i = 0, max = parameterTypeNames.size(); i < max; i++) {
				wstring pkg ;
				if (parameterPackageNames[i].empty() != true && parameterPackageNames[i].length() > 0) {
					pkg = parameterPackageNames[i] + L"."; //$NON-NLS-1$
				}
				auto typeName = parameterTypeNames[i];
				if (typeName.find('.') > 0)
					boost::replace_all(typeName, L".", L"$");
				parameterTypes.push_back(pkg + typeName);
			}
		}
	}

	bool MethodSymbol::canBeSeenBy(InvocationSite* invocationSite, AstNodeScope* scope)
	{
		
		if (ACC_PUBLIC()) return true;



		TypeSymbol* invocationType = ScopeUtil::enclosingSourceType(scope);
		if (!invocationType){
			return false;
		}

	
		if (invocationType == containing_type) 
			return true;

		if (ACC_PROTECTED()) {
			// answer true if the receiver is in the same package as the invocationType
			if (invocationType->package == containing_type->package) return true;
			return invocationSite->isSuperAccess();
		}

		if (ACC_PRIVATE()) {
			// answer true if the invocationType and the declaringClass have a common enclosingType
			// already know they are not the identical type
			auto outerInvocationType = invocationType;
			auto temp = outerInvocationType->EnclosingType();
			while (temp != nullptr) {
				outerInvocationType = temp;
				temp = temp->EnclosingType();
			}

			auto  outerDeclaringClass = containing_type;
			temp = outerDeclaringClass->EnclosingType();
			while (temp != nullptr) {
				outerDeclaringClass = temp;
				temp = temp->EnclosingType();
			}
			return outerInvocationType == outerDeclaringClass;
		}

		// isDefault()
		return invocationType->package == containing_type->package ;
	}

	/* Answer true if the receiver is visible to the type provided by the scope.
	* InvocationSite implements isSuperAccess() to provide additional information
	* if the receiver is protected.
	*
	* NOTE: Cannot invoke this method with a compilation unit scope.
	*/
	  bool MethodSymbol::canBeSeenBy(TypeSymbol* receiverType, InvocationSite* invocationSite, AstNodeScope* scope) {
		if (ACC_PUBLIC()) return true;

		TypeSymbol* invocationType = ScopeUtil::enclosingSourceType(scope);
		if (!invocationType) {
			return false;
		}


		if (invocationType == containing_type && invocationType == receiverType) return true;

		if (ACC_PROTECTED()) {
			// answer true if the invocationType is the declaringClass or they are in the same package
			// OR the invocationType is a subclass of the declaringClass
			//    AND the receiverType is the invocationType or its subclass
			//    OR the method is a static method accessed directly through a type
			//    OR previous assertions are true for one of the enclosing type
			if (invocationType == containing_type) return true;
			if (invocationType->package == containing_type->package) return true;

			auto currentType = invocationType;
			do {
				if ( currentType->IsSubclass(containing_type)) {
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
					if (currentType == receiverType || receiverType->IsSubclass( currentType ) ) {
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
			if (receiverType != containing_type) return false;

			if (invocationType != containing_type) {
				auto outerInvocationType = invocationType;
				auto temp = outerInvocationType->EnclosingType();
				while (temp != nullptr) {
					outerInvocationType = temp;
					temp = temp->EnclosingType();
				}

				auto outerDeclaringClass = containing_type;
				temp = outerDeclaringClass->EnclosingType();
				while (temp != nullptr) {
					outerDeclaringClass = temp;
					temp = temp->EnclosingType();
				}
				if (outerInvocationType != outerDeclaringClass) return false;
			}
			return true;
		}

		// isDefault()
		if (invocationType->package != containing_type->package ) return false;

		// receiverType can be an array binding in one case... see if you can change it
		if (receiverType->IsArray())
			return false;
		auto type = receiverType;

		auto declaringPackage = containing_type->package;
		do {
			if (containing_type == type) return true;
			if (declaringPackage != type->package) return false;
		} while ((type = type->super) != nullptr);
		return false;
	}



	void MethodSymbol::ProcessMethodSignature(Semantic* sem,
		Token* token_location)
	{
		if (!type_)
		{
			assert(sem);

			int num_parameters = 0;
			const char* signature = SignatureString();
			assert(*signature == U_LEFT_PARENTHESIS);
			signature++; // +1 to skip initial '('

						 //
						 // For the constructor of an inner type, skip the "this$0" argument.
						 //
			if (containing_type->EnclosingType() &&
				!containing_type->EnclosingType()->ACC_PRIVATE() &&
				Identity() == sem->lookup_env.nameEnv->init_name_symbol)
			{
				TypeSymbol* enclosing = sem->ProcessSignature(containing_type,
					signature,
					token_location);
				assert(enclosing == containing_type->EnclosingType());
			}

			while (*signature && *signature != U_RIGHT_PARENTHESIS)
			{
				//
				// Make up a name for each parameter.
				//
				NameSymbol* name_symbol =
					sem->lookup_env.MakeParameter(++num_parameters);
				VariableSymbol* symbol = new VariableSymbol(name_symbol);
				symbol->SetType(sem->ProcessSignature(containing_type,
					signature,
					token_location));
				symbol->MarkComplete();
				AddFormalParameter(symbol);
			}
			assert(*signature == U_RIGHT_PARENTHESIS);
			signature++; // skip ')'

						 //
						 // Now set the type of the method.
						 //
			if (Identity() == sem->lookup_env.nameEnv->init_name_symbol)
			{
				assert(*signature++ == U_V);
				SetType(containing_type);
			}
			else
			{
				SetType(sem->ProcessSignature(containing_type, signature,
					token_location));
			}
			assert(!*signature);

			//
			// Create a symbol table for this method for consistency, and in
			// order to release the space used by the variable paramaters later.
			//
			BlockSymbol* block_symbol = new BlockSymbol(num_parameters);
			for (int k = 0; k < num_parameters; k++)
				block_symbol->InsertVariableSymbol((*formal_parameters)[k]);
			block_symbol->CompressSpace(); // space optimization
			SetBlockSymbol(block_symbol);
		}
	}


	void MethodSymbol::ProcessMethodThrows(Semantic* sem, Token* tok)
	{
		if (throws_signatures)
		{
			assert(sem);

			//
			// Process throws clause
			//
			for (unsigned i = 0; i < NumThrowsSignatures(); i++)
			{
				TypeSymbol* type =
					sem->ReadTypeFromSignature(containing_type,
						ThrowsSignature(i),
						strlen(ThrowsSignature(i)),
						tok);
				AddThrows(type);
				delete[] ThrowsSignature(i);
			}

			delete throws_signatures;
			throws_signatures = NULL;
		}
	}


	//
	// In addition to (re)setting the signature, this updates the
	// max_variable_index if needed.
	//
	void MethodSymbol::SetSignature(LookupEnvironment& control, TypeSymbol* placeholder)
	{
		unsigned i;
		bool is_constructor = Identity() == control.nameEnv->init_name_symbol;
		auto _type = Type();
		int len = 3;
		if(_type)
		{
			auto _sig = _type->SignatureString();
			if(_sig)
				len = is_constructor ? 3 : 2 + strlen(_sig);
			else
				len = is_constructor ? 3 : 2;
		}
		 
		// +1 for '(' +1 for ')'; constructors have type 'V'

		TypeSymbol* this0_type = containing_type->EnclosingType();
		int variable_index = ACC_STATIC() ? 0 : 1;

		if (is_constructor && this0_type)
		{
			len += strlen(this0_type->SignatureString());
			variable_index++;
		}
		for (i = 0; i < NumFormalParameters(); i++)
		{
			TypeSymbol* formal_type = FormalParameter(i)->Type();
			if(formal_type->SignatureString())
				len += strlen(formal_type->SignatureString());
			variable_index += (control.IsDoubleWordType(formal_type) ? 2 : 1);
		}
		if (is_constructor)
		{
			for (i = 0; i < containing_type->NumConstructorParameters(); i++)
			{
				TypeSymbol* shadow_type =
					containing_type->ConstructorParameter(i)->Type();
				if(shadow_type->SignatureString())
					len += strlen(shadow_type->SignatureString());
				variable_index += (control.IsDoubleWordType(shadow_type) ? 2 : 1);
			}
			if (placeholder)
			{
				if(placeholder->SignatureString())
					len += strlen(placeholder->SignatureString());
				variable_index++;
			}
		}
		if (block_symbol && variable_index > block_symbol->max_variable_index)
			block_symbol->max_variable_index = variable_index;

		char* method_signature = new char[len + 1]; // +1 for '\0'
		method_signature[0] = U_LEFT_PARENTHESIS;
		int s = 1;
		if (is_constructor && this0_type)
		{
			for (const char* str = this0_type->SignatureString();
			*str; str++, s++)
			{
				method_signature[s] = *str;
			}
		}
		for (i = 0; i < NumFormalParameters(); i++)
		{
			TypeSymbol* formal_type = FormalParameter(i)->Type();
			for (const char* str = formal_type->SignatureString();
			*str; str++, s++)
			{
				method_signature[s] = *str;
			}
		}
		if (is_constructor)
		{
			for (i = 0; i < containing_type->NumConstructorParameters(); i++)
			{
				TypeSymbol* shadow_type =
					containing_type->ConstructorParameter(i)->Type();
				for (const char* str = shadow_type->SignatureString();
				*str; str++, s++)
				{
					method_signature[s] = *str;
				}
			}
			if (placeholder)
				for (const char* str = placeholder->SignatureString();
			*str; str++, s++)
			{
				method_signature[s] = *str;
			}
		}
		method_signature[s++] = U_RIGHT_PARENTHESIS;
		if (is_constructor)
		{
			assert(Type() == containing_type);
			method_signature[s++] = U_V;
		}
		else
		{
			for (const char* str = Type()->SignatureString(); *str; str++, s++)
				method_signature[s] = *str;
		}
		method_signature[s] = U_NULL;

		signature = control.nameEnv->Utf8_pool.FindOrInsert(method_signature, len);

		delete[] method_signature;
	}


	MethodSymbol::~MethodSymbol()
	{
		for (unsigned i = 0; i < NumThrowsSignatures(); i++)
			delete[] ThrowsSignature(i);
		delete throws_signatures;
		delete formal_parameters;
		delete throws;

		delete block_symbol; // overload(s)
		delete[] header;
		delete file_location;
	}


} // Close namespace Jikes block


