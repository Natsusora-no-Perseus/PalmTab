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

const int BKCOLOR = BLACK;//Background color, black
const int LINECOLOR = WHITE;//Line color, white
const int NODECOLOR = YELLOW;//Node color, yellow
const int CURVECOLOR = LIGHTGREEN;//Curve points color
const int RETICLE_COLOR = LIGHTGRAY;//Reticle color
const int CROSSCOLOR = LIGHTCYAN;

bool isInCurveBox = false;//If cursor is in curve box

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
	line(xCood, 1, xCood, WindowHeight);
	line(1, yCood, WindowWidth, yCood);
	setcolor(LINECOLOR);
}

Point convertNodePos(CurveEditor::NodePos inputNodePos)//Converts curve coordinates to display coordinates
{
	Point outputNodePos;
	outputNodePos.xCood = 50 + inputNodePos.xPos * 2;
	outputNodePos.yCood = 560 - inputNodePos.yPos * 2;	
	
	return (outputNodePos);
}

int main()
{
        
	//Init window:
	initwindow(WindowWidth, WindowHeight);
	setbkcolor(BKCOLOR);
	cleardevice();
	setcolor(LINECOLOR);
	
	CurveEditor testCurve;
	testCurve.setScalingFactor(0.6);
	testCurve.setBezierSubIntv(128);
	
	int yReturnVal = 0;
	bool canDrawCurve = false;
	
	uint8_t nodeSelectSta = 0;//0 = Not active; 1 = Selecting; 2 = Creating new node
	uint8_t selectedNode = 0;//Selected node
	
	testCurve.setNode(30, 40);
	testCurve.setNode(90, 120);
	testCurve.setNode(120, 60);
	testCurve.setNode(130, 40);
	testCurve.setNode(160, 50);
	testCurve.setNode(10, 10);
	testCurve.setNode(210, 210);
	testCurve.setNode(180, 10);
	testCurve.setNode(190, 100);
	
	
	
	while (!(ismouseclick(WM_LBUTTONDOWN) && ((mousex() >= 750) && (mousey() >= 575))))
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
		
		//## Draw "EXIT" button
		rectangle(750, 575, 800, 600);
		moveto(760, 580);
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
		//## Draw all nodes
		for (int i = 0; i < testCurve._nodesList.size(); i++)
		{
			drawNode(convertNodePos(testCurve._nodesList[i]).xCood, convertNodePos(testCurve._nodesList[i]).yCood);
		}
		// Draw all shifted nodes
		for (int i = 0; i < testCurve._shiftedPointsList.size(); i++)
		{
			drawCross(convertNodePos(testCurve._shiftedPointsList[i]).xCood, convertNodePos(testCurve._shiftedPointsList[i]).yCood);
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
		
		
		
		
				
		//Draw the curve:
		if (canDrawCurve == true)
		{
			drawNode(3, 580);//==Probe==
			for (int i = 0; i <= 128; i += 1)
			{
				//yReturnVal = testCurve.getCurveVal(i);
				yReturnVal = testCurve._resultNodes[i].yPos;
				putpixel(i * 2 + 50, 560 - yReturnVal * 2, CURVECOLOR);
				
				//yReturnVal = testCurve.getCurveVal(i);
				//putpixel(i * 2 + 50, yReturnVal * 2 + 50, LIGHTCYAN);
				
			}
			
			/*
			char chkNodesChar[5];
			sprintf(chkNodesChar, "%i", testCurve.chkNodesSize());
			moveto(100, 50);
			outtext(chkNodesChar);
				
			char chkMidptChar[5];
			sprintf(chkMidptChar, "%i", testCurve.chkMidptSize());
			moveto(100, 70);
			outtext(chkMidptChar);
			
			char chkShiftedChar[5];
			sprintf(chkShiftedChar, "%i", testCurve.chkShiftedSize());
			moveto(100, 90);
			outtext(chkShiftedChar);

			char chkResultNodesChar[5];
			sprintf(chkResultNodesChar, "%i", testCurve._resultNodes.size());
			moveto(100, 110);
			outtext(chkResultNodesChar);			
			
			
			
			moveto(200, 1);
			
			char chkResultChar[5];
			
			for (int i = 0; i < 50; i += 1)
			{
				moveto(570, i * 12);
				sprintf(chkResultChar, "%i", testCurve._resultNodes[i].yPos);
				outtext(chkResultChar);
			}
			for (int i = 50; i < 100; i += 1)
			{
				moveto(605, (i - 50) * 12);
				sprintf(chkResultChar, "%i", testCurve._resultNodes[i].yPos);
				outtext(chkResultChar);
			}
			
			char chkGeneralNodesChar[5];
			for (int i = 0; i < 14; i += 1)
			{
				moveto(635, i * 15);
				sprintf(chkGeneralNodesChar, "%i", testCurve._shiftedPointsList[i].yPos);
				outtext(chkGeneralNodesChar);
			}
			*/
			
			
			
			
			//drawNode(mousex(), testCurve.tempNodePos(mousex()));
			//yReturnVal = testCurve.getCurveVal(mousex());
			//drawNode(mousex(), yReturnVal);
			
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
