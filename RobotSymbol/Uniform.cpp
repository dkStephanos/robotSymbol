#include "Uniform.h"

UniformDataType Uniform::getType(const string& typeName)
{
	UniformDataType type = Integer;
	if (typeName == "Float") type = Float;
	else if (typeName == "FloatArray") type = FloatArray;
	else if (typeName == "Matrix4") type = Matrix4;
	else if (typeName == "Vector3") type = Vector3;
	else if (typeName == "Vector3Array") type = Vector3Array;
	else if (typeName == "Vector4") type = Vector4;
	else if (typeName == "Sampler2D") type = Sampler2D;
	return type;
}