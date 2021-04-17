// $Id: tab.h,v 1.11 2001/09/14 05:31:34 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef tab_INCLUDED
#define tab_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"


namespace Jikes { // Open namespace Jikes block


class Tab
{
public:
    enum { DEFAULT_TAB_SIZE = 4 };

    inline static int TabSize() { return tab_size; }
    inline static void SetTabSize(int value) { tab_size = value; }

    static int Wcslen(wchar_t *line, int start, int end);
	static int GetPos(wchar_t *line, int line_start, int line_end, int column);
private:
    static int tab_size;
};


} // Close namespace Jikes block


#endif // tab_INCLUDED

