
#ifndef ASTResolving_Jikes_Correction_INCLUDED_INCLUDED
#define ASTResolving_Jikes_Correction_INCLUDED_INCLUDED
#include <string>
#include <vector>
#include <JCDT_Lib/internal/impl/ITypeRequestor.h>

namespace Jikes { // Open namespace Jikes block
	class Option;
	class INameEnvironment;
	class Parser;
	class Scanner;
	class LookupEnvironment;
	class UnitFileSymbol;
	class TypeSymbol;

	namespace  JavaModel
	{
		class JavaModel;
		class SourceMapperFile;
	}


namespace Correction{

	class CorrectionAssistASTVisitor;

	class ASTResolving:public ITypeRequestor
	{
	public:
		std::wstring file;
		int line;
		int column;
		Option* option;
		INameEnvironment* nameEnv;
		Parser* parser;
		Scanner* scanner;
		UnitFileSymbol* lexFile;
		CorrectionAssistASTVisitor* visitor;
		LookupEnvironment *lookupEnvironment;
		~ASTResolving();

		ASTResolving(int _line,
		             int _column,

		             JavaModel::JavaModel* pJm,
		             std::wstring& projectName,
		             std::wstring& _file
		);

		bool findSelectedNode();

		std::vector<std::wstring> GetTypeNames();
		bool HeadParse(FileSymbol*) override;
		bool accept(FileSymbol*) override;
		AstPackageDeclaration* ParseForPackage(FileSymbol*, AstStoragePool*) override;
		TypeSymbol * getMethodInvocatePossibleTypeBinding();
		bool BindingProcess();
		TypeSymbol* 	getTypeBinding();
		TypeSymbol* getPossibleTypeBinding();
	};


}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

