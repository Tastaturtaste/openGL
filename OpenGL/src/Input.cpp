#include "Input.h"


Input::Input()
{
	mouse_pos = std::make_pair<double, double>(0,0);
	keys.fill(false);
	buttons.fill(false);
}

Input::~Input() { }

bool Input::isKeyPressed(int key)
{
	return keys[key];
}

bool Input::isButtonPressed(int button)
{
	return buttons[button];
}

std::pair<double, double> Input::getMousePosition()
{
	return mouse_pos;
}


