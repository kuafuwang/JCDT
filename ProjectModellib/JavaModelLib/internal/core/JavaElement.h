
#ifndef JavaElement_jikes_INCLUDED_INCLUDED
#define JavaElement_jikes_INCLUDED_INCLUDED
#include "IJavaElement.h"
#include "IJavaElementType.h"
namespace Jikes { // Open namespace Jikes block

	
	namespace JavaModel {

		class JavaElement : public IJavaElement
		{
		public:
			PathModel::MosesPath* moses;
			  wstring getPath() override
			{
				if (fParent)
					return fParent->getPath();
				else
					return{};
			}
			 void GetIndendentInfoAndSepareteFromParent() override
			{
				  fParent = nullptr;
			 }
			std::vector<IJavaElement*>* getChildren() override {
				return nullptr;
			}
			void addChild(IJavaElement* child) override {
				
			}
			bool hasChildren() override {
				return false;
			}
			std::vector<IJavaElement*>* getChildrenOfType(
				IJavaElementType _type) override {
				return nullptr;
			}
			 bool isSepareteFromParent() override {
				return fParent == nullptr;
			}
			JavaElement(IJavaElement* parent,IJavaElementType, const wstring&  name);
			 JavaElement(JavaElement& o);
			
			unsigned short fOccurrenceCount = 1;
			void setOccurrenceCount(int count) {
				fOccurrenceCount = count;
			}

		
			virtual bool equals(IJavaElement* o) override;

			/**
			* This element's name, or an empty <code>String</code> if this
			* element does not have a name.
			*/
			 wstring fName;

			
			 virtual std::wstring getElementName()override
			 {
				 return fName;
			 }

			
			
			virtual ~JavaElement();






	



			void setParent(IJavaElement* p) override
			{
				fParent = p;
			};
			 IOpenable* getOpenable() override
			{
				if (!fParent)
					return nullptr;
				return fParent->getOpenable();
			};
			 IOpenable* getOpenableParent() override
			{
				if (!fParent)
					return nullptr;
				return fParent->getOpenableParent();
			};

			IJavaProject* getJavaProject() override {
				if (!fParent)
					return nullptr;
				return fParent->getJavaProject();
			}

			IJavaElement* getParent() override
			{
				return fParent;
			};

			JavaModel* getJavaModel() override {
				if (!fParent)
					return nullptr;
				return fParent->getJavaModel();
			}

protected:
			
			IJavaElement* fParent;

		};

		
	}

} // Close namespace Jikes block


#endif // _INCLUDED

