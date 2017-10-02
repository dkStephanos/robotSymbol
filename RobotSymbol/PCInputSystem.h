#pragma once
#ifndef PC_INPUT_SYSTEM
#define PC_INPUT_SYSTEM

/**
  Models a Person Computer input system with 256 keys
*/
class PCInputSystem
{
private:
	unsigned short oldMouseX;
	unsigned short oldMouseY;
	short dirX;
	short dirY;

protected:
	unsigned short mouseX;
	unsigned short mouseY;

public:
   // Assumes the number of keys
   const static int NUM_KEYS = 256;
   // Set to true or false depending whether the key is pressed or not
   bool keys[NUM_KEYS];

public:
   PCInputSystem(void);

   ~PCInputSystem(void);

   void resetKeys();

   void resetMouse();

   void setMousePosition(unsigned short mouseX, unsigned short mouseY);

   bool isMouseMovingLeft() { return this->dirX < 0; }

   bool isMouseMovingRight() { return this->dirX > 0; }

   bool isMouseMovingUp() { return this->dirY < 0; }

   bool isMouseMovingDown() { return this->dirY > 0; }
};

#endif

