// $Id: JCDT_Lib/compiler/jikesapi.h,v 1.19 2004/02/28 17:09:45 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef JikesOption_H_FLAG_
#define JikesOption_H_FLAG_


namespace Jikes { // Open namespace Jikes block


class JikesOption
{

public:

    char* bootclasspath; // Location of the libraries
    char* extdirs;       // Location of external drop-in jars
    char* classpath;     // Location of source and user class files
    char* sourcepath;    // Location of source files only
    char* directory;     // Target directory for output
    char* encoding;      // Character encoding name

    // Each of these fields is a boolean value
    // 0 if false, non-zero if true
    int nowrite;         // Don't generate output, useful with verbose
    int deprecation;     // Warn about deprecated code
    int optimize;        // Enable optimizations
    int verbose;         // Verbosely track compilation progress
    int depend;          // Require full dependency check
	bool old_classpath_search_order; // Use older classpath search order
	 
    int help;            // Display a usage help message
    int version;         // Display a version message

    enum DebugLevel
    {
        NONE = 0,
        SOURCE = 1,
        LINES = 2,
        VARS = 4
    };

    enum ReleaseLevel
    {
        UNKNOWN,
        SDK1_1,
        SDK1_2,
        SDK1_3,
        SDK1_4,
        SDK1_4_2,
        SDK1_5
    };

    // This field can be 0 through 7 to represent all debug level combinations.
    int g;               // Annotate class files with debugging information

    //
    // The JDK release number of the syntax rules to obey (for example,
    // assert was added in 1.4), as well as the VM level to target.
    //
    ReleaseLevel source;
    ReleaseLevel target;

    enum ToleranceLevel
    {
        NO_WARNINGS = 0,
        CAUTIONS_ARE_ERRORS = 1,
        WARNINGS_ARE_ERRORS = 2,
        DEFAULT = 4
    };

    ToleranceLevel tolerance;

    virtual ~JikesOption();
	void Copy(const JikesOption& rhs);
	void Clear();
protected:
    JikesOption();
};



} // Close namespace Jikes block


#endif // JikesOption_H_FLAG_
