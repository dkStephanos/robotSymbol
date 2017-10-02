#include "OGL2DTextDisplay.h"
#include "OGL2DTexture.h"

OGL2DTextDisplay::OGL2DTextDisplay() : OGLGameObject("OGL2DTextDisplay")
{
	this->texture = nullptr;
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
}

OGL2DTextDisplay::~OGL2DTextDisplay()
{
	this->clear();
	glDeleteVertexArrays(1, &this->vao);
}

void OGL2DTextDisplay::addText(const string& text, float x, float y)
{
	TextXY* textxy = new TextXY();
	textxy->text = text;
	textxy->x = x;
	textxy->y = y;
	this->texts.push_back(textxy);
}

void OGL2DTextDisplay::clear()
{ 
	for (size_t i = 0; i < this->texts.size(); i++) {
		delete this->texts[i];
	}
	this->texts.clear(); 
}

void OGL2DTextDisplay::render()
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
		for (size_t i = 0; i < this->texts.size(); i++) {
			this->renderText(this->texts[i]->text, this->texts[i]->x, this->texts[i]->y);
		}

		glUseProgram(0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);
	}
}

void OGL2DTextDisplay::renderText(const string& text, float x, float y)
{
	size_t len = text.length();
	float cx = x;
	for (size_t i = 0; i < len; i++) {
		this->renderChar(text[i], cx, y);
		cx += 0.02f;
	}
}

void OGL2DTextDisplay::renderChar(char ch, float x, float y)
{
	GlyphPos pos;
	pos.lx = x;
	pos.ty = y;
	pos.rx = pos.lx + .025f;
	pos.by = pos.ty - .035f;
	GlyphPos tex = this->getTexPosition(ch);
	GLfloat box[4][4] = {
		{ pos.lx, pos.by, tex.lx, tex.by },
		{ pos.rx, pos.by, tex.rx, tex.by },
		{ pos.lx, pos.ty, tex.lx, tex.ty },
		{ pos.rx, pos.ty, tex.rx, tex.ty },
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

GlyphPos OGL2DTextDisplay::getTexPosition(char ch)
{
	float widthPercent = 1 / 10.0f;
	float dx = widthPercent;
	float heightPercent = 1 / 9.0f;
	float dy = heightPercent;
	GlyphPos tex;
	tex.ty = 0.0f;
	tex.by = 0.0f;
	tex.lx = 0.0f;
	tex.rx = 0.0f;

	int offset;
	if (ch >= 'A' && ch <= 'Z') {
		tex.ty = 1.0f;
		tex.by = tex.ty - dy;
		if (ch <= 'J') {
			offset = 10 - ((int)('J' - ch) + 1);
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
		else if (ch <= 'T') {
			tex.ty -= dy;
			tex.by = tex.ty - dy;
			offset = 10 - ((int)('T' - ch) + 1);
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
		else {
			tex.ty -= (dy * 2.0f);
			tex.by = tex.ty - dy;
			offset = 6 - ((int)('Z' - ch) + 1);
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
	}
	else if (ch >= 'a' && ch <= 'z') {
		tex.ty = 1.0;
		tex.by = tex.ty - dy;
		if (ch <= 'd') {
			tex.ty -= (dy * 2.0f);
			tex.by = tex.ty - dy;
			offset = 6 + (4 - ((int)('d' - ch) + 1));
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
		else if (ch <= 'n') {
			tex.ty -= (dy * 3.0f);
			tex.by = tex.ty - dy;
			offset = 10 - ((int)('n' - ch) + 1);
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
		else if (ch <= 'x') {
			tex.ty -= (dy * 4.0f);
			tex.by = tex.ty - dy;
			offset = 10 - ((int)('x' - ch) + 1);
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
		else {
			tex.ty -= (dy * 5.0f);
			tex.by = tex.ty - dy;
			offset = 2 - ((int)('z' - ch) + 1);
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
	}
	else if (ch >= '0' && ch <= '9') {
		tex.ty = 1.0;
		tex.by = tex.ty - dy;
		if (ch <= '7') {
			tex.ty -= (dy * 5.0f);
			tex.by = tex.ty - dy;
			offset = 2 + (8 - ((int)('7' - ch) + 1));
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
		else if (ch <= '9') {
			tex.ty -= (dy * 6.0f);
			tex.by = tex.ty - dy;
			offset = 2 - ((int)('9' - ch) + 1);
			tex.lx = offset * widthPercent;
			tex.rx = tex.lx + dx;
		}
	}
	return tex;
}
