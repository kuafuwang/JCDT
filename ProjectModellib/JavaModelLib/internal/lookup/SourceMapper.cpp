#include "stdafx.h"
#include "SourceMapper.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "JCDT_Lib/internal/parser/parser.h"
#include "JCDT_Lib/internal/lex/scanner.h"
#include "JCDT_Lib/internal/impl/option.h"
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <JavaModelLib/internal/core/ClassFile.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/JarPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/BinaryType.h>
#include <JCDT_Lib/internal/ast/CompilationUnitDeclaration.h>
#include <JCDT_Lib/internal/ast/AstAnnotationDeclaration.h>
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#include <boost/algorithm/string.hpp>
#include <queue>
#include <JCDT_Lib/internal/ast/AstClassDeclaration.h>
#include <JCDT_Lib/internal/ast/AstEnumDeclaration.h>
#include <JCDT_Lib/internal/ast/AstInterfaceDeclaration.h>
#include <boost/filesystem/operations.hpp>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/BinaryMethod.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <JavaModelLib/internal/core/IField.h>
#include "SourceMapperFile.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
using namespace std;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	SourceMapper::SourceMapper(): parsedOptions(nullptr)
	{
		option = new Option();
		JikesClassPaths _no_paths;
		name_env = new INameEnvironment(option,_no_paths);
		parser = new Parser();
		scanner = new Scanner(option,name_env);
	}

	SourceMapper::~SourceMapper()
	{
		delete name_env;
		delete scanner;
		delete option;
		delete parser;
		
	}
	
	
	void MapperType(list< BinaryType* >& typeList, BinaryType* type, LexStream* lex_stream, AstClassBody* class_body)
	{
		
		if (typeList.empty() || !class_body)
			return;
		auto upperType = typeList.front();
	
		unsigned i, num;
		for (i = 0, num = class_body->NumNestedClasses(); i < num; i++)
		{
			ClassTypeDeclaration* decl = class_body->NestedClass(i);
			if(!decl || !decl->class_body || !decl->class_body->identifier_token)
				continue;

			auto name_symbol = decl->class_body->identifier_token->getName();
			if(name_symbol == upperType->getElementName()){
				if(upperType == type)
					SourceMapper::BuildSourceNameRange(type,decl, lex_stream);
				else
				{
					typeList.pop_front();
					MapperType(typeList, type, lex_stream, decl->class_body);
				}
				return;
			}
		}
		for (i = 0, num = class_body->NumNestedEnums(); i < num; i++)
		{
			EnumTypeDeclaration* decl = class_body->NestedEnum(i);
			if (!decl || !decl->class_body || !decl->class_body->identifier_token)
				continue;
			auto name_symbol = decl->class_body->identifier_token->getName();
			if (name_symbol == upperType->getElementName()) {
				if (upperType == type)
					SourceMapper::BuildSourceNameRange(type,decl, lex_stream);
				else
				{
					typeList.pop_front();
					MapperType(typeList, type, lex_stream, decl->class_body);
				}
				return;
			}
		}
		for (i = 0, num = class_body->NumNestedInterfaces(); i < num; i++)
		{
			InterfaceTypeDeclaration* decl = class_body->NestedInterface(i);
			if (!decl || !decl->class_body || !decl->class_body->identifier_token)
				continue;
			auto name_symbol = decl->class_body->identifier_token->getName();
			if (name_symbol == upperType->getElementName()) {
				if (upperType == type)
					SourceMapper::BuildSourceNameRange(type,decl, lex_stream);
				else
				{
					typeList.pop_front();
					MapperType(typeList, type, lex_stream, decl->class_body);
				}
				return;
			}
		}
		for (i = 0, class_body->NumNestedAnnotations(); i < num; i++)
		{
			AnnotationTypeDeclaration* decl = class_body->NestedAnnotation(i);
			if (!decl || !decl->class_body || !decl->class_body->identifier_token)
				continue;
			auto name_symbol = decl->class_body->identifier_token->getName();
			if (name_symbol == upperType->getElementName()) {
				if (upperType == type)
					SourceMapper::BuildSourceNameRange(type,decl, lex_stream);
				else
				{
					typeList.pop_front();
					MapperType(typeList, type, lex_stream, decl->class_body);
				}
				return;
			}
		}
	}


	void SourceMapToFieldDeclaration(FieldDeclaration* field_declaration, LexStream* lex,const vector<IField*>& fieldsToMatch)
	{
		auto size_fieldsToMatch = fieldsToMatch.size();
		auto num = field_declaration->NumVariableDeclarators();
		for (unsigned i = 0; i < num; i++) 
		{
			AstVariableDeclarator* declarator = field_declaration->VariableDeclarator(i);
				
			if (!declarator)
				continue;

			AstVariableDeclaratorId* name =	declarator->variable_declarator_name;
			
			if (!name ||!name->identifier_token)
				continue;

			auto fieldName = name->identifier_token->getName();

			for (auto k = 0; k < size_fieldsToMatch; ++k) {
				auto binaryField = fieldsToMatch[k];
				if (binaryField && binaryField->getElementName() == fieldName)
				{
					if (lex) {
						auto left = declarator->LeftToken();
						auto right = declarator->RightToken();
						if (right && left) {
							auto  left_line_no = lex->LineFromPos(left->start_location);
							auto left_column_no = lex->ColumnFromPos(left->start_location);
							auto  right_line_no = lex->LineFromPos(right->start_location);
							auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
							auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
							binaryField->setSourceRange(rng);
						}

						auto name_identifier = name->identifier_token;
						auto  left_line_no = lex->LineFromPos(name_identifier->start_location);
						auto left_column_no = lex->ColumnFromPos(name_identifier->start_location);
						auto ene_pos = name_identifier->EndLocation();
						auto  right_line_no = lex->LineFromPos(ene_pos);
						auto  right_column_no = lex->RightColumnFromPos(ene_pos);
						auto name_rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
						binaryField->setNameRange(name_rng);
					}
					break;
				}
			}
		}
	}



	void SourceMapToFieldDeclaration(FieldDeclaration* field_declaration, LexStream* lex,
		vector<IField*>& fieldsToMatch);

	void SourceMapToMethodMembers(AbstractMethodDeclaration* md,
		AstMethodDeclarator* method_declarator,
		LexStream* lex,
		const vector<IMethod*>* iMethods)
	{
		auto iMethodSize = 0;
		if (!method_declarator || !iMethods || (iMethodSize = iMethods->size()) == 0) {
			return;
		}
		vector<IMethod*> mathchs;

		if (!method_declarator->identifier_token)
			return;

		if (!md)
			return;

		std::wstring constructor_name = method_declarator->identifier_token->getName();
		auto iMethods_data = iMethods->data();
		auto  num_paramenter = method_declarator->NumFormalParameters();
		for (auto k = 0; k < iMethodSize; ++k) {
			auto method = iMethods_data[k];

			if (method && method->getElementName() == constructor_name &&
				method->getNumParameterTypes() == num_paramenter) {
				mathchs.push_back(method);
			}
		}

		if(mathchs.empty()){
			return;
		}

		vector<std::wstring> parameterTypes;
		vector<std::wstring> parameterNames;
		md->getParameterTypesAndNames(parameterTypes, parameterNames);
		for (auto it : mathchs) {
			if (!it)
				continue;

			bool final_match = true;
			auto  parameterTypes_in_it = it->getParameterTypes();
			for (unsigned l = 0; l < num_paramenter; l++) {
				if (parameterTypes_in_it[l].rfind(parameterTypes[l]) == std::wstring::npos) {
					final_match = false;
					break;
				}
			}
			if (!final_match)
				continue;

			auto binary_md = reinterpret_cast<BinaryMethod*>(it);
			binary_md->SetParamenterNames(parameterNames);
			auto right = md->RightToken();
			auto left = md->LeftToken();
			if (right && left) {
				auto  left_line_no = lex->LineFromPos(left->start_location);
				auto left_column_no = lex->ColumnFromPos(left->start_location);
				auto  right_line_no = lex->LineFromPos(right->start_location);
				auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());

				auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
				binary_md->setSourceRange(rng);
			}
			auto name_identifier = method_declarator->identifier_token;

			auto  left_line_no = lex->LineFromPos(name_identifier->start_location);
			auto left_column_no = lex->ColumnFromPos(name_identifier->start_location);
			auto ene_pos = name_identifier->EndLocation();
			auto  right_line_no = lex->LineFromPos(ene_pos);
			auto  right_column_no = lex->RightColumnFromPos(ene_pos);
			auto name_rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
			binary_md->setNameRange(name_rng);
		}
	}
	void ProcessMethodMembers(AstClassBody*class_body
		, LexStream* lex, const vector<IMethod*>* methods)
	{

		if (!methods || !class_body)
			return;
		auto num = class_body->NumMethods();
		for (unsigned i = 0; i < num; i++) {
			auto md = class_body->Method(i);

			if (!md)
				continue;

			SourceMapToMethodMembers(md,md->method_declarator, lex, methods);
		}
		num = class_body->NumConstructors();
		for (unsigned i = 0; i < num; i++) {
			auto cd = class_body->Constructor(i);

			if (!cd)
				continue;

			SourceMapToMethodMembers(cd,cd->method_declarator, lex, methods);
		}

	}

	void ProcessFieldMembers(AstClassBody*class_body
		, LexStream* lex,const std::vector<IField*>* _fields)
	{
		if (!_fields || !class_body) {
			return;
		}
		for (unsigned i = 0; i < class_body->NumInstanceVariables(); i++)
			SourceMapToFieldDeclaration(class_body->InstanceVariable(i), lex, *_fields);

		for (unsigned i = 0; i < class_body->NumClassVariables(); i++)
			SourceMapToFieldDeclaration(class_body->ClassVariable(i), lex, *_fields);
	}


	void SourceMapper::BuildSourceNameRange(BinaryType* _type, TypeDeclaration* type_declaration,
		LexStream* lex)
	{
		if (!_type || !type_declaration || !lex)
			return;

		auto left = type_declaration->LeftToken();
		auto right = type_declaration->RightToken();
		if (right && left) {
			auto  left_line_no = lex->LineFromPos(left->start_location);
			auto left_column_no = lex->ColumnFromPos(left->start_location);
			auto  right_line_no = lex->LineFromPos(right->start_location);
			auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
			auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
			_type->setSourceRange(rng);
		}
		auto class_body = type_declaration->class_body;

		if (!class_body || !class_body->identifier_token)
			return;

		auto name_identifier = class_body->identifier_token;

		auto  left_line_no = lex->LineFromPos(name_identifier->start_location);
		auto left_column_no = lex->ColumnFromPos(name_identifier->start_location);
		auto ene_pos = name_identifier->EndLocation();
		auto  right_line_no = lex->LineFromPos(ene_pos);
		auto  right_column_no = lex->RightColumnFromPos(ene_pos);
		auto name_rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);

		_type->setNameRange(name_rng);

		auto methods = _type->getMethods();
		ProcessMethodMembers(class_body, lex, methods);
		auto _fields = _type->getFields();
		ProcessFieldMembers(class_body, lex, _fields);
	
	}

	void SourceMapper::MapperSource(BinaryType* type){
		if(!type){
			assert(false);
			return;
		}
		list< BinaryType* > typeList;
		
		auto find_type = type;
		while (find_type){
			auto fParent = find_type->getParent();
			if (!fParent)
				return;

			typeList.push_back((find_type));

			bool bFindMainType = false;

			switch (fParent->fLEType)
			{
				case JAR_CLASS_FILE:
				case CLASS_FILE:
				case COMPILATION_UNIT:
				case JAR_COMPILATION_UNIT: 
				{
					bFindMainType = true;
				}
				break;
				default:
					find_type = reinterpret_cast<BinaryType*>(fParent);
					break;
			}

			if (bFindMainType) {
				break;
			}
		}
		auto mainType = typeList.front();
		auto file = reinterpret_cast<ClassFile*>(mainType->getParent());
		if (!file)
			return;

		string source_path;
		if(!file->isSepareteFromParent())
		{
			auto pkg = file->getParent();
			if (!pkg)
				return;
			auto pkgRoot = reinterpret_cast<JarPackageFragmentRoot*>(pkg->getParent());
			if (!pkgRoot)
				return;
			auto source_root_path = pkgRoot->getSourceAttachmentRootPath();

			
			auto pkg_part = boost::replace_all_copy(pkg->getElementName(), L".", L"/");
			auto temp = source_root_path + L"/" + pkg_part + L"/" + file->getElementName() + L".java";
			//source_path = Util::UnicodeToUtf8(temp);
		}
		/*else
		{
			auto _pos = file->GetPosition();
			auto  g_pSolutionModel = GetGolbalSolutionModel();
			if (!_pos || !g_pSolutionModel){
				return;
			}
			boost::shared_ptr<OriginalJavaProject>& project = g_pSolutionModel->pJM->getJavaProject(_pos->proj);
			if (!project)
				return;

			auto pkgRoot = project->findPackageFragmentRootByName(_pos->projectPos.root);
			if (!pkgRoot)
				return;
			auto source_root_path = pkgRoot->getSourceAttachmentRootPath();
		
			auto pkg_part = boost::replace_all_copy(_pos->projectPos.pkg, L".", L"/");
			auto temp = source_root_path + L"/" + pkg_part + L"/" + file->getElementName() + L".java";
			source_path = Util::UnicodeToUtf8(temp);
		}*/
		
		namespace fs = boost::filesystem;
	
		if (!fs::exists(source_path)) {
			return;
		}
		
		SourceMapperFile* lexFile = new SourceMapperFile(source_path);
		scanner->Scan(lexFile);
		auto lex = lexFile->lex_stream;
		if(!lex)
		{
			delete lexFile;
			return;
		}

		auto unit = parser->HeaderParse(lexFile->lex_stream);

	    if(!unit){
			delete lexFile;
			return;
	    }
		auto num = unit->NumTypeDeclarations();
		for(auto i = 0; i < num; ++i )
		{
			TypeDeclaration* declaration =
				unit->GetTypeDeclaration(i);

			if (!declaration || !declaration->class_body || !declaration->class_body->identifier_token)
				continue;

			auto name_symbol = declaration->class_body->identifier_token->getName();

			if(name_symbol == mainType->getElementName())
			{
				if(mainType == type){
					SourceMapper::BuildSourceNameRange(type,declaration, lex);
					break;
				}
				else{
					typeList.pop_front();
					MapperType(typeList, type, lex, declaration->class_body);
				}
			}
		}

		delete unit->ast_pool;
		delete lexFile;
	}
}	

} // Close namespace Jikes block


