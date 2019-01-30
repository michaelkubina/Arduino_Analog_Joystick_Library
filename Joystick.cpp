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
	
	_leftGap = _initialX;
	_rightGap = _initialX;
	
	_upperGap = _initialY;
	_lowerGap = _initialY;
};

/*
 *	Calibrating the joystick
 */

void Joystick::calibrate()
{
	// Requires the joystick to be moved to the limits in all directions
	// Confirm by pressing the select button
	
	int leftLimit = _initialX;
	int rightLimit = _initialX;
	int upperLimit = _initialY;
	int lowerLimit = _initialY;
	
	// set starting-time of calibration for timeout
	unsigned long calibrationStarted = millis();
	
	if(!isCalibrated())
	{
		while(!isPressed())
		{		
			// lowest x-value
			if(readRawX() < leftLimit)
			{
				leftLimit = readRawX();
			}
			
			// highest x-value
			if(readRawX() > rightLimit)
			{
				rightLimit = readRawX();
			}
			
			// lowest y-value
			if(readRawY() < upperLimit)
			{
				upperLimit = readRawY();
			}
			
			// highest y-value
			if(readRawY() > lowerLimit)
			{
				lowerLimit = readRawY();
			}
			
			// timeout after 15 seconds
			if(millis() > calibrationStarted + 15000)
			{
				return;
			}
		}
	}
	
	// calculate the gaps in all directions
	_leftGap = leftLimit;
	_rightGap = 1023 - rightLimit;
	_upperGap = upperLimit;
	_lowerGap = 1023 - lowerLimit;
	
	_isCalibrated = true;
	
};

bool Joystick::isCalibrated()
{
	return _isCalibrated;
};

void Joystick::getStats()
{
	// Debug
	Serial.println("### STATUS ###");
	
	Serial.print(" Initial X: ");
	Serial.print(_initialX);
	Serial.print(" Offset from center: ");
	Serial.print(_offsetX);
	
	Serial.println("");
	
	Serial.print(" Initial Y: ");
	Serial.print(_initialY);
	Serial.print(" Offset from center: ");
	Serial.print(_offsetY);
	
	Serial.println("");
	
	if(isCalibrated())
	{
		Serial.println("### CALIBRATION DATA ###");
		
		Serial.print(" Far left (raw): ");
		Serial.print(_leftGap);
		Serial.print(" Far right (raw): ");
		Serial.print(1023 - _rightGap);
		
		Serial.println("");
		
		Serial.print(" Far up (raw): ");
		Serial.print(1023 - _upperGap);
		Serial.print(" Far down (raw): ");
		Serial.print(_lowerGap);
		
		Serial.println("");
	}
	else
	{
		Serial.println("### NO CALIBRATION DATA ###");
	}
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
 *  The offset from the center theoretical center is taken into account,
 *	so that the center is at both zero x and y
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
	
	if(!isCalibrated())
	{
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
	}
	else
	{
		if(cleanX < 0)
		{
			float percentX = static_cast<float> (cleanX) / static_cast<float> (511+_offsetX-_leftGap) * 100;
			x = static_cast<int>(percentX);
		}
		
		if(cleanX > 0)
		{
			float percentX = static_cast<float> (cleanX) / static_cast<float> (511-_offsetX-_rightGap) * 100;
			x = static_cast<int>(percentX);
		}
	}
	
	return x;
};

int Joystick::readY()
{
	int cleanY = readCleanY();
	int y = 0;
	
	if(!isCalibrated())
	{
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
	}
	else
	{
		if(cleanY < 0)
		{
			float percentY = static_cast<float> (cleanY) / static_cast<float> (511+_offsetY-_upperGap) * 100;
			y = static_cast<int>(percentY);
		}
		
		if(cleanY > 0)
		{
			float percentY = static_cast<float> (cleanY) / static_cast<float> (511-_offsetY-_lowerGap) * 100;
			y = static_cast<int>(percentY);
		}
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

bool Joystick::isUpperLeft()
{
	if(isUp() && isLeft())
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool Joystick::isUpperRight()
{
	if(isUp() && isRight())
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool Joystick::isLowerLeft()
{
	if(isDown() && isLeft())
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool Joystick::isLowerRight()
{
	if(isDown() && isRight())
	{
		return true;
	}
	else
	{
		return false;
	}
};
	
bool Joystick::isCenter()
{
	if(!isUp() && !isDown() && !isLeft() && !isRight())
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