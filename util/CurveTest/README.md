# CurveTester
## Description
This is a very sketchy GUI program for playing around with the CurveEditor library.<br/>
You can place nodes and visualize the output curve using it.<br/>
*The CurveEditor.h used here is slightly different from the one used with the arduino program.*<br/>

## How to use
The interface includes a white box on the left for displaying the curve and a menu bar on the right.<br/>
A white crosshair displays the position of mouse cursor.<br/>
The yellow circular reticles displays the position of user-placed nodes.<br/>
The light cyan 'X's displays the position of auto-generated control points for bezier curve. (Mainly for debugging)<br/>
The purple points represents the output of the curve.<br/>

[x] and [y] at the top of menu bar shows the cursor's position in the window's coordinate system, while the [x*] and [y*] shows the position in the curve box's coordinate system.<br/>

Click on "Select Node" to start selecting a node. A red ">  <" will highlight the selected node, and the cursor crosshair will turn cyan when it's inside the curve box.<br/>
Click on "<" or ">" to select a different node.<br/>
When the cursor crosshair is cyan, clicking inside the curve box will move the selected node to the cursor's position.<br/>
Click on "New Node" to create a node. The cursor crosshair will turn green when it's inside the curve box. Click again inside the curve box will place a new node there.<br/>
Click on "Delete Node" to delete the selected node.<br/>
Click on "Update Nodes" after you have finished changing the nodes. This will refresh the curve.<br/>
If you want to cancel selecting / moving / creating a node, move your cursor to the blank space around the curve box. The cursor crosshair will turn red, and clicking there will cancel the behaviour.<br/>

You can find "nodeList" "shiftedPoints" "resultNodes" "curveVal" buttons below the "Delete Nodes" button. Click on them to show / hide these points in the curve box. A "#" will appear next to the button if it is set to "show".<br/>

The three sliders, from left to right, respectively controls ScalingFactor, BezierSubIntv, and OrderScalingFactor. Click on them to change the value of these parameters, which is then displayed at the bottom of the sliders. Click on the "Confirm" button above them to confirm the changes.<br/>
* ScalingFactor controls the "sharpness" of the curve. The larger its value is, the "sharper" the curve is.
* BezierSubIntv controls the "precision" of the curve. The larger its value is, the more accurate and smooth the curve is.
* OrderScalingFactor controls the "responsiveness" of the curve. As more nodes are added, each node will have a smaller effect on the curve. Increasing this parameter can compensate this reduction in effectiveness more.<br/>

Click on the "EXIT" button in the bottom-right corner to end the program.<br/>

## Notes
As mentioned above, this program is a quite sketchy one. Sometimes a mouse click may not be recognized, and when that happens, please try again.<br/>
For the same reason, no value-limiting function has been implemented. Please keep your nodes some distance away from the borders of the curve box, otherwise some unprecedented errors may occur.
