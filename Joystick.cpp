/*
	joystick.h - Library for using a analog 2-axis joystick with select-button.
	Copyright (C) by Michael Kubina, January 25, 2019.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Arduino.h"
#include "Joystick.h"

/*
 *	Constructor
 */

Joystick::Joystick(byte horizontalPin, byte verticalPin, byte selectPin)
{
	_horizontalPin = horizontalPin;
	_verticalPin = verticalPin;
	_selectPin = selectPin;
	
	pinMode(_selectPin, INPUT_PULLUP);
};

/*
 *	Initializing the joystick
 */

void Joystick::initialize()
{
	// Get the joysticks resting position upon initialization
	_initialX = readRawX();
	_initialY = readRawY();
	
	_offsetX = _initialX - 511;
	_offsetY = _initialY - 511;
	
	_minimalX = _initialX;
	_maximalX = _initialX;
	
	_minimalY = _initialY;
	_maximalY = _initialY;
};

/*
 *	Calibrating the joystick
 */

void Joystick::calibrate()
{
	// Debug
	Serial.println("### INITIALIZED ###");
	
	Serial.print(" Initial X: ");
	Serial.print(_initialX);
	Serial.print(" Offset: ");
	Serial.print(_offsetX);
	
	Serial.println("");
	
	Serial.print(" Initial Y: ");
	Serial.print(_initialY);
	Serial.print(" Offset: ");
	Serial.print(_offsetY);
	
	Serial.println("");
	
	Serial.println("### MOVE JOYSTICK IN ALL DIRECTIONS AND PRESS BUTTON TO CONFIRM ###");
	
	if(!isCalibrated())
	{
		while(!isPressed())
		{		
			// Minimal X
			if(readRawX() < _minimalX)
			{
				_minimalX = readRawX();
			}
			
			// Maximum X
			if(readRawX() > _maximalX)
			{
				_maximalX = readRawX();
			}
			
			// Minimal Y
			if(readRawY() < _minimalY)
			{
				_minimalY = readRawY();
			}
			
			// Maximal Y
			if(readRawY() > _maximalY)
			{
				_maximalY = readRawY();
			}
		}
	}
	
	_isCalibrated = true;
	
	// Debug
	Serial.println("### CALIBRATED ###");
	
	Serial.print(" Minimal X: ");
	Serial.print(_minimalX);
	Serial.print(" Maximal X: ");
	Serial.print(_maximalX);
	
	Serial.println("");
	
	Serial.print(" Minimal Y: ");
	Serial.print(_minimalY);
	Serial.print(" Maximal Y: ");
	Serial.print(_maximalY);
	
	Serial.println("");
	
};

bool Joystick::isCalibrated()
{
	return _isCalibrated;
};

/*
 *	Adjusting the sensivity of the joystick from 0 (LOW) to 10 (HIGH)
 */

void Joystick::setSensivity(unsigned int sensivity)
{
	if(sensivity > 10)
	{
		_sensivity = 10;
	}
	else
	{
		_sensivity = 110-(sensivity*10);
	}
};

unsigned int Joystick::getSensivity()
{
	return _sensivity;
};

/*
 *	Read the absolute analog values of the joystick
 */

int Joystick::readRawX()
{
	return analogRead(_horizontalPin);
};

int Joystick::readRawY()
{
	return analogRead(_verticalPin);
};

/*
 *	Read a cleaned-up analog value of the joystick
 *	Range: 	-511 to +512
 *	Error:	+/- offset
 */

int Joystick::readCleanX()
{
	return readRawX() - 511 - _offsetX;
};

int Joystick::readCleanY()
{
	return readRawY() - 511 - _offsetY;
};

/*
 *	Read the relative analog values of the joystick in percent
 *	Range: -100 to +100
 */

int Joystick::readX()
{
	int cleanX = readCleanX();
	int x = 0;
	
	if(cleanX < 0)
	{
		float percentX = static_cast<float> (cleanX) / static_cast<float> (511+_offsetX) * 100;
		x = static_cast<int>(percentX);
	}
	
	if(cleanX > 0)
	{
		float percentX = static_cast<float> (cleanX) / static_cast<float> (511-_offsetX) * 100;
		x = static_cast<int>(percentX);
	}
	
	return x;
};

int Joystick::readY()
{
	int cleanY = readCleanY();
	int y = 0;
	
	if(cleanY < 0)
	{
		float percentY = static_cast<float> (cleanY) / static_cast<float> (511+_offsetY) * 100;
		y = static_cast<int>(percentY);
	}
	
	if(cleanY > 0)
	{
		float percentY = static_cast<float> (cleanY) / static_cast<float> (511-_offsetY) * 100;
		y = static_cast<int>(percentY);
	}
	
	return y;
};

/*
 *	Checks if the joystick is leaning into the specified direction
 */

bool Joystick::isLeft()
{
	int cleanX = readCleanX();
	int s = -_sensivity;
	
	if(cleanX < s)
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool Joystick::isRight()
{
	int cleanX = readCleanX();
	int s = _sensivity;
	
	if(cleanX > s)
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool Joystick::isUp()
{
	int cleanY = readCleanY();
	int s = -_sensivity;
	
	if(cleanY < s)
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool Joystick::isDown()
{
	int cleanY = readCleanY();
	int s = _sensivity;
	
	if(cleanY > s)
	{
		return true;
	}
	else
	{
		return false;
	}
};

/*
 *	Checks for the state of the select-button
 */

bool Joystick::isPressed()
{
	if(digitalRead(_selectPin) == LOW)
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool Joystick::isReleased()
{
	if(digitalRead(_selectPin) == HIGH)
	{
		return true;
	}
	else
	{
		return false;
	}
};