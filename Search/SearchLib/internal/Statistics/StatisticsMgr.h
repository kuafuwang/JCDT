
#ifndef JCDT_StatisticsMgr__Search_H_FLAG_
#define JCDT_StatisticsMgr__Search_H_FLAG_

#include <set>
#include <map>

using std::set;
using std::wstring;

namespace Jikes {

	// Open namespace Jikes block

/**
 * API to jikes compiler.
 */
class JikesError;
class AstStoragePool;
class Option;
class Scanner;
class Parser;
class Semantic;
class LexStream;
class AstPackageDeclaration;
class AstName;
class TypeDependenceChecker;
class INameEnvironment;
class LookupEnvironment;
	namespace JavaModel
	{
		class ISelectionRequestor;
		class IMethod;
		class JavaModel;
		
	}
namespace Search 
{
	class InFilesFindReferenceOption;
	class ReferenceFileResultElement;

	class StatisticsMgr
	{
	public:
	
		StatisticsMgr();
		 ~StatisticsMgr(){};
		 void Process(InFilesFindReferenceOption* process_item, JavaModel::JavaModel* pJM, std::map<wstring, ReferenceFileResultElement*>& result);
	};

	
}//namespace Search

} // Close namespace Jikes block


#endif // JCDT_StatisticsMgr__Search_H_FLAG_
