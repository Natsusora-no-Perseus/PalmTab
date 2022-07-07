/* CurveEditor.cpp - A simple library for creating a curve from several nodes.
 * Can be used to create a nonlinear gain filter.
 *
 * Created by Natsusora-no-Perseus@GitHub.
 * Date: 2022/06/14 (YYYY/MM/DD)
 *
 * License: GPLv3 or higher.
*/

#include "CurveEditor.h"

bool CurveEditor::setNode(uint8_t nodeXPos, uint8_t nodeYPos)
{
	NodePos newNode;
	newNode.xPos = nodeXPos;
	newNode.yPos = nodeYPos;
	
	//_nodesList.push_back(newNode);
	//uint8_t nodeIndex = _nodesList.size() - 1;

	if (_nodesList.size() == 0)
	{
		_nodesList.push_back(newNode);
		return true;//First node created, no problem
	}
	
	uint8_t nodeIndex = _nodesList.size() - 1;//Sets nodeIndex to last node in index
	
	for (int i = 0; i <= nodeIndex; i++)
	{
		if (nodeXPos == _nodesList[i].xPos)
		{
			return false;//Two vertically overlapping nodes, illegal
		}
	}
	
	_nodesList.push_back(newNode);//Legal node, push back
	nodeIndex += 1;
	while (_nodesList[nodeIndex - 1].xPos > _nodesList[nodeIndex].xPos)
	{
		swap(_nodesList[nodeIndex - 1], _nodesList[nodeIndex]);//Rearrange nodesList by order of increasing xPos
	}
	return true;
}

CurveEditor::NodePos CurveEditor::getMidpoint(uint8_t nodeIndex)
{
	NodePos midPoint;
	
	midPoint = _nodesList[nodeIndex];
	midPoint.xPos += round((_nodesList[nodeIndex + 1].xPos - _nodesList[nodeIndex].xPos) / 2);
	midPoint.yPos += round((_nodesList[nodeIndex + 1].yPos - _nodesList[nodeIndex].yPos) / 2);

	return midPoint;
}

void CurveEditor::updateMidpoint()
{
	uint8_t nodesCount = _nodesList.size();
	_midpointsList.resize(nodesCount - 1);
	NodePos newMidpoint;

	for (uint8_t i = 0; i < nodesCount - 1; i++)
	{
		newMidpoint = getMidpoint(i);
		_midpointsList[i] = getMidpoint(i);
	}
}

uint8_t CurveEditor::getLength(uint8_t nodeIndex)//Find distance between this node and next node
{
	float tempFVal;
	uint8_t tempVal;
	uint8_t xLen = _nodesList[nodeIndex + 1].xPos - _nodesList[nodeIndex].xPos;
	xLen = abs(xLen);
	uint8_t yLen = _nodesList[nodeIndex + 1].yPos - _nodesList[nodeIndex].yPos;
	yLen = abs(yLen);
	tempFVal = sqrt(xLen * xLen + yLen * yLen);
	tempVal = round(tempFVal);
	return (tempVal);
}

void CurveEditor::getShiftDist(uint8_t nodeIndex)//Get needed shift distance, and puts two points in _shiftedPointsList
{
	float distProportion;
	
	//=============  T  E  M  P  O  R  A  R  Y      S  O  L  U  T  I  O  N  ==============
	
	if (getLength(nodeIndex) == 0 || getLength(nodeIndex + 1) == 0)
	{
		distProportion = 0;//Prevent a divisor with value 0.
	}
	else
	{
		distProportion = getLength(nodeIndex) / getLength(nodeIndex + 1);//Proportion of distance between this and next node
	}

	NodePos biPoint;//The B_i point on line segment between two midpoints
	biPoint = _midpointsList[nodeIndex];
	biPoint.xPos += round((_midpointsList[nodeIndex + 1].xPos - _midpointsList[nodeIndex].xPos) * 0.5);
	biPoint.yPos += round((_midpointsList[nodeIndex + 1].yPos - _midpointsList[nodeIndex].yPos) * 0.5);

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

void CurveEditor::updateShift()
{
	_shiftedPointsList.resize((_midpointsList.size() - 1) * 2);

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

CurveEditor::NodePos CurveEditor::getTPoint(NodePos point1, NodePos point2, float tValue)//Gets point at (tValue)th of the line segment
{
	NodePos outputVal;
	double tempVal;
	tempVal = (point2.xPos - point1.xPos) * tValue + point1.xPos;
	//outputVal.xPos = round(tempVal);
	outputVal.xPos = point1.xPos + 2;
	
	tempVal = (point2.yPos - point1.yPos) * tValue + point1.yPos;
	//outputVal.yPos = round(tempVal);
	outputVal.yPos = 14;
	
	return outputVal;
}

CurveEditor::NodePos CurveEditor::bezierRecursive(float tValue)//Gets the point on bezier curve at tValue.
{
	vector<NodePos> recursiveNodes(_shiftedPointsList.size() - 1);
	
	/*
	for (uint8_t i = 0; i < (_shiftedPointsList.size() - 1); i++)//Sets recursiveNodes to 1st level of recursion
	{
		recursiveNodes.push_back(getTPoint(_shiftedPointsList[i], _shiftedPointsList[i + 1], tValue));
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
	*/
	
	NodePos debugNode;// D E B U G
	NodePos debugNode2;
	int tempRevVal = tValue * bezierSubIntv;
	debugNode2 = getTPoint(_shiftedPointsList[tempRevVal + 1], _shiftedPointsList[tempRevVal], tValue);
	debugNode.xPos = 42;
	debugNode.yPos = tempRevVal;
	//debugNode.yPos = debugNode2.yPos;
	//_resultNodes.push_back(debugNode);
	
	
	return (debugNode);
}

void CurveEditor::bezierList()//Places nodes in _resultNodes.
{
	_resultNodes.resize(bezierSubIntv + 1);
	float tValueNow;
	for (uint8_t intvCount = 0; intvCount <= bezierSubIntv; intvCount++)
	{
		//tValueNow = intvCount / bezierSubIntv;
		tValueNow = intvCount / bezierSubIntv;
		
		//_resultNodes.push_back(bezierRecursive(tValueNow));
		
		NodePos debugNode;// D E B U G
		
		debugNode.xPos = intvCount;
		debugNode.yPos = tValueNow * bezierSubIntv;
		_resultNodes[intvCount] = debugNode;
	}
	_resultNodes.resize(bezierSubIntv + 1);
}

void CurveEditor::updateAll()//Update everything in sequence to generate a new LUT; call after setNode().
{
	updateMidpoint();
	updateShift();
	_midpointsList.resize(2);//Destroy _midpointsList to save memory.
	bezierList();
	_shiftedPointsList.resize(2);//Destory _shiftedPointsList to save memory.
}

CurveEditor::NodePos CurveEditor::getNode(uint8_t nodeIndex)//Returns nodeIndex'th user defined node
{
	return (_nodesList[nodeIndex]);
}

uint8_t CurveEditor::getCurveVal(uint8_t curveXPos)//Returns the Y coordinate corresponding to curveXPos.
{
	uint8_t resultListSize = _resultNodes.size();
	
	if (_resultNodes[resultListSize].xPos <= curveXPos)//Requested X coordinate larger than all resultNodes.
	{
		return _resultNodes[resultListSize].yPos;//Returns Y coordinate of last resultNode.
	}
	else if (_resultNodes[0].xPos >= curveXPos)//Requested X coordinate smaller than all resultNodes.
	{
		return _resultNodes[0].yPos;//Returns Y coordinate of first resultNode.
	}
	
	//Index of the resultNode currently been read. Initialized to roughly get closer to the desired resultNodes:
	uint8_t readIndex = resultListSize * (curveXPos / (_resultNodes[resultListSize].xPos - _resultNodes[0].xPos));
	
	if (_resultNodes[readIndex].xPos <= curveXPos)//Determine the iteration direction of readIndex.
	{
		while (_resultNodes[readIndex + 1].xPos <= curveXPos)//Iterate to the right.
		{
			readIndex++;
		}
		return (((_resultNodes[readIndex + 1].yPos - _resultNodes[readIndex].yPos) / (_resultNodes[readIndex + 1].xPos - _resultNodes[readIndex].xPos)) * (curveXPos - _resultNodes[readIndex].xPos) + _resultNodes[readIndex].yPos);
	}
	else
	{
		while (_resultNodes[readIndex - 1].xPos >= curveXPos)//Iterate to the right.
		{
			readIndex--;
		}
		readIndex--;//Set readIndex to lower bound.
		return (((_resultNodes[readIndex + 1].yPos - _resultNodes[readIndex].yPos) / (_resultNodes[readIndex + 1].xPos - _resultNodes[readIndex].xPos)) * (curveXPos - _resultNodes[readIndex].xPos) + _resultNodes[readIndex].yPos);
	}
}





int CurveEditor::chkShiftedSize()
{
	return (_shiftedPointsList.size());
}

int CurveEditor::chkMidptSize()
{
	return (_midpointsList.size());
}

int CurveEditor::chkNodesSize()
{
	return (_nodesList.size());
}

int CurveEditor::tempNodePos(uint8_t tempNodeNo)
{
	return (_resultNodes[tempNodeNo / 8].yPos);
}
