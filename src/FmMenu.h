/* FmMenu.h - A simple library for creating a tree menu structure.
 * Used to support a simple GUI menu.
 *
 * Note: This particular version of FmMenu is only meant to be used in the PalmTab project.
 *
 * Created by Natsusora-no-Perseus@GitHub.
 * Date: 2022/07/18 (YYYY/MM/DD)
 *
 * License: GPLv3 or higher.
 */

#ifndef FmMenu_h
#define FmMenu_h

#include <iostream>
#include <vector>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;//I do know it's wrong to do this in a library. Will remove it later.

class FmMenu
{
public:

	void confirmClick();//A regular click.
	void escapeClick();//Escape key click.
	void xClick(int8_t clickCount);//A upward/downward click. clickCount for multiple clicks.
	void yClick(int8_t clickCount);//A left/rightward click. clickCount for multiple clicks.








private:

	bool setindex(int8_t setStep, int8_t setDepth);//Select an item. Use setStep to change in local depth, use setDepth to change depth. Positive value for relative change, negative value for absolute change.

	uint8_t menuIndex[2] = {0, 0};//The current selected item's index.



};


#endif
