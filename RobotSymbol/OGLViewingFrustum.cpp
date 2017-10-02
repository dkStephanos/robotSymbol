#include "OGLViewingFrustum.h"



OGLViewingFrustum::OGLViewingFrustum()
{
}

OGLViewingFrustum::~OGLViewingFrustum()
{
}

glm::mat4 OGLViewingFrustum::getMatrix()
{
	glutil::MatrixStack persMatrix;
	persMatrix.Perspective(
		this->fieldOfView,
		this->aspectRatio,
		this->zNear,
		this->zFar);
	return persMatrix.Top();
}
