
#ifndef ISourceReference_java_model_INCLUDED_INCLUDED
#define ISourceReference_java_model_INCLUDED_INCLUDED

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class ISourceRange;
	class ISourceReference
	{
	public:
		virtual ~ISourceReference()
		{
		}
		virtual  void figureRange() = 0;

		virtual ISourceRange* getSourceRange() = 0;
		virtual  void  setSourceRange(ISourceRange*) = 0;


		virtual ISourceRange* getNameRange() = 0;
		virtual  void  setNameRange(ISourceRange*) = 0;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

