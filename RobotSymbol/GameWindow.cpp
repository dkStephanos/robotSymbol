#include "GameWindow.h"
#include "GraphicsSystem.h"
#include "GameWorld.h"
#include "ViewingFrustum.h"
#include "OGLSphericalCamera.h"
#include "GameEngine.h"
#include "PCInputSystem.h"
#include "Logger.h"

#include <Windowsx.h>
#include <GL\glew.h>

GameWindow *GameWindow::self;

GameWindow::GameWindow(wstring title, int width, int height):
	Win32OGLWindow(title, width, height)
{
	this->gameEngine = NULL;
	this->WndProcedure = GameWindow::WndProc;
	this->self = this;
}

GameWindow::~GameWindow()
{
}

void GameWindow::runOneFrame(float elapsedSeconds)
{
	GraphicsSystem *graphics = this->gameEngine->getGraphicsSystem();

	graphics->update(elapsedSeconds);

	graphics->clearViewPort(0, 0, this->width, this->height, this->background);
	graphics->render();

	SwapBuffers(this->deviceContext);
}

void GameWindow::updateWindowSize(int width, int height)
{
	Win32OGLWindow::updateWindowSize(width, height);
	GraphicsSystem *graphics = this->gameEngine->getGraphicsSystem();
	if (graphics != NULL) {
		graphics->getViewingFrustum()->setAspectRatio(this->getAspectRatio());
	}
}

LRESULT CALLBACK GameWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCInputSystem *inputSystem = (PCInputSystem *)self->gameEngine->getInputSystem();

	switch (message) {
	case WM_KEYDOWN:
	case WM_CHAR: // Needed for the proper repeat rate
		inputSystem->keys[wParam] = true;
		switch (wParam) {
		case VK_ESCAPE:
			self->log("Exiting!");
			self->exit();
			break;
		}
		return 0;
	case WM_KEYUP:
		inputSystem->keys[wParam] = false;
		return 0;
	case WM_MOUSEMOVE:
		inputSystem->setMousePosition(
			GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	default:
		//inputSystem->resetKeys();
		break;
	}

	return Win32OGLWindow::WndProc(hWnd, message, wParam, lParam);
}
