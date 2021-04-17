#include "stdafx.h"

#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include <JCDT_Lib/internal/lex/Token.h>
#include "JCDT_Lib/internal/lex/LexStream.h"
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/ast/AstPackageDeclaration.h>
#include <JCDT_Lib/internal/ast/AstImportDeclaration.h>
#include <JCDT_Lib/internal/ast/ASTNode.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/util/stringex.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include "JCDT_Lib/internal/parser/diagnose/diagnose.h"
#include <JavaModelLib/internal/core/JavaModel.h>

#include "CompletionEngine.h"
#include <queue>
#include "CompleteAssistASTVisitor.h"
#include "CompleteLex.h"

using namespace Jikes::JavaModel;

namespace Jikes { // Open namespace Jikes block

	namespace CodeAssist {
		Initializer* BuildInstalizer(CompleteLex* lex, Expression* exptr_ast,
			AstStoragePool* ast_pool, int pos, int leftPos)
		{
			if(!lex || !ast_pool)
			{
				return nullptr;
			}
			auto initializer = ast_pool->NewInitializerDeclaration();

			auto methoBody = ast_pool->GenMethodBody();
			methoBody->label_opt = nullptr;

			methoBody->left_brace_token = lex->GetNextTokenAndReturnToken(leftPos);
			methoBody->left_brace_token->SetKind(TK_LBRACKET);

			methoBody->right_brace_token = lex->GetNextTokenAndReturnToken(pos);
			methoBody->right_brace_token->SetKind(TK_RBRACKET);

			methoBody->AllocateStatements(1);
			initializer->block = methoBody;
			auto expressionStatement = ast_pool->GenExpressionStatement();
			methoBody->AddStatement(expressionStatement);
			expressionStatement->expression = exptr_ast;
			expressionStatement->semicolon_token_opt = nullptr;
			return initializer;
		}

		void  getFlatTypeNames(AstName*  theName, stack<AstName*>& flatNames)
		{
			do
			{
				flatNames.push(theName);
				theName = theName->base_opt;
			} while (theName);
		}

		vector<Ast*>  buildClassScopeExpression(CompleteAssistASTVisitor* visitor, CompleteLex* lex, Expression* exptr_ast,
			AstStoragePool* ast_pool, int pos,
			TypeDeclaration* scopeNode,
			INameEnvironment* nameEnv)
		{

			if (!scopeNode || !exptr_ast ||  !ast_pool || !nameEnv || !lex || !visitor ||!scopeNode->class_body)
			{
				return{};
			}


			auto class_body = scopeNode->class_body;
			switch (exptr_ast->kind)
			{
			case Ast::NAME:
			{

				//组成一个 field
				FieldDeclaration* p = ast_pool->NewFieldDeclaration();
				p->modifiers_opt = nullptr;

				auto  nameAst = reinterpret_cast<AstName*>(exptr_ast);
				auto nameSymbol = nameEnv->FindOrInsertName(RecoveryScanner::FAKE_IDENTIFIER.c_str(),
					RecoveryScanner::FAKE_IDENTIFIER.size());

				if (visitor->tokenKind != TK_DOT)
					p->type = ast_pool->GenTypeName(nameAst);
				else
				{
					auto fakeToken = lex->GetNextTokenAndReturnToken(pos);
					fakeToken->SetKind(TK_Identifier);
					fakeToken->SetNameSymbol(nameSymbol);
					auto fakeName = ast_pool->GenName(fakeToken);
					fakeName->base_opt = nameAst;
					p->type = ast_pool->GenTypeName(fakeName);
				}


				auto declaratorId = ast_pool->GenVariableDeclaratorId();
				declaratorId->identifier_token = lex->GetNextTokenAndReturnToken(pos);

				declaratorId->identifier_token->SetNameSymbol(nameSymbol);
				declaratorId->brackets_opt = nullptr;

				auto declarator = ast_pool->GenVariableDeclarator();
				declarator->owner = p;
				declarator->variable_declarator_name = declaratorId;
				declarator->variable_initializer_opt = nullptr;

				p->semicolon_token = lex->GetNextTokenAndReturnToken(pos);
				p->semicolon_token->SetKind(TK_SEMICOLON);

				p->AllocateVariableDeclarators(1);


				p->AddVariableDeclarator(declarator);
				if (!class_body->class_body_declarations)
				{
					class_body->AllocateClassBodyDeclarations();
				}
				if (!class_body->instance_variables) {
					class_body->AllocateInstanceVariables();
				}
				class_body->AddClassBodyDeclaration(p);
				return{ p->type };

			}
			break;
			case Ast::PRE_UNARY:
			case Ast::ASSIGNMENT:
			case Ast::CALL:
			case Ast::POST_UNARY:
			case Ast::CLASS_CREATION:
			{
				//	先组成一个instalizer
				// 然后把这这个instalizer 加到class中
				// 然后组成一个 expression statement;

				auto initializer = BuildInstalizer(lex, exptr_ast, ast_pool, pos, visitor->homeMakeAstLPos);
				if(!initializer)
				{
					return{};
				}
				if (!class_body->class_body_declarations)
				{
					class_body->AllocateClassBodyDeclarations();
				}

				if (!class_body->instance_variables)
				{
					class_body->AllocateInstanceInitializers();
				}

				class_body->AddClassBodyDeclaration(initializer);

				return{ exptr_ast };
			}
			break;

			case Ast::INSTANCEOF:
			case Ast::DOT:
			case Ast::SUPER_EXPRESSION:
			case Ast::THIS_EXPRESSION:
			case Ast::BINARY:
			case Ast::ARRAY_ACCESS:
			case Ast::ARRAY_CREATION:
			case Ast::CONDITIONAL:
			case Ast::CAST:
			case Ast::PARENTHESIZED_EXPRESSION:
			{

				auto identifier_token = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
				if (!identifier_token)
					return{};
				auto nameSymbol = nameEnv->FindOrInsertName(RecoveryScanner::FAKE_IDENTIFIER.c_str(),
					RecoveryScanner::FAKE_IDENTIFIER.size());
				identifier_token->SetNameSymbol(nameSymbol);
				auto _name = ast_pool->GenName(identifier_token);

				auto equal_token = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
				equal_token->SetKind(TK_EQUAL);

				AstAssignmentExpression* p =
					ast_pool->NewAssignmentExpression(AssignmentExpressionEnum::SIMPLE_EQUAL, equal_token);
				p->left_hand_side = _name;

				if (exptr_ast->kind == Ast::DOT && visitor->tokenKind == TK_DOT)
				{
					FieldReference* fakeFieldAcess = ast_pool->NewFieldAccess();
					fakeFieldAcess->base = exptr_ast;
					auto fake_token = lex->GetNextTokenAndReturnToken(pos);
					fake_token->SetNameSymbol(nameSymbol);
					fake_token->SetKind(TK_Identifier);
					fakeFieldAcess->identifier_token = fake_token;

					p->expression = fakeFieldAcess;
				}
				else {
					p->expression = exptr_ast;
				}
				auto initializer = BuildInstalizer(lex, p, ast_pool, pos, visitor->homeMakeAstLPos);
				if (!class_body->class_body_declarations)
				{
					class_body->AllocateClassBodyDeclarations();
				}

				if (!class_body->instance_variables)
				{
					class_body->AllocateInstanceInitializers();
				}

				class_body->AddClassBodyDeclaration(initializer);
				return{ exptr_ast };
			}
			default:
				return{};
			}

			return{};
		}




		vector<Ast*>  buildBlockScopeExpression(CompleteAssistASTVisitor* visitor, CompleteLex* lex, Expression* exptr_ast,
			AstStoragePool* ast_pool, unsigned int pos,
			AstBlock* scopeNode,
			INameEnvironment* nameEnv)
		{
			if(!visitor || !lex || !ast_pool || !scopeNode || !nameEnv || !exptr_ast)
			{
				return{};
			}
			vector<Ast*> result;
			AstBlock* actualscopeNode;
			if (scopeNode->Tag() == AstBlock::SWITCH) {

				AstSwitchBlockStatement* p = ast_pool->NewSwitchBlockStatement();
				scopeNode->AddStatement(p);
				p->parent_scope = scopeNode;



				p->AllocateSwitchLabels(1);
				AstSwitchLabel* label = ast_pool->NewSwitchLabel();

				auto fakeToken = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
				if(!fakeToken)
				{
					return{};
				}
				fakeToken->SetKind(TK_case);
				label->case_token = fakeToken;

				label->expression_opt = nullptr;

				fakeToken = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
				if (!fakeToken)
				{
					return{};
				}
				fakeToken->SetKind(TK_COLON);
				label->colon_token = fakeToken;

				p->AddSwitchLabel(label);
				p->AllocateStatements(1);

				fakeToken = lex->GetNextTokenAndReturnToken(pos);
				if (!fakeToken)
				{
					return{};
				}
				fakeToken->SetKind(TK_RBRACE);
				p->right_brace_token = fakeToken;
				actualscopeNode = p;

			}
			else {
				actualscopeNode = scopeNode;
			}


			auto nameSymbol = nameEnv->FindOrInsertName(RecoveryScanner::FAKE_IDENTIFIER.c_str(),
				RecoveryScanner::FAKE_IDENTIFIER.size());

			switch (exptr_ast->kind)
			{
			case Ast::NAME:
			{
				if (visitor->completNodeType == CompleteAssistASTVisitor::CompletionOnLocalFieldType ||
					visitor->completNodeType == CompleteAssistASTVisitor::CompletionOnAstTypeName)
				{
					//组成一个MethodInvocation
					AstMethodInvocation* p = ast_pool->NewMethodInvocation(lex->GetNextTokenAndReturnToken(pos));
					p->identifier_token->SetKind(TK_Identifier);
					p->identifier_token->SetNameSymbol(nameSymbol);
					p->base_opt = exptr_ast;
					p->type_arguments_opt = nullptr;


					auto left = lex->GetNextTokenAndReturnToken(pos);
					auto right = lex->GetNextTokenAndReturnToken(pos);
					if(!left ||!right)
					{
						return{};
					}
					left->SetKind(TK_LPAREN);
					right->SetKind(TK_RPAREN);
					p->arguments = ast_pool->GenArguments(left, right);


					auto expressionStatement = ast_pool->GenExpressionStatement();

					expressionStatement->expression = p;
					expressionStatement->semicolon_token_opt = nullptr;
					if (!actualscopeNode->block_statements) {
						actualscopeNode->AllocateStatements();
					}
					actualscopeNode->AddStatement(expressionStatement);

					result.emplace_back(exptr_ast);
				}
				else
				{

					if (true)
					{
						//组成一个 field
						LocalDeclaration* p = ast_pool->NewLocalVariableStatement();
						p->modifiers_opt = nullptr;
						auto cloneExpre = reinterpret_cast<AstName*>(exptr_ast->Clone(ast_pool));

						if (visitor->tokenKind != TK_DOT)
							p->type = ast_pool->GenTypeName(cloneExpre);
						else
						{
							auto fakeToken = lex->GetNextTokenAndReturnToken(pos);
							if (!fakeToken)
								return{};

							fakeToken->SetKind(TK_Identifier);
							fakeToken->SetNameSymbol(nameSymbol);
							auto fakeName = ast_pool->GenName(fakeToken);
							fakeName->base_opt = cloneExpre;
							p->type = ast_pool->GenTypeName(fakeName);
						}

						auto declaratorId = ast_pool->GenVariableDeclaratorId();

						declaratorId->identifier_token = lex->GetNextTokenAndReturnToken(pos);



						declaratorId->identifier_token->SetNameSymbol(nameSymbol);
						declaratorId->identifier_token->SetKind(TK_Identifier);
						declaratorId->brackets_opt = nullptr;

						auto declarator = ast_pool->GenVariableDeclarator();
						declarator->owner = p;
						declarator->variable_declarator_name = declaratorId;
						declarator->variable_initializer_opt = nullptr;

						p->semicolon_token_opt = lex->GetNextTokenAndReturnToken(pos);
						p->semicolon_token_opt->SetKind(TK_SEMICOLON);

						p->AllocateVariableDeclarators(1);
						p->AddVariableDeclarator(declarator);
						if (!actualscopeNode->block_statements)
						{
							actualscopeNode->AllocateStatements(1);
						}
						actualscopeNode->AddStatement(p);

						result.emplace_back(p->type);
					}
					if (true)
					{
						// 先组成AstParenthesizedExpression， 然后FieldReference，最后 AstAssignmentExpression
						auto fakeToken = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
						fakeToken->SetKind(TK_LPAREN);
						AstParenthesizedExpression* parenthesize = ast_pool->NewParenthesizedExpression();
						parenthesize->left_parenthesis_token = fakeToken;
						parenthesize->expression = exptr_ast;

						fakeToken = lex->GetNextTokenAndReturnToken(pos);
						fakeToken->SetKind(TK_RPAREN);
						parenthesize->right_parenthesis_token = fakeToken;

						FieldReference* fieldAccess = ast_pool->NewFieldAccess();
						fieldAccess->base = parenthesize;

						fakeToken = lex->GetNextTokenAndReturnToken(pos);
						fakeToken->SetKind(TK_Identifier);
						fakeToken->SetNameSymbol(nameSymbol);
						fieldAccess->identifier_token = fakeToken;



						// 构建 AstAssignmentExpression
						auto identifier_token = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
						identifier_token->SetNameSymbol(nameSymbol);
						identifier_token->SetKind(TK_Identifier);
						auto _name = ast_pool->GenName(identifier_token);

						auto equal_token = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
						equal_token->SetKind(TK_EQUAL);

						AstAssignmentExpression* p =
							ast_pool->NewAssignmentExpression(AssignmentExpressionEnum::SIMPLE_EQUAL, equal_token);
						p->left_hand_side = _name;
						p->expression = fieldAccess;


						auto expressionStatement = ast_pool->GenExpressionStatement();
						expressionStatement->expression = p;
						expressionStatement->semicolon_token_opt = nullptr;


						if (!actualscopeNode->block_statements) {
							actualscopeNode->AllocateStatements();
						}
						actualscopeNode->AddStatement(expressionStatement);
						result.emplace_back(exptr_ast);
					}
				}
			}
			break;
			case Ast::PRE_UNARY:
			case Ast::ASSIGNMENT:
			case Ast::CALL:
			case Ast::POST_UNARY:
			case Ast::CLASS_CREATION:
			{

				//组成一个 expression statement;
				auto expressionStatement = ast_pool->GenExpressionStatement();
				expressionStatement->expression = exptr_ast;
				expressionStatement->semicolon_token_opt = nullptr;

				if (!actualscopeNode->block_statements) {
					actualscopeNode->AllocateStatements();
				}
				actualscopeNode->AddStatement(expressionStatement);

				result.emplace_back(exptr_ast);
			}
			break;

			case Ast::INSTANCEOF:
			case Ast::DOT:
			case Ast::SUPER_EXPRESSION:
			case Ast::THIS_EXPRESSION:
			case Ast::BINARY:
			case Ast::ARRAY_ACCESS:
			case Ast::ARRAY_CREATION:
			case Ast::CONDITIONAL:
			case Ast::CAST:
			case Ast::PARENTHESIZED_EXPRESSION:
			{
				auto identifier_token = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
				identifier_token->SetNameSymbol(nameSymbol);
				identifier_token->SetKind(TK_Identifier);
				auto _name = ast_pool->GenName(identifier_token);

				auto equal_token = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
				equal_token->SetKind(TK_EQUAL);

				AstAssignmentExpression* p =
					ast_pool->NewAssignmentExpression(AssignmentExpressionEnum::SIMPLE_EQUAL, equal_token);
				p->left_hand_side = _name;
				if(exptr_ast->kind == Ast::DOT && visitor->tokenKind == TK_DOT)
				{
					FieldReference* fakeFieldAcess = ast_pool->NewFieldAccess();
					fakeFieldAcess->base = exptr_ast;
					auto fake_token = lex->GetNextTokenAndReturnToken(pos);
					fake_token->SetNameSymbol(nameSymbol);
					fake_token->SetKind(TK_Identifier);
					fakeFieldAcess->identifier_token = fake_token;

					p->expression = fakeFieldAcess;
				}
				else{
					p->expression = exptr_ast;
				}
				auto expressionStatement = ast_pool->GenExpressionStatement();
				expressionStatement->expression = p;
				expressionStatement->semicolon_token_opt = nullptr;


				if (!actualscopeNode->block_statements) {
					actualscopeNode->AllocateStatements();
				}
				actualscopeNode->AddStatement(expressionStatement);
				result.emplace_back(exptr_ast);

			}
			default:
				break;
			}
			return  result;
		}



		int GetPreTokenBeginIndex(vector<Token*>& token_stream, int beginIndex, int endIndex)
		{
			auto tokenData = token_stream.data();
			auto tokenLen = token_stream.size();
			int curIndex = endIndex;
			while (curIndex >= beginIndex)
			{
				auto token = tokenData[curIndex];
				switch (token->_kind)
				{
				case TK_SEMICOLON:
				case TK_LBRACE:
				case TK_RBRACE:
				case TK_AT:
				case TK_abstract:
				case TK_case:
				case TK_catch:
				case TK_assert:
				case TK_break:
				case TK_class:
				case TK_continue:
				case TK_default:
				case TK_do:
				case TK_else:
				case TK_extends:
				case TK_finally:
				case TK_for:
				case TK_if:
				case TK_implements:
				case TK_import:
				case TK_interface:
				case TK_native:
				case TK_package:
				case TK_private:
				case TK_protected:
				case TK_public:
				case TK_return:
				case TK_static:
				case TK_strictfp:
				case TK_switch:
				case TK_synchronized:
				case TK_throws:
				case TK_transient:
				case TK_try:
				case TK_volatile:
				case TK_void:
				{
					if (curIndex == endIndex)
					{
						return  -1;
					}
					else
						return  curIndex + 1;
				}

				default:
					--curIndex;
					break;
				}

			}

			if (curIndex < beginIndex) {
				return beginIndex;
			}
			else
				return  curIndex;
		}

		int GetPreTokenBeginIndexForUnitScop(vector<Token*>& token_stream, int beginIndex, int endIndex)
		{
			auto tokenData = token_stream.data();
			auto tokenLen = token_stream.size();
			int curIndex = endIndex;
			int lastTokenKind = 0;

			auto token = tokenData[curIndex];
			if (token->_kind == TK_DOT)
			{
				lastTokenKind = TK_Identifier;
			}
			else if (token->_kind == TK_Identifier) {
				lastTokenKind = TK_DOT;
			}

			while (curIndex >= beginIndex) {
				token = tokenData[curIndex];
				switch (token->_kind) {

				case TK_Identifier:
				{
					if (lastTokenKind != TK_DOT)
					{
						return  curIndex + 1;
					}
					--curIndex;
					lastTokenKind = token->_kind;
				}
				break;
				case TK_DOT:
				{
					if (lastTokenKind != TK_Identifier) {
						return  curIndex + 1;
					}
					--curIndex;
					lastTokenKind = token->_kind;
				}
				break;
				default:
				{
					if (curIndex == endIndex)
					{
						return  -1;
					}
					else
						return  curIndex + 1;
				}
				}
			}
			if (curIndex < beginIndex) {
				return beginIndex;
			}
			else
				return  curIndex;
		}


		Expression* CompletionEngine::BuildExpressAstNode(AstNodeScope*scope, CompleteLex* lex,
			int pos,
			INameEnvironment* nameEnv,
			CompleteAssistASTVisitor* visitor,
			Parser* parser, CompilationUnitDeclaration* compilation_unit
		)
		{
			if (!lex || !nameEnv || !visitor || !compilation_unit || !parser || !scope)
				return nullptr;

			// 传统做法
			int endIndex = lex->FindGoodTokenIndexInOrigianlTokenStream(pos);

			if (endIndex <= 0) {
				return nullptr;
			}

			int beginIndex = lex->FindGoodTokenIndexInOrigianlTokenStream(visitor->homeMakeAstLPos);
			if (beginIndex <= 0)
				beginIndex = 1;

			if (beginIndex > endIndex) {
				return nullptr;
			}

			auto orginalTokenStream = lex->getOriginalTokenStream();
			int   affectBeginIdx;
			switch (scope->GetScopeKind())
			{
			case AstNodeScope::COMPILATION_UNIT_SCOPE: {
				affectBeginIdx = GetPreTokenBeginIndexForUnitScop(orginalTokenStream, beginIndex, endIndex);
				if (affectBeginIdx <= 0)
					return nullptr;
				auto line = lex->LineFromPos(orginalTokenStream[affectBeginIdx]->start_location);
				ErrorString outline;
				lex->OutputLine(line, outline);
				Jikes::wstringex exStr = outline.ArrayString();
				exStr.trim();
				auto ___pos = exStr.find(' ');
				if (___pos != wstring::npos)
				{
					auto firstIdentifer = exStr.substr(0, ___pos);
					if (CharOperation::prefixEquals(firstIdentifer, L"import", false)) {
						hasImportWord = true;
					}
				}
			}

		     break;
			 			default:

				affectBeginIdx = GetPreTokenBeginIndex(orginalTokenStream, beginIndex, endIndex);
				break;
			}

			if (affectBeginIdx <= 0)
				return nullptr;

			vector<Token*> expressionTokenStream;



			expressionTokenStream.reserve(endIndex - affectBeginIdx + 1);

			if(lex->token_stream.empty())
			{
				return nullptr;
			}

			expressionTokenStream.push_back(lex->token_stream[0]);


			auto index = 1;
			for (auto i = affectBeginIdx; i <= endIndex; ++i)
			{
				auto token = orginalTokenStream[i];
				token->index = index++;

				expressionTokenStream.emplace_back(token);
			}

			auto eofToken = lex->token_stream[lex->token_stream.size() - 1];
			eofToken->index = index;

			expressionTokenStream.emplace_back(eofToken);



			lex->token_stream.swap(expressionTokenStream);

			auto end_token_index = LexStream::LEX_INFINITY;
			auto start_token_index = LexStream::LEX_START;
			auto dianose = new DiagnoseParser(lex, TK_ExpressionMarker,
				start_token_index, end_token_index, nullptr, *lookupEnvironment);


			dianose->diagnoseParse(true);
			auto exptr_ast = parser->ExpressParse(lex, compilation_unit->ast_pool);
			if (!exptr_ast)
			{
				eofToken->index = expressionTokenStream.size() - 1;
				lex->token_stream.swap(expressionTokenStream);
				delete dianose;
				return nullptr;
			}
			lex->token_stream.swap(expressionTokenStream);


			auto old_size = lex->token_stream.size() - 1;
			lex->token_stream[old_size]->index = old_size;

			size_t i = 1, size = expressionTokenStream.size() - 1;

			index = i + old_size;
			for (; i < size; ++i)
			{
				auto token = expressionTokenStream[i];
				token->index = index++;
				lex->token_stream.emplace_back(token);
			}
			eofToken->index = index;
			lex->token_stream.emplace_back(eofToken);
			delete dianose;

			return exptr_ast;
		}

		vector<Ast*> CompletionEngine::GetHomeMakeAst(CompleteAssistASTVisitor* visitor, unsigned int pos, INameEnvironment* nameEnv,
			CompilationUnitDeclaration* compilation_unit, CompleteLex* lex,
			Parser* parser)
		{

			if (!lex || !nameEnv || !visitor || !compilation_unit || !parser )
				return {};

			auto scope = visitor->completeScope;
			auto exptr_ast = BuildExpressAstNode(scope, lex, pos, nameEnv, visitor, parser, compilation_unit);

			if (!exptr_ast || !scope) {
				return{};
			}
			auto ast_pool = compilation_unit->ast_pool;

			switch (scope->GetScopeKind())
			{
			case AstNodeScope::COMPILATION_UNIT_SCOPE:
			{
				if (exptr_ast->kind == Ast::NAME) {
					//组成 一个 import
					auto  scopeNode = dynamic_cast<CompilationUnitDeclaration*>(scope);
					AstImportDeclaration* p = ast_pool->NewImportDeclaration();
					p->import_token = lex->GetNextTokenAndReturnToken(visitor->homeMakeAstLPos);
					p->import_token->SetKind(TK_import);
					p->static_token_opt = nullptr;
					p->name = reinterpret_cast<AstName*>(exptr_ast);
					p->star_token_opt = nullptr;
					p->semicolon_token = lex->GetNextTokenAndReturnToken(pos);
					p->semicolon_token->SetKind(TK_SEMICOLON);

					if (!scopeNode->import_declarations) {
						scopeNode->AllocateImportDeclarations();
					}

					scopeNode->AddImportDeclaration(p);
					return{ p };
				}
				else {
					return{};
				}
			}
			break;
			case AstNodeScope::CLASS_SCOPE: {
				auto  scopeNode = dynamic_cast<TypeDeclaration*>(scope);
				return  buildClassScopeExpression(visitor, lex, exptr_ast, ast_pool, pos, scopeNode, nameEnv);
			}
											break;

			case AstNodeScope::METHOD_SCOPE: {
				if (exptr_ast->kind == Ast::NAME)
				{
					//组成一个 formalParameter
					auto scopeNode = dynamic_cast<AbstractMethodDeclaration*>(scope);
					// 需要看是哪里了 在函数头()中 要么把它 组成AstFormalParameter 
					AstFormalParameter* p = ast_pool->NewFormalParameter();
					p->type = ast_pool->NewTypeName(reinterpret_cast<AstName*>(exptr_ast));
					AstVariableDeclarator* formal_declarator =
						ast_pool->NewVariableDeclarator();
					p->formal_declarator = formal_declarator;

					formal_declarator->owner = p;
					formal_declarator->variable_declarator_name = ast_pool->NewVariableDeclaratorId();
					formal_declarator->variable_declarator_name->identifier_token = lex->GetNextTokenAndReturnToken(pos);


					auto nameSymbol = nameEnv->FindOrInsertName(RecoveryScanner::FAKE_IDENTIFIER.c_str(),
						RecoveryScanner::FAKE_IDENTIFIER.size());
					formal_declarator->variable_declarator_name->identifier_token->SetNameSymbol(nameSymbol);
					if (!scopeNode->method_declarator->formal_parameters) {
						scopeNode->method_declarator->AllocateFormalParameters(1);
					}
					scopeNode->method_declarator->AddFormalParameter(p);

					return{ p };
				}
			}
			break;
			case AstNodeScope::BLOCK_SCOPE:
			{

				auto  scopeNode = dynamic_cast<AstBlock*>(scope);
				return  buildBlockScopeExpression(visitor, lex, exptr_ast, ast_pool, pos, scopeNode, nameEnv);
			}
			break;
			default:
				return{};
			}

			return{};
		}
		
} // Close namespace Jikes block

} // Close namespace CodeAssist block

