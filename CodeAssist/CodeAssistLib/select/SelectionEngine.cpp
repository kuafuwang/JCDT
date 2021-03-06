#include "stdafx.h"
#include "SelectionEngine.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"

#include <JCDT_Lib/internal/lex/Token.h>
#include "JCDT_Lib/internal/lex/LexStream.h"
#include "SelectAssistASTVisitor.h"
#include <CodeAssistLib/impl/AstNoFound.h>
#include "SelectionOnNameReference.h"
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include  <boost/algorithm/string.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <CodeAssistLib/impl/AssistInfo.h>
#include "SelectWhenDebugAssistASTVisitor.h"
#include "SelectionWhenDebug.h"
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <JavaModelLib/internal/core/IField.h>
#include "ExpressionConverter.h"
#include "SelecctResult.h"
#include "FindLocalFieldOwnerVisitor.h"
#include <JavaModelLib/internal/core/SourceField.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/Initializer.h>
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <CodeAssistLib/complete/CompleteLex.h>
#include <JCDT_Lib/internal/lookup/UnitFileSymbol.h>
#include <JCDT_Lib/internal/lex/scanner.h>
#include <JCDT_Lib/internal/parser/ParserUtil.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JCDT_Lib/internal/ast/ScopeUtil.h>
#include "ScopetAssistASTVisitor.h"
#include "SelectUtil.h"

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {


		unsigned  SelectionEngine::checkSelection(LexStream* lex, unsigned int position, AssistInfo& _info)
		{
			if(!lex)
			{
				return TK_ERROR;
			}
			if(! Engine::checkSelection(lex, position) ){
				return TK_ERROR;
			}
			
			int idx = lex->FindGoodTokenIndex(position);
			if (	_info.what_for == AssistInfo::SELECT_FOR_EDIT_TIPS
				||  _info.what_for == AssistInfo::SELECT_FOR_EDIT_GO_TO_DECL
				) {
				if (idx == PositionNode::NO_FOUND) {
					return TK_ERROR;
				}
				Token* token = lex->getTokenObjNoIndexNoNext(idx);
				auto kind = token->Kind();
				switch (kind)
				{
				case TK_Identifier:
				case TK_super:
				case TK_this:
					return kind;
				default:
					return TK_ERROR;
				}
			}
			
			if (idx == PositionNode::NO_FOUND) {
				return TK_ExpressionMarker;// ????????????
			}

			Token* token = lex->getTokenObjNoIndexNoNext(idx);
			auto kind = token->Kind();
			if (_info.what_for == AssistInfo::SELECT_FOR_SCOPE)
			{
				return kind;
			}
			else
			{
				switch (kind)
				{
				case TK_Identifier:
				case TK_super:
				case TK_this:
				case TK_LBRACKET:
				case TK_RBRACKET:
					return kind;

				default:
					return TK_ERROR;
				}
			}

		}


		JavaModel::IJavaElement* SelectionEngine::getSelectJavaElement(CodeAssist::AssistInfo* cur_info,
			JavaModel::JavaModel* pJM, bool tryNoCopyChildren)
		{
			if(!cur_info || !pJM)
			{
				return nullptr;
			}
			auto _pos = pJM->getLayerPos(cur_info->doc_path);
			auto java_project = pJM->getJavaProject(_pos->proj);
			if (!java_project) {
				delete _pos;
				return nullptr;
			}

			CodeAssist::SelectionEngine* engine = nullptr;
			JavaModel::SelectionRequestor* requestor = nullptr;



			Option* option = new Option();
			option->bytecode = false;
			JikesClassPaths collector;
			wstring projName = _pos->proj;
			pJM->getExpandClasspath(collector, projName);

			requestor = new JavaModel::SelectionRequestor(java_project, _pos);

			INameEnvironment* nameEnv = new INameEnvironment(option, collector);
			set<wstring> has_problem_files;
			java_project->GetErrorFilse(has_problem_files);
			engine = new CodeAssist::SelectionEngine(nameEnv, option, requestor, *cur_info, has_problem_files);

			auto _ret = engine->select();
			delete _ret;

			JavaModel::IJavaElement* element;
			auto elements = requestor->getElements();
			if (!elements.size()) {
				delete engine;
				delete requestor;
				return nullptr;
			}

			element = elements[0];

			if(!element)
			{
				return nullptr;
			}

			if(tryNoCopyChildren)
			{
				if(element->fLEType == JavaModel::IJavaElementType::TYPE)
				{
					JavaModel::IType* _type = reinterpret_cast<JavaModel::IType*>(element);
					element = _type->DeepCopyButNoChildren(_type->getParent());
				}
				else
				{
					element = element->DeepCopy(element->getParent());
				}
			}
			else
			{
				element = element->DeepCopy(element->getParent());
			}
			element->GetIndendentInfoAndSepareteFromParent();
			engine->clear();
			delete engine;
			delete requestor;
			return element;

		}

		wstring SelectionEngine::getDeclareTypeName(vector<TypeDeclaration*>& type_stack) {
			wstring _typeName;
			auto _size = type_stack.size();
			for (size_t i = 0; i < _size; ++i) {
				if(!type_stack[i])
					continue;

				auto _name = type_stack[i]->class_body->identifier_token->NameString();
				if (i == 0) {
					_typeName.append(_name);
				}
				else {
					_typeName.append(L".").append(_name);
				}
			}
			return _typeName;
		}

	
		void SelectionEngine::selectFrom(TypeSymbol* type_symbol)
		{
			auto _type = getSelectOnType(type_symbol);
			if(_type){
				requestor->recordMatchSelectElement(_type);
			}
		
		}
		vector<JavaModel::IMethod*> SelectionEngine::getSelectOnMethod(MethodSymbol* method_symbol, 
			wstring selector, bool construtor)
		{
			return SelectUtil::getSelectOnMethod(method_symbol, selector, requestor);
		}

		JavaModel::IType* SelectionEngine::getSelectOnType(TypeSymbol* type_symbol)
		{
			return SelectUtil::getSelectOnType(type_symbol, requestor);

		}

		JavaModel::IJavaElement* UtilFieldIsNotLocal(JavaModel::ISelectionRequestor* requestor,
			VariableSymbol* field_symbol)
		{
			if(!requestor || !field_symbol)
			{
				return nullptr;
			}

			wstring declaringTypePackageName;
			wstring declaringTypeName;
			wstring name;
			auto type_symbol_parameter = field_symbol->ContainingType();
			if(!type_symbol_parameter)
			{
				return nullptr;
			}
			const wchar_t*  temp = field_symbol->Name();
			if (temp)
				name.assign(temp, temp + field_symbol->NameLength());

			auto packae_symbol_parameter = type_symbol_parameter->ContainingPackage();
			if(packae_symbol_parameter)
			{
				declaringTypePackageName = packae_symbol_parameter->PackageNameString();
			}

			declaringTypeName = type_symbol_parameter->qualifiedSourceName('.');

			boost::replace_all(declaringTypePackageName, L"/", L".");

			auto type = requestor->getType(declaringTypePackageName, declaringTypeName,
				JavaModel::NameLookup::ACCEPT_CLASSES | JavaModel::NameLookup::ACCEPT_INTERFACES);
			if (type != nullptr) {
				auto field = type->getField(name);
				if (field) {
					auto info = reinterpret_cast <JavaModel::SourceFieldElementInfo*>(field->getElementInfo());
					if (field_symbol->initial_value && !info->initial_value) {
						info->initial_value = field_symbol->initial_value->Clone();
					}
					
					return field;
				}
			}

			return nullptr;
		}
		void SelectionEngine::selectOnField(VariableSymbol* field_symbol, AstNodeScope* scope)
		{
			auto field = SelectUtil::selectOnField(scope, field_symbol, requestor);
			if(field)
			{
				requestor->recordMatchSelectElement(field);
			}
		}
		void SelectionEngine::SelectOnExpression(Expression* expr, AstNodeScope* scope)
		{
		   if(!expr)
		   {
			   return;
		   }
			auto symbol = expr->symbol;
			if (!symbol)
				return;

			switch (symbol->Kind())
			{
			case Symbol::PACKAGE:
			{
				PackageSymbol* package_symbol = PackageSymbol::PackageCast(symbol);
			    if(package_symbol)
			    {
					const wchar_t* temp = package_symbol->PackageName();
					wstring pkgName = package_symbol->PackageNameString();
					boost::replace_all(pkgName, L"/", L".");
					requestor->acceptPackage(pkgName);
			    }
			}
			break;
			case Symbol::TYPE:
			{
				auto type_symbol = TypeSymbol::TypeCast(symbol);
				if (!type_symbol)
					return;
				if(!type_symbol->Bad())
					selectFrom(type_symbol);
				else if(expr->kind == Ast::CALL && _info.what_for == AssistInfo::SELECT_FOR_EDIT_GO_TO_DECL){
					auto method_call = reinterpret_cast<AstMethodInvocation*>(expr);
					TypeSymbol* methodOwner = nullptr;
					if(method_call->base_opt ){
						auto base_opt_symbol = method_call->base_opt->symbol;
						if(base_opt_symbol)
						{
							if (Symbol::TYPE == base_opt_symbol->_kind) {
								methodOwner = reinterpret_cast<TypeSymbol*>(base_opt_symbol);
							}
							else if(Symbol::VARIABLE == base_opt_symbol->_kind)
							{
								auto variable_item =  reinterpret_cast<VariableSymbol*>(base_opt_symbol);
								methodOwner  = variable_item->Type();
							}
						}

					}
					else{
						SelectAssistASTVisitor* selectVisitor = reinterpret_cast<SelectAssistASTVisitor*>(visitor);
						methodOwner = ScopeUtil::enclosingSourceType(selectVisitor->selectedScope);
					}
					if (!methodOwner || methodOwner->Bad())
						return;

					auto package_symbol = methodOwner->ContainingPackage();
					if (!package_symbol)
						return ;
					
					wstring declaringTypePackageName = package_symbol->PackageNameString();
					
					wstring declaringTypeName = methodOwner->qualifiedSourceName('.');

					boost::replace_all(declaringTypePackageName, L"/", L".");
					wstring selector = method_call->identifier_token->getName();
					auto	_ret = requestor->getMethod(declaringTypePackageName, declaringTypeName, selector, false);
					for (auto it : _ret) {
						requestor->recordMatchSelectElement(it);
					}

				}
			}
			break;

			case Symbol::METHOD:
			{
				auto method_symbol = MethodSymbol::MethodCast(symbol);
				
				wstring selector = method_symbol->getNameString();
			
				auto _ret=  getSelectOnMethod(method_symbol, selector, method_symbol->IsConstructor());
				for (auto it : _ret) {
					requestor->recordMatchSelectElement(it);
				}
			}
			break;

			case  Symbol::VARIABLE:
			{
				selectOnField(VariableSymbol::VariableCast(symbol), scope);
			}
			break;
			default:
				break;
			}
		}


		void GetParameterNames(AstArguments* arguments,vector<wstring>& parameterPackageNames, vector<wstring>& parameterTypeNames)
		{
			if (!arguments)
				return;

			auto _num = arguments->NumArguments();
			for (auto i = 0; i < _num; ++i)
			{
				auto parameter = arguments->Argument(i);

				if (!parameter)
					continue;

				wstring parameterPackageName;
				wstring parameterTypeName;
				auto type_symbol_parameter = parameter->Type();
				if (type_symbol_parameter) {
					auto packae_symbol_parameter = type_symbol_parameter->ContainingPackage();
					if(packae_symbol_parameter)
					{
						parameterPackageName = packae_symbol_parameter->PackageNameString();	
						boost::replace_all(parameterPackageName, L"/", L".");
						
					}

					parameterTypeName = type_symbol_parameter->qualifiedSourceName('.');
					
				}
				parameterPackageNames.push_back(parameterPackageName);
				parameterTypeNames.push_back(parameterTypeName);

			}
		}

		void SelectionEngine::SelectOnConstructor(TypeSymbol* methodOwner, AstArguments* arguments)
		{
			if (!methodOwner || methodOwner->Bad())
				return;

			auto package_symbol = methodOwner->ContainingPackage();
			if (!package_symbol)
				return;

		
			wstring declaringTypePackageName = package_symbol->PackageNameString();
			
			wstring declaringTypeName = methodOwner->qualifiedSourceName('.');
			
			vector<wstring> parameterPackageNames;
			vector<wstring> parameterTypeNames;
			GetParameterNames(arguments, parameterPackageNames, parameterTypeNames);


			boost::replace_all(declaringTypePackageName, L"/", L".");
			wstring selector = methodOwner->getNameString();
			auto _ret = requestor->getMethod(declaringTypePackageName, declaringTypeName, selector,
				parameterPackageNames, parameterTypeNames, true);
			if (_ret.empty()) {
				_ret = requestor->getMethod(declaringTypePackageName, declaringTypeName, selector, true);
			}
			for (auto it : _ret) {
				requestor->recordMatchSelectElement(it);
			}
		}

		void SelectionEngine::SelectOnConstructorCall(SelectionOnNameReference& expetion)
		{
			AstArguments* arguments;
			TypeSymbol* methodOwner = nullptr;
			if (!expetion.node)
			{
				return;
			}
			if (expetion.kind == SelectionOnNameReference::SelectionOnSuperCall) {
				auto ast_super = reinterpret_cast<AstSuperCall*>(expetion.node);

				auto	method = ast_super->symbol;
				if (method)
					methodOwner = method->containing_type;

				else if (ast_super->base_opt)
				{
					if (ast_super->base_opt->symbol && ast_super->base_opt->symbol->_kind == Symbol::TYPE) {
						methodOwner = reinterpret_cast<TypeSymbol*> (ast_super->base_opt->symbol);
					}
				}
				else
				{
					SelectAssistASTVisitor* selectVisitor = reinterpret_cast<SelectAssistASTVisitor*>(visitor);
					methodOwner = ScopeUtil::enclosingSourceType(selectVisitor->selectedScope);
					if (methodOwner && !methodOwner->Bad()) {
						methodOwner = methodOwner->super;
					}

				}
				arguments = ast_super->arguments;

			}
			else {
				auto ast_this = reinterpret_cast<AstThisCall*>(expetion.node);
				auto	method = ast_this->symbol;
				if (method)
					methodOwner = method->containing_type;
				else
				{
					SelectAssistASTVisitor* selectVisitor = reinterpret_cast<SelectAssistASTVisitor*>(visitor);
					methodOwner = ScopeUtil::enclosingSourceType(selectVisitor->selectedScope);

				}
				arguments = ast_this->arguments;
			}

			if (methodOwner) {
				SelectOnConstructor(methodOwner, arguments);
			}
		}


		SelecctResult* SelectionEngine::select()
		{

			wstring& file = _info.doc_path;
			int _line = _info.line;
			int column = _info.column;
			set<wstring> file_to_compiled;
			for (auto it : error_files) {
				file_to_compiled.insert(it);
			}
			file_to_compiled.insert(file);
			auto name_symbol = lookupEnvironment->nameEnv->FindOrInsertName(file.c_str(), file.size());
			if(!name_symbol)
			{
				return nullptr;
			}
			string utf_8_name = name_symbol->Utf8Name();
			cunit = new UnitFileSymbol(name_symbol, utf_8_name);

			CompleteLex* lex = new CompleteLex(option, cunit);
			lex->ReadInput();


			scanner->Scan(lex);
			if (!lex->InputBuffer()) {
				delete cunit;
				cunit = nullptr;
				return nullptr;
			}
			auto pos = cunit->lex_stream->FindPosition(_line, column);
			const unsigned token_type = checkSelection(cunit->lex_stream, pos, _info);
			if (TK_ERROR == token_type) {
				delete cunit;
				cunit = nullptr;
				return nullptr;
			}

			CompilationUnitDeclaration* compilation_unit = nullptr;
			if (HeadParse(cunit))
			{
				compilation_unit = cunit->compilation_unit;
				assert(compilation_unit);
				if (compilation_unit->type_declarations) {
					AstArray<TypeDeclaration*>& types = *(compilation_unit->type_declarations);
					for (int i = 0, len = types.Length(); i < len; i++) {
						ExcludeUselessMethod(types[i], pos);
					}
				}
				
				if (compilation_unit->state == CompilationUnitDeclaration::HEAD_PARSE) {
					const auto temp_num = compilation_unit->NumTypeDeclarations();
					for (unsigned k = 0; k <temp_num; k++) {
						TypeDeclaration* declaration = compilation_unit->GetTypeDeclaration(k);
						if (!declaration)
							continue;
						if (!EmptyTypeDeclaration::EmptyDeclarationCast(declaration)) {
							ParserUtil::CheckNestedMembers(cunit->lex_stream, declaration->class_body, parser);
						}
					}
					compilation_unit->state = CompilationUnitDeclaration::ALL_PARSE;
				}
			}
			if (!compilation_unit)
			{
				delete cunit;
				cunit = nullptr;
				return nullptr;
			}
			
			set<FileSymbol*> _set = { cunit };
			lookupEnvironment->RecordcompilationUnit(_set);
			lookupEnvironment->ProcessFileHeaders(cunit);
			

			if (AssistInfo::SELECT_FOR_SCOPE == _info.what_for)
			{
					lookupEnvironment->ProcessFileBodys();
					ScopetAssistASTVisitor* selectVisitor = new ScopetAssistASTVisitor();
					selectVisitor->pos = pos;
					visitor = selectVisitor;
					compilation_unit->traverse(visitor, nullptr);
					auto scope = selectVisitor->selectedScope;
					SelectForScope* result = nullptr;
				    if(!scope)
				    {
						return  result;
				    }
					Ast * scope_ast;
					Token* left = nullptr;
					Token *right = nullptr;
					
					if (scope->GetScopeKind() == AstNodeScope::BLOCK_SCOPE)
					{
					
						auto tempBlock = dynamic_cast<AstBlock*>(scope);
						scope_ast = tempBlock->owner_ast;
						right = tempBlock->RightToken();
						switch (tempBlock->Tag())
						{
						case	AstBlock::TRY_CLAUSE_WITH_FINALLY:
						case	AstBlock::TRY_CLAUSE_WITH_CATCH:
						case	AstBlock::ABRUPT_TRY_FINALLY:
						{
							auto element = dynamic_cast<TryStatement*>(scope_ast);
							if (element)
							{
								left = element->LeftToken();
								if (element->block == scope)
								{
									right = element->LeftToken();
								}
								else
								{
									left = tempBlock->LeftToken();
								}
							}
							else
							{
								left = tempBlock->LeftToken();
							
							}
						}
							break;


						

						case AstBlock::NONE:
							{
								left = tempBlock->LeftToken();
							
							}

							break;

						case	AstBlock::SWITCH_BLOCK_STATEMENT:
							{
								left = tempBlock->LeftToken();
							
							}
								break;

						case	AstBlock::IF_TRUE_STATEMENT_BLOCK:
						case	AstBlock::IF_FALSE_STATEMENT_BLOCK:
							{
								auto element = dynamic_cast<IfStatement*>(scope_ast);
							
								if(element && element->true_statement == scope)
								{
									left = element->LeftToken();
								}
								else
								{
									left = tempBlock->LeftToken();
									
								}
							}
								break;

						case	AstBlock::SWITCH:
						case	AstBlock::FOREACH_STATEMENT_BLOCK:
						case	AstBlock::FINALLY:
						case	AstBlock::SYNCHRONIZED:
						case	AstBlock::FOR_STATEMENT_BLOCK:
						case	AstBlock::DO_STATEMENT_BLOCK:
						case	AstBlock::WHILE_STATEMENT_BLOCK:
						case	AstBlock::METHOD_BODY_BLOCK:
							{

								scope_ast = tempBlock->owner_ast;
								if(scope_ast)
									left = scope_ast->LeftToken();
								else
									left = tempBlock->LeftToken();

							}
								
								break;
						default:
							break;

						}


							
						if (right && left)
						{
							result = new SelectForScope();
							auto  left_line_no = lex->LineFromPos(left->start_location);
							auto left_column_no = lex->ColumnFromPos(left->start_location);
							auto  right_line_no = lex->LineFromPos(right->start_location);
							auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());

							result->range = new JavaModel::ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
							
						}

						return  result;
				}
						

					switch (scope->GetScopeKind())
					{
						case AstNodeScope::METHOD_SCOPE:
						{
							  scope_ast = dynamic_cast<AbstractMethodDeclaration*>(scope);
						}
						break;
						case AstNodeScope::CLASS_SCOPE:
						{
							  scope_ast = dynamic_cast<TypeDeclaration*>(scope);
						}
						break;
						case AstNodeScope::COMPILATION_UNIT_SCOPE:
						{
							  scope_ast = dynamic_cast<CompilationUnitDeclaration*>(scope);
							
						}
						break;
						default:
							return result;
					
					}
				    if(!scope_ast)
				    {
						return result;
				    }
					left = scope_ast->LeftToken();
					right = scope_ast->RightToken();
					if (right && left) 
					{
						result = new SelectForScope();
						auto  left_line_no = lex->LineFromPos(left->start_location);
						auto left_column_no = lex->ColumnFromPos(left->start_location);
						auto  right_line_no = lex->LineFromPos(right->start_location);
						auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());

						result->range = new JavaModel::ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
					}

					return  result;
				
			}
			vector<TypeDeclaration*> type_stack;
			try {

				if (	_info.what_for == AssistInfo::SELECT_FOR_EDIT_TIPS
					||  _info.what_for == AssistInfo::SELECT_FOR_EDIT_GO_TO_DECL) {
	
					{
						SelectAssistASTVisitor* selectVisitor = new SelectAssistASTVisitor(type_stack);
						selectVisitor->pos = pos;
						visitor = selectVisitor;
						cunit->compilation_unit->traverse(visitor, nullptr);
					}
					
					
				}
				else{
					SelectWhenDebugAssistASTVisitor* select_for_debug_visitor 
						= new SelectWhenDebugAssistASTVisitor(type_stack);
					select_for_debug_visitor->pos = pos;
					visitor = select_for_debug_visitor;
					cunit->compilation_unit->traverse(visitor, nullptr);
				}
			}
			catch (AstFound& expection)
			{
				AstFound::AstFoundKind _kind = expection.kind;
				Ast* node = expection.node;

				if (_kind == AstFound::FIND_TYPE) {
					if (!compilation_unit->package_declaration_opt)
						return nullptr;
					wstring pkgName;
					compilation_unit->package_declaration_opt->name->getCompoundNames(pkgName);
					auto _interface = InterfaceTypeDeclaration::InterfaceDeclarationCast(node);
					if (_interface) {
						
						requestor->acceptInterface(pkgName, getDeclareTypeName(type_stack), false);
						
					}
					auto _class = ClassTypeDeclaration::ClassDeclarationCast(node);
					if (_class) {
						
						requestor->acceptClass(pkgName, getDeclareTypeName(type_stack), false);
						
					}

				}
				else if (_kind == AstFound::FIND_PACKAGE)
				{
					auto pkg = AstPackageDeclaration::PackageDeclarationCast(node);
					if (pkg) {
						wstring pkgName;
						pkg->name->getCompoundNames(pkgName);	
						requestor->acceptPackage(pkgName);	
					}
				}
				else if (_kind == AstFound::FIND_IMPORT)
				{
					auto import_ = AstImportDeclaration::ImportDeclarationCast(node);
					if (import_)
					{
						wstring pkgName;
						import_->name->getCompoundNames(pkgName);
						
						requestor->acceptPackage(pkgName);
						if(!requestor->acceptCount())
						{
							auto index = pkgName.rfind(L".");
							if(index != wstring::npos)
							{
								wstring declarPackageName = pkgName.substr(0, index);
								wstring typeName = pkgName.substr(index + 1, pkgName.size());
								requestor->acceptType(declarPackageName, typeName, false);
							}
						}
						
					}
				}
			}
			
			catch (SelectionOnNameReference& expetion)
			{
				lookupEnvironment->ProcessFileBodys();
				auto  _kind = expetion.kind;
				auto node = expetion.node;
				if(!node)
				{
					goto __RETURNSELECTRESULET;
				}
				
				switch (_kind)
				{
				case SelectionOnNameReference::SelectionOnExpression:
				{
					Expression* expr = reinterpret_cast< Expression* >(node);
					SelectOnExpression(expr, expetion.scope);
				}
					break;
				
				case SelectionOnNameReference::SelectionOnSuperCall:
				case SelectionOnNameReference::SelectionOnThisCall:{
					SelectOnConstructorCall(expetion);
				}
				break;

				case SelectionOnNameReference::SelectionOnMethod:
				{
					auto method_decl = MethodDeclaration::MethodDeclarationCast(node);
					Token*selector_tokon;
					if(method_decl  && method_decl->method_declarator &&  (method_decl->method_declarator->identifier_token) )
					{
						selector_tokon = method_decl->method_declarator->identifier_token;
						
					}
					else
					{
						goto __RETURNSELECTRESULET;
					}
				   
					 

					wstring selector;
					const wchar_t* temp = selector_tokon->NameString();
					if (temp)
						selector.assign(temp, temp + selector_tokon->NameStringLength());

					auto _ret = getSelectOnMethod(method_decl->method_symbol, selector, false);
					for (auto it : _ret) {
						requestor->recordMatchSelectElement(it);
					}

				}
				break;

				case SelectionOnNameReference::SelectionOnConstructor: {
					const auto method_decl = ConstructorDeclaration::ConstructorDeclarationCast(node);

					if(!method_decl ||!method_decl->method_declarator || !method_decl->method_declarator->identifier_token)
						goto __RETURNSELECTRESULET;

					
					auto selector_tokon = method_decl->method_declarator->identifier_token;

					wstring selector;
					const wchar_t* temp = selector_tokon->NameString();
					if (temp)
						selector.assign(temp, temp + selector_tokon->NameStringLength());

					auto _ret = getSelectOnMethod(method_decl->method_symbol , selector, true);
					for (auto it : _ret) {
						requestor->recordMatchSelectElement(it);
					}
				}
				 break;

				case SelectionOnNameReference::SelectionOnField:
				{
					const auto variableDeclarate = AstVariableDeclarator::VariableDeclaratorCast(node);
					if(variableDeclarate)
						selectOnField(variableDeclarate->symbol, expetion.scope);
				}
					break;

				case SelectionOnNameReference::SelectionClassCreation:
				{
					const auto expresion= reinterpret_cast<AstClassCreationExpression*>(node);
					if(expresion &&expresion->class_type)
						SelectOnConstructor(expresion->class_type->symbol, expresion->arguments);
				}
				default:
					break;
				}
			}
			catch (SelectionWhenDebug&exception)
			{
				lookupEnvironment->ProcessFileBodys();
				auto  _kind = exception.kind;
				Ast* node = exception.node;

				switch (_kind)
				{
				case SelectionWhenDebug::OnAstVariableDeclarator:
				{
					auto variableDeclarate = AstVariableDeclarator::VariableDeclaratorCast(node);
					if(!variableDeclarate)
					{
						goto __RETURNSELECTRESULET;
					}
				
					const auto _ret = new SelectForDebug();
					_ret->expr = ExpressionConverter::ProcessSimpleNameVariable(variableDeclarate->symbol);
					return _ret;
				}
				break;

				case SelectionWhenDebug::ArrayReferenceExepression:
				case SelectionWhenDebug::AstSuperExpressionOnSuper:
				case SelectionWhenDebug::AstThisExpressionOnThis:
				case SelectionWhenDebug::AstExpressOnFieldReference:
				case SelectionWhenDebug::AstExpressOnName:{
					auto converter = new ExpressionConverter(*lookupEnvironment);
					Expression* expr = (Expression*)(node);
					auto result = converter->ProcessExpression(expr );
					delete converter;
					if(!result){
						SelectOnExpression(expr, exception.scope);
					}
					else
					{
						auto _ret = new SelectForDebug();
						_ret->expr = result;
						return _ret;
					}
					
					
				}
				break;

				default:
					break;
				}
			}
			catch (AstNoFound&)
			{
				goto __RETURNSELECTRESULET;
			}
	__RETURNSELECTRESULET:		
			return new SelecctResult();
		}
		
		SelectionEngine::SelectionEngine(INameEnvironment* env, Option* opt,
			JavaModel::ISelectionRequestor* requestor_, AssistInfo& info, set<wstring>& __error_files)
			: Engine(env, opt), visitor(nullptr), error_files(__error_files), file_type(JavaModel::NO_MY_GUY), _info(info)
		{
			requestor = requestor_;
		}

		SelectionEngine::~SelectionEngine()
		{
			if (visitor)
				delete visitor;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

