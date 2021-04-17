// $Id: JCDT_Lib/internal/env/JavaArgumentPathUtil.h,v 1.57 2004/04/04 19:45:48 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef JavaArgumentPath_INCLUDED
#define JavaArgumentPath_INCLUDED
#include <string>
#include "JCDT_Lib/internal/env/platform.h"
#include <set>


namespace Jikes {
	class JikesClassPaths;
	// Open namespace Jikes block


class Option;

//
// This class represents the control information common across all compilation
// units.  It provides a cache for essential classes and objects, as well as
// the command line options in force.
//
class JavaArgumentPathUtil : public StringConstant
{
public:
   
	JavaArgumentPathUtil(Option*);
	~JavaArgumentPathUtil();

	void GetClassPaths(JikesClassPaths&);

private:

	Option* option;
	void PreProcessExtDirs(char* path_to_be_deal, JikesClassPaths&);
	void PreProcessPath(char* path_to_be_deal, bool source_path_ony, JikesClassPaths&);

  
   
};

} // Close namespace Jikes block


#endif // control_INCLUDED

