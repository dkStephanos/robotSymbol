#include "OGL2DHUD.h"
#include "OGL2DTexture.h"
#include "OGLShaderProgram.h"


OGL2DHUD::OGL2DHUD(const string& name, float x, float y) : OGLGameObject(name)
{
	this->texture = nullptr;
	this->originX = x;
	this->originY = y;
	this->flyingHealth = new OGL2DTextDisplay();
	this->soldierHealth = new OGL2DTextDisplay();
	this->tankHealth = new OGL2DTextDisplay();
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
}

OGL2DHUD::~OGL2DHUD()
{
	glDeleteVertexArrays(1, &this->vao);
}

void OGL2DHUD::render()
{
	if (this->texture) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(this->vao);
		glUseProgram(this->shaderProgram);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);

		this->texture->select();
		this->renderHUD(this->originX, this->originY);

		glUseProgram(0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);
	}
}

void OGL2DHUD::renderHUD(float x, float y)
{

	GlyphPos pos;
	pos.lx = x;
	pos.ty = y;
	pos.rx = pos.lx + 1.0f;
	pos.by = pos.ty - 1.0f;
	GlyphPos tex;
	tex.lx = -1.0f;
	tex.rx = 1.0f;
	tex.by = -1.0f;
	tex.ty = 1.0f;
	GLfloat box[4][4] = {
		{ pos.lx, pos.by, tex.lx, tex.by },
		{ pos.rx, pos.by, tex.rx, tex.by },
		{ pos.lx, pos.ty, tex.lx, tex.ty },
		{ pos.rx, pos.ty, tex.rx, tex.ty },
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	this->flyingHealth->render();
	this->soldierHealth->render();
	this->tankHealth->render();
}

void OGL2DHUD::setTextDisplayShader(OGLShaderProgram *shader)
{
	this->flyingHealth->setShaderProgram(shader->getHandle());
	this->soldierHealth->setShaderProgram(shader->getHandle());
	this->tankHealth->setShaderProgram(shader->getHandle());
}