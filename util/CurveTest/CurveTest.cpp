/* CurveTest.cpp - A sketchy tester for creating curves using CurveEditor.h
 * Used to test the library and test curves.
 * Uses WinBGIM library (https://home.cs.colorado.edu/~main/cs1300/doc/bgi/bgi.html) for graphics.
 *
 * Created by Natsusora-no-Perseus@GitHub.
 * Date: 2022/07/02 (YYYY/MM/DD)
 *
 * License: GPLv3 or higher.
*/
#include <graphics.h>
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

const int WindowWidth = 800;
const int WindowHeight = 600;

const int BkColor = 0;//Background color, black
const int LineColor = 15;//Line color, white
const int NodeColor = 14;//Node color, yellow

struct Point
{
	int xCood;
	int yCood;
};

void drawNode(int xCood, int yCood)//Draw a circle with reticle lines
{
	setcolor(NodeColor);//Set color to yellow
	circle(xCood, yCood, 5);
	line(xCood + 4, yCood, xCood - 4, yCood);
	line(xCood, yCood + 4, xCood, yCood - 4);
	setcolor(LineColor);//Reset color to default
}

int main()
{
	//Init window:
	initwindow(WindowWidth, WindowHeight);
	setbkcolor(BkColor);
	cleardevice();
	setcolor(LineColor);
	
	//*Test:*
	line(1, 1, 90, 90);
	drawNode(90, 90);
	
	//Draw the exit button:
	rectangle(750, 575, 800, 600);
	moveto(760, 580);
	outtext("EXIT");
	
	while (!(ismouseclick(WM_LBUTTONDOWN) && ((mousex() >= 750) && (mousey() >= 575))))
    {
    	//Display cursor position:
    	char xPosStr[5];
		char yPosStr[5];
    	sprintf(yPosStr, "%03i", mousey());   
		sprintf(xPosStr, "%03i", mousex());
		moveto(702, 1);
		outtext("===Mouse===");
		moveto(700, 20);
		outtext("==[x]== ==[y]==");
		moveto(712, 40);
		outtext(xPosStr);
		moveto(763, 40);
		outtext(yPosStr);
		
		
    }
	
	delay(500);
	return 0;
}