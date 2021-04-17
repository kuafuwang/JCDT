// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "JCDT_Lib/internal/impl/OptionError.h"
#include "JCDT_Lib/internal/problem/ErrorString.h"


namespace Jikes { // Open namespace Jikes block



wstring OptionError::GetErrorMessage()
{
    ErrorString s;
    s << "Error: ";

    switch (kind)
    {
    case INVALID_OPTION:
        s << '\"' << name << "\" is an invalid option.";
        break;
    case MISSING_OPTION_ARGUMENT:
        s << '\"' << name << "\" requires an argument.";
        break;
    case INVALID_SOURCE_ARGUMENT:
        if (ENABLE_SOURCE_15)
        {
            s << "\"-source\" only recognizes Java releases 1.3 (JLS 2 "
              << "features), 1.4 (assert statement), and 1.5 (partial "
              << "support beta, see NEWS for supported features).";
            break;
        }
        s << "\"-source\" only recognizes Java releases 1.3 (JLS 2 features) "
          << "and 1.4 (assert statement).";
        break;
    case INVALID_TARGET_ARGUMENT:
        s << "\"-target\" only recognizes Java releases 1.1, 1.2, 1.3, 1.4, "
          << "and 1.4.2.";
        break;
    case INVALID_K_OPTION:
        s << "No argument specified for +K option. The proper form is "
          << "\"+Kxxx=xxx\" (with no intervening space).";
        break;
    case INVALID_K_TARGET:
        s << '\"' << name
          << "\" is not a valid target in a +K option. The target must be a "
          << "numeric type or boolean.";
        break;
    case INVALID_TAB_VALUE:
        s << '\"' << name
          << "\" is not a valid tab size. An integer value is expected.";
        break;
    case INVALID_P_ARGUMENT:
        s << '\"' << name
          << "\" is not a recognized flag for controlling pedantic warnings.";
        break;
    case INVALID_DIRECTORY:
        s << "The directory specified in the \"-d\" option, \"" << name
          << "\", is either invalid or it could not be expanded.";
        break;
    case INVALID_AT_FILE:
        s << "The @ file \"" << name
          << "\", is either invalid or it could not be located.";
        break;
    case NESTED_AT_FILE:
        s << "The @ file \"" << name
          << "\" must not reference another @ file.";
        break;
    case UNSUPPORTED_ENCODING:
        s << "Unsupported encoding: \"" << name << "\".";
        break;
    case UNSUPPORTED_OPTION:
        s << "The option \"" << name
          << "\" is unsupported in this build.";
        break;
    case DISABLED_OPTION:
        s << "The option \"" << name
          << "\" has been temporarily disabled.";
        break;
    default:
        assert(false && "invalid OptionError kind");
    }
    return s.ArrayString();
}
	



} // Close namespace Jikes block



