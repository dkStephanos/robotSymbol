#include "OGL3DObject.h"
#include "Axis.h"

#include <glm\gtc\type_ptr.hpp>

OGL3DObject::OGL3DObject(const string& name, const string& uniformMatrixName) : 
	OGLObject(name)
{
	this->setUniformMatrixName(uniformMatrixName);
}

OGL3DObject::~OGL3DObject()
{
}

void OGL3DObject::render()
{
	this->render(this->referenceFrame);
}

void OGL3DObject::render(OGLReferenceFrame& frame)
{
	this->sendMatrixToGPU(frame.orientation);
	this->sendMaterialDataToGPU();
	this->renderBufferedObjects();
}

void OGL3DObject::sendMatrixToGPU(const glm::mat4& mat)
{
	auto location =
		glGetUniformLocation(
			this->shaderProgram, this->uniformMatrixName.c_str());

	glUseProgram(this->shaderProgram);
	glUniformMatrix4fv(
		location,
		1,
		GL_FALSE,
		glm::value_ptr(mat));
	glUseProgram(0);
}