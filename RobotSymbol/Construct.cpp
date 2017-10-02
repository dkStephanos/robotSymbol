#include "Construct.h"
#include "OGLTexturedFlatSurface.h"
#include "Axis.h"

Construct::Construct(const string& name) : OGL3DCompositeObject(name)
{
}

Construct::~Construct()
{
}

void Construct::setShaderProgram(GLuint shaderProgram)
{
}

void Construct::update(float elapsedSeconds)
{
	OGL3DCompositeObject::update(elapsedSeconds);
	auto it = this->surfaces.begin();
	while (it != this->surfaces.end()) {
		it->second->update(elapsedSeconds);
		it++;
	}
}

void Construct::render()
{
	this->frameStack.setBaseFrame(this->referenceFrame);
	auto it = this->surfaces.begin();
	while (it != this->surfaces.end()) {
		this->frameStack.push();
		{
			this->frameStack.applyTransformsFrom(it->second->referenceFrame);
			it->second->referenceFrame.orientation = this->frameStack.top().orientation;
			it->second->render();
			if (this->axis) {
				this->axis->render(this->frameStack.top());
			}
		}
		this->frameStack.pop();
		it++;
	}
}
