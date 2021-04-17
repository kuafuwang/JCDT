
#ifndef SelectionEngine_INCLUDED
#define SelectionEngine_INCLUDED
#include "CodeAssistLib/impl/Engine.h"
#include <JavaModelLib/internal/lookup/ISelectionRequestor.h>
#include <CodeAssistLib/impl/AssistInfo.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>


namespace Jikes{
	class IProgressMonitor;
}

namespace Jikes{
	class IProgressMonitor;
	
}

namespace Jikes { // Open namespace Jikes block
	
	class AstNodeScope;

namespace CodeAssist{
	class SelectionOnNameReference;
	class SelecctResult;

	class SelectionEngine:public Engine
	{
	public:
		static JavaModel::IJavaElement*  getSelectJavaElement(CodeAssist::AssistInfo* cur_info, JavaModel::JavaModel* pJM,bool tryNoCopyChildren);

		wstring getDeclareTypeName(vector<TypeDeclaration*>& type_stack);


		SelectionEngine(INameEnvironment*, Option*,
			JavaModel::ISelectionRequestor*, 
			AssistInfo& info,set<wstring>& __error_files
		);
		virtual ~SelectionEngine();
		SelecctResult* select();

	protected:
		 unsigned checkSelection(LexStream*, unsigned int, AssistInfo&) ;
		JavaModel::ISelectionRequestor* requestor;
		ASTVisitor*   visitor;
	private:
		
		set<wstring> error_files;
		JavaModel::FileTypeInJavaProject  file_type;
		void selectOnField(VariableSymbol* field_symbol, AstNodeScope* scope);

		void SelectOnExpression(Expression* expr, AstNodeScope* scope);


		void SelectOnConstructor(TypeSymbol* methodOwner, AstArguments* arguments);
		
		void SelectOnConstructorCall(SelectionOnNameReference& expetion);


		vector<JavaModel::IMethod*> getSelectOnMethod(MethodSymbol* method_symbol,
			wstring selector,
			bool construtor);
		JavaModel::IType* getSelectOnType(TypeSymbol* type_symbol);
		void selectFrom(TypeSymbol* type_symbol);

		AssistInfo& _info;
	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

