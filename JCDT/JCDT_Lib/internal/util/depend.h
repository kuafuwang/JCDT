// $Id: JCDT_Lib/internal/util/depend.h,v 1.15 2002/06/18 04:06:55 cabbey Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef depend_INCLUDED
#define depend_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"


namespace Jikes { // Open namespace Jikes block


class Semantic;
class TypeSymbol;
class FileSymbol;
class AstClassBody;
class ConstructorDeclaration;
class SymbolSet;
class LookupEnvironment;

template <typename T>
class CycleChecker
{
public:
    enum { OMEGA = -1, CYCLE_INFINITY = INT_MAX };

    inline int Min(int x, int y) { return (x < y ? x : y); }

protected:
    class Stack
    {
    public:
        void Push(T *t) { info.Next() = t; }
        void Pop() { if (info.Length() > 0) info.Reset(info.Length() - 1); }
        int Size() { return info.Length(); }
        T *Top()
        {
            return (T *) (info.Length() > 0 ? info[info.Length() - 1] : NULL);
        }
    private:
        Tuple<T *> info;
    } stack;
};

class TypeCycleChecker : public CycleChecker<TypeSymbol>
{
public:

    TypeCycleChecker(vector<TypeSymbol *> &type_list_) : type_list(type_list_)
    {}
    void PartialOrder(vector<Semantic *> &, int);
    void PartialOrder(SymbolSet &);

private:
    vector<TypeSymbol *> &type_list;

    void ProcessSubtypes(TypeSymbol *);
    void ReverseTypeList();
};


class ConstructorCycleChecker : public CycleChecker<ConstructorDeclaration>
{
public:
    ConstructorCycleChecker(AstClassBody *);

private:
    void CheckConstructorCycles(ConstructorDeclaration *);
};


class TypeDependenceChecker : public CycleChecker<TypeSymbol>
{
public:
    TypeDependenceChecker(LookupEnvironment *control_,
                          SymbolSet &file_set_,
                          vector<TypeSymbol *> &type_trash_bin_) : file_set(file_set_),
                                                                  control(control_),
                                                                  type_trash_bin(type_trash_bin_)
    {}

    ~TypeDependenceChecker() {}

    void PartialOrder();
    void OutputDependences();

    Tuple<TypeSymbol *> &TypeList() { return type_list; }

    SymbolSet &file_set;

private:
    LookupEnvironment *control;
    vector<TypeSymbol *> &type_trash_bin;

    void OutputMake(FILE *, char *, Tuple<FileSymbol *> &);
    void OutputMake(FileSymbol *);

    Tuple<TypeSymbol *> type_list;

    void ProcessType(TypeSymbol *);
};


class TopologicalSort
{
public:
    TopologicalSort(SymbolSet &, vector<TypeSymbol *> &);
    ~TopologicalSort();

    void Sort();

private:
    void Process(TypeSymbol *);

    SymbolSet *pending;

    SymbolSet &type_collection;
    vector<TypeSymbol *> &type_list;
};


} // Close namespace Jikes block


#endif // depend_INCLUDED

