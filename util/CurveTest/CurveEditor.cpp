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
		swapNodes(&_nodesList[nodeIndex - 1]);//Rearrange nodesList by order of increasing xPos
		nodeIndex --;
	}
	return true;
}

CurveEditor::NodePos CurveEditor::getMidpoint(uint8_t nodeIndex)
{
	NodePos midPoint;
	
	midPoint = _nodesList[nodeIndex];
	midPoint.xPos = round((_nodesList[nodeIndex + 1].xPos + _nodesList[nodeIndex].xPos) / 2);
	midPoint.yPos = round((_nodesList[nodeIndex + 1].yPos + _nodesList[nodeIndex].yPos) / 2);

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
	float tempVal;
	float xLen = _nodesList[nodeIndex + 1].xPos - _nodesList[nodeIndex].xPos;
	float yLen = _nodesList[nodeIndex + 1].yPos - _nodesList[nodeIndex].yPos;
	tempVal = round(sqrt((float)xLen * (float)xLen + (float)yLen * (float)yLen));
	return (tempVal);
	//QUESTIONABLE
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
		distProportion = (float)getLength(nodeIndex) / ((float)getLength(nodeIndex) + (float)getLength(nodeIndex + 1));
		//Proportion of distance between this and next node
	}

	NodePos biPoint;//The B_i point on line segment between two midpoints
	biPoint = _midpointsList[nodeIndex];
	biPoint.xPos += round((_midpointsList[nodeIndex + 1].xPos - _midpointsList[nodeIndex].xPos) * (float)distProportion);
	biPoint.yPos += round((_midpointsList[nodeIndex + 1].yPos - _midpointsList[nodeIndex].yPos) * (float)distProportion);

	NodePos shiftedPoint1, shiftedPoint2;//Two control points
	
	shiftedPoint1 = _nodesList[nodeIndex + 1];
	shiftedPoint2 = shiftedPoint1;
	
	shiftedPoint1.xPos += round((_midpointsList[nodeIndex].xPos - biPoint.xPos) * (float)_scalingFactor);
	shiftedPoint1.yPos += round((_midpointsList[nodeIndex].yPos - biPoint.yPos) * (float)_scalingFactor);
	
	shiftedPoint2.xPos += round((_midpointsList[nodeIndex + 1].xPos - biPoint.xPos) * (float)_scalingFactor);
	shiftedPoint2.yPos += round((_midpointsList[nodeIndex + 1].yPos - biPoint.yPos) * (float)_scalingFactor);
	
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
	float tempVal;
	tempVal = ((point2.xPos - point1.xPos) * (float)tValue) + point1.xPos;
	outputVal.xPos = round(tempVal);
	
	tempVal = ((point2.yPos - point1.yPos) * (float)tValue) + point1.yPos;
	outputVal.yPos = round(tempVal);
	
	return outputVal;
}

CurveEditor::NodePos CurveEditor::bezierRecursive(float tValue)//Gets the point on bezier curve at tValue.
{
	vector<NodePos> recursiveNodes(_shiftedPointsList.size() - 1);
	
	uint8_t recursionDepth = _shiftedPointsList.size() - 1;//The required levels of calculation
	
	for (uint8_t i = 0; i < recursionDepth; i++)//Sets recursiveNodes to 1st level of recursion
	{
		recursiveNodes[i] = getTPoint(_shiftedPointsList[i], _shiftedPointsList[i + 1], tValue);
	}//ok
	
	recursionDepth = recursiveNodes.size();//The required levels of calculation
	
	for (uint8_t n = 0; n < recursionDepth - 1; n++)//ok
	{
		for (uint8_t m = 0; m < recursionDepth - n - 1; m++)
		{
			recursiveNodes[m] = getTPoint(recursiveNodes[m], recursiveNodes[m + 1], tValue);
		}
		
		recursiveNodes.resize(recursionDepth - n);
	}
	
	
	/*
	NodePos debugNode;// D E B U G
	NodePos debugNode2;
	int tempRevVal = tValue * bezierSubIntv;
	debugNode2 = getTPoint(_shiftedPointsList[tempRevVal + 1], _shiftedPointsList[tempRevVal], tValue);
	debugNode.xPos = 42;
	debugNode.yPos = tempRevVal;
	//debugNode.yPos = debugNode2.yPos;
	//_resultNodes.push_back(debugNode);
	*/
	
	
	return (recursiveNodes[0]);
}

void CurveEditor::bezierList()//Places nodes in _resultNodes.
{
	_resultNodes.resize(bezierSubIntv + 1);
	float tValueNow = 0.00;
	for (uint8_t intvCount = 0; intvCount <= bezierSubIntv; intvCount++)
	{
		tValueNow = (float)intvCount / (float)bezierSubIntv;
		
		//_resultNodes.push_back(bezierRecursive(tValueNow));
		_resultNodes[intvCount] = bezierRecursive(tValueNow);
		//_resultNodes[intvCount] = getTPoint(_shiftedPointsList[intvCount], _shiftedPointsList[intvCount + 1], 0.550);
		
		/*
		NodePos debugNode;// D E B U G
		debugNode.xPos = intvCount;
		debugNode.yPos = tValueNow * bezierSubIntv;
		_resultNodes[intvCount] = debugNode;
		*/
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
	
	if (_resultNodes[resultListSize - 1].xPos <= curveXPos)//Requested X coordinate larger than all resultNodes.
	{
		return (_resultNodes[resultListSize - 1].yPos);//Returns Y coordinate of last resultNode.
	}
	else if (_resultNodes[0].xPos >= curveXPos)//Requested X coordinate smaller than all resultNodes.
	{
		return (_resultNodes[0].yPos);//Returns Y coordinate of first resultNode.
	}
	
	//Index of the resultNode currently been read. Initialized to roughly get closer to the desired resultNodes:
	uint8_t readIndex = resultListSize * (curveXPos / (_resultNodes[resultListSize].xPos - _resultNodes[0].xPos));
	
	float locationRatio;//The desired node's position on the line between two nodes
	
	if (_resultNodes[readIndex].xPos <= curveXPos)//Determine the iteration direction of readIndex.
	{
		while (_resultNodes[readIndex + 1].xPos <= curveXPos)//Iterate to the right.
		{
			readIndex++;//Set readIndex to lower bound.
		}
		
		locationRatio = (curveXPos - _resultNodes[readIndex].xPos) / (_resultNodes[readIndex + 1].xPos - _resultNodes[readIndex].xPos);
		return ((_resultNodes[readIndex + 1].yPos - _resultNodes[readIndex].yPos) * (float)locationRatio + _resultNodes[readIndex].yPos);
	}
	else
	{
		while (_resultNodes[readIndex - 1].xPos >= curveXPos)//Iterate to the right.
		{
			readIndex--;
		}
		readIndex--;//Set readIndex to lower bound.
		locationRatio = (curveXPos - _resultNodes[readIndex].xPos) / (_resultNodes[readIndex + 1].xPos - _resultNodes[readIndex].xPos);
		return ((_resultNodes[readIndex + 1].yPos - _resultNodes[readIndex].yPos) * (float)locationRatio + _resultNodes[readIndex].yPos);
	}
}

void CurveEditor::swapNodes(NodePos *firstNode)//Swap two nodes.
{
  NodePos tempNode = *(firstNode + 1);//Stores 2nd value
  *(firstNode + 1) = *firstNode;
  *firstNode = tempNode;
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
