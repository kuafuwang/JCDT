// $Id: diagnose.h,v 1.18 2004/03/25 13:32:27 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#ifndef diagnose_INCLUDED
#define diagnose_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/parser/parser.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "RecoveryScanner.h"
#include <JCDT_Lib/internal/problem/ParseErrorCode.h>
#include "JCDT_Lib/internal/parser/SecondaryRepairInfo.h"
#include "JCDT_Lib/internal/parser/PrimaryRepairInfo.h"




namespace Jikes { // Open namespace Jikes block




struct RepairCandidate
{
    int symbol;
    Location location;
};

struct StateInfo
{
    int state,
        next;
};



class DiagnoseParser : public Parser
{
public:

	explicit DiagnoseParser(BaseLexStream* lex_stream_,int firstToken_,
		TokenObject start_token_index_, TokenObject end_token_index_, 
		ParseError* error, LookupEnvironment& lookup_env);
	
   virtual ~DiagnoseParser();
    void diagnoseParse(bool record);
   
private:
	 ParseError* parser_error_ ;
	bool _recordError;
    int next_stack_top;
    int* next_stack;

    int prev_stack_top;
    int* prev_stack;

    int scope_stack_top;
    int* scope_index;
    int* scope_position;

    int list[NUM_SYMBOLS + 1];

    enum { NIL = -1 };
    Tuple<StateInfo> state_pool;
    // this variable is managed entirely by the function "scope_trial"
    int* state_seen;
	TokenIndex errorToken;
	int errorTokenStart;
   
	

	virtual  void ReallocateStacks() override;

    RepairCandidate ErrorRecovery(TokenObject error_token);
    RepairCandidate PrimaryPhase(TokenObject error_token);
    int MergeCandidate(int state, int buffer_position);
    PrimaryRepairInfo CheckPrimaryDistance(int stack[],
                                           int stack_top,
                                           PrimaryRepairInfo repair);
    RepairCandidate PrimaryDiagnosis(PrimaryRepairInfo repair);
    int GetTermIndex(int stack[], int stack_top,
                     int tok, int buffer_position);
    int GetNtermIndex(int start, int sym, int buffer_position);
    int Misspell(int sym, TokenObject tok);
    RepairCandidate SecondaryPhase(TokenObject error_token);
    SecondaryRepairInfo MisplacementRecovery
             (int stack[],
              int stack_top,
              int last_index,
              SecondaryRepairInfo misplaced, bool stack_flag);
    SecondaryRepairInfo SecondaryRecovery
             (int stack[],
              int stack_top,
              int last_index,
              SecondaryRepairInfo repair, bool stack_flag);
    void SecondaryDiagnosis(SecondaryRepairInfo repair);

 //   void RepairParse(TokenObject);

    PrimaryRepairInfo ScopeTrial(int stack[], int stack_top,
                                 PrimaryRepairInfo repair);
    void ScopeTrialCheck(int stack[], int stack_top,
                         PrimaryRepairInfo& repair, int indx);
    bool SecondaryCheck(int stack[], int stack_top,
                        int buffer_position, int distance);


	RecoveryScanner* recoveryScanner = nullptr;
	void reportPrimaryError(int msgCode, int nameIndex, int token, int scopeNameIndex);
	void reportSecondaryError(int msgCode, int nameIndex, int leftToken, int rightToken, int scopeNameIndex);
	void reportError(int msg_level, ParseErrorCode msgCode, int nameIndex, int leftToken, int rightToken, int scopeNameIndex = 0);
	std::vector<int>  getNTermTemplate(int sym);

};


} // Close namespace Jikes block


#endif // diagnose_INCLUDED

