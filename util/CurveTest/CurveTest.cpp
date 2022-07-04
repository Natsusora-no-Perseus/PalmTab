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

#include "CurveEditor.h"

using namespace std;

const int WindowWidth = 800;
const int WindowHeight = 600;

const int BkColor = 0;//Background color, black
const int LineColor = 15;//Line color, white
const int NodeColor = 14;//Node color, yellow
const int CurveColor = 2;//Curve points color, green

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

void drawReticle(int xCood, int yCood, int color)
{
	setcolor(color);
	line(xCood, 1, xCood, WindowHeight);
	line(1, yCood, WindowWidth, yCood);
	setcolor(LineColor);
}

int main()
{
        
	//Init window:
	initwindow(WindowWidth, WindowHeight);
	setbkcolor(BkColor);
	cleardevice();
	setcolor(LineColor);
	
	CurveEditor testCurve;
	testCurve.setScalingFactor(0.6);
	testCurve.setBezierSubIntv(128);
	
	int yReturnVal = 42;
	bool canDrawCurve = false;
	
	while (!(ismouseclick(WM_LBUTTONDOWN) && ((mousex() >= 750) && (mousey() >= 575))))
    {
        //# Menu Bar:
        line(690, 1, 690, 600);
    	//## Display cursor position:
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
		drawReticle(mousex(), mousey(), 7);
		//## Draw 'Update Nodes' button
		rectangle(690, 100, 800, 140);
		moveto(700, 110);
		outtext("Update Nodes");
		
		//## Menu bar actions:
		if (ismouseclick(WM_LBUTTONDOWN) && mousex() >= 690)
		{
			if (mousey() >= 100 && mousey() <= 140)
			{	
				testCurve.updateMidpoint();
				testCurve.updateShift();
				testCurve.bezierList();
				canDrawCurve = true;
			}
		}
		
		
		
		//Draw the exit button:
		rectangle(750, 575, 800, 600);
		moveto(760, 580);
		outtext("EXIT");
		
		//*Test Section:*
		line(1, 1, 90, 90);
		drawNode(90, 90);
		
		//*Test Curve:*
		//Note: Conversion method: n * 2 + 50.
		testCurve.setNode(10, 10);
		testCurve.setNode(30, 40);
		testCurve.setNode(90, 120);
		testCurve.setNode(120, 60);
		testCurve.setNode(130, 40);
		testCurve.setNode(160, 50);
		
		
		
		
		drawNode(70, 70);
		drawNode(110, 130);
		drawNode(230, 290);
		drawNode(290, 170);
		drawNode(310, 130);
		drawNode(370, 150);
		rectangle(50, 50, 562, 562);
		
		//Draw the curve:
		if (canDrawCurve == true)
		{
			drawNode(3, 580);//==Probe==
			for (int i = 0; i <= 160; i += 5)
			{
				//yReturnVal = testCurve.getCurveVal(i);
				drawNode(i, yReturnVal);
			}
		}
		
		    //Get mouse status:
    	if (ismouseclick(WM_LBUTTONDOWN))
		{
			clearmouseclick(WM_LBUTTONDOWN);
    	}
		
		//Clear screen:
		delay(20);
		swapbuffers();
		cleardevice();
    }
	
	delay(500);
	return(0);
}
