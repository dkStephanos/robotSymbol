#pragma once
#ifndef OGL_2D_TEXT_DISPLAY
#define OGL_2D_TEXT_DISPLAY

#include "OGLGameObject.h"

#include <vector>
using std::vector;

#include <string>
using std::string;

struct TextXY {
	string text;
	float x, y;
};

struct GlyphPos {
	float lx, ty;
	float rx, by;
};

class OGL2DTexture;

class OGL2DTextDisplay :
	public OGLGameObject
{
protected:
	vector<TextXY*> texts;
	OGL2DTexture* texture;
	GLuint vbo;

public:
	OGL2DTextDisplay();
	virtual ~OGL2DTextDisplay();

	void addText(const string& text, float x, float y);

	void setTexture(OGL2DTexture* texture) { this->texture = texture; }

	void clear();

	void render();

	glm::vec3 getPosition() { return glm::vec3(0, 0, 0); }

protected:
	GlyphPos getTexPosition(char ch);

	void renderText(const string& text, float x, float y);

	void renderChar(char ch, float x, float y);
};
#endif