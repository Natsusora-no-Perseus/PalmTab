/* CurveEditor.cpp - A simple library for creating a curve from several nodes.
 * Can be used to create a nonlinear gain filter.
 *
 * Created by Natsusora-no-Perseus@GitHub.
 * Date: 2022/06/14 (YYYY/MM/DD)
 *
 * License: GPLv3 or higher.
*/

#include "CurveEditor.h"

bool CurveEditor::setNode(int8_t nodeXPos, int8_t nodeYPos)
{
	NodePos newNode;
	newNode.xPos = nodeXPos;
	newNode.yPos = nodeYPos;
	nodesList.push_back(newNode);

	if (nodeIndex == 0)
	{
		return true;//First node created, no problem
	}
	else if (_nodesList[nodeIndex].xPos == nodeList[nodeIndex - 1].xPos)
	{
		return false;//Two vertically overlapping nodes, illegal
	}
	else if (nodeIndex != 0)
	{
		while (_nodesList[nodeIndex - 1].xPos > _nodesList[nodeIndex].xPos
		{
			swap(_nodesList[nodeIndex - 1], _nodesList[nodeIndex - 1]);//Rearrange nodesList by order of increasing xPos
		}
		return true;
	}
}

NodePos CurveEditor::getMidpoint(uint8_t nodeIndex)
{
	NodePos midPoint;
	midPoint.xPos = round((_nodesList[nodeIndex + 1].xPos - _nodesList[nodeIndex].xPos) / 2);
	midPoint.yPos = round((_nodesList[nodeIndex + 1].yPos - _nodesList[nodeIndex].yPos) / 2);

	return midPoint;
}

NodePos CurveEditor::updateMidpoint()
{
	uint8_t nodesCount = _nodesList.size();
	_midpointsList.resize(nodesCount - 1);
	NodePos newMidpoint;

	for (uint8_t i = 0; i < nodesCount - 1; i++)
	{
		newMidpoint = getMidpoint(i);
		_midpointsList.push_back(newMidpoint);
	}
}

uint8_t CurveEditor::getLength(uint8_t nodeIndex)
{
	uint8_t xLen = abs(_nodesList[nodeIndex + 1].xPos - _nodesList[nodeIndex].xPos);
	uint8_t yLen = abs(_nodesList[nodeIndex + 1].yPos - _nodesList[nodeIndex].yPos);
	return (round(sqrt(xLen * xLen + yLen * yLen)));
}

NodePos CurveEditor::getShiftDist(uint8_t nodeIndex)//Get needed shift distance, and puts two points in _shiftedPointsList
{
	float distProportion = getLength(nodeIndex) / getLength(nodeIndex + 1);//Proportion of distance between this and next node

	NodePos biPoint;//The B_i point on line segment between two midpoints
	biPoint = _midpointsList[nodeIndex];
	biPoint.xPos += round((_midpointsList[nodeIndex + 1].xPos - _midpointsList[nodeIndex].xPos) * distProportion);
	biPoint.yPos += round((_midpointsList[nodeIndex + 1].yPos - _midpointsList[nodeIndex].yPos) * distProportion);

	NodePos shiftedPoint1, shiftedPoint2;

	shiftedPoint1.xPos = _nodesList[nodeIndex + 1].xPos - biPoint.xPos;
	shiftedPoint1.yPos = _nodesList[nodeIndex + 1].yPos - biPoint.yPos;//Shifting distance of midpoints (B_i to next node)
	shiftedPoint2 = shiftedPoint1;

	shiftedPoint1.xPos += _midpointsList[nodeIndex].xPos;
	shiftedPoint1.yPos += _midpointsList[nodeIndex].yPos;
	shiftedPoint2.xPos += _midpointsList[nodeIndex + 1].xPos;
	shiftedPoint2.yPos += _midpointsList[nodeIndex + 1].yPos;//Shifts without scaling

	shiftedPoint1.xPos += round((biPoint.xPos - _midpointsList[nodeIndex].xPos) * _scalingFactor);
	shiftedPoint1.yPos += round((biPoint.yPos - _midpointsList[nodeIndex].yPos) * _scalingFactor);

	shiftedPoint2.xPos += round((biPoint.xPos - _midpointsList[nodeIndex].xPos) * _scalingFactor);
	shiftedPoint2.yPos += round((biPoint.yPos - _midpointsList[nodeIndex].yPos) * _scalingFactor);

	_shiftedPointsList[2 * nodeIndex] = shiftedPoint1;
	_shiftedPointsList[2 * nodeIndex + 1] = shiftedPoint2;
}

NodePos CurveEditor::updateShift()
{
	_shiftedPointsList.resize(_midpointsList.size() * 2 + 1);

	for (int i = 0; i < _midpointsList.size() - 1; i++)
	{
		getShiftDist(i);
	}
}

void CurveEditor::setScalingFactor(float scalingFactor)
{
	_scalingFactor = scalingFactor;
}

void CurveEditor::setBezierSubIntv(uint8_t inputSubIntv)
{
	bezierSubIntv = inputSubIntv;
}

FloatNodePos CurveEditor::getTPoint(FloatNodePos point1, FloatNodePos point2, float tValue)//Gets point at (tValue)th of the line segment
{
	FloatNodePos outputVal;
	outputVal.xPos = (point2.xPos - point1.xPos) * tValue + point1.xPos;
	outputVal.yPos = (point2.yPos - point1.yPos) * tValue + point1.xPos;
	
	return outputVal;
}

NodePos CurveEditor::bezierRecursive(float tValue)//Gets the point on bezier curve at tValue.
{
	vector<FloatNodePos> recursiveNodes(_shiftedPointsList.size() - 1);
	
	for (uint8_t i = 0; i <= _shiftedPointsList.size() - 1; i++)//Sets recursiveNodes to 1st level of recursion
	{
		recursiveNodes[i] = getTPoint(_shiftedPointsList[i], _shiftedPointsList[i + 1], tValue);
	}
	
	uint8_t recursionDepth = recursiveNodes.size() - 1;//The required levels of calculation
	
	for (uint8_t n = 0; n < recursionDepth; n++)
	{
		for (uint8_t m = 0; m < recursionDepth - n; n++)
		{
			recursiveNodes[m] = getTPoint(recursiveNodes[m], recursiveNodes[m + 1], tValue);
		}
		
		recursiveNodes.resize(recursionDepth - n);
	}
	
	uint8_t outputVal = round(recursiveNodes[0]);
	return (outputVal);
}

NodePos CurveEditor::bezierList()//Places nodes in _resultNodes.
{
	float tValueNow;
	for (uint8_t intvCount = 0; intvCount <= bezierSubIntv; intvCount++)
	{
		tValueNow = intvCount * (1 / bezierSubIntv);
		
		_resultNodes.push_back(bezierRecursive(tValueNow));
	}
	_resultNodes.resize(bezierSubIntv + 1);
}
