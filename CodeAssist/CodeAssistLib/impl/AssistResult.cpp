#include "stdafx.h"
#include "AssistResult.h"
#include <CodeAssistLib/complete/ResultCollector.h>
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <JavaModelLib/internal/core/IJavaElementType.h>
#include "CodeAssistLib/ast/AssistExpression.h"


namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

		 AssistResult::~AssistResult()
		{
			delete info;
		}

		ResultForTips::ResultForTips(): rng(nullptr)
		{
		}

		ResultForTips::~ResultForTips()
		{
			if (rng)
			{
				delete rng;
			}
		}

		GoToDeclItem::GoToDeclItem(wstring& lpsz_item_name, Jikes::JavaModel::IJavaElementType i_type,
		                           const Jikes::AccessFlags& _access, const wstring& file_path, JavaModel::ISourceRange* _rng,
		                           bool _binary): lpszItemName(lpsz_item_name),
		                                          iType(i_type),
		                                        
		                                          filePath(file_path),
		                                          rng(_rng),
		                                          is_binary(_binary)
		{
			this->access = new AccessFlags(_access);
		}

		GoToDeclItem::~GoToDeclItem()
		{
			delete rng;
		}

		ResultForDebug::~ResultForDebug()
		{
			delete expr;
		}

		ResultForScope::~ResultForScope()
		{
			delete range;
		}

		ResultCodeComplete::ResultCodeComplete(): lenEntered(0), collector(nullptr){
		}

		ResultCodeComplete::~ResultCodeComplete()
		{
			delete collector;
		}

		ResultForCorrectionError::~ResultForCorrectionError()
		{
			delete pTipsInfo;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

