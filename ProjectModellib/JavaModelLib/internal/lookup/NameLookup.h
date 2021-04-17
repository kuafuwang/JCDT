
#ifndef JavaModelNameLookup_java_model_INCLUDED_INCLUDED
#define JavaModelNameLookup_java_model_INCLUDED_INCLUDED

#include <vector>
#include <map>
#include <string>
#include <JavaModelLib/internal/core/IPackageFragmentRoot.h>
#include <JCDT_Lib/internal/util/PerThreadObject.h>
#include <JavaModelLib/internal/core/BaseProject.h>
using std::map;
using std::vector;
using std::wstring;

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class PackageFragmentRoot;
	
	class IPackageFragment;
	class ICompilationUnit;
	class IType;
	class IJavaProject;
	class IJavaElementRequestor;
	class CloneJavaProject;
	class OriginalJavaProject;
	class LayerPosInJavaModel;
	/**
	* A <code>NameLookup</code> provides name resolution within a Java project.
	* The name lookup facility uses the project's classpath to prioritize the
	*
	* <p>Name lookup only returns a handle when the named element actually
	* exists in the model; otherwise <code>nullptr</code> is returned.
	*
	* <p>There are two logical sets of methods within this interface.  Methods
	* which start with <code>find*</code> are intended to be convenience methods for quickly
	* finding an element within another element, i.e. finding a class within a
	* do comprehensive searches of the <code>IJavaProject*</code> returning hits
	* in real time through an <code>IJavaElementRequestor*</code>.
	*
	*/
	class NameLookup {
	public:
		 static const wchar_t* fireware_root;
		 static const wchar_t* fireware_pkg;
		 static const wchar_t* fireware_file;
		IType* findPrimitiveType(const wstring& typeName);

		IType* findArrayType(IType* baseType, unsigned dimension);

		static	bool IsPrimitiveType(const wstring& typeName)
		{
			if (typeName == L"byte")
			{
				return true;
			}
			else if (typeName == L"char")
			{
				return true;
			}
			else if (typeName == L"short")
			{
				return true;
			}
			else if (typeName == L"int")
			{
				return true;
			}
			else if (typeName == L"long")
			{
				return true;
			}
			else if (typeName == L"float")
			{
				return true;
			}
			else if (typeName == L"double")
			{
				return true;
			}	
			else
			{
				return false;
			}
		}

		void beginToOccupy(){
			EnterCriticalSection(usage_reference);
			configureFromProject();
		}
		void endToOccupy(){

			LeaveCriticalSection(usage_reference);	
		}
		ICompilationUnit* GetCodeResoveUnit(LayerPosInJavaModel* _pos);

		CRITICAL_SECTION* usage_reference;// 
		OriginalJavaProject* owner;
		volatile unsigned new_version ;
		void UpdateNameLookupStamp() {
			::InterlockedIncrement(&new_version);
		}
		 unsigned cur_version ;
		map < wstring, 
			boost::shared_ptr<CloneJavaProject> > clone_projects;
		

		/**
		* Accept flag for specifying classes.
		*/
		 static const int ACCEPT_CLASSES = 0x00000002;

		/**
		* Accept flag for specifying interfaces.
		*/
		 static const int ACCEPT_INTERFACES = 0x00000004;

		/**
		* The <code>IPackageFragmentRoot</code>'s associated
		* with the classpath of this NameLookup facility's
		* project.
		*/
		 map< wstring, IPackageFragmentRoot*> fPackageFragmentRoots ;

		/**
		* Table that maps package names to lists of package fragments for
		* all package fragments in the package fragment roots known
		* by this name lookup facility. To allow > 1 package fragment
		* with the same name, values are arrays of package fragments
		* ordered as they appear on the classpath.
		*/
		 map< wstring, vector<IPackageFragment*> >  fPackageFragments;

		/**
		* The <code>IWorkspace</code> that this NameLookup
		* is configure within.
		*/
		// IWorkspace workspace;

	
		 NameLookup(OriginalJavaProject* project);
		 ~NameLookup();
		/**
		* Returns true if:<ul>
		*  <li>the given type is an existing class and the flag's <code>ACCEPT_CLASSES</code>
		*      bit is on
		*  <li>the given type is an existing interface and the <code>ACCEPT_INTERFACES</code>
		*      bit is on
		*  <li>neither the <code>ACCEPT_CLASSES</code> or <code>ACCEPT_INTERFACES</code>
		*      bit is on
		*  </ul>
		* Otherwise, false is returned.
		*/
		 bool acceptType(IType* type, int acceptFlags);

		/**
		* Configures this <code>NameLookup</code> based on the
		* info of the given <code>IJavaProject*</code>.
		*
		* @throws JavaModelException if the <code>IJavaProject*</code> has no classpath.
		*/
		


		/**
		* Finds every type in the project whose simple name matches
		* the prefix, informing the requestor of each hit. The requestor
		* is polled for cancellation at regular intervals.
		*
		* <p>The <code>partialMatch</code> argument indicates partial matches
		* should be considered.
		*/
		 void findAllTypes(const wstring& prefix, bool partialMatch, int acceptFlags,
			 IJavaElementRequestor* requestor);

		/**
		* Returns the <code>ICompilationUnit*</code> which defines the type
		* named <code>qualifiedTypeName</code>, or <code>null</code> if
		* none exists. The domain of the search is bounded by the classpath
		* of the <code>IJavaProject*</code> this <code>NameLookup</code> was
		* obtained from.
		* <p>
		* The name must be fully qualified (eg "java.lang.Object", "java.util.Hashtable$Entry")
		*/
		 ICompilationUnit* findCompilationUnit(const wstring& qualifiedTypeName);

		/**
		* Returns the package fragment whose path matches the given
		* (absolute) path, or <code>null</code> if none exist. The domain of
		* the search is bounded by the classpath of the <code>IJavaProject*</code>
		* this <code>NameLookup</code> was obtained from.
		* The path can be:
		* 	- internal to the workbench: "/Project/src"
		*  - external to the workbench: "c:/jdk/classes.zip/java/lang"
		*/
		 IPackageFragment* findPackageFragment(const wstring& path);

		/**
		* Returns the package fragments whose name matches the given
		* (qualified) name, or <code>null</code> if none exist.
		*
		* The name can be:
		*	- empty: ""
		*	- qualified: "pack.pack1.pack2"
		* @param partialMatch partial name matches qualify when <code>true</code>,
		*	only exact name matches qualify when <code>false</code>
		*/
		 vector<IPackageFragment*> findPackageFragments( wstring name, bool partialMatch);

		/**
		*
		*/
		 IType* findType(const wstring& typeName,  wstring packageName, bool partialMatch, int acceptFlags);
		/**
		* Returns all the package fragments found in the specified
		* package fragment roots. Make sure the returned fragments have the given
		* project as great parent. This ensures the name lookup will not refer to another
		* project (through jar package fragment roots)
		*/
		 vector<IPackageFragment*> getPackageFragmentsInRoots(vector<IPackageFragmentRoot*>& roots, IJavaProject* project);

		/**
		* Returns the first type in the given package whose name
		* matches the given (unqualified) name, or <code>null</code> if none
		* exist. Specifying a <code>null</code> package will result in no matches.
		* The domain of the search is bounded by the Java project from which
		* this name lookup was obtained.
		*
		* @param name the name of the type to find
		* @param pkg the package to search
		* @param partialMatch partial name matches qualify when <code>true</code>,
		*	only exact name matches qualify when <code>false</code>
		* @param acceptFlags a bit mask describing if classes, interfaces or both classes and interfaces
		* 	are desired results. If no flags are specified, all types are returned.
		*
		* @see #ACCEPT_CLASSES
		* @see #ACCEPT_INTERFACES
		*/
		 IType* findType(const wstring& name, IPackageFragment* pkg, bool partialMatch, int acceptFlags);




		/**
		* Returns the type specified by the qualified name, or <code>null</code>
		* if none exist. The domain of
		* the search is bounded by the Java project from which this name lookup was obtained.
		*
		* @param name the name of the type to find
		* @param partialMatch partial name matches qualify when <code>true</code>,
		*	only exact name matches qualify when <code>false</code>
		* @param acceptFlags a bit mask describing if classes, interfaces or both classes and interfaces
		* 	are desired results. If no flags are specified, all types are returned.
		*
		* @see #ACCEPT_CLASSES
		* @see #ACCEPT_INTERFACES
		*/
		 IType* findType(const wstring& name, bool partialMatch, int acceptFlags);

		/**
		* Returns true if the given element's name matches the
		* specified <code>searchName</code>, otherwise false.
		*
		* <p>The <code>partialMatch</code> argument indicates partial matches
		* should be considered.
		* NOTE: in partialMatch mode, the case will be ignored, and the searchName must already have
		*          been lowercased.
		*/
		static bool nameMatches(const wstring& searchName, IJavaElement* element, bool partialMatch);

		/**
		* Notifies the given requestor of all package fragments with the
		* given name. Checks the requestor at regular intervals to see if the
		* requestor has canceled. The domain of
		* the search is bounded by the <code>IJavaProject*</code>
		* this <code>NameLookup</code> was obtained from.
		*
		* @param partialMatch partial name matches qualify when <code>true</code>;
		*	only exact name matches qualify when <code>false</code>
		*/
		 void seekPackageFragments(const wstring& name, bool partialMatch, IJavaElementRequestor* requestor);

		/**
		* Notifies the given requestor of all types (classes and interfaces) in the
		* given package fragment with the given (unqualified) name.
		* Checks the requestor at regular intervals to see if the requestor
		* has canceled. If the given package fragment is <code>null</code>, all types in the
		* project whose simple name matches the given name are found.
		*
		* @param name The name to search
		* @param pkg The corresponding package fragment
		* @param partialMatch partial name matches qualify when <code>true</code>;
		*	only exact name matches qualify when <code>false</code>
		* @param acceptFlags a bit mask describing if classes, interfaces or both classes and interfaces
		* 	are desired results. If no flags are specified, all types are returned.
		* @param requestor The requestor that collects the result
		*
		* @see #ACCEPT_CLASSES
		* @see #ACCEPT_INTERFACES
		*/
		 void seekTypes(const wstring& name, IPackageFragment* pkg, bool partialMatch, 
			 int acceptFlags, IJavaElementRequestor* requestor);

		/**
		* Performs type search in a binary package.
		*/
		 void seekTypesInBinaryPackage(const wstring& name, IPackageFragment* pkg,
			bool partialMatch, int acceptFlags, IJavaElementRequestor* requestor);

		/**
		* Performs type search in a source package.
		*/
		 void seekTypesInSourcePackage(const wstring& name, IPackageFragment* pkg,
			bool partialMatch, int acceptFlags, IJavaElementRequestor* requestor);
	
		/**
		* Notifies the given requestor of all types (classes and interfaces) in the
		* given type with the given (possibly qualified) name. Checks
		* the requestor at regular intervals to see if the requestor
		* has canceled.
		*
		* @param partialMatch partial name matches qualify when <code>true</code>,
		*  only exact name matches qualify when <code>false</code>
		*/
		 void seekQualifiedMemberTypes(const wstring& qualifiedName, IType* type,
			bool partialMatch, IJavaElementRequestor* requestor, int acceptFlags);


		 IType* findArrayType(const wstring& packageName, const wstring& className, unsigned dimension);
	
	
		 private:
			 void configureFromProject();

};
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

