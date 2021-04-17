
#ifndef IClasspathEntry_jikes_PathModel_INCLUDED_INCLUDED
#define IClasspathEntry_jikes_PathModel_INCLUDED_INCLUDED
#include <string>
namespace Jikes { // Open namespace Jikes block


	namespace PathModel {



class IClasspathEntry {

public:
	/**
	* Entry kind constant describing a classpath entry identifying a
	* library. A library is a folder or JAR containing package
	* fragments consisting of pre-compiled binaries.
	*/
	static const int CPE_LIBRARY = 1;

	/**
	* Entry kind constant describing a classpath entry identifying a
	* required project.
	*/
	static const int CPE_PROJECT = 2;

	/**
	* Entry kind constant describing a classpath entry identifying a
	* folder containing package fragments with source code
	* to be compiled.
	*/
	static const int CPE_SOURCE = 4;

	/**
	* Entry kind constant describing a classpath entry representing
	* a name classpath container.
	*
	*/
	static const int CPE_CONTAINER = 8;


	/**
	* Returns the kind of this classpath entry.
	*
	* @return one of:
	* <ul>
	* <li><code>CPE_SOURCE</code> - this entry describes a source root in
	its project
	* <li><code>CPE_LIBRARY</code> - this entry describes a folder or JAR
	containing binaries
	* <li><code>CPE_PROJECT</code> - this entry describes another project
	*
	* <li><code>CPE_VARIABLE</code> - this entry describes a project or library
	*  	indirectly via a classpath variable in the first segment of the path
	* *
	* <li><code>CPE_CONTAINER</code> - this entry describes set of entries
	*  	referenced indirectly via a classpath container
	* </ul>
	*/
	virtual  int getEntryKind() = 0;

	virtual ~IClasspathEntry(){}


};


	}// Close namespace PathModel block


} // Close namespace Jikes block


#endif // _INCLUDED
