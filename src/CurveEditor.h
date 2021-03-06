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
	
	struct NodePos
	{
		uint8_t xPos;
		uint8_t yPos;
	};

	/* Mutator functions: */
	bool setNode(uint8_t nodeXPos, uint8_t nodeYPos);//Returns false if node can't be set
	void setScalingFactor(float scalingFactor);//Should be set between 0~1. Larger = "sharper" curve. Suggested value is 0.6.
	void setBezierSubIntv(uint8_t inputSubIntv);//Sets resolution of output nodes: Larger = more nodes, but higher memory usage.
	void setOrderScalingFactor(float scalingFactor);//Shifts control points further. 0 = no effect.
	
	void updateAll();//Updates everything and generates a new list of output nodes
	
	/* Accessor functions: */
	NodePos getNode(uint8_t nodeIndex);//Returns nodeIndex'th user defined node.
	uint8_t getCurveVal(uint8_t curveXPos);//Returns the Y coordinate corresponding to curveXPos.
	
	
	
private:
	
	vector<NodePos> _nodesList;//Stores list of NodePos
	vector<NodePos> _midpointsList;//Stores list of midpoints
	vector<NodePos> _shiftedPointsList;//Stores list of shifted midpoints
	vector<NodePos> _resultNodes;//Stores list of result NodePos
	
	struct FNodePos
	{
		float xPos;
		float yPos;
	};
	
	float _scalingFactor;//Shrinks the distance between pairs of control points; adjusts "sharpness" of curve
	uint8_t bezierSubIntv;//Number of subintervals that the bezier curve should have.
	float _orderScalingFactor;//Shifts control points further
	
	/* Utility functions for use inside this library: */
	NodePos getMidpoint(uint8_t nodeIndex);//Find midpoint between this node and next node
	uint8_t getLength(uint8_t nodeIndex);//Find distance between this node and next node
	void getShiftDist(uint8_t nodeIndex);//Find shift distance for this and next midpoint
	FNodePos getTPoint(NodePos point1, NodePos point2, float tValue);//Gets point at (tValue)th of the line segment
	FNodePos getFTPoint(FNodePos point1, FNodePos point2, float tValue);
	NodePos bezierRecursive(float tValue);//Gets the point on bezier curve at tValue.
	void swapNodes(NodePos *firstNode);//Swap two nodes

	/* Process functions in this library: */
	//Process for setting up a curve: setNode => updateMidpoint => updateShift => bezierList
	void updateMidpoint();//Updates list of midpoints
	void updateShift();//Updates list shifted midpoints (also control points for bezier curve)
	void bezierList();//Places nodes in _resultNodes, for LUT use.


};

#endif

