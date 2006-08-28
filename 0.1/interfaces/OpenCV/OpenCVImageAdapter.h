#ifndef __OPENCVIMAGEADAPTER_H
#define __OPENCVIMAGEADAPTER_H

#include <cv.h>
#include "OvImageAdapter.h"

class OpenCVImageAdapter :	public OvImageAdapter
{
public:
	OpenCVImageAdapter(IplImage*im = 0);
	virtual ~OpenCVImageAdapter();

	virtual void getPixel(float * value, int row, int column);
	virtual void setPixel(float * value, int row, int column);	

protected:
	IplImage*mIplImage;	

	void (OpenCVImageAdapter::*getPixelfptr) (float * value, int row, int column);
	void (OpenCVImageAdapter::*setPixelfptr) (float * value, int row, int column);

	template<class T> void getPixelT(float * value, int row, int column);
	template<class T> void setPixelT(float * value, int row, int column);
};

#endif
