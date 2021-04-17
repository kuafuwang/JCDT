// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef ImportReference_INCLUDED
#define ImportReference_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block

	class ImportReference : public Ast
	{
	public:
		virtual ~ImportReference()
		{

		}

		ImportReference(AstKind k, AstTag t = NO_TAG);

		AstName* name;
		Token* semicolon_token;

		vector<wstring > getNames() const;

		wstring getImportName(wchar_t separator = '.') const;

		virtual bool onDemand() = 0;
	};
	

} // Close namespace Jikes block


#endif // ImportReference_INCLUDED

