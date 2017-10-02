#pragma once
#ifndef UNIFORM
#define UNIFORM

#include <string>
using std::string;

enum UniformDataType {
	Integer, Float, FloatArray, Matrix4, Vector3, Vector3Array, Vector4,
	Sampler2D
};

struct Uniform {
	unsigned int location;
	UniformDataType type;
	void *data;

	static UniformDataType getType(const string& typeName);
};

#endif
