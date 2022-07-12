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

const int WINDOWWIDTH = 800;
const int WINDOWHEIGHT = 600;

const int BKCOLOR = BLACK;//Background color, black
const int LINECOLOR = WHITE;//Line color, white
const int NODECOLOR = YELLOW;//Node color, yellow
const int CURVECOLOR = LIGHTGREEN;//Curve points color
const int RESULTCOLOR = LIGHTMAGENTA;//ResultNodes color
const int RETICLE_COLOR = LIGHTGRAY;//Reticle color
const int CROSSCOLOR = LIGHTCYAN;
const int SLIDERCOLOR = LIGHTCYAN;

bool isInCurveBox = false;//If cursor is in curve box

bool showNodesList = true, showShiftedPoints = false, showResultNodes = false, showCurveVal = true;

float tempScalingFa = 0.500, tempBezierSubIntv = 128, tempOrderScalingFa = 0.20;

void drawSlider(int topLX, float fullRange, float& currVal, bool numDispFmt)//Draw a slider
{
	rectangle(topLX, 380, topLX + 35, 575);//Slider box
	line(topLX + 18, 390, topLX + 18, 550);//Slider rail
	
	if (ismouseclick(WM_LBUTTONDOWN) && ((mousey() <= 550 && mousey() >= 390) && (mousex() > topLX && mousex() < topLX + 35)))
	{
		currVal = (((float)mousey() - 390) / 160) * fullRange;
	}
	
	int tabPos = ((currVal / fullRange) * 160) + 390;
	
	for (int i = tabPos - 2; i <= tabPos + 2; i ++)
	{
		setcolor(SLIDERCOLOR);
		line(topLX + 9, i, topLX + 27, i);//Slider tab
		setcolor(LINECOLOR);
	}
	
	char printBuff[5];
	if (numDispFmt == true)
	{
		sprintf(printBuff, "%3.2f", currVal);//For 0.00
	}
	else
	{
		sprintf(printBuff, "%3.0f", currVal);//For 100
	}
	moveto(topLX + 3, 555);
	outtext(printBuff);
}

struct Point
{
	int xCood;
	int yCood;
};

void drawNode(int xCood, int yCood)//Draw a circle with reticle lines
{
	setcolor(NODECOLOR);//Set color to yellow
	circle(xCood, yCood, 5);
	line(xCood + 4, yCood, xCood - 4, yCood);
	line(xCood, yCood + 4, xCood, yCood - 4);
	setcolor(LINECOLOR);//Reset color to default
}

void drawCross(int xCood, int yCood)
{
	setcolor(CROSSCOLOR);//Set color to yellow
	line(xCood + 7, yCood + 7, xCood - 7, yCood - 7);
	line(xCood + 7, yCood - 7, xCood - 7, yCood + 7);
	setcolor(LINECOLOR);//Reset color to default
}

void drawReticle(int xCood, int yCood, int color)
{
	setcolor(color);
	line(xCood, 1, xCood, WINDOWHEIGHT);
	line(1, yCood, WINDOWWIDTH, yCood);
	setcolor(LINECOLOR);
}

Point convertNodePos(CurveEditor::NodePos inputNodePos)//Converts curve coordinates to display coordinates
{
	Point outputNodePos;
	outputNodePos.xCood = 50 + inputNodePos.xPos * 2;
	outputNodePos.yCood = 560 - inputNodePos.yPos * 2;	
	
	return (outputNodePos);
}

void toggleBoolState(bool& inputBool)
{
	if (inputBool == true)
	{
		inputBool = false;
	}
	else
	{
		inputBool = true;
	}
}

int main()
{
	//Init window:
	initwindow(WINDOWWIDTH, WINDOWHEIGHT, "Curve Tester");
	setbkcolor(BKCOLOR);
	cleardevice();
	setcolor(LINECOLOR);
	
	CurveEditor testCurve;
	testCurve.setScalingFactor(0.500);
	testCurve.setBezierSubIntv(128);
	testCurve.setOrderScalingFactor(0.20);
	
	int resultNodeValue = 0;
	int yReturnVal = 0;
	bool canDrawCurve = false;
	
	uint8_t nodeSelectSta = 0;//0 = Not active; 1 = Selecting; 2 = Creating new node
	uint8_t selectedNode = 0;//Selected node
	
	
	//Pre-loaded nodes
	testCurve.setNode(30, 40);
	testCurve.setNode(90, 120);
	testCurve.setNode(120, 60);
	testCurve.setNode(160, 50);
	testCurve.setNode(10, 10);
	testCurve.setNode(210, 210);
	testCurve.setNode(180, 60);
	testCurve.setNode(190, 100);
	
	
	while (!(ismouseclick(WM_LBUTTONDOWN) && ((mousex() >= 690) && (mousey() >= 575))))
    {
    	//# Curve Box:
    	rectangle(50, 50, 562, 562);
        //# Menu Bar:
        line(690, 1, 690, 600);
        
    	//## Display cursor position:
    	//### Screen cursor position
    	char xPosStr[5];
		char yPosStr[5];
    	sprintf(yPosStr, "%03i", mousey());   
		sprintf(xPosStr, "%03i", mousex());
		moveto(700, 1);
		outtext("===Mouse===");
		moveto(702, 20);
		outtext("=[x]=     =[y]=");
		moveto(707, 40);
		outtext(xPosStr);
		moveto(758, 40);
		outtext(yPosStr);
		//### Converted cursor position
		char xConvtdStr[5];//Converted position
		char yConvtdStr[5];
		sprintf(xConvtdStr, "%03i", (mousex() - 50) / 2);
		sprintf(yConvtdStr, "%03i", (560 - mousey()) / 2);
		moveto(700, 60);
		outtext("=[x*]=   =[y*]=");
		moveto(707, 80);
		outtext(xConvtdStr);
		moveto(758, 80);
		outtext(yConvtdStr);
		
		//## Draw "Update Nodes" button
		rectangle(690, 100, 800, 140);
		moveto(700, 112);
		outtext("Update Nodes");
		
		//## Draw "Select Node" button
		rectangle(690, 140, 800, 180);
		moveto(707, 152);
		outtext("Select Node");
		//### Draw selection arrow keys
		rectangle(690, 180, 800, 200);
		moveto(713, 182);
		outtext("<             >");
		line(745, 180, 745, 200);
		//### Draw "New Node" button
		line(690, 240, 800, 240);
		moveto(714, 212);
		outtext("New Node");
		//### Draw "Delete Node" button
		line(690, 280, 800, 280);
		moveto(707, 252);
		outtext("Delete Node");
		
		//## Draw indicator separater
		line(710, 280, 710, 360);
		//### Draw "Confirm" button
		moveto(720, 362);
		outtext("Confirm");
		//### Draw "nodesList" button
		line(690, 300, 800, 300);
		moveto(727, 282);
		outtext("nodeList");		
		//### Draw "shiftedPointsList" button
		line(690, 320, 800, 320);
		moveto(713, 302);
		outtext("shiftedPoints");		
		//### Draw "resultNodes" button
		line(690, 340, 800, 340);
		moveto(717, 322);
		outtext("resultNodes");
		//### Draw "curveVal" button
		line(690, 360, 800, 360);
		moveto(728, 342);
		outtext("curveVal");
		
		
		//## Draw "EXIT" button
		rectangle(690, 575, 800, 600);
		moveto(735, 580);
		outtext("EXIT");
		
		//## Menu bar actions:
		if (ismouseclick(WM_LBUTTONDOWN) && mousex() >= 690)//Mouse left button pressed
		{
			//### "Update Nodes"
			if (mousey() > 100 && mousey() < 140)
			{	
				testCurve.updateMidpoint();
				testCurve.updateShift();
				testCurve.bezierList();
				canDrawCurve = true;
			}
			//### "Select Nodes"
			if (mousey() > 140 && mousey() < 180)
			{
				nodeSelectSta = 1;
			}
			//### Node selection arrow keys
			if ((mousey() > 180 && mousey() < 200) && (nodeSelectSta == 1 || nodeSelectSta == 3))
			{
				if (mousex() < 745)//"<" button
				{
					if (selectedNode == 0)
					{
						selectedNode = testCurve._nodesList.size() - 1;//Select last node
					}
					else
					{
						selectedNode -= 1;
					}
				}
				else//">" button
				{
					if (selectedNode == testCurve._nodesList.size() - 1)
					{
						selectedNode = 0;
					}
					else
					{
						selectedNode += 1;
					}
				}
			}
			//### "New Node"
			if (mousey() > 200 && mousey() < 240)
			{
				nodeSelectSta = 2;
			}
			//### "Delete Node"
			if (mousey() > 240 && mousey() < 280)
			{
				nodeSelectSta = 3;
				for (int i = selectedNode; i < testCurve._nodesList.size() - 1; i++)
				{
					testCurve._nodesList[i] = testCurve._nodesList[i + 1];
				}
				testCurve._nodesList.resize(testCurve._nodesList.size() - 1);
				nodeSelectSta = 0;
			}
			
			//## "Show Points"
			if (mousex() > 690)
			{	
				if (mousey() > 280 && mousey() < 300)
				{
					toggleBoolState(showNodesList);
				}
				if (mousey() > 300 && mousey() < 320)
				{
					toggleBoolState(showShiftedPoints);
				}
				if (mousey() > 320 && mousey() < 340)
				{
					toggleBoolState(showResultNodes);
				}
				if (mousey() > 340 && mousey() < 360)
				{
					toggleBoolState(showCurveVal);
				}
			}
			
			//## "Confirm"
			if (mousey() > 360 && mousey() < 380)
			{
				testCurve.setScalingFactor(tempScalingFa);
				testCurve.setBezierSubIntv(tempBezierSubIntv);
				testCurve.setOrderScalingFactor(tempOrderScalingFa);
			}
		}
		
		//# Draw Nodes:
		//## Highlight selected node
		if (nodeSelectSta == 1)
		{
			moveto(convertNodePos(testCurve._nodesList[selectedNode]).xCood-14, convertNodePos(testCurve._nodesList[selectedNode]).yCood-9);
			setcolor(LIGHTRED);
			outtext(">   <");
			setcolor(LINECOLOR);
		}
		//## Draw all user-defined nodes
		if (showNodesList == true)
		{
			moveto(695, 282);
			outtext("#");
			
			for (int i = 0; i < testCurve._nodesList.size(); i++)
			{
				drawNode(convertNodePos(testCurve._nodesList[i]).xCood, convertNodePos(testCurve._nodesList[i]).yCood);
			}
		}
		
		//## Draw all shifted nodes
		if (showShiftedPoints == true)
		{
			moveto(695, 302);
			outtext("#");
			
			for (int i = 0; i < testCurve._shiftedPointsList.size(); i++)
			{
				drawCross(convertNodePos(testCurve._shiftedPointsList[i]).xCood, convertNodePos(testCurve._shiftedPointsList[i]).yCood);
			}
		}
		
		//# Draw reticle:
		//## Read mouse zone:
		if ((mousex() > 50 && mousey() > 50) && (mousex() < 562 && mousey() < 562))
		{
			isInCurveBox = true;
		}
		else
		{
			isInCurveBox = false;
		}
		//## Display reticle
		if (nodeSelectSta == 0)
		{
			drawReticle(mousex(), mousey(), WHITE);
		}
		else if (isInCurveBox == true)
		{
			if (nodeSelectSta == 1)//Change node
			{
				drawReticle(mousex(), mousey(), LIGHTCYAN);
			}
			else if (nodeSelectSta == 2)//Create node
			{
				drawReticle(mousex(), mousey(), LIGHTGREEN);
			}
		}
		else
		{
			drawReticle(mousex(), mousey(), LIGHTRED);
		}
		//# Node changing actions:
		if (ismouseclick(WM_LBUTTONDOWN) && isInCurveBox == true)
		{
			if (nodeSelectSta == 1)//Change a node
			{
				for (int i = selectedNode; i < testCurve._nodesList.size() - 1; i++)
				{
					testCurve._nodesList[i] = testCurve._nodesList[i + 1];
				}
				testCurve._nodesList.resize(testCurve._nodesList.size() - 1);
				testCurve.setNode((mousex() - 50) / 2, (560 - mousey()) / 2);
				nodeSelectSta = 0;
			}
			else if (nodeSelectSta == 2)//Create a node
			{
				testCurve.setNode((mousex() - 50) / 2, (560 - mousey()) / 2);
				nodeSelectSta = 0;
			}
		}
		else if ((ismouseclick(WM_LBUTTONDOWN) && isInCurveBox == false) && mousex() < 690)//Cancel selection
		{
			nodeSelectSta = 0;
		}
		
		//# Draw the curve:
		if (canDrawCurve == true)
		{
			//## Draw resultNodes
			if (showResultNodes == true)
			{
				moveto(695, 322);
				outtext("#");
				
				for (int i = 0; i <= 128; i ++)
				{
					resultNodeValue = testCurve._resultNodes[i].yPos;
					putpixel(testCurve._resultNodes[i].xPos * 2 + 50, 560 - resultNodeValue * 2, RESULTCOLOR);
				}
			}
			
			//## Draw curveVal
			if (showCurveVal == true)
			{
				moveto(695, 342);
				outtext("#");
				
				for (int i = 50; i <= 562; i ++)
				{
					yReturnVal = testCurve.getCurveVal((i - 50) / 2);
					putpixel(i, 560 - yReturnVal * 2, CURVECOLOR);
				}
			}
			
		}
		
		//# Draw sliders
		drawSlider(691, 3.00, tempScalingFa, true);
		drawSlider(727, 256, tempBezierSubIntv, false);
		drawSlider(763, 1.00, tempOrderScalingFa, true);
		
		
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
	
	delay(200);
	return(0);
}
