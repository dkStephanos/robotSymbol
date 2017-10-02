#include "Win32Window.h"
#include "WindowsConsoleLogger.h"
#include "ITimer.h"

Win32Window::Win32Window(wstring title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;
	this->windowHandle = NULL;
	this->windowInstance = NULL;
	this->WndProcedure = WndProc;
	this->timeToExit = false;
}

Win32Window::~Win32Window()
{
}

bool Win32Window::create()
{
	this->windowInstance = GetModuleHandle(NULL);
	if (this->windowInstance == 0) {
		this->log("Could not get module handle!");
		return false;
	}

	WNDCLASS windowClass;
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC)this->WndProcedure;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = this->windowInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = this->title.c_str();

	if (!RegisterClass(&windowClass)) {
		this->log("Could not register the window class!");
		return false;
	}

	this->windowHandle = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		this->title.c_str(),
		this->title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, this->width, this->height,
		NULL,
		NULL,
		this->windowInstance,
		NULL
		);

	if (this->windowHandle == NULL) {
		this->log("Could not create the window!");
		return false;
	}

	return true;
}

void Win32Window::show()
{
	if (this->windowHandle != NULL) {
		ShowWindow(this->windowHandle, SW_MAXIMIZE);
		UpdateWindow(this->windowHandle);
	}
}

void Win32Window::listenForEvents(ITimer* timer)
{
	timer->startTiming();
	MSG msg;
	while (!this->timeToExit) {
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT) {
			this->exit();
		}
		else {
			this->runOneFrame(timer->getElapsedTimeInSeconds());
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	timer->stop();
}

LRESULT CALLBACK Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
