#include "GenericWindow.h"
#include "Logger.h"

GenericWindow::GenericWindow()
{
	this->logger = NULL;
}

GenericWindow::~GenericWindow()
{
}

void GenericWindow::log(string text, bool debug)
{
	if (this->logger != NULL) {
		if (!debug) {
			this->logger->log(text);
		}
		else {
			this->logger->debug(text);
		}
	}
}

void GenericWindow::log(wstring text, bool debug)
{
	if (this->logger != NULL) {
		if (!debug) {
			this->logger->log(text);
		}
		else {
			this->logger->debug(text);
		}
	}
}
