
#ifndef JCDT_StatisticsEngine__Search_H_FLAG_
#define JCDT_StatisticsEngine__Search_H_FLAG_

#include <set>

#include <JCDT_Lib/compiler/Compiler.h>
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
namespace Search {
	class ReferenceFileResultElement;
	
	class ReferenceVisitorResult;
	class FindReferenceOption;
	class ReferenceVisitorBase;
	class StatisticsEngine:public  Compiler
	{
	public:
	
		StatisticsEngine(INameEnvironment*, Option*, JavaModel::JavaModel*, FindReferenceOption*  _reference_option);
		virtual ~StatisticsEngine();
		void clear();
		void Staticstics(set<wstring>& filePaths);
		
		map<wstring, ReferenceFileResultElement*> result;

		map<wstring, JavaModel::ISelectionRequestor*> selectorRequestorMap;
	private:

		void ProcessMehod(map < wstring, vector< pair<Symbol*, unsigned int>  > >&files_map);

	
		void PreProcess(set<wstring>& filePaths);

		JavaModel::JavaModel* m_pJM;
		void getUnits();
		map<wstring, FileSymbol*> files;
	
		ReferenceVisitorBase* visitor;
		FindReferenceOption*  reference_option;
	};
}//namespace Search

} // Close namespace Jikes block


#endif // JCDT_StatisticsEngine__Search_H_FLAG_
