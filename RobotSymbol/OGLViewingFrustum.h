#pragma once
#ifndef OGL_VIEWING_FRUSTUM
#define OGL_VIEWING_FRUSTUM

#include <gl\glew.h>
#include <glm\glm.hpp>

#include "ViewingFrustum.h"
#include <glutil\glutil.h>

class OGLViewingFrustum :
	public ViewingFrustum
{
public:
	

public:
	OGLViewingFrustum();
	virtual ~OGLViewingFrustum();

	glm::mat4 getMatrix();
};

#endif

