// $Id: parser.cpp,v 1.28 2004/03/25 13:32:28 ericb Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#include "JCDT_Lib/internal/parser/parser.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include "JCDT_Lib/internal/parser/diagnose/diagnose.h"
#include <string>
#include <vector>
#include  "JCDT_Lib/internal/lookup/ASTVisitor.h"
#include <JCDT_Lib/internal/parser/RecoverStatementsVisitor.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>


namespace Jikes { // Open namespace Jikes block


void Parser::recoverStatements() {
	MethodVisitor* methodVisitor = new MethodVisitor();
	TypeVisitor* typeVisitor = new TypeVisitor();
	methodVisitor->typeVisitor = typeVisitor;
	typeVisitor->methodVisitor = methodVisitor;
	if(dynamic_cast<MethodDeclaration*>(referenceContext)){
		((MethodDeclaration*)referenceContext)->traverse(methodVisitor,nullptr);
	}
	else if(dynamic_cast<ConstructorDeclaration*>(referenceContext))
	{
		((ConstructorDeclaration*)referenceContext)->traverse(methodVisitor,nullptr);
	}
	else if(dynamic_cast<AstClassBody*>(referenceContext))
	{
		AstClassBody* class_body = (AstClassBody*)referenceContext;
		unsigned i;
		for (i = 0; i < class_body->NumStaticInitializers(); i++) {
			methodVisitor->enclosingTypeClassBody = class_body;
			class_body->StaticInitializer(i)->traverse(methodVisitor,nullptr);
		}
		for (i = 0; i < class_body->NumInstanceInitializers(); i++) 
			methodVisitor->enclosingTypeClassBody = class_body;
			class_body->InstanceInitializer(i)->traverse(methodVisitor,nullptr);
	}
}


	Parser::Parser(bool no_semantic_action_) : ast_pool(NULL),
		 parse_header_only(false),
		 parse_package_header_only(false),
		 location_stack(nullptr),
		 locationStartStack(nullptr),
		 parse_stack(nullptr),
		 stack_length(0),
		 stack(NULL),
		 temp_stack(NULL),
		 no_semantic_action(no_semantic_action_)
	 {
		 if (!no_semantic_action)
			 consumeRule();
		 nestedType = 0;
		 return;
	 }

	 Parser::~Parser()
	 {
		 delete[] stack;
		 delete[] location_stack;
		 delete[] locationStartStack;
		 delete[] parse_stack;
		 delete[] temp_stack;
	 }
void Parser::ReallocateStacks()
{
    int old_stack_length = stack_length;

    stack_length += STACK_INCREMENT;

    assert(stack_length <= SHRT_MAX);

    int* old_stack = stack;
    stack = (int*) memcpy(new int[stack_length], stack,
                          old_stack_length * sizeof(int));
    delete [] old_stack;

    Location* old_location_stack = location_stack;
    location_stack = (Location*) memcpy(new Location[stack_length],
                                        location_stack,
                                        old_stack_length * sizeof(Location));
    delete [] old_location_stack;

    Ast** old_parse_stack = parse_stack;
    parse_stack = (Ast**) memcpy(new Ast*[stack_length], parse_stack,
                                 old_stack_length * sizeof(Ast*));
    delete [] old_parse_stack;
    // The first time through, we initialize parse_stack[0] to NULL.
    parse_stack[old_stack_length] = NULL;

    int* old_temp_stack = temp_stack;
    temp_stack = (int*) memcpy(new int[stack_length], temp_stack,
                               old_stack_length * sizeof(int));
    delete [] old_temp_stack;
}


AstListNode *Parser::AllocateListNode()
{
    AstListNode *p;

    if (free_list_nodes)
    {
        p = free_list_nodes;
        free_list_nodes = free_list_nodes -> next;
    }
    else p = list_node_pool -> NewListNode();

    return p;
}


void Parser::FreeCircularList(AstListNode* tail)
{
    if (tail)
    {
        AstListNode* root = tail -> next;
        tail -> next = free_list_nodes;
        free_list_nodes = root;
    }
}

CompilationUnitDeclaration* Parser::CompilationUnitParse(BaseLexStream* lex,
	AstStoragePool* ast_pool_ )
{
	
	lex_stream = lex;
	end_token_index = LexStream::LEX_INFINITY;
	start_token_index = LexStream::LEX_START;
	
	goForCompilationUnit();
	if(parse_header_only == true)
	{	
		body_pool = new AstStoragePool(lex_stream->NumTokens());
		ast_pool = (ast_pool_ ? ast_pool_ : body_pool);

	}
	else{
		assert(ast_pool_);
		ast_pool = ast_pool_;
	}

	
	list_node_pool = new AstStoragePool(lex_stream->NumTokens());
	free_list_nodes = NULL;
	// We are parsing the whole input and not just a segment.

	
	
	parse();
	
	CompilationUnitDeclaration *compilation_unit = nullptr;
	Ast* ast = parse_stack[0];
	if (ast && CompilationUnitDeclaration::CompilationUnitCast(ast)){
		compilation_unit = (CompilationUnitDeclaration*)ast;
		/*if (lastAct == ERROR_ACTION)
			compilation_unit->MarkBad();*/

	}
	delete list_node_pool;
	return compilation_unit;

}
AstPackageDeclaration* Parser::PackageHeaderParse(BaseLexStream* lex,
                                                  AstStoragePool* ast_pool_)
{
	assert(lex);
	
    AstPackageDeclaration* package_declaration = NULL;
    if (lex-> PackageToken()){
		
		parse_package_header_only = true;
		CompilationUnitDeclaration *compilation_unit = CompilationUnitParse(lex, ast_pool_);
		parse_package_header_only = false;
		if (compilation_unit && !CompilationUnitDeclaration::BadCompilationUnitCast(compilation_unit))
		{
			package_declaration =
				compilation_unit->package_declaration_opt;
		}
    }

    return package_declaration;
}


CompilationUnitDeclaration* Parser::HeaderParse(
	BaseLexStream* lex,AstStoragePool* ast_pool_)
{
	
    parse_header_only = true;
	auto compilation_unit  = CompilationUnitParse(lex, ast_pool_);
	parse_header_only = false;
    if (compilation_unit){
        if (! CompilationUnitDeclaration::BadCompilationUnitCast(compilation_unit))
        {
            if (compilation_unit -> NumTypeDeclarations() == 0)
                compilation_unit -> MarkEmpty();
        }
    }

    // If we succesfully parsed a compilation unit, allocate a storage pool
    // for it. Subtract the amount of space that's already been allocated for
    // the headers from the estimate for the bodies.
    if (compilation_unit)
         compilation_unit -> ast_pool = body_pool;
    else delete body_pool;

   // delete list_node_pool; // free the pool of list nodes

    return compilation_unit;
}

AstMethodBody* Parser::ParseMethodBody(AstMethodBody* block){
	
	end_token_index = block->right_brace_token->index; // last token in the body
	start_token_index = block->left_brace_token->index;

	goForMethodBody();
	parse();
	auto ast = parse_stack[0];
	return  DYNAMIC_CAST<AstMethodBody *>(ast);

 }

bool Parser::BodyParse(BaseLexStream* lex, AstClassBody* class_body)
{
	
	assert(AstClassBody::UnparsedClassBodyCast(class_body));
	

    lex_stream = lex;

    ast_pool = class_body -> pool;
    body_pool = class_body -> pool;
    list_node_pool = new AstStoragePool(lex_stream-> NumTokens());
    free_list_nodes = NULL;

    bool success = Body(class_body);

    delete list_node_pool; // free the pool of list nodes

    class_body -> MarkParsed();

    return success;
}

	Expression* Parser::ExpressParse(BaseLexStream* lex, AstStoragePool* _ast_pool)
	{
		lex_stream = lex;
		ast_pool = _ast_pool;
		body_pool = ast_pool;

		list_node_pool = new AstStoragePool(lex_stream->NumTokens());
		free_list_nodes = nullptr;

		end_token_index = LexStream::LEX_INFINITY;
		start_token_index = LexStream::LEX_START;

		goForExpression();
		parse();
		auto ast = parse_stack[0];
		auto _ret = dynamic_cast<Expression*>(ast);
		delete list_node_pool; // free the pool of list nodes
		return _ret;

	}

	AstMethodBody* Parser::ParseMethodBody(BaseLexStream* lex, AstStoragePool* _ast_pool)
	{
		lex_stream = lex;
		ast_pool = _ast_pool;
		body_pool = ast_pool;

		list_node_pool = new AstStoragePool(lex_stream->NumTokens());
		free_list_nodes = nullptr;

		end_token_index = LexStream::LEX_INFINITY;
		start_token_index = LexStream::LEX_START;

		goForMethodBody();
		parse();
		auto ast = parse_stack[0];
		auto _ret = dynamic_cast<AstMethodBody*>(ast);
		delete list_node_pool; // free the pool of list nodes
		return _ret;
	}

	bool  Parser::parse(ReferenceContext* rc, MethodDeclaration* method_decl) {

	referenceContext = rc;
	assert(method_decl);
	bool errors_detected = false;
	if (method_decl->method_body_opt  &&!method_decl->body_parse)
	{
		AstMethodBody* block = method_decl->method_body_opt;
		auto new_block = ParseMethodBody(block);
		if (!new_block) // a bad block ?
			errors_detected = true;
		else {
			method_decl->method_body_opt = new_block;
			new_block->owner_ast = method_decl;
			//new_block->Print(*lex_stream);
		}
		method_decl->body_parse = true;
	}
	return errors_detected;
}
bool  Parser::parse(ReferenceContext* rc,ConstructorDeclaration* constructor_decl) {
	assert(constructor_decl);
	
	bool errors_detected = false;
	referenceContext = rc;
	AstMethodBody* block = constructor_decl->constructor_body;
	if (block && !constructor_decl->body_parse)
	{
		auto new_body = ParseMethodBody(block);
		if (!new_body)
			errors_detected = true;
		else
		{
			constructor_decl->constructor_body = new_body;
			new_body->owner_ast = constructor_decl;
		}

		constructor_decl->body_parse = true;
	}
	return errors_detected;
}
bool  Parser::parse(ReferenceContext* rc,Initializer* initializer) {
	assert(initializer);
	referenceContext = rc;
	bool errors_detected = false;
	AstMethodBody* block = initializer->block;
	if (block) {
		auto new_body = ParseMethodBody(block);
		initializer->block = new_body;

		if(new_body)
			new_body->owner_ast = initializer;

		if (!initializer->block)
		{
			errors_detected = true;
			// Restore old empty block.
			initializer->block = block;
		}
	}
	return errors_detected;
}
bool Parser::Body(AstClassBody* class_body)
{
    bool errors_detected = false;
    unsigned i;

    for (i = 0; i < class_body -> NumConstructors(); i++){
		auto cd = class_body->Constructor(i);
		errors_detected = errors_detected || parse(cd, cd);
    }

    for (i = 0; i < class_body -> NumMethods(); i++){
		auto md = class_body->Method(i);
		errors_detected = errors_detected || parse(md, md);
    }

    for (i = 0; i < class_body -> NumNestedClasses(); i++)
        errors_detected = errors_detected ||
            ! Body(class_body -> NestedClass(i) -> class_body);
    for (i = 0; i < class_body -> NumNestedInterfaces(); i++)
        errors_detected = errors_detected ||
            ! Body(class_body -> NestedInterface(i) -> class_body);
    return ! errors_detected;
}


bool Parser::InitializerParse(BaseLexStream* lex, AstClassBody* class_body)
{
    lex_stream = lex;
    ast_pool = class_body -> pool;
    body_pool = class_body -> pool;
    list_node_pool = new AstStoragePool(lex_stream-> NumTokens());
    free_list_nodes = nullptr;
	
    bool success = InitializerClassBody(class_body);

    delete list_node_pool; // free the pool of list nodes
    return success;
}


	void Parser::initialize()
	{
		if (stack_length)
			parse_stack[0] = NULL;

		state_stack_top = -1;
		//nestedMethod.clear();
		//nestedMethod.push_back(0);
		//nestedMethod[nestedType = 0] = 0;
		has_localType_inScope = false;
	}

	bool Parser::InitializerClassBody(AstClassBody* class_body)
{
    bool errors_detected = false;
    unsigned i;

    for (i = 0; i < class_body -> NumStaticInitializers(); i++){
		errors_detected = errors_detected || parse(class_body,class_body->StaticInitializer(i));
    }
    for (i = 0; i < class_body -> NumInstanceInitializers(); i++){
		errors_detected = errors_detected || parse(class_body,class_body->InstanceInitializer(i));
    }
    for (i = 0; i < class_body -> NumNestedClasses(); i++)
        errors_detected = errors_detected ||
            !InitializerClassBody(class_body -> NestedClass(i) -> class_body);
    for (i = 0; i < class_body -> NumNestedInterfaces(); i++)
        errors_detected = errors_detected ||
            !InitializerClassBody(class_body -> NestedInterface(i) -> class_body);
    return ! errors_detected;
}


void Parser::parse()
{
   
    // The next call to Gettoken will return the start_token_index.
    // However, we initialize curtok to start_token_index in order
    // to obtain a valid location for the BodyMarker.
    //
	initialize();
	assert(!no_semantic_action);
	lex_stream->Reset(start_token_index);
	auto last_end_token_index = lex_stream->GetEndTokenIndex();
	lex_stream->SetEndTokenIndex(end_token_index);

	
    TokenObject curtok = start_token_index; // get the location of the start_token_index
    int act = START_STATE,
         current_kind = getFirstToken();
	
/*****************************************************************/
/* Start parsing.                                                */
/*****************************************************************/
   

    //
    // Process a terminal
    //
    while (true)
    {
        if (++state_stack_top >= stack_length)
            ReallocateStacks();

        stack[state_stack_top] = act;
        location_stack[state_stack_top] = Loc(curtok);

        act = t_action(act, current_kind, lex_stream);

        if (act <= NUM_RULES)
            state_stack_top--; // make reduction look like a shift-reduce
        else if (act > ERROR_ACTION)
        {
            curtok = lex_stream -> Gettoken(/*end_token_index*/);
            current_kind = lex_stream -> Kind(curtok);

            act -= ERROR_ACTION;
        }
        else if (act < ACCEPT_ACTION)
        {
            curtok = lex_stream -> Gettoken(/*end_token_index*/);
            current_kind = lex_stream -> Kind(curtok);

            continue;
        }
        else break;

        //
        // Process a nonterminal
        //
        do
        {
            state_stack_top -= (rhs[act] - 1);
            (this ->* rule_action[act])();
			//consumeRule(act);
            act = nt_action(stack[state_stack_top], lhs[act]);
        } while (act <= NUM_RULES);
    } /* process_terminal */

	lastAct = act;
    if (act == ERROR_ACTION){
 		if(parse_package_header_only == false)
			RepairParse(curtok);
    }
	lex_stream->SetEndTokenIndex(last_end_token_index);
   
}


void Parser::RepairParse(TokenObject curtok)
{
    //
    // Repair an error
    //
    while (true)
    {
        //
        // Pop state stack up to first state that had an
        // action on the error token.  The net effect is to
        // remove all default reductions on an empty rule
        // caused by the error token.
        //
        int k;
        for (k = state_stack_top - 1;
             k >= 0 && location_stack[k] == Loc(curtok); k--);
        k++;

        state_stack_top = k;

        ErrorRepair(curtok);

        curtok = lex_stream -> Gettoken(/*end_token_index*/);
        int act = stack[state_stack_top--];
        int current_kind = lex_stream -> Kind(curtok);

        //
        // Process a terminal
        //
        while (true)
        {
            if (++state_stack_top >= stack_length)
                 ReallocateStacks();

            stack[state_stack_top] = act;
            location_stack[state_stack_top] = Loc(curtok);

            act = t_action(act, current_kind, lex_stream);

			if (act <= NUM_RULES)
				state_stack_top--; // make reduction look like a shift-reduce
			else if (act > ERROR_ACTION)
			{
				curtok = lex_stream->Gettoken(/*end_token_index*/);
				current_kind = lex_stream->Kind(curtok);

				act -= ERROR_ACTION;
			}
			else if (act < ACCEPT_ACTION)
			{
				curtok = lex_stream->Gettoken(/*end_token_index*/);
				current_kind = lex_stream->Kind(curtok);

				continue;
			}
			//
			// Since the null string is a valid Java program, this function
			// will always succeed even if it has to delete the whole input.
			//
			else if (act == ACCEPT_ACTION || current_kind == TK_EOF)
				return;
			else
				break;
                // loop around and keep trying until something is accepted.
               

            //
            // Process a nonterminal
            //
            do
            {
                state_stack_top -= (rhs[act] - 1);
               (this ->* rule_action[act])();
				//consumeRule(act);
                act = nt_action(stack[state_stack_top], lhs[act]);
            } while (act <= NUM_RULES);
        } /* process_terminal */
    }
}


//
// This routine is invoked when an error is encountered in a "repair"
// parser. It will place the parser back into a working configuration
// by adjusting the state stack, the current token and the buffer.
//
void Parser::ErrorRepair(TokenObject error_token)
{
	auto max_index = lex_stream->NumTokens() - 1;
	if (end_token_index < max_index)
		max_index = end_token_index;

    SecondaryRepairInfo repair;
	int old_state_stack_top = state_stack_top;
    repair.code = ERROR_CODE;
    do
    {
        repair.distance = 0;
        repair.num_deletions = state_stack_top + BUFF_UBOUND;

        buffer[1] = error_token;
        buffer[0] = lex_stream -> Previous(buffer[1]);

        for (int k = 2; k < BUFF_SIZE; k++)
            buffer[k] = lex_stream -> Next(buffer[k - 1]);

        int last_index;
        for (last_index = MAX_DISTANCE - 1;
             last_index >= 1 &&
                 lex_stream -> Kind(buffer[last_index]) == EOFT_SYMBOL;
             last_index--);
        last_index++;

        repair = ErrorSurgery(stack, state_stack_top, last_index, repair);
        error_token = buffer[MAX_DISTANCE - MIN_DISTANCE + 2];
	    if(max_index <= error_token){
			break;
	    }
    } while (repair.code == 0 );
	
	if (max_index <= error_token) {
		if(repair.code != 0 && max_index == error_token)
		{
			state_stack_top = repair.stack_position;
			lex_stream->Reset(buffer[repair.buffer_position]);
		}
		else
		{
			state_stack_top = old_state_stack_top;
			lex_stream->Reset(buffer[2]);
		}
	}
	else
	{
		state_stack_top = repair.stack_position;
		lex_stream->Reset(buffer[repair.buffer_position]);
	}

	
    
}


//
// Keep cutting "stuff" out around the error until a stable configuration
// is found.
//
SecondaryRepairInfo Parser::ErrorSurgery
         (int stck[], int stack_top,
          int last_index, SecondaryRepairInfo repair)
{
    int stack_deletions = 0;
    Location  previous_loc = Loc(buffer[2]);

    for (int top = stack_top;
         top >= 0 && repair.num_deletions >= stack_deletions; top--)
    {
        if (location_stack[top] < previous_loc)
            stack_deletions++;
        previous_loc = location_stack[top];

        for (int i = 1;
             i <= last_index &&
                 repair.num_deletions >= (stack_deletions + i - 1); i++)
        {
            int j = ParseCheck(stck, top, lex_stream -> Kind(buffer[i]),
                               i + 1);

            if ((j - i + 1) > MIN_DISTANCE)
            {
                int k = stack_deletions + i - 1;
                if ((k < repair.num_deletions) ||
                    (j - k) > (repair.distance - repair.num_deletions))
                {
                    repair.code = DELETION_CODE;
                    repair.distance = j;
                    repair.stack_position = top;
                    repair.buffer_position = i;
                    repair.num_deletions = k;
                }
            }
        }
    }

    return repair;
}


/*****************************************************************/
/* Try to parse until first_token and all tokens in BUFFER have  */
/* been consumed, or an error is encountered. Return the number  */
/* of tokens that were expended before the parse blocked.        */
/*****************************************************************/
int Parser::ParseCheck(int stck[], int stack_top, int first_token,
                       int buffer_position)
{
    int max_pos,
        indx,
        ct,
        act,
        lhs_symbol;

/*****************************************************************/
/* Initialize pointer for temp_stack and initialize maximum      */
/* position of state stack that is still useful.                 */
/*****************************************************************/
    act = stck[stack_top];
    if (first_token > NT_OFFSET)
    {
        temp_stack_top = stack_top;
        max_pos = stack_top;
        indx = buffer_position;
        ct = lex_stream -> Kind(buffer[indx]);
        lex_stream -> Reset(lex_stream -> Next(buffer[indx]));
        lhs_symbol = first_token - NT_OFFSET;
        act = nt_action(act, lhs_symbol);
        if (act <= NUM_RULES)
            goto process_non_terminal;
    }
    else
    {
        temp_stack_top = stack_top - 1;
        max_pos = temp_stack_top;
        indx = buffer_position - 1;
        ct = first_token;
        lex_stream -> Reset(buffer[buffer_position]);
    }

 process_terminal:
    while (true)
    {
        if (++temp_stack_top >= stack_length)  /* Stack overflow!!! */
            return indx;
        temp_stack[temp_stack_top] = act;

        act = t_action(act, ct, lex_stream);

        if (act <= NUM_RULES)                   /* reduce action */
            temp_stack_top--;
        else if (act < ACCEPT_ACTION ||          /* shift action */
                 act > ERROR_ACTION)        /*shift-reduce action*/
        {
            if (indx == MAX_DISTANCE)
                return indx;
            indx++;
            ct = lex_stream -> Kind(buffer[indx]);
            lex_stream -> Reset(lex_stream -> Next(buffer[indx]));
            if (act > ERROR_ACTION)
                 act -= ERROR_ACTION;
            else goto process_terminal;
        }
        else if (act == ACCEPT_ACTION)           /* accept action */
             return MAX_DISTANCE;
        else return indx;                         /* error action */

        process_non_terminal:
        do
        {
            temp_stack_top -= (rhs[act]-1);
            lhs_symbol = lhs[act];
            act = (temp_stack_top > max_pos
                                  ? temp_stack[temp_stack_top]
                                  : stck[temp_stack_top]);
            act = nt_action(act, lhs_symbol);
        } while (act <= NUM_RULES);

        max_pos = Min(max_pos, temp_stack_top);
    } // process_terminal;

    return 0;
}


} // Close namespace Jikes block


