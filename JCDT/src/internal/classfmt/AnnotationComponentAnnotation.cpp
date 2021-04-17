
#include <JCDT_Lib/internal/classfmt/AnnotationComponentAnnotation.inl>
#include <JCDT_Lib/internal/classfmt/Annotation.inl>

namespace Jikes { // Open namespace Jikes block



	AnnotationComponentAnnotation::AnnotationComponentAnnotation(ClassFile& buffer)
		: AnnotationComponentValue(COMPONENT_annotation)
	{
		attr_value = new Annotation(buffer);
	}

	AnnotationComponentAnnotation::~AnnotationComponentAnnotation()
	{
		delete attr_value;
	}

	u2 AnnotationComponentAnnotation::Length() const
	{
		return 1 + attr_value->Length(); // +1 tag
	}

	void AnnotationComponentAnnotation::Put(OutputBuffer& out) const
	{
		AnnotationComponentValue::Put(out);
		attr_value->Put(out);
	}

#ifdef JIKES_DEBUG
	void AnnotationComponentAnnotation::Print(Ostream& os,const ConstantPool& pool) 
	{
		attr_value->Print(os,pool);
	}
#endif // JIKES_DEBUG



} // Close namespace Jikes block



