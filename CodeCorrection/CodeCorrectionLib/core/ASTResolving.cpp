#include "stdafx.h"

#include "ASTResolving.h"
#include <JCDT_Lib/internal/parser/ParserUtil.h>
#include <JCDT_Lib/internal/lex/scanner.h>
#include <JCDT_Lib/internal/impl/option.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/parser/parser.h>
#include <CodeAssistLib/impl/Engine.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/lookup/unitfilesymbol.h>
#include "CorrectionAssistASTVisitor.h"

using namespace  std;
using namespace  Jikes::JavaModel;
using namespace  Jikes::CodeAssist;
namespace Jikes { // Open namespace Jikes block


	namespace Correction {
		ASTResolving::~ASTResolving()
		{
			if (lookupEnvironment == nullptr)
			{
				delete nameEnv;
				delete lexFile;
			}
			else
			{
				delete  lookupEnvironment;
			}


			delete parser;
			delete scanner;

			delete visitor;
		}

		ASTResolving::ASTResolving(int _line, int _column, JavaModel::JavaModel* pJm, std::wstring& projectName,
			std::wstring& _file) : file(_file), line(_line), column(_column), nameEnv(nullptr), scanner(nullptr), lexFile(nullptr), visitor(nullptr), lookupEnvironment(nullptr)
		{
			option = new Option();
			option->bytecode = false;
			parser = new Parser();
			JikesClassPaths jikes_class_paths;
			pJm->getExpandClasspath(jikes_class_paths, projectName);

			nameEnv = new INameEnvironment(option, jikes_class_paths);
			scanner = new Scanner(option, nameEnv);


			auto name_symbol = nameEnv->FindOrInsertName(file.c_str(), file.size());
			string utf_8_name = name_symbol->Utf8Name();
			lexFile = new UnitFileSymbol(name_symbol, utf_8_name);


		}

		AstPackageDeclaration* ASTResolving::ParseForPackage(FileSymbol* file_symbol, AstStoragePool* ast_pool)
		{
			if (!file_symbol || !ast_pool)
				return nullptr;

			if (!file_symbol->lex_stream)
				scanner->Scan(file_symbol);

			AstPackageDeclaration* package_declaration = nullptr;
			if (file_symbol->lex_stream) {
				package_declaration = parser->PackageHeaderParse(file_symbol->lex_stream, ast_pool);
			}
			return package_declaration;
		}

		bool ASTResolving::BindingProcess()
		{

			lookupEnvironment = new LookupEnvironment(option, nameEnv, this);
			set<FileSymbol*> _set = { lexFile };
			lookupEnvironment->RecordcompilationUnit(_set);
			lookupEnvironment->ProcessFileHeaders(lexFile);
			lookupEnvironment->ProcessFileBodys();
			return true;

		}

		TypeSymbol* ASTResolving::getTypeBinding()
		{
			auto binding = getPossibleTypeBinding();
			if(binding != nullptr)
			{
				if(    binding == lookupEnvironment->no_type
					|| binding == lookupEnvironment->null_type 
					|| binding == lookupEnvironment->void_type)
				{
					return  lookupEnvironment->Object();
				}
				if(binding->Anonymous())
				{
					binding->super;
				}
				return binding;
			}
		
		    return  lookupEnvironment->Object();
			
			
		}

		MethodDeclaration* getMethodDecl(vector<Ast*>& trackAstNodes)
		{
			for (auto i = trackAstNodes.size() - 1; i > 0; --i)
			{
				auto parent = trackAstNodes[i];
				if(parent && parent->kind == Ast::METHOD)
				{
					return  (MethodDeclaration*)parent;
				}

			}
			return  nullptr;
		}

		TypeSymbol* ASTResolving::getMethodInvocatePossibleTypeBinding()
		{
			vector<Ast*>& trackAstNodes = visitor->trackAstNodes;
			auto node = visitor->astNode;

			if(!node)
			{
				return  lookupEnvironment->no_type;
			}

			if(node->kind != Ast::CALL){
				return  lookupEnvironment->void_type;
			}

			auto i = trackAstNodes.size() - 1;
			if (i > 0)
			{
				if (trackAstNodes[i] == node)
				{
					i -= 1;
				}
			}
			for (; i > 0; --i)
			{
				auto parent = trackAstNodes[i];
				if(!parent)
				{
					continue;
				}

				switch (parent->kind)
				{
				case Ast::ASSIGNMENT:
				{
					auto assignment = reinterpret_cast<AstAssignmentExpression*>(parent);
					if (node->RightToken() == assignment->RightToken() && assignment->left_hand_side)
					{
						return  assignment->left_hand_side->Type();
					}
					return  lookupEnvironment->void_type;
				}
				break;
				case Ast::BINARY:
				{
					auto infix = reinterpret_cast<BinaryExpression*>(parent);
					if (infix->LeftToken() == node->LeftToken() && infix->right_expression )
					{
						return infix->right_expression->Type();
					}

					if (!infix->binary_operator_token)
						return  lookupEnvironment->no_type;

					auto _op = infix->binary_operator_token->_kind;

					if (_op == TK_LEFT_SHIFT || _op == TK_RIGHT_SHIFT || _op == TK_UNSIGNED_RIGHT_SHIFT)
					{
						return lookupEnvironment->short_type;
					}
					if (!infix->left_expression)
						return  lookupEnvironment->no_type;

					return infix->left_expression->Type();

				}
				break;
				case Ast::INSTANCEOF:
				{
					auto instanceofExpression = reinterpret_cast<InstanceofExpression*>(parent);

					if (!instanceofExpression->type)
						return  lookupEnvironment->no_type;

					return  instanceofExpression->type->symbol;
				}
				break;
				case Ast::VARIABLE_DECLARATOR:
				{
					auto   frag = reinterpret_cast<AstVariableDeclarator*>(parent);
					auto  _type = frag->Type();

					if (!_type)
						return  lookupEnvironment->no_type;

					return  _type->symbol;

				}
				break;
				case Ast::CALL:
				{
					
					continue;

				}
				break;

				case Ast::SUPER_CALL:
				{
					continue;
				}
				break;

				case Ast::THIS_CALL:
				{
					continue;
				}
				break;

				case Ast::CLASS_CREATION:
				{
					return lookupEnvironment->short_type;
				}
				break;

				case Ast::PARENTHESIZED_EXPRESSION:
				{
					continue;
				}
				break;
				case Ast::ARRAY_ACCESS:
				{
					return  lookupEnvironment->short_type;
				}
				break;
				case Ast::ARRAY_CREATION:
				{
					return  lookupEnvironment->short_type;
				}
				break;	case Ast::ARRAY_INITIALIZER:
				{
					continue;
				}
				break;
				case Ast::CONDITIONAL:
				{
					return  lookupEnvironment->boolean_type;
				}
				break;
				case Ast::POST_UNARY:
				{
					return  lookupEnvironment->short_type;
				}
				break;
				case Ast::PRE_UNARY:
				{
					return  lookupEnvironment->short_type;
				}
				break;

				case Ast::IF:
				case Ast::WHILE:
				case Ast::DO:
				{
					if (dynamic_cast<Expression*>(node))
					{
						return lookupEnvironment->boolean_type;
					}

				}
				break;
				case Ast::SWITCH_LABEL:
				{
					return lookupEnvironment->short_type;

				}
				break;
				case Ast::RETURN:
				{
					auto decl = getMethodDecl(trackAstNodes);
					if (decl != nullptr)
					{
						return  decl->type->symbol;
					}
					return lookupEnvironment->no_type;

				}
				break;	
				case Ast::CAST:
				{
					auto castDecl = reinterpret_cast<CastExpression*>(parent);
					auto _type = castDecl->type;
					if(!_type)
					{
						return lookupEnvironment->no_type;
					}

					return  _type->symbol;
				}
				break;

				default:
					break;
				}

			}
			return lookupEnvironment->void_type;
		}




		TypeSymbol* ASTResolving::getPossibleTypeBinding()
		{
			vector<Ast*>& trackAstNodes = visitor->trackAstNodes;
			auto node = visitor->astNode;

		    if(!node)
		    {
				return lookupEnvironment->no_type;
		    }

			auto i = trackAstNodes.size() -1;
			if(i > 0)
			{
				if(trackAstNodes[i ] == node)
				{
					i -= 1;
				}
			}
			for(; i > 0 ; --i)
			{
				auto parent = trackAstNodes[i];

				if (!parent)
					continue;

				switch (parent->kind)
				{  
				case Ast::ASSIGNMENT:
				{
					auto assignment  = reinterpret_cast<AstAssignmentExpression*>(parent);
					if(node->LeftToken() == assignment->LeftToken() && assignment->expression)
					{
						return  assignment->expression->Type();
					}

					if(assignment->left_hand_side)
						return  assignment->left_hand_side->Type();

				}
				break;
				case Ast::BINARY:
				{

					auto infix = reinterpret_cast<BinaryExpression*>( parent);
					if(infix->LeftToken() == node->LeftToken() && infix->left_expression)
					{
						return infix->left_expression->Type();
					}

					if (!infix->binary_operator_token)
						break;

					auto _op = infix->binary_operator_token->_kind;
					if(_op == TK_LEFT_SHIFT || _op == TK_RIGHT_SHIFT || _op == TK_UNSIGNED_RIGHT_SHIFT)
					{
						return lookupEnvironment->short_type;
					}

					if(infix->left_expression)
						return infix->left_expression->Type();

				}
				break;
				case Ast::INSTANCEOF:
				{
					auto instanceofExpression = reinterpret_cast<InstanceofExpression*>(parent);
					if(instanceofExpression->type)
						return  instanceofExpression->type->symbol;
				}
				break;
				case Ast::VARIABLE_DECLARATOR:
				{
					auto   frag = reinterpret_cast<AstVariableDeclarator*>(parent);
					auto  _type  =  frag->Type();
				    if(_type)
						return  _type->symbol;
				}
				break;
				case Ast::CALL:
				{
					/*auto methodInvocation  = reinterpret_cast<AstMethodInvocation*>(parent);
					if(methodInvocation->base_opt)
					{
						return methodInvocation->receiverType();
					}*/
					return  lookupEnvironment->no_type;;

				}
				break;

				case Ast::SUPER_CALL:
				{

				}
				break;

				case Ast::THIS_CALL:
				{

				}
				break;
				
				case Ast::CLASS_CREATION:
				{

				}
				break;	
				
				case Ast::PARENTHESIZED_EXPRESSION:
				{

				}
				break;
				case Ast::ARRAY_ACCESS:
				{
					return  lookupEnvironment->short_type;
				}
				break;
				case Ast::ARRAY_CREATION:
				{
					return  lookupEnvironment->short_type;
				}
				break;	case Ast::ARRAY_INITIALIZER:
				{

				}
				break;
				case Ast::CONDITIONAL:
				{
					return  lookupEnvironment->boolean_type;
				}
				break;
				case Ast::POST_UNARY:
				{
					return  lookupEnvironment->short_type;
				}
				break;
				case Ast::PRE_UNARY:
				{
					return  lookupEnvironment->short_type;
				}
				break;

				case Ast::IF:
				case Ast::WHILE:
				case Ast::DO:
				{
					if( dynamic_cast<Expression*>(node))
					{
						return lookupEnvironment->boolean_type;
					}

				}
				break;
				case Ast::SWITCH_LABEL:
				{
					return lookupEnvironment->short_type;

				}
				break;
				case Ast::RETURN:
				{
				   auto decl = 	getMethodDecl(trackAstNodes);
				   if(decl != nullptr)
				   {
					   return  decl->type->symbol;
				   }
				   return nullptr;
				}
				break;	case Ast::CAST:
				{
					auto castDecl = reinterpret_cast<CastExpression*>(parent);
					if(castDecl->type)
						return  castDecl->type->symbol;
				}
				break;
				
				default:
					break;
				}
				
			}
			return lookupEnvironment->no_type;
		}

		bool ASTResolving::HeadParse(FileSymbol* file_symbol)
		{
			if (!file_symbol)
				return false;

			if (!file_symbol->lex_stream) {
				scanner->Scan(file_symbol);
			}

			if (file_symbol->lex_stream) {
				if (!file_symbol->parser_error) {
					file_symbol->parser_error = new ParseError(option);
				}
				// If we have a compilation unit, analyze it, process its types.
				auto compilation_unit = file_symbol->compilation_unit;

				if (!compilation_unit) {
					file_symbol->parser_error->Clear();
					ParserUtil::DiagnoseAndCure(file_symbol->lex_stream,
						(file_symbol->parser_error), option->errors, *lookupEnvironment);

					compilation_unit =
						parser->HeaderParse(file_symbol->lex_stream, nullptr);
					/*if(file_symbol->parser_error && file_symbol->parser_error->NumErrors()){
					file_symbol->compilation_unit->MarkBad();
					}
					*/
					if (!compilation_unit) {
						return false;
					}

					file_symbol->compilation_unit = compilation_unit;
				}
			}
			else {
				return false;
			}

			return true;
		}

		bool ASTResolving::accept(FileSymbol* file_symbol)
		{
			assert(file_symbol);
			if (!file_symbol)
				return false;

			auto _info = HeadParse(file_symbol);
			if (!_info)
				return false;
			auto compilation_unit = file_symbol->compilation_unit;

			assert(compilation_unit);
			if (!compilation_unit)
				return false;

			if (compilation_unit->state == CompilationUnitDeclaration::HEAD_PARSE) {
				for (unsigned k = 0; k < compilation_unit->NumTypeDeclarations(); k++) {
					TypeDeclaration* declaration = compilation_unit->GetTypeDeclaration(k);
					if (declaration && !EmptyTypeDeclaration::EmptyDeclarationCast(declaration)) {
						ParserUtil::CheckNestedMembers(file_symbol->lex_stream, declaration->class_body, parser);
					}
				}
				compilation_unit->state = CompilationUnitDeclaration::ALL_PARSE;
			}
			return true;
		}
		bool ASTResolving::findSelectedNode()
		{
			scanner->Scan(lexFile);
			CompilationUnitDeclaration* compilation_unit = nullptr;
			auto lex = lexFile->lex_stream;
			if (lex){
				compilation_unit = parser->HeaderParse(lex);
				if (!compilation_unit){
					delete lexFile;
					return false;
				}
			}
			else
			{
				delete lexFile;
				return false;
			}

			auto actualCompletionPosition = lex->FindPosition(line, column);
			if (compilation_unit->type_declarations){
				AstArray<TypeDeclaration*>& types = *(compilation_unit->type_declarations);
				for (int i = 0, len = types.Length(); i < len; i++){
					auto type = types[i];
					CodeAssist::ExcludeUselessMethod(type, actualCompletionPosition);
				}
			}

			assert(compilation_unit);
			if (compilation_unit->state == CompilationUnitDeclaration::HEAD_PARSE){
				for (unsigned k = 0; k < compilation_unit->NumTypeDeclarations(); k++){
					TypeDeclaration* declaration = compilation_unit->GetTypeDeclaration(k);
					if (!EmptyTypeDeclaration::EmptyDeclarationCast(declaration)){
						ParserUtil::CheckNestedMembers(lex, declaration->class_body, parser);
					}
				}
				compilation_unit->state = CompilationUnitDeclaration::ALL_PARSE;
			}
			lexFile->compilation_unit = compilation_unit;

			visitor = new CorrectionAssistASTVisitor(actualCompletionPosition);
	
			compilation_unit->traverse(visitor, nullptr);

			return true;
		}

		std::vector<std::wstring> ASTResolving::GetTypeNames()
		{
			vector<wstring> typeNames;
			if (!findSelectedNode())
			{
				return typeNames;
			}
			//if (visitor->astNode)
			//{

			//	switch (visitor->completNodeType)
			//	{
			//	case CorrectionAssistASTVisitor::CompletionOnMemberAccess:

			//		//(reinterpret_cast<FieldReference*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnMessageSend:
			//		//(reinterpret_cast<AstMethodInvocation*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnExplicitConstructorCall:
			//		///(visitor->astNode, visitor->completeScope);
			//		break;


			//	case CorrectionAssistASTVisitor::CompletionOnAstTypeName:
			//		//(reinterpret_cast<AstTypeName*>(visitor->astNode), visitor->completeScope);

			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnImportReference:
			//		//(reinterpret_cast<AstImportDeclaration*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnPackageReference:
			//		///(reinterpret_cast<AstPackageDeclaration*>(visitor->astNode), visitor->completeScope);
			//		break;

			//	case CorrectionAssistASTVisitor::CompletionOnClassCreationExpressionOnTypeName:
			//		///(reinterpret_cast<AstClassCreationExpression*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnClassCreationExpressionDotSymbolInArguent:
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnMethodName:
			//		//(reinterpret_cast<MethodDeclaration*>(visitor->astNode)	, visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnMethoDotSymbolInArguent:
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnMethodReturnType:
			//		//(reinterpret_cast<MethodDeclaration*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnClassReference:
			//	case CorrectionAssistASTVisitor::CompletionOnInterfaceReference:
			//	case CorrectionAssistASTVisitor::CompletionOnExceptionReference:
			//		//(reinterpret_cast<AstTypeName*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnFieldName:
			//		//(reinterpret_cast<AstVariableDeclarator*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnLocalFieldName:
			//		//(reinterpret_cast<AstVariableDeclarator*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnFieldType:
			//		//(reinterpret_cast<FieldDeclaration*>(visitor->astNode)->type, visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnLocalFieldType:
			//		//(reinterpret_cast<LocalDeclaration*>(visitor->astNode)->type, visitor->completeScope);
			//		break;

			//	case CorrectionAssistASTVisitor::CompletionOnNameReference:
			//		//(reinterpret_cast<AstName*>(visitor->astNode), visitor->completeScope);
			//		break;
			//	case CorrectionAssistASTVisitor::CompletionOnModifiers:
			//		break;

			//	case CorrectionAssistASTVisitor::CompletionOnExplicittThisExpr:
			//	case CorrectionAssistASTVisitor::CompletionOnExplicittSuperExpr:
			//	case CorrectionAssistASTVisitor::CompletionOnArrayReference:
			//	case  CorrectionAssistASTVisitor::CompletionCastExpression:
			//	case CorrectionAssistASTVisitor::CompletionParenthesizedExpression:
			//		//(reinterpret_cast<Expression*>(visitor->astNode), visitor->completeScope);
			//		break;

			//	case CorrectionAssistASTVisitor::CompletionOnAssignmentExpressionEqualOpt:
			//	case CorrectionAssistASTVisitor::CompletionOnAssignmentExpressionBLANK:
			//		//(reinterpret_cast<AstAssignmentExpression*>(visitor->astNode), visitor->completeScope);
			//		break;

			//	case CorrectionAssistASTVisitor::CompletionOnReturnStateMent:
			//	
			//		break;

			//	case CorrectionAssistASTVisitor::CompletionOnCaseStatement:
			//		break;
			//	default:
			//		break;

			//	}
			//}

			if (visitor->astNode)
			{
				switch (visitor->completNodeType)
				{
				case CorrectionAssistASTVisitor::CompletionOnAstTypeName:
					{
						auto _type = reinterpret_cast<AstTypeName*>(visitor->astNode);
						typeNames = _type->getTypeName();
					}
					break;

				case CorrectionAssistASTVisitor::CompletionOnClassCreationExpressionOnTypeName:
					{
						auto temp = reinterpret_cast<AstClassCreationExpression*>(visitor->astNode);
						auto _type = temp->class_type;
						typeNames = _type->getTypeName();
					}


					break;

				case CorrectionAssistASTVisitor::CompletionOnMethodReturnType:
					{
						auto temp = reinterpret_cast<MethodDeclaration*>(visitor->astNode);
						auto _type = dynamic_cast<AstTypeName*>(temp->type);
						typeNames = _type->getTypeName();
					}
					break;

				case CorrectionAssistASTVisitor::CompletionOnClassReference:
				case CorrectionAssistASTVisitor::CompletionOnInterfaceReference:
				case CorrectionAssistASTVisitor::CompletionOnExceptionReference:
					{
						auto _type = reinterpret_cast<AstTypeName*>(visitor->astNode);
						typeNames = _type->getTypeName();
					}

					break;


				case CorrectionAssistASTVisitor::CompletionOnFieldType:
					{
						auto _type = (AstTypeName*) reinterpret_cast<FieldDeclaration*>(visitor->astNode)->type;
						if (_type)
							typeNames = _type->getTypeName();
					}

					break;
				case CorrectionAssistASTVisitor::CompletionOnLocalFieldType:
					{
						auto _type = (AstTypeName*)reinterpret_cast<LocalDeclaration*>(visitor->astNode)->type;
						if(_type)
							typeNames = _type->getTypeName();
					}
					break;

				case CorrectionAssistASTVisitor::CompletionOnNameReference:
					{
						auto temp = reinterpret_cast<AstName*>(visitor->astNode);
						
						GetAstNameAllNames(temp, typeNames);
					}
					break;

				default:
					break;
				}
			}
			return typeNames;
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

