/*
  joystick.h - Library for using a analog 2-axis joystick with select-button.
  Created by Michael Kubina, January 25, 2019.
  Released into the public domain.
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