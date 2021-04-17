
#ifndef IndependentMoses_java_model_INCLUDED_INCLUDED
#define IndependentMoses_java_model_INCLUDED_INCLUDED
#include <PathModelLib/MosesPath/MosesPath.h>
#include <JavaModelLib/internal/info/SourceTypeElementInfo.h>
using namespace Jikes::PathModel;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class JavaElementInfo;
	class SourceFieldElementInfo;
	class SourceMethodElementInfo;
	class SourceTypeElementInfo;
	class IField;
	class IType;
	class BaseMethod;

	class IndependentMoses
	{
	public:
		IndependentMoses(){
		}

		virtual ~IndependentMoses(){
		}

		static void copyTypeElementInfoWithMosesPointHelp(
			BaseJavaType* parent,
			PathModel::MosesPath* moses_point, 
			SourceTypeElementInfo* info,
			bool isBinary);

		static void copyFieldElementInfoWithMosesPointHelp(
			IField* parent, 
			MosesPath* moses_point,
			SourceFieldElementInfo* info,
			bool isBinary);
		static void copyMethodElementInfoWithMosesPointHelp(
			BaseMethod* parent,
			MosesPath* moses_point,
			SourceMethodElementInfo* info, 
			bool isBinary,
			const vector<wstring>& formal_parameters);

	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

