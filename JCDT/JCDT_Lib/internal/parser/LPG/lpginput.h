// $Id: lpginput.h,v 1.10 2004/03/25 13:32:28 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#ifndef lpginput_INCLUDED
#define lpginput_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"


namespace Jikes { // Open namespace Jikes block


typedef TokenIndex TokenObject;
typedef TokenIndex Location;

inline Location Loc(TokenObject i) { return i; }


} // Close namespace Jikes block


#include "JCDT_Lib/internal/parser/LPG/javasym.h" /* mapping of lexical symbols  */
#include "JCDT_Lib/internal/parser/LPG/javadef.h" /* definition of parsing names */
#include "javaprs.h" /* parsing action functions    */

#endif // lpginput_INCLUDED
