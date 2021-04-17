#include "stdafx.h"
#include <JCDT_Lib/internal/ast/ast.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>
#include <JavaModelLib/internal/core/PackageFragment.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/SourceType.h>
#include  <boost/algorithm/string.hpp>
#include <JavaModelLib/internal/core/IPackageDeclaration.h> 
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JavaModelLib/internal/core/PackageDeclaration.h>
#include <JavaModelLib/internal/core/BaseProject.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>

#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/parser/parser.h>
#include <JCDT_Lib/internal/lex/scanner.h>
#include <JavaModelLib/internal/lookup/SourceMapperFile.h>
#include <JavaModelLib/internal/core/IImportDeclaration.h>
#include <JavaModelLib/internal/core/ImportDeclaration.h>
#include <JCDT_Lib/internal/impl/option.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	bool CompilationUnit::generateInfos(IProgressMonitor* pm)
	{
		if(is_Clone){
			return CopyChildFromOriginalWhenIsClone();
		}
		if (!fParent)
			return false;

		Option* option = new Option();
		JikesClassPaths _no_paths;
		INameEnvironment*  name_env = new INameEnvironment(option, _no_paths);
		Parser* parser = new Parser();
		Scanner* scanner = new Scanner(option, name_env);

		if (m_path.empty()){
			m_path = BaseJavaFile::getPath();
		}
		do{
			SourceMapperFile* lexFile = new SourceMapperFile(CharOperation::ConvertUnicodeToUtf8(m_path));
			scanner->Scan(lexFile);
			auto lex = lexFile->lex_stream;
			if (lex) {
				auto unit = parser->HeaderParse(lex);
				if (!unit) {
					delete lexFile;
					break;
				}
				if (unit->package_declaration_opt){
					if (!packageDecls)
						packageDecls = new vector<IPackageDeclaration*>();
					auto _pkgName = unit->package_declaration_opt->getImportName();
					auto pkg = new PackageDeclaration(_pkgName, this);
					packageDecls->push_back(pkg);

					auto left = unit->package_declaration_opt->LeftToken();
					auto right = unit->package_declaration_opt->RightToken();
					if (right && left) {
						auto  left_line_no = lex->LineFromPos(left->start_location);
						auto left_column_no = lex->ColumnFromPos(left->start_location);
						auto  right_line_no = lex->LineFromPos(right->start_location);
						auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
						auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
						pkg->setSourceRange(rng);
						pkg->setNameRange(rng->Clone());
					}

				}

				auto num = unit->NumTypeDeclarations();
				if (num){
					if (!types)
						types = new vector<IType*>();
					else{
						types->clear();
					}
				;
					vector<wstring> compoundPkg;
					auto pkgName = fParent->getElementName();
					boost::split(compoundPkg, pkgName, boost::is_any_of(L"."));
					for (auto i = 0; i < num; ++i) {
						TypeDeclaration* declaration =unit->GetTypeDeclaration(i);
						if(!declaration->class_body || !declaration->class_body->identifier_token)
							continue;

						auto name = declaration->class_body->identifier_token->getName();
						auto type = new SourceType(this, name);
						types->push_back(type);
						type->info = new SourceTypeElementInfo();

						type->info->qualifiedTypeNames.assign(compoundPkg.begin(), compoundPkg.end());
						type->info->name = (name);
						SourceType::BuildStructure(type, type->info, declaration, lex);
					}
				}


				num = unit->NumImportDeclarations();

				if (num) {
					if (!importDecls)
						importDecls = new vector<IImportDeclaration*>(num);
					else{
						importDecls->assign(num, nullptr);
					}
					IImportDeclaration** import_data = importDecls->data();
					for (unsigned i = 0; i < num; i++) {
						auto _import_decl = unit->ImportDeclaration(i);
						auto import_item = new ImportDeclaration(this, _import_decl->getFullName());
						import_data[i] = import_item;
						auto left = _import_decl->LeftToken();
						auto right = _import_decl->RightToken();
						if (right && left) {
							auto  left_line_no = lex->LineFromPos(left->start_location);
							auto left_column_no = lex->ColumnFromPos(left->start_location);
							auto  right_line_no = lex->LineFromPos(right->start_location);
							auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
							auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
							import_item->setSourceRange(rng);
							import_item->setNameRange(rng->Clone());
						}
					}
				}
				delete unit->ast_pool;
			}
			delete lexFile;
		} while (false);
		
	
		delete scanner;
		delete option;
		delete parser;
		delete name_env;
		return true;

	}
	
	



	//IType* CompilationUnit::createIndependentType(const wstring& name)
	//{
	//	auto temp = new IndependentSourceType(this, name);
	//	temp->getMosesPath();
	//	return temp;
	//}


	wstring CompilationUnit::getPath()
	{
		if(m_path.empty()){
			m_path = BaseJavaFile::getPath();
		}
		return m_path;
	}

	IImportDeclaration* CompilationUnit::getImport(const wstring& name)
	{
		if(importDecls){
			auto data = importDecls->data();
			for (size_t i = 0, size = importDecls->size(); i < size; ++i)
			{
				auto temp = data[i];
				if (temp && temp->getElementName() == name) {
					return temp;
				}
			}
		}
		
		return nullptr;
	}

	const vector<IImportDeclaration*>* CompilationUnit::getImports()
	{
		LazyLoad();
		return importDecls;
	}

	const vector<IType*>* CompilationUnit::getTypes()
	{
		LazyLoad();
		return types;
	}

	IType* CompilationUnit::getType(const wstring& name)
	{
		LazyLoad();
		if(types)
		{
			for (auto it : *types)
			{
				if (it && it->getElementName() == name)
				{
					return it;
				}
			}
		}
		
		return nullptr;
	}

	const vector<IPackageDeclaration*>* CompilationUnit::getPackageDeclarations()
	{
		LazyLoad();
		return packageDecls;
	}

	IPackageDeclaration* CompilationUnit::getPackageDeclaration(const wstring& name)
	{
		LazyLoad();
		if(packageDecls)
		{
			for (auto it : *packageDecls)
			{
				if (it && it->getElementName() == name)
				{
					return it;
				}
			}
		}

		return nullptr;
	}

	IJavaElement* CompilationUnit::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new CompilationUnit(*this);
		clone->fParent = parent;
		clone->MarkClone();
		return clone;
	}

	CompilationUnit::CompilationUnit(IJavaElementType _type, IPackageFragment* parent, 
		const wstring& name_symbol_)
		: ICompilationUnit(_type, parent, name_symbol_){

	}

	CompilationUnit::CompilationUnit(CompilationUnit& o)
		:ICompilationUnit(o)
	{
		
	}

	

}// Close namespace JavaModel block
	
} // Close namespace Jikes block



