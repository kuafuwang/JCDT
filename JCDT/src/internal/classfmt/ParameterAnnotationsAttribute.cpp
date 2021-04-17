
#include <JCDT_Lib/internal/classfmt/ParameterAnnotationsAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block



	ParameterAnnotationsAttribute::ParameterAnnotationsAttribute(ClassFile& buffer,
		bool visible)
		: AttributeInfo((visible ? ATTRIBUTE_RuntimeVisibleParameterAnnotations
			: ATTRIBUTE_RuntimeInvisibleParameterAnnotations), buffer)
		, num_parameters(buffer.GetU1())
		, parameters(NULL)
	{
		unsigned length = 1 + 2 * num_parameters;
		// +1 num_parameters, +2 num_annotations * num_parameters
		if (num_parameters)
			parameters = new Tuple<Annotation*>[num_parameters];
		for (unsigned i = 0; i < num_parameters; i++)
		{
			unsigned count = buffer.GetU2();
			while (count--)
			{
				Annotation* value = new Annotation(buffer);
				parameters[i].Next() = value;
				length += value->Length();
			}
		}
		if (length != attribute_length)
			buffer.MarkInvalid("bad parameter annotations attribute length");
	}


	


	

	

} // Close namespace Jikes block



