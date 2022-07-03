/* CurveEditor.h - A simple library for creating a curve from several nodes.
 * Can be used to create a nonlinear gain filter.
 *
 * Created by Natsusora-no-Perseus@GitHub.
 * Date: 2022/06/14 (YYYY/MM/DD)
 *
 * License: GPLv3 or higher.
*/

#ifndef CurveEditor_h
#define CurveEditor_h

#include <iostream>
#include <vector>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class CurveEditor
{
public:
	CurveEditor();

	/* Mutator functions: */
	bool setNode(int8_t nodeXPos, int8_t nodeYPos);//Returns false if node can't be set
	void setScalingFactor(float scalingFactor);//Should be set between 0~1. Larger = "sharper" curve. Suggested value is 0.6.
	void setBezierSubIntv(uint8_t inputSubIntv);//Sets resolution of output nodes: Larger = more nodes, but higher memory usage.

	/* Process functions: */
	//Process for setting up a curve: setNode => updateMidpoint => updateShift => bezierList
	void updateAll();//Updates everything and generates a new list of output nodes



private:

	struct NodePos
	{
		int8_t xPos;
		int8_t yPos;
	};
	
	vector<NodePos> _nodesList;//Stores list of NodePos
	vector<NodePos> _midpointsList;//Stores list of midpoints
	vector<NodePos> _shiftedPointsList;//Stores list of shifted midpoints
	vector<NodePos> _resultNodes;//Stores list of result NodePos
	
	
	
	float _scalingFactor;//Shrinks the distance between pairs of control points; adjusts "sharpness" of curve
	uint8_t bezierSubIntv;//Number of subintervals that the bezier curve should have.
	
	

	/* Utility functions for use inside this library: */
	NodePos getMidpoint(uint8_t nodeIndex);//Find midpoint between this node and next node
	uint8_t getLength(uint8_t nodeIndex);//Find distance between this node and next node
	NodePos getShiftDist(uint8_t nodeIndex);//Find shift distance for this and next midpoint
	NodePos getTPoint(NodePos point1, NodePos point2, float tValue);//Gets point at (tValue)th of the line segment
	NodePos bezierRecursive(float tValue);//Gets the point on bezier curve at tValue.

	/* Process functions in this library: */
	void updateMidpoint();//Updates list of midpoints
	void updateShift();//Updates list shifted midpoints (also control points for bezier curve)
	void bezierList();//Places nodes in _resultNodes, for LUT use.


};

#endif