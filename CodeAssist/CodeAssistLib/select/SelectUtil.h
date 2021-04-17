#pragma once
#include <vector>



namespace Jikes {
	class MethodSymbol;
	class TypeSymbol;
	class VariableSymbol;
	class AstNodeScope;

	namespace  JavaModel
	{
		class ISelectionRequestor;
		class IMethod;
		class IType;
		class IJavaElement;
		
	}
	namespace CodeAssist {
		class SelectUtil
		{
		public:
		static std::vector<JavaModel::IMethod*> getSelectOnMethod(
			MethodSymbol* method_symbol,
			std::wstring selector, 
			JavaModel::ISelectionRequestor* requestor
			);
	
		static  JavaModel::IType* getSelectOnType(TypeSymbol* type_symbol,
			JavaModel::ISelectionRequestor* requestor);

		static  JavaModel::IJavaElement* selectOnField(AstNodeScope*scope,VariableSymbol* field_symbol,
			JavaModel::ISelectionRequestor* requestor);

		};
		
	}//namespace CodeAssist
} // Close namespace Jikes block


