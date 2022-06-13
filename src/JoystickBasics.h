/* JoystickBasics.h - A simple library created to setup, calibrate and read from joysticks.
 *
 * Created by Natsusora-no-Perseus@GitHub.
 * Date: 2022/06/23 (YYYY/MM/DD)
 *
 * License: GPLv3 or higher.
*/

#include "Arduino.h"

class JoystickBasics
{
public:

	void calibrateInput();
	void setSensitivity(uint8_t sensitivityFactor);

	int16_t getPos();

	JoystickBasics(uint8_t inputPin, uint16_t midPoint, bool invertInput);

private:

	bool _invertInput;
	uint8_t _sensitivityFactor;
	uint8_t _inputPin;
	uint16_t _midPoint;
	int16_t _calibrationValue;

};
