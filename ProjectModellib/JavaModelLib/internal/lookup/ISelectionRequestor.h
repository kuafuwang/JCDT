
#ifndef ISelectionRequestor_java_model_INCLUDED_INCLUDED
#define ISelectionRequestor_java_model_INCLUDED_INCLUDED
#include <string>
using std::wstring;
#include <vector>
using std::vector;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IMethod;
	class IType;
	class IJavaElement;
	/**
	* A selection requestor accepts results from the selection engine.
	*/
	 class  ISelectionRequestor {
		/**
		* Code assist notification of a class selection.
		* @param packageName const wstring&
		* 		Declaring package name of the class.
		*
		* @param className const wstring&
		* 		Name of the class.
		*
		* @param needQualification bool
		* 		Flag indicating if the type name
		*    	must be qualified by its package name (depending on imports).
		*
		* NOTE - All package and type names are presented in their readable form:
		*    Package names are in the form "a.b.c".
		*    Nested type names are in the qualified form "A.M".
		*    The default package is represented by an empty array.
		*/
	 public:
		 enum PrimitiveType
		 {
			 _SHORT,
			 _INT,
			 _BYTE,
			 _CHAR,
			 _FLOAT,
			 _DOUBLE,
			 _LONG
		 };
		 virtual ~ISelectionRequestor()
		 {
			 
		 }
		virtual void  acceptClass(
			const wstring& packageName,
			const wstring& className,
			bool needQualification) = 0;

		

	
	
		/**
		* Code assist notification of a field selection.
		* @param declaringTypePackageName const wstring&
		* 		Name of the package in which the type that contains this field is declared.
		*
		* @param declaringTypeName const wstring&
		* 		Name of the type declaring this new field.
		*
		* @param name const wstring&
		* 		Name of the field.
		*
		* NOTE - All package and type names are presented in their readable form:
		*    Package names are in the form "a.b.c".
		*    Nested type names are in the qualified form "A.M".
		*    The default package is represented by an empty array.
		*/
		virtual void  acceptField(
			const wstring& declaringTypePackageName,
			const wstring& declaringTypeName,
			const wstring& name) = 0;

		/**
		* Code assist notification of an interface selection.
		* @param packageName const wstring&
		* 		Declaring package name of the interface.
		*
		* @param interfaceName const wstring&
		* 		Name of the interface.
		*
		* @param needQualification bool
		* 		Flag indicating if the type name
		*    	must be qualified by its package name (depending on imports).
		*
		* NOTE - All package and type names are presented in their readable form:
		*    Package names are in the form "a.b.c".
		*    Nested type names are in the qualified form "A.I".
		*    The default package is represented by an empty array.
		*/
		virtual void  acceptInterface(
			const wstring& packageName,
			const wstring& interfaceName,
			bool needQualification) = 0;

		/**
		* Code assist notification of a method selection.
		* @param declaringTypePackageName const wstring&
		* 		Name of the package in which the type that contains this new method is declared.
		*
		* @param declaringTypeName const wstring&
		* 		Name of the type declaring this new method.
		*
		* @param selector const wstring&
		* 		Name of the new method.
		*
		* @param parameterPackageNames vector<wstring>&
		* 		Names of the packages in which the parameter types are declared.
		*    	Should contain as many elements as parameterTypeNames.
		*
		* @param parameterTypeNames vector<wstring>&
		* 		Names of the parameters types.
		*    	Should contain as many elements as parameterPackageNames.
		*
		*  @param isConstructor bool
		* 		Answer if the method is a constructor.
		*
		* NOTE - All package and type names are presented in their readable form:
		*    Package names are in the form "a.b.c".
		*    Base types are in the form "int" or "bool".
		*    Array types are in the qualified form "M[]" or "int[]".
		*    Nested type names are in the qualified form "A.M".
		*    The default package is represented by an empty array.
		*/
		virtual void  acceptMethod(
			const wstring& declaringTypePackageName,
			const wstring& declaringTypeName,
			const wstring& selector,
			vector<wstring>& parameterPackageNames,
			vector<wstring>& parameterTypeNames,
			bool isConstructor) = 0;


		virtual vector<IMethod*> getMethod(const wstring& declaringTypePackageName,
			const wstring& declaringTypeName,
			const wstring& selector,
			vector<wstring>& parameterPackageNames,
			vector<wstring>& parameterTypeNames,
			bool isConstructor) = 0;


		virtual vector<IMethod*> getMethod(const wstring& declaringTypePackageName,const wstring& declaringTypeName,
			const wstring& selector,bool isConstructor) = 0;



		virtual	IType* getType(const wstring& packageName,
			const wstring& typeName, int acceptFlags) =0;
		/**
		* Code assist notification of a package selection.
		* @param packageName const wstring&
		* 		The package name.
		*
		* NOTE - All package names are presented in their readable form:
		*    Package names are in the form "a.b.c".
		*    The default package is represented by an empty array.
		*/
		virtual void  acceptPackage(const wstring& packageName) = 0;

		virtual unsigned  acceptCount() = 0;

		
		virtual void recordMatchSelectElement(IJavaElement*) = 0;

		/**
		* Resolve the type, adding to the resolved elements.
		*/
		virtual void acceptType(const wstring& packageName,
			const wstring& typeName, bool needQualification) = 0;

	};
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

