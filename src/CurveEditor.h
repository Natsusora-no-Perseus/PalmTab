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

#include "Arduino.h"

class CurveEditor
{
public:

	CurveEditor();

	bool setNode(int8_t nodeXPos, int8_t nodeYPos);//Returns false if node can't be set



private:

	struct NodePos
	{
		int8_t xPos;
		int8_t yPos;
	};

	float scalingFactor;//Shrinks the distance between pairs of control points; adjusts "sharpness" of curve

	vector<NodePos> _nodesList(2);//Stores list of NodePos
	vector<NodePos> _midpointsList(2);//Stores list of midpoints
	vector<NodePos> _shiftedPointsList(2);//Stores list of shifted midpoints

	NodePos getMidpoint(uint8_t nodeIndex);//Find midpoint between this node and next node

	NodePos updateMidpoint();//Updates list of midpoints

	uint8_t getLength(uint8_t nodeIndex);//Find distance between this node and next node

	NodePos getShiftDist(uint8_t nodeIndex);//Find shift distance for this and next midpoint

	NodePos updateShift();//Updates list shifted midpoints (also control points for bezier curve)
};

#endif
