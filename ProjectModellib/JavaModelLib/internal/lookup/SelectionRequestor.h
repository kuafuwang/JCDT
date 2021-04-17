
#ifndef SelectionRequestor_java_model_INCLUDED_INCLUDED
#define SelectionRequestor_java_model_INCLUDED_INCLUDED
#include "ISelectionRequestor.h"
#include  <vector>
#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
using std::wstring;
using std::vector;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IType;
	class NameLookup;
	class IJavaElement;
	class ICompilationUnit;
	class LayerPosInJavaModel;
	class IMethod;
	class OriginalJavaProject;
	/**
	* Implementation of <code>ISelectionRequestor</code> to assist with
	* code resolve in a compilation unit. Translates names to elements.
	*/
	 class SelectionRequestor :public ISelectionRequestor
	 {
	 public:

		  void recordMatchSelectElement(IJavaElement*) override;
		  ~SelectionRequestor();
	
		
		 /**
		 * Creates a selection requestor that uses that given
		 * name lookup facility to resolve names.
		 *
		 * Fix for 1FVXGDK
		 */
		 SelectionRequestor(boost::shared_ptr<OriginalJavaProject>, LayerPosInJavaModel* _pos);


		
		 /**
		 * Resolve the class.
		 */
		 void acceptClass(const wstring& packageName, 
			 const wstring& className, bool needQualification) override;
	
		/**
		* Resolve the field.
		*/
		 void acceptField(const wstring& declaringTypePackageName,
			 const wstring& declaringTypeName, const wstring& name) override;
		/**
		* Resolve the interface
		*/
		 void acceptInterface(const wstring& packageName,
			 const wstring& interfaceName, bool needQualification) override;
		/**
		* Resolve the method
		*/

		 void acceptMethod(const wstring& declaringTypePackageName, 
			 const wstring& declaringTypeName,
			 const wstring& selector,
			  vector<wstring>& parameterPackageNames,
			  vector<wstring>& parameterTypeNames,
			 bool isConstructor) override;

		 vector<IMethod*> getMethod(const wstring& declaringTypePackageName,
			 const wstring& declaringTypeName,
			 const wstring& selector,
			 vector<wstring>& parameterPackageNames,
			 vector<wstring>& parameterTypeNames,
			 bool isConstructor) override;




		/**
		* Resolve the package
		*/
		 void acceptPackage(const wstring& packageName) override;
	
		 
		/**
		* Returns the resolved elements.
		*/
		const vector<IJavaElement*>& getElements() const
		{
			return fElements;
		}

		 unsigned  acceptCount() override
		{
		
			return fElements.size();
		}


		 void Clear()
		 {
			 fElements.clear();
		 }

		 /**
		 * Resolve the type, adding to the resolved elements.
		 */
		 void acceptType(const wstring& packageName,
		                 const wstring& typeName, bool needQualification);

		 IType* getType(const wstring& packageName,
			 const wstring& typeName, int acceptFlags) override;
	 private:


	/**
	* Resolve the type, adding to the resolved elements.
	*/
	void acceptType(const wstring& packageName,
		const wstring& typeName,
		int acceptFlags, bool needQualification);


	NameLookup* getNameLookUp();
	/**
	* The name lookup facility used to resolve packages
	*/
	NameLookup* fNameLookup = nullptr;
	LayerPosInJavaModel* pos;
	boost::shared_ptr<OriginalJavaProject> project;
	/**
	* Fix for 1FVXGDK
	*
	* The compilation unit we are resolving in
	*/
	ICompilationUnit* fCodeResolve = nullptr;


	/**
	* The collection of resolved elements.
	*/
	vector<IJavaElement*> fElements;

	vector<IMethod*> resolveMethod(IType* type,
		const wstring& selector,
		const vector<wstring>& parameterPackageNames,
		const  vector<wstring>& parameterTypeNames);

	
		 /**
		* Resolve the type
		*/
		 IType* resolveType(const wstring& packageName,
			 const wstring& typeName, int acceptFlags) ;

	 public:
		 vector<IMethod*> getMethod(const wstring& declaringTypePackageName, const wstring& declaringTypeName, const wstring& selector, bool isConstructor) override;
	 };

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

