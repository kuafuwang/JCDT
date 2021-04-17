/*******************************************************************************
 * Copyright (c) 2000, 2006 IBM Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     IBM Corporation - initial API and implementation
 *******************************************************************************/
 #ifndef ICompilationUnit_INCLUDED
#define ICompilationUnit_INCLUDED
#include <string>
#include <vector>
using std::vector;
using std::wstring;

#ifdef HAVE_JIKES_NAMESPACE
namespace Jikes { // Open namespace Jikes block
#endif
class IDependent;
/**
 * This interface denotes a compilation unit, providing its name and content.
 */
  ICompilationUnit : public IDependent {
/**
 * Answer the contents of the compilation unit.
 *
 * In normal use, the contents are requested twice.
 * Once during the initial lite parsing step, then again for the
 * more detailed parsing step.
 */
 virutal const wchar_t* getContents() = 0;
/**
 * Answer the name of the top level public type.
 * For example, {Hashtable}.
 */
  virutal wstring getMainTypeName() = 0;
/**
 * Answer the name of the package according to the directory structure
 * or null if package consistency checks should be ignored.
 * For example, {java, lang}.
 */
virutal void getPackageName(vector<wstring>& pkgs) = 0;
};
#ifdef HAVE_JIKES_NAMESPACE
} // Close namespace Jikes block
#endif

#endif // ICompilationUnit_INCLUDED
