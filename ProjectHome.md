  * Authors:
    * [Abhijit Ogale](http://www.cs.umd.edu/users/ogale): Overall library design, Dense stereo and optical flow code.
    * [Justin Domke](http://www.cs.umd.edu/users/domke): 3D egomotion estimation
  * Acknowledgements:
    * Open-source grant from [Google](http://www.google.com).
    * [Professor Yiannis Aloimonos](http://www.cfar.umd.edu/~yiannis), Computer Vision Lab, University of Maryland.
  * [DOWNLOAD OpenVis3D](http://openvis3d.googlecode.com/files/openvis3dsrc.zip)
  * [README.txt](http://openvis3d.googlecode.com/files/README.txt)
  * [Code documentation](http://www.cs.umd.edu/users/ogale/openvis3d/docs/annotated.html)
  * [References](http://www.cs.umd.edu/users/ogale/download/code.html)
  * Sample results (Symmetric dense stereo and optical flow with occlusions)
    * ![http://www.cs.umd.edu/users/ogale/download/results1.jpg](http://www.cs.umd.edu/users/ogale/download/results1.jpg)

## Goal ##
The goal of this project is to provide a library of efficient 3D computer vision routines for image and video processing. It includes routines for dense stereo matching, optical flow (motion) estimation, occlusion detection, and egomotion (3D self-motion) estimation.

## Benefits ##
  * Provides symmetric dense stereo matching with occlusions, symmetric dense optical flow with occlusions, probabilistic 3D egomotion estimation.
  * Designed as a template library.
  * Uses adapters to be compatible with Matlab, OpenCV, and can be easily tailored to be used with any other image processing library. Example code is included for OpenCV and Matlab. See the README.txt within the download.
  * [Internal image template class OvImageT](http://www.cs.umd.edu/users/ogale/openvis3d/docs/class_ov_image_t.html) can be wrapped around OpenCV or Matlab image data, and provides Matlab-style functionality, including:
    * Operators:
      1. Arithmetic: +  -  /  **+=  -=  /=**=
      1. Comparison: <  <=  ==  >=  >
      1. Logical:   !   ||  &&
      1. Math: cos  sin  tan  acos  asin  atan  atan2 cosh  sinh  tanh  exp  log  log10  abs  ceil  floor round mod  pow  pow  sqrt
      1. Bottom line:   We can have expressions like iflag = (1+exp(-i2\*i3)) < i4 , where iflag is a boolean image and i2, i3, i4 are double images (say).
    * Direct efficient pixel access by overloading function call operator:
      1. A(row,col,channel)
      1. E.g., A(1,5,2) = 10;
    * Masked assignment and sub-image access, e.g.,
      1. A.copyMasked(mask, B);
      1. A.copyMasked(mask, 5);
      1. A = B.getSubImage(5,10,3,11);
    * Matlab-like functions:
      1. convolve2D, filter2D with arbitrary 2D kernel
      1. medianFilter2D, meanfilter2D, minFilter2D, maxFilter2D
      1. min, max, mean, sum along any dimension.
      1. repmat, reshape, meshgrid.
      1. transpose, flipLR, flipUD, shiftImageXY, resize (nn, bilinear), random, gaussian, oriented gabor, rgb2gray.
      1. And many more ...
  * Modular structure of Stereo and Optical flow code makes it easy to add new algorithms for local matching, global matching, pre- and post-processing.
  * Well-documented code (see the [Doxygen-generated documentation](http://www.cs.umd.edu/users/ogale/openvis3d/docs/annotated.html))