// $Id: lpginput.cpp,v 1.10 2001/09/14 05:31:34 ericb Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 2000, 2001 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#include "JCDT_Lib/internal/parser/LPG/lpginput.h"
#include "JCDT_Lib/internal/parser/LPG/javadcl.h"

#include "JCDT_Lib/internal/parser/parser.h"

namespace Jikes { // Open namespace Jikes block


	const int  Parser::name_size = sizeof(javaprs_table::name_start) / sizeof(unsigned short);

	const int Parser::terminal_index_length = sizeof(javaprs_table::terminal_index) / sizeof(unsigned short);
	const int Parser::non_terminal_index_length = sizeof(javaprs_table::non_terminal_index) / sizeof(unsigned short);

	const int Parser::scope_rhs_length = sizeof(javaprs_table::scope_rhs) / sizeof(unsigned short);




} // Close namespace Jikes block



