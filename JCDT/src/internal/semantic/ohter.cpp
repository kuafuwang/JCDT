// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include <JCDT_Lib/internal/semantic/semantic.h>
#include "JCDT_Lib/internal/ast/ast.h"
#include <JCDT_Lib/internal/parser/diagnose/diagnose.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/impl/option.h>


namespace Jikes { // Open namespace Jikes block
	void Semantic::TransferError(vector<JikesError*>& errer_owner){
		if (error) {
			error->TransferError(errer_owner);
		}
	}

void Jikes::Semantic::PrintMessages()
	{

		if (this != lookup_env.system_semantic)
		{
			if (lex_stream->NumErrorTokens() > 0)
			{
				lex_stream->PrintMessages();
				return_code = 1;
			}
			else if (lex_stream->NumWarnTokens() > 0)
				lex_stream->PrintMessages();

			if (!compilation_unit ||
				CompilationUnitDeclaration::BadCompilationUnitCast(compilation_unit))
			{
			}
		}

#ifdef FOR_CONSOLE
		if (error && error.size() > 0 &&
			error->PrintMessages() > return_code)
		{
			return_code = 1;
		}
#endif
		
		// Once we have processed the errors, reset the error object
}

	

} // Close namespace Jikes block



