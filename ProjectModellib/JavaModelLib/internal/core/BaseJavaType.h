
#ifndef BaseJavaType_java_model_INCLUDED_INCLUDED
#define BaseJavaType_java_model_INCLUDED_INCLUDED
#include "IType.h"
#include <vector>
#include <JCDT_Lib/internal/impl/access.h>
#include <JavaModelLib/internal/info/SourceTypeElementInfo.h>

using std::vector;
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		class Initializer;
		class BaseJavaType :public IType{
		public:

			 IType* DeepCopyButNoChildren(IJavaElement* parent)override;
			PathModel::MosesPath* getMosesPath() override;
			IJavaElement* DeepCopy(IJavaElement*) override;
			wstring getFullName() override;

			bool hasChildren() override;
			std::vector<IJavaElement*>* getChildren() override;
		protected:
			enum
			{
				ANONYMOUS = 0x0001,
				DEPRECATED = 0x0002,
				ENUM_TYPE = 0x0004, // can't use ACC_ENUM on types :(
				INTERFACE_TYPE = 0x0008,
				CALSS_TYPE = 0x0010,
				PRIMITIV_TYPE = 0x0020,
				ARRAY_TYPE = 0x0040
			};


		public:
			BaseJavaType(IJavaElement* parent, const wstring& name);
			BaseJavaType(BaseJavaType& o);
			virtual ~BaseJavaType();
			void SortOutLineNameIdentity() override;
			void GetIndendentInfoAndSepareteFromParent() override;
			void   addChild(IJavaElement* child) override;


			std::vector<IJavaElement*>* getChildrenOfType(
				IJavaElementType _type) override;

	

			const std::vector<IType*>* getTypes()override;

			IMethod* getMethod(const wstring& name,
				const vector<wstring>& parameterTypes)override;

			IField* getField(const wstring& name) override;

			const std::vector<IField*>* getFields() override;



			const vector<IMethod*>* getMethods() override;

			IType* getType(const wstring& name) override;



			void MarkAnonymous() { status |= ANONYMOUS; }
			bool Anonymous() const { return (status & ANONYMOUS) != 0; }


			void MarkDeprecated() { status |= DEPRECATED; }
			void ResetDeprecated() { status &= ~DEPRECATED; }
			bool IsDeprecated() const { return (status & DEPRECATED) != 0; }

			void MarkEnum() { status |= ENUM_TYPE; }
			void ResetEnum() { status &= ~ENUM_TYPE; }
			bool IsEnum() const { return (status & ENUM_TYPE) != 0; }


			bool IsPrimitiveType() const
			{
				return (status & PRIMITIV_TYPE) != 0;
			}
			void MarkPrimitiveType() { status |= PRIMITIV_TYPE; }
			void ResetPrimitiveType() { status &= ~PRIMITIV_TYPE; }

			bool IsArrayType() const {
				return (status & ARRAY_TYPE) != 0;
			}
			void MarkIsArrayType() { status |= ARRAY_TYPE; }
			void ResetIsArrayType() { status &= ~ARRAY_TYPE; }





			bool isClass() override {
				return (status & CALSS_TYPE) != 0;
			};
			void MarkClass()
			{
				status |= CALSS_TYPE;
			}
			void ResetClass()
			{
				status &= ~CALSS_TYPE;
			}

			void MarkInterface()
			{
				status |= INTERFACE_TYPE;
			}
			void ResetInterface()
			{
				status &= ~INTERFACE_TYPE;
			}
			bool IsInterface() const
			{
				return (status & INTERFACE_TYPE) != 0;
			}



			unsigned short status;

			BaseJavaFile* file_symbol;
		
			const vector<IType*>* getNestTypes() {
				auto _info = reinterpret_cast<SourceTypeElementInfo*>( getElementInfo());
				return  _info->nested_types;
			}
			SourceTypeElementInfo* info;
			MemberElementInfo* getElementInfo() override {
				if (!info) {
					info = new SourceTypeElementInfo();
				}
				return info;
			};

		protected:
			void HelpToDeepCopy(BaseJavaType& o);
			void HelpToDeepCopyButNoChildren(BaseJavaType& o);


		public:
			IInitializer* getInitializer(int start_pos) override;
			std::wstring getJavaCardDocUrlFragment() override;
		};

	}// Close namespace JavaModel block


} // Close namespace Jikes block


#endif // _INCLUDED

