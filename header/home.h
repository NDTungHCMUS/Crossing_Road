#pragma once
#include <iostream>
#include <raylib.h>
#include "screen.h"

class Home
{
private:
	Texture2D homeBackground;
	short mode;
public:
	Home();
	Screen update();
	void draw();
	~Home();
};