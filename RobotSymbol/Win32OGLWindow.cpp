#include "Win32OGLWindow.h"

#include <sstream>
using std::stringstream;

#include <GL\glew.h>

Win32OGLWindow *Win32OGLWindow::self;

Win32OGLWindow::Win32OGLWindow(wstring title, int width, int height):
	Win32Window(title, width, height)
{
	this->renderingContext = NULL;
	this->deviceContext = NULL;
	this->WndProcedure = Win32OGLWindow::WndProc;
	this->self = this;
	this->background.red = 0.0f;
	this->background.green = 0.0f;
	this->background.blue = 0.0f;
	this->background.alpha = 1.0f;
}

Win32OGLWindow::~Win32OGLWindow()
{
	if (this->deviceContext != NULL) {
		wglMakeCurrent(this->deviceContext, 0);
		wglDeleteContext(this->renderingContext);
		ReleaseDC(this->windowHandle, this->deviceContext);
	}
}

bool Win32OGLWindow::create()
{
	if (!Win32Window::create()) return false;

	this->deviceContext = GetDC(this->windowHandle);
	if (this->deviceContext == NULL) {
		this->log("Could not get the device context!");
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;

	int pixelFormat = ChoosePixelFormat(this->deviceContext, &pfd);
	if (pixelFormat == 0) {
		this->log("Could not choose the pixel format!");
		return false;
	}

	BOOL result = SetPixelFormat(this->deviceContext, pixelFormat, &pfd);
	if (!result) {
		this->log("Could not set the pixel format!");
		return false;
	}

	HGLRC tempOpenGLContext = wglCreateContext(this->deviceContext);
	wglMakeCurrent(this->deviceContext, tempOpenGLContext);

	if (glewInit() != GLEW_OK) {
		this->log("Could not initialize glew!");
		return false;
	}

	// Read the opengl version
	stringstream stringStream;
	stringStream << glGetString(GL_VERSION);
	getline(stringStream, this->oglVersion);

	this->log("OpenGL version: " + this->oglVersion);

	return true;
}

void Win32OGLWindow::runOneFrame(float elapsedSeconds)
{
	glViewport(0, 0, this->width, this->height);

	glClearColor(
		this->background.red, 
		this->background.green,
		this->background.blue,
		this->background.alpha
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SwapBuffers(this->deviceContext);
}

void Win32OGLWindow::updateWindowSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

LRESULT CALLBACK Win32OGLWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_SIZE:
		self->updateWindowSize(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	return Win32Window::WndProc(hWnd, message, wParam, lParam);
}



