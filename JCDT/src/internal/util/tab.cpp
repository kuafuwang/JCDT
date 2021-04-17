// $Id: tab.cpp,v 1.9 2002/08/02 21:29:48 ericb Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "JCDT_Lib/internal/util/tab.h"



namespace Jikes { // Open namespace Jikes block


int Tab::tab_size = Tab::DEFAULT_TAB_SIZE;

//
// Compute the length of a wide character string segment after expanding tabs,
// and any non-printable ASCII characters in unicode expansion mode.
//
int Tab::Wcslen(wchar_t *line, int start, int end)
{
    bool expand = Coutput.ExpandWchar();
    for (int i = start--; i <= end; i++)
    {
        if (line[i] == U_HORIZONTAL_TAB)
        {
            int offset = (i - start) - 1;
            start -= ((tab_size - 1) - offset % tab_size);
        }
        else if (expand && (line[i] < U_SPACE || line[i] > 0xFF))
        {
            start -= 5;
            assert(line[i] != U_CARRIAGE_RETURN && line[i] != U_LINE_FEED);
        }
    }

    return (end - start);
}
 int Tab::GetPos(wchar_t *line, int line_start, int line_end, int column)
{
	int start = line_start;
	int _pos = line_start;
	bool expand = Coutput.ExpandWchar();
	for (int i = start--; i <= line_end; i++)
	{
		if (line[i] == U_HORIZONTAL_TAB)
		{
			int offset = (i - start) - 1;
			start -= ((tab_size - 1) - offset % tab_size);
		}
		else if (expand && (line[i] < U_SPACE || line[i] > 0xFF))
		{
			start -= 5;
			assert(line[i] != U_CARRIAGE_RETURN && line[i] != U_LINE_FEED);
		}

	
		int _colummn_candidate = (_pos - start);
		if (_colummn_candidate == column)
			return _pos;
		++_pos;

	}
	return 0;
}

} // Close namespace Jikes block


