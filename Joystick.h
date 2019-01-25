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

#ifndef Joystick_h
#define Joystick_h

#include "Arduino.h"

class Joystick
{
	private:
		byte _horizontalPin;
		byte _verticalPin;
		byte _selectPin;
		
		unsigned int _sensivity;
		unsigned short int _mode;
		
		int _initialX;
		int _initialY;
		
		int _offsetX;
		int _offsetY;
		
		int _minimalX;
		int _minimalY;
		
		int _maximalX;
		int _maximalY;
		
		bool _isCalibrated;

	private:
		
		/*
		 *	Read the raw analog values of the joystick
		 */
		
		int readRawX();
		int readRawY();
		
		/*
		 *	Read a cleaned-up analog value of the joystick
		 *	Range: 	-511 to +512
		 *	Error:	+/- offset
		 */
		
		int readCleanX();
		int readCleanY();
	
	public:

		/*
		 *	Constructor
		 */
		
		Joystick(byte horizontalPin, byte verticalPin, byte selectPin);
		
		/*
		 *	Initializing the joystick
		 */
		
		void initialize();
		
		/*
		 *	Calibrating the joystick
		 */
		
		void calibrate();
		bool isCalibrated();
		
		/*
		 *	Adjusting the sensivity of the joystick from 0 (LOW) to 10 (HIGH)
		 */
		
		void setSensivity(unsigned int sensivity);
		unsigned int getSensivity();
		
		/*
		 *	Read the relative analog values of the joystick in percent
		 *	Range: -100 to +100
		 */
		
		int readX();
		int readY();
		
		/*
		 *	Checks if the joystick is leaning into the specified direction
		 */
		
		bool isLeft();
		bool isRight();
		bool isUp();
		bool isDown();
		
		/*
		 *	Checks for the state of the select-button
		 */
		
		bool isPressed();
		bool isReleased();
};

#endif