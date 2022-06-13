/* JoystickBasics.cpp - A simple library created to setup, calibrate and read from joysticks.
 *
 * Created by Natsusora-no-Perseus@GitHub.
 * Date: 2022/06/23 (YYYY/MM/DD)
 *
 * License: GPLv3 or higher.
*/

#include "JoystickBasics.h"

JoystickBasics::JoystickBasics(uint8_t inputPin, uint16_t midPoint, bool invertInput)
{
	_inputPin = inputPin;
	_invertInput = invertInput;
	_midPoint = midPoint;
}

void JoystickBasics::calibrateInput()
{
	_calibrationValue = _midPoint - analogRead(_inputPin);
}

void JoystickBasics::setSensitivity(uint8_t sensitivityFactor)
{
	_sensitivityFactor = sensitivityFactor;
}

int16_t JoystickBasics::getPos()
{
	if (_invertInput == true)
	{
		return (_midPoint - analogRead(_inputPin) - _calibrationValue);
	}
	else
	{
		return (analogRead(_inputPin) - _midPoint + _calibrationValue);
	}
}
