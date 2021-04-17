
#ifndef IJavaElement_java_model_INCLUDED
#define IJavaElement_java_model_INCLUDED
#include  <string>
#include <vector>
using std::wstring;
namespace Jikes { // Open namespace Jikes block
	class IProgressMonitor;
	namespace PathModel
	{
		class MosesPath;
	}

	namespace JavaModel {
		class IJavaProject;
		enum IJavaElementType;
		class IOpenable;
		class JavaModel;
		class IJavaElement
		{
		public:
		
			IJavaElement(IJavaElementType _type) :fLEType(_type)
			{
				
			}
			virtual ~IJavaElement()
			{
				
			}
			IJavaElement(IJavaElement& o) :fLEType(o.fLEType)
			{

			}
			virtual bool equals(IJavaElement* o) = 0;
			virtual IJavaElement* DeepCopy(IJavaElement*) =0;

			virtual IJavaElement* ShallowCopy(IJavaElement* parent) = 0;

			virtual PathModel::MosesPath* getMosesPath()
			{
				return  nullptr;
			};
			
			virtual std::wstring getElementName() = 0;
			
			virtual wstring getFullName()
			{
				return wstring();
			}
		
			 virtual wstring getPath() = 0;
			

		
			
			virtual bool isSepareteFromParent() {
				return false;
			}
			virtual void GetIndendentInfoAndSepareteFromParent() = 0;
			
			virtual std::vector<IJavaElement*>* getChildren() = 0;


			virtual void  addChild(IJavaElement* child) = 0;

			

			virtual bool hasChildren() = 0;

			virtual std::vector<IJavaElement*>* getChildrenOfType(
				IJavaElementType _type) = 0;



		


			IJavaElement* getAncestor(int ancestorType) {
				return nullptr;
			}

			virtual void setParent(IJavaElement* p) = 0;

			virtual IOpenable* getOpenable() = 0;
			virtual IOpenable* getOpenableParent() = 0;

			virtual IJavaProject* getJavaProject() = 0;
			virtual IJavaElement* getParent() = 0;
			virtual JavaModel*   getJavaModel() = 0;

			virtual IJavaElementType getElementType()
			{
				return fLEType;
			}
			/**
			* This element's type - one of the constants defined
			* in IJavaLanguageElementTypes.
			*/
			IJavaElementType fLEType;
		

		};

	}

} // Close namespace Jikes block


#endif // _INCLUDED

