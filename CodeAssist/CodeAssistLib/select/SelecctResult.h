#ifndef Jikes_Jikes_SelecctResult_INCLUDED
#define Jikes_Jikes_SelecctResult_INCLUDED
#include <string>

namespace Jikes { // Open namespace Jikes block
	class Ast;
	namespace  JavaModel
	{
		class ISourceRange;
	}
namespace CodeAssist{

	class AssistExpression;
	class AssistInfo;
	class SelecctResult{
	public:
		enum EnumType
		{
			SELECT_FOR_DEBUG_TYPE,
			SELECT_FOR_EDIT_TYPE,
			SELECT_FOR_SCOPE_TYPE
		};
		virtual ~SelecctResult()
		{
		}

		virtual EnumType GetType();
	};

	
	class SelectForDebug :public SelecctResult
	{
	public:

		virtual EnumType GetType();
		std::wstring expr_string_;
		AssistExpression* expr = nullptr;
		~SelectForDebug();

		AssistExpression* TransferExpr();
	};
	class SelectForScope :public SelecctResult
	{
	public:
		JavaModel::ISourceRange*  range;
		virtual EnumType GetType();

		~SelectForScope();

		JavaModel::ISourceRange* TransferRange();
	};
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

