#include "OGLReferenceFrame.h"
#include "MathUtil.h"

OGLReferenceFrame::OGLReferenceFrame() : orientation(1.0f)
{
}

OGLReferenceFrame::~OGLReferenceFrame()
{
}

void OGLReferenceFrame::setPosition(const glm::vec3& position)
{
	this->orientation[3] = glm::vec4(position, 1.0f);
}

void OGLReferenceFrame::setPosition(float x, float y, float z)
{
	this->orientation[3] = glm::vec4(x, y, z, 1.0f);
}

void OGLReferenceFrame::rotateZ(float degrees)
{
	float radians = DegToRad(degrees);

	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	glm::mat4 rotZMat(1.0f);
	rotZMat[0] = glm::vec4(cosTheta, sinTheta, 0.0f, 0.0f);
	rotZMat[1] = glm::vec4(-sinTheta, cosTheta, 0.0f, 0.0f);

	this->orientation *= rotZMat;
}

void OGLReferenceFrame::rotateWorldZ(float degrees)
{
	float radians = DegToRad(degrees);

	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	glm::mat4 rotZMat(1.0f);
	rotZMat[0] = glm::vec4(cosTheta, sinTheta, 0.0f, 0.0f);
	rotZMat[1] = glm::vec4(-sinTheta, cosTheta, 0.0f, 0.0f);

	this->orientation = rotZMat * this->orientation;
}

void OGLReferenceFrame::rotateY(float degrees)
{
	float radians = DegToRad(degrees);

	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	glm::mat4 rotYMat(1.0f);
	rotYMat[0] = glm::vec4(cosTheta, 0.0f, -sinTheta, 0.0f);
	rotYMat[2] = glm::vec4(sinTheta, 0.0f, cosTheta, 0.0f);

	this->orientation *= rotYMat;
}


void OGLReferenceFrame::rotateWorldY(float degrees)
{
	float radians = DegToRad(degrees);

	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	glm::mat4 rotYMat(1.0f);
	rotYMat[0] = glm::vec4(cosTheta, 0.0f, -sinTheta, 0.0f);
	rotYMat[2] = glm::vec4(sinTheta, 0.0f, cosTheta, 0.0f);

	this->orientation = rotYMat * this->orientation;
}

void OGLReferenceFrame::rotateX(float degrees)
{
	float radians = DegToRad(degrees);

	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	glm::mat4 rotXMat(1.0f);
	rotXMat[1] = glm::vec4(0.0f, cosTheta, sinTheta, 0.0f);
	rotXMat[2] = glm::vec4(0.0f, -sinTheta, cosTheta, 0.0f);

	this->orientation *= rotXMat;
}

void OGLReferenceFrame::rotateWorldX(float degrees)
{
	float radians = DegToRad(degrees);

	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	glm::mat4 rotXMat(1.0f);
	rotXMat[1] = glm::vec4(0.0f, cosTheta, sinTheta, 0.0f);
	rotXMat[2] = glm::vec4(0.0f, -sinTheta, cosTheta, 0.0f);

	this->orientation = rotXMat * this->orientation;
}

void OGLReferenceFrame::move(const glm::vec3& direction, float speed)
{
	glm::mat4 translateMat(0.0f);
	translateMat[3] = glm::vec4(direction * speed, 0.0f);

	this->orientation += translateMat;
}

void OGLReferenceFrame::move(const glm::vec3& velocity)
{
	glm::mat4 translateMat(0.0f);
	translateMat[3] = glm::vec4(velocity, 0.0f);

	this->orientation += translateMat;
}

void OGLReferenceFrame::moveForward(float speed)
{
	// The 3rd column is the z-axis
	this->move(glm::vec3(this->orientation[2]), speed);
}

void OGLReferenceFrame::moveBackward(float speed)
{
	// The 3rd column is the z-axis
	this->move(glm::vec3(-this->orientation[2]), speed);
}

void OGLReferenceFrame::translate(float dx, float dy, float dz)
{
	glm::mat4 translation(1.0f);
	translation[3].x = dx;
	translation[3].y = dy;
	translation[3].z = dz;

	this->orientation *= translation;
}

void OGLReferenceFrame::translateWorld(float dx, float dy, float dz)
{
	glm::mat4 translation(1.0f);
	translation[3].x = dx;
	translation[3].y = dy;
	translation[3].z = dz;

	this->orientation = translation * this->orientation;
}

void OGLReferenceFrame::applyTransforms()
{
	for (auto i = 0; i < this->transforms.size(); i++) {
		if (this->transforms[i].type == "TL") {
			this->translate(this->transforms[i].x, this->transforms[i].y, this->transforms[i].z);
		}
	}
}
