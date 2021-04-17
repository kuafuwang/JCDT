#include "stdafx.h"
#include "ImportEditCtrl.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <JCDT_Lib/internal/ast/CompilationUnitDeclaration.h>
#include <JCDT_Lib/internal/ast/AstPackageDeclaration.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>
#include <JavaModelLib/internal/core/IImportDeclaration.h>

using namespace  std;
using namespace  Jikes::JavaModel;
namespace Jikes { // Open namespace Jikes block


	namespace Correction {

		vector<wstring>	 ImportEditCtrl::excludeImports =
		{
			L"void", L"boolean", L"byte", L"char", L"short", L"int", L"long", L"float", L"double", L"java.lang.object", L"java.lang.*"
		}; //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$


		ImportEditCtrl::ImportEditCtrl(LexStream* lex,CompilationUnitDeclaration* unit)
		{
			assert(lex);

			assert(unit);

			if (unit->package_declaration_opt) {
				wstring  _pacakge_name = unit->package_declaration_opt->getImportName('.');
				boost::to_lower(_pacakge_name);
				importWithStart.insert(_pacakge_name);
				auto r_token = unit->package_declaration_opt->RightToken();
				assert(r_token);
				packageLineNo = lex->LineFromPos(r_token->start_location) + 1;
			}
			else
			{
				packageLineNo = 1;
			}
			auto num = unit->NumImportDeclarations();
			for (unsigned i = 0; i < num; i++) {
				auto _import_decl = unit->ImportDeclaration(i);

				assert(_import_decl);

				auto _import_name = _import_decl->getImportName('.');
				boost::to_lower(_import_name);
				if (_import_decl->star_token_opt) {
					importWithStart.insert(_import_name);
				}
				else {
					singleImports.insert(_import_name);
				}
			}
		}

		ImportEditCtrl::ImportEditCtrl(JavaModel::CompilationUnit* unit):packageLineNo(1)
		{
			assert(unit);
			auto packageDecls = unit->getPackageDeclarations();
			if(packageDecls)
			{
				// 插入的内容 判断是否为interface
				// 查看import 是否需要。 找到import的位置
				IPackageDeclaration* packageDecl = packageDecls->operator[ ](packageDecls->size() - 1);
				assert(packageDecl);
				auto pkgNameRange = packageDecl->getNameRange();
				if (pkgNameRange)
					packageLineNo = pkgNameRange->m_nStartLine + 1;

				else
					packageLineNo = 1;
				importWithStart.insert(packageDecl->getElementName());
			}
			auto importDecls = unit->getImports();
			if(importDecls){
				for(auto it : *importDecls){
					assert(it);
					AddImportEdit(it->getElementName());
				}
			}
		}

		bool ImportEditCtrl::AddImportEdit(const std::wstring& importName)
		{
			

			if (importName.empty())return  false;
			
		


			auto lowerCaseName = boost::to_lower_copy(importName);
			boost::replace_all(lowerCaseName, L"/", L".");

			auto idx = lowerCaseName.find(L"[");
			if (idx != wstring::npos){
				lowerCaseName = lowerCaseName.substr(0, idx);
			}
			

			for(auto it : excludeImports)
			{
				if (it == lowerCaseName)
					return  false;
			}

			bool start;
			wstring  packageName;
			auto size = lowerCaseName.size();
		    if(lowerCaseName[size - 1] == '*')
		    {
				start = true;
				if (size < 2)
					return  false;
				packageName = lowerCaseName.substr(0, size - 2);
		    }
		    else
		    {
				start = false;
				auto idx = lowerCaseName.rfind(L".");
				if(idx != wstring::npos){
					packageName = lowerCaseName.substr(0, idx);
				}
				
		    }

			for (auto it : importWithStart){
				if (it == packageName) {
					return  false;
				}
			}
			if(start){
				importWithStart.insert(packageName);
			}
			else{
				for(auto it : singleImports){
					if(it == lowerCaseName){
						return false;
					}
				}
				singleImports.insert(lowerCaseName);
			}
			return true;
		}

		bool ImportEditCtrl::IsNeedToImportEdit(const  std::wstring& importName) const
		{
			if (importName.empty())return  false;

		
			auto lowerCaseName = boost::to_lower_copy(importName);
			boost::replace_all(lowerCaseName, L"/", L".");

			auto idx = lowerCaseName.find(L"[");
			if (idx != wstring::npos) {
				lowerCaseName = lowerCaseName.substr(0, idx);
			}
			for (auto it : excludeImports)
			{
				if (it == lowerCaseName)
					return  false;
			}

			bool start;
			wstring  packageName;
			auto size = lowerCaseName.size();
			if (lowerCaseName[size - 1] == '*')
			{
				start = true;
				if (size < 2)
					return  false;
				packageName = lowerCaseName.substr(0, size - 2);
			}
			else
			{
				start = false;
				idx = lowerCaseName.rfind(L".");
				if (idx != wstring::npos) {
					packageName = lowerCaseName.substr(0, idx);
				}

			}

			for (auto it : importWithStart) {
				if (it == packageName) {
					return  false;
				}
			}
			if (start) {
				
			}
			else {
				for (auto it : singleImports) {
					if (it == lowerCaseName) {
						return false;
					}
				}
				
			}
			return true;

		}

		ImportEditCtrl* ImportEditCtrl::Clone()
		{
			return  new ImportEditCtrl(packageLineNo, importWithStart, singleImports);
		}

		ImportEditCtrl::ImportEditCtrl(int _packageLineNo, std::set<std::wstring>& _importWithStart, std::set<std::wstring>& _singleImports)
			:packageLineNo(_packageLineNo),importWithStart(_importWithStart),singleImports(_singleImports)
		{

		}
	} // Close namespace Correction block

} // Close namespace Jikes block

