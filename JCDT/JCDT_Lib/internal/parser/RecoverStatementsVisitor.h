// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef RecoverStatementsVisitor_INCLUDED
#define RecoverStatementsVisitor_INCLUDED
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include "parser.h"


namespace Jikes { // Open namespace Jikes block

	class MethodVisitor :public ASTVisitor
	{
	public:
		Parser* pParser;

		virtual ~MethodVisitor() {
			delete[] types;
		}
		ASTVisitor* typeVisitor;

		AstClassBody* enclosingTypeClassBody; // used only for initializer

		TypeDeclaration** types;
		int types_length = 0;
		int typePtr = -1;

		void endVisit(ConstructorDeclaration* constructorDeclaration , AstNodeScope* scope)
		{
			vector<TypeDeclaration*> foundTypes;
			int length = 0;
			if (typePtr > -1) {
				length = typePtr + 1;
				foundTypes.assign(types, types + length);
			}

			ReferenceContext* oldContext = pParser->referenceContext;
			pParser->parse(constructorDeclaration, constructorDeclaration);
			pParser->referenceContext = oldContext;
			for (int i = 0; i < length; i++) {
				foundTypes[i]->traverse(typeVisitor, scope);
			}
		}

		void endVisit(Initializer* initializer, AstNodeScope* scope)
		{
			vector<TypeDeclaration*> foundTypes;
			int length = 0;
			if (typePtr > -1) {
				length = typePtr + 1;
				foundTypes.assign(types, types + length);
			}

			ReferenceContext* oldContext = pParser->referenceContext;
			pParser->parse(enclosingTypeClassBody, initializer);
			pParser->referenceContext = oldContext;
			for (int i = 0; i < length; i++) {
				foundTypes[i]->traverse(typeVisitor, scope);
			}
		}

		void endVisit(MethodDeclaration* methodDeclaration , AstNodeScope* scope)
		{
			vector<TypeDeclaration*> foundTypes;
			int length = 0;
			if (typePtr > -1) {
				length = typePtr + 1;
				foundTypes.assign(types, types + length);
			}

			ReferenceContext* oldContext = pParser->referenceContext;
			pParser->parse(methodDeclaration, methodDeclaration);
			pParser->referenceContext = oldContext;
			for (int i = 0; i < length; i++) {
				foundTypes[i]->traverse(typeVisitor, scope);
			}
		}


		bool visit(ConstructorDeclaration* constructorDeclaration, Ast* parent)
		{
			typePtr = -1;
			return true;
		}

		bool visit(Initializer* initializer, Ast* parent)
		{
			typePtr = -1;
			return true;
		}

		bool visit(MethodDeclaration* methodDeclaration, Ast* parent)
		{
			typePtr = -1;
			return true;
		}

		bool visit(TypeDeclaration* typeDeclaration, Ast* parent)
		{
			if (types_length <= ++typePtr) {
				int length = typePtr;
				//System.arraycopy(types, 0, types = new TypeDeclaration[length * 2 + 1], 0, length);
				types_length = length * 2 + 1;
				TypeDeclaration** temp = new TypeDeclaration*[types_length];
				::memcpy(temp, types, length);
				delete[] types;
				types = temp;
			}
			types[typePtr] = typeDeclaration;
			return false;
		}



	};
	class TypeVisitor :public ASTVisitor
	{
	public:

		virtual ~TypeVisitor() {
			delete[] types;
		}
		MethodVisitor* methodVisitor;

		TypeDeclaration** types = NULL;
		int types_length = 0;
		int typePtr = -1;

		void endVisit(TypeDeclaration* typeDeclaration, Ast* parent)
		{
			endVisitType();
		}



		void endVisitType()
		{
			typePtr--;
		}

		bool visit(ConstructorDeclaration* constructorDeclaration, AstNodeScope* scope)
		{
			constructorDeclaration->traverse(methodVisitor, scope);
			return false;
		}

		bool visit(Initializer* initializer, AstNodeScope* scope)
		{
			methodVisitor->enclosingTypeClassBody = types[typePtr]->class_body;
			initializer->traverse(methodVisitor, scope);
			return false;
		}

		bool visit(MethodDeclaration* methodDeclaration, AstNodeScope* scope)
		{
			methodDeclaration->traverse(methodVisitor, scope);
			return false;
		}

		bool visit(TypeDeclaration* typeDeclaration, Ast* parent)
		{
			if (types_length <= ++typePtr) {
				int length = typePtr;
				//System.arraycopy(types, 0, types = new TypeDeclaration[length * 2 + 1], 0, length);
				types_length = length * 2 + 1;
				TypeDeclaration** temp = new TypeDeclaration*[types_length];
				::memcpy(temp, types, length);
				delete[] types;
				types = temp;
			}
			types[typePtr] = typeDeclaration;
			return true;
		}


	};



	

} // Close namespace Jikes block


#endif // _INCLUDED

