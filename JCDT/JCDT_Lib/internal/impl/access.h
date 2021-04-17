// $Id: JCDT_Lib/internal/impl/access.h,v 1.24 2004/02/23 13:47:11 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef access_INCLUDED
#define access_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include <vector>
#include <string>


namespace Jikes { // Open namespace Jikes block


class AccessFlags
{
public:
	u2 access_flags;
    enum AccessFlagsType
    {
        ACCESS_PUBLIC = 0x0001,
        ACCESS_PRIVATE = 0x0002,
        ACCESS_PROTECTED = 0x0004,
        ACCESS_STATIC = 0x0008,
        ACCESS_FINAL = 0x0010,
        ACCESS_SYNCHRONIZED = 0x0020,
        ACCESS_SUPER = 0x0020,
        ACCESS_VOLATILE = 0x0040,
        ACCESS_BRIDGE = 0x0040, // JSR 14
        ACCESS_TRANSIENT = 0x0080,
        ACCESS_VARARGS = 0x0080, // JSR 201
        ACCESS_NATIVE = 0x0100,
        ACCESS_ENUM = 0x0100, // JSR 201
        ACCESS_INTERFACE = 0x0200,
        ACCESS_ABSTRACT = 0x0400,
        ACCESS_STRICTFP = 0x0800,
        ACCESS_SYNTHETIC = 0x1000, // JSR 201
        ACCESS_ANNOTATION = 0x2000, // JSR 175
        ACCESS_ACCESS = ACCESS_PUBLIC | ACCESS_PROTECTED | ACCESS_PRIVATE
    };

    AccessFlags(u2 flags = 0) : access_flags(flags) {}

    bool IsSet(u2 flag) const { return (access_flags & flag) != 0; }
    bool ACC_PUBLIC() const { return IsSet(ACCESS_PUBLIC); }
    bool ACC_PRIVATE() const { return IsSet(ACCESS_PRIVATE); }
    bool ACC_PROTECTED() const { return IsSet(ACCESS_PROTECTED); }
    bool ACC_STATIC() const { return IsSet(ACCESS_STATIC); }
    bool ACC_FINAL() const { return IsSet(ACCESS_FINAL); }
    bool ACC_SYNCHRONIZED() const { return IsSet(ACCESS_SYNCHRONIZED); }
    bool ACC_SUPER() const { return IsSet(ACCESS_SUPER); }
    bool ACC_VOLATILE() const { return IsSet(ACCESS_VOLATILE); }
    bool ACC_BRIDGE() const { return IsSet(ACCESS_BRIDGE); }
    bool ACC_TRANSIENT() const { return IsSet(ACCESS_TRANSIENT); }
    bool ACC_VARARGS() const { return IsSet(ACCESS_VARARGS); }
    bool ACC_NATIVE() const { return IsSet(ACCESS_NATIVE); }
    bool ACC_ENUM() const { return IsSet(ACCESS_ENUM); }
    bool ACC_INTERFACE() const { return IsSet(ACCESS_INTERFACE); }
    bool ACC_ABSTRACT() const { return IsSet(ACCESS_ABSTRACT); }
    bool ACC_STRICTFP() const { return IsSet(ACCESS_STRICTFP); }
    bool ACC_SYNTHETIC() const { return IsSet(ACCESS_SYNTHETIC); }
    bool ACC_ANNOTATION() const { return IsSet(ACCESS_ANNOTATION); }

    void SetFlags(u2 flag) { access_flags |= flag; }
    void SetFlags(const AccessFlags& af)  { access_flags = af.access_flags; }
    void SetACC_PUBLIC() { SetFlags(ACCESS_PUBLIC); }
    void SetACC_PRIVATE() { SetFlags(ACCESS_PRIVATE); }
    void SetACC_PROTECTED() { SetFlags(ACCESS_PROTECTED); }
    void SetACC_STATIC() { SetFlags(ACCESS_STATIC); }
    void SetACC_FINAL() { SetFlags(ACCESS_FINAL); }
    void SetACC_SYNCHRONIZED() { SetFlags(ACCESS_SYNCHRONIZED); }
    void SetACC_SUPER() { SetFlags(ACCESS_SUPER); }
    void SetACC_VOLATILE() { SetFlags(ACCESS_VOLATILE); }
    void SetACC_BRIDGE() { SetFlags(ACCESS_BRIDGE); }
    void SetACC_TRANSIENT() { SetFlags(ACCESS_TRANSIENT); }
    void SetACC_VARARGS() { SetFlags(ACCESS_VARARGS); }
    void SetACC_NATIVE() { SetFlags(ACCESS_NATIVE); }
    void SetACC_ENUM() { SetFlags(ACCESS_ENUM); }
    void SetACC_INTERFACE() { SetFlags(ACCESS_INTERFACE); }
    void SetACC_ABSTRACT() { SetFlags(ACCESS_ABSTRACT); }
    void SetACC_STRICTFP() { SetFlags(ACCESS_STRICTFP); }
    void SetACC_SYNTHETIC() { SetFlags(ACCESS_SYNTHETIC); }
    void SetACC_ANNOTATION() { SetFlags(ACCESS_ANNOTATION); }

    void ResetFlags(u2 flag = 0xffff) { access_flags &= ~ flag; }
    void ResetACC_PUBLIC() { ResetFlags(ACCESS_PUBLIC); }
    void ResetACC_PRIVATE() { ResetFlags(ACCESS_PRIVATE); }
    void ResetACC_PROTECTED() { ResetFlags(ACCESS_PROTECTED); }
    void ResetACC_STATIC() { ResetFlags(ACCESS_STATIC); }
    void ResetACC_FINAL() { ResetFlags(ACCESS_FINAL); }
    void ResetACC_SYNCHRONIZED() { ResetFlags(ACCESS_SYNCHRONIZED); }
    void ResetACC_SUPER() { ResetFlags(ACCESS_SUPER); }
    void ResetACC_VOLATILE() { ResetFlags(ACCESS_VOLATILE); }
    void ResetACC_BRIDGE() { ResetFlags(ACCESS_BRIDGE); }
    void ResetACC_TRANSIENT() { ResetFlags(ACCESS_TRANSIENT); }
    void ResetACC_VARARGS() { ResetFlags(ACCESS_VARARGS); }
    void ResetACC_NATIVE() { ResetFlags(ACCESS_NATIVE); }
    void ResetACC_ENUM() { ResetFlags(ACCESS_ENUM); }
    void ResetACC_INTERFACE() { ResetFlags(ACCESS_INTERFACE); }
    void ResetACC_ABSTRACT() { ResetFlags(ACCESS_ABSTRACT); }
    void ResetACC_STRICTFP() { ResetFlags(ACCESS_STRICTFP); }
    void ResetACC_SYNTHETIC() { ResetFlags(ACCESS_SYNTHETIC); }
    void ResetACC_ANNOTATION() { ResetFlags(ACCESS_ANNOTATION); }



    u2 Flags() const { return access_flags; }

    inline const wchar_t* AccessString() const
    {
        return (ACC_PUBLIC() ? StringConstant::US_public
                : ACC_PROTECTED() ? StringConstant::US_protected
                : ACC_PRIVATE() ? StringConstant::US_private
                : StringConstant::US_default);
    }

    inline bool LegalAccess()
    {
        u2 acc = access_flags & ACCESS_ACCESS;
        return (acc & - acc) == acc;
    }

    //
    // Return true if adding flag will not violate recommended ordering of
    // modifiers based on the modifiers already set.
    //
	bool RecommendedOrder(const u2 flag);

#ifdef JIKES_DEBUG
    enum AccessMetatype
    {
        ACCESS_TYPE,
        ACCESS_VARIABLE,
        ACCESS_METHOD
    };

	void Print(Ostream& os, AccessMetatype metatype) const;

	void GetModifers(std::vector<std::wstring>& modifiers) const;
#endif // JIKES_DEBUG
};

} // Close namespace Jikes block


#endif // access_INCLUDED

