OpenVis3D Open Source 3D Vision Library

The goal of this project is to provide a library of efficient 3D computer vision routines for image and video processing. It will include routines for dense stereo matching, optical flow (motion) estimation, occlusion detection, egomotion (3D self-motion) estimation, and structure from motion.

*****************************
Copyright 2006 Abhijit Ogale 

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0 

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*****************************

Documentation:

http://www.cs.umd.edu/users/ogale/openvis3d/docs/index.html


Example usage:

A sample program OvStereoFlowTester.cpp which tests the stereo and optical flow code on the two included image pairs is provided. 

A Makefile is also provided. Although the OpenVis3D library can be used with OpenCV, Matlab, and any other library of your choice, this particular example uses OpenCV. Hence, please modify the following lines of the Makefile to suit your OpenCV installation:

A) Change include path:

INC = -I /usr/include/opencv

B) Change the library path and the names of the libraries as needed on this line:

LDFLAGS = -L /usr/lib -L . -lm -lcv -lhighgui -lcvaux


