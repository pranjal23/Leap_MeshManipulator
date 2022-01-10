# Leap_MeshManipulator

Build & Run
---------------------------------------------------------------------------------------------------------------------------------------------

Windows
To build the project on Windows (Windows 10 – 64-bit) please install the following:

1.	Visual Studio 2013 – Required by Qt Creator from https://www.visualstudio.com/en-us/news/vs2013-community-vs.aspx

2.	Windows SDK – It will install the latest OpenGL and GLU lib files required by the project from https://www.microsoft.com/en-us/download/details.aspx?id=8279

3.	Qt Creator (Visual Studio 2013 version) – Download from https://download.qt.io/official_releases/qt/5.5/5.5.1/qt-opensource-windows-x86-msvc2013_64-5.5.1.exe.mirrorlist

4.	Leap Motion Device, SDK and Software

5.	Open and Run MeshViewQt project – In Qt creator open the MeshViewQt.pro file from src, this will open the project. The project can now be built and run. Developers will need to update the relative path of Leap lib and include folders.

6.	To RUN the directly without compiling – Install Windows SDK, Leap Motion Software (step 2 & 3) and go to the release folder and run MeshViewQt.exe


Minimum Requirements: 
•	Windows x86_64 PC with Windows 10 – 64-bit, will run of windows 7 and 8 but not tested on those PC’s.

•	x86_64 PC is important as I used quint64 data type in the parsing algorithm which may not be available on 32-bit PC.

•	Installation of latest Windows SDK is essential as it will install latest Glu dll which has functions like gluNewQuadric() etc.

•	Leap Motion Device is required to interact with the Software.

INTERACTION STYLE PROPOSED

Run the MeshViewQt executable, to start the application from release folder.

CHANGE MODE
The Software runs in three modes ZOOM Mode, PAN Mode and ROTATE Mode. To change mode bring up both the hand in the Leap Motion Visible area in parallel, the left hand should be on the left and right hand on the right separated by some distance.


TRANSLATION

ZOOM MODE
Zoom in/Zoom out - To zoom in and zoom out, Change the Mode to ZOOM. Using one hand make circle gestures in clockwise and anti-clockwise direction to Zoom in and Zoom out

PAN MODE 
Left-Right, Up/Down – To pan left/right and up/down , Change the Mode to PAN. Using one hand make circle gestures in clockwise and anti-clockwise direction to Pan left and Pan Right. To Pan in up/down directions use the swipe up and swipe down gestures.

ROTATION MODE
X-Axis, Y-Axis and Z-Axis – To rotate in X, Y and Z axis, Change the Mode to ROTATE. Using one hand make and closed palm rotate around Z axis as pivot using the palm’s roll value, with one finger extended to rotate around X axis as pivot using the palm’s pitch value and with two fingers extended two rotate around Y axis as pivot using the palm’s yaw value.


Reasons to choose the proposed interaction style
1.	Leap Motion provides a three-dimensional user interface which frees the users from the hassles of a touch interface. The benefits are more evident when considering interaction non-traditional display devices like holographic displays, virtual reality or augmented reality headsets.
2.	The idea was to formalize the dynamics to control virtual objects using a 3 dimensional input device, thus it was essential to develop an intuitive interface language that the users can guess or easily learn.
3.	Interaction styles were derived from that of physical devices such as SLR camera where circular motion is used to zoom in and out, Slide and swipe is used to Pan in devices such as Globe and grab & rotate motion which is very common in knobs.


Advantages over traditional input devices
1.	This interaction style can work with modern display devices such as holographic display and virtual/augmented reality displays.
2.	Allows users to use an extra dimension while interacting with the computer, thus making it more versatile.
3.	Enhances the aesthetics of interaction, thus making interaction more fun.


Suggestions for improvement
1.	The proposed mechanism is a basic proof of concept, it can be made better by normalizing the motion and tweaking the parameters to react better to the gestures.
2.	Screen overlays to select various modes can be used to easily switch between interaction modes instead of sequential switching of modes.
3.	GUI can provide certain HUD’s to help users with feedback about the action they are performing, such as the direction of rotation etc.
