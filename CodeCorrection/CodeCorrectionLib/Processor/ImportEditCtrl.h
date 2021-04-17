
#ifndef ImportEditCtrl_Jikes_Correction_INCLUDED_INCLUDED
#define ImportEditCtrl_Jikes_Correction_INCLUDED_INCLUDED
#include <set>
#include <string>
#include <vector>


namespace Jikes { // Open namespace Jikes block

	class  CompilationUnitDeclaration;
	class LexStream;
	namespace  JavaModel
	{
		class CompilationUnit;
	}
namespace Correction{
	class ImportEditCtrl
	{
	public:
		int  packageLineNo;
		std::set< std::wstring> importWithStart;
		std::set< std::wstring> singleImports;
		static std::vector<std::wstring>  excludeImports;
		ImportEditCtrl(LexStream* lex,CompilationUnitDeclaration*);

		ImportEditCtrl(JavaModel::CompilationUnit* );
		bool AddImportEdit(const std::wstring& importName);
		bool IsNeedToImportEdit(const std::wstring& importName) const ;
		ImportEditCtrl* Clone();
	protected:
		ImportEditCtrl(int  _packageLineNo,
			std::set< std::wstring>& _importWithStart,
			std::set< std::wstring>& _singleImports);
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

