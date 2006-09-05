#ifndef __OPENCVIMAGEADAPTER_H
#define __OPENCVIMAGEADAPTER_H

#include <cv.h>
#include "OvImageAdapter.h"

class OpenCVImageAdapter :	public OvImageAdapter
{
public:
	OpenCVImageAdapter(IplImage*im = 0);
	virtual ~OpenCVImageAdapter();

	virtual double getPixel(int row, int column, int channel) const;
	virtual void   setPixel(double value, int row, int column, int channel);

protected:
	IplImage*mIplImage;	

	double (OpenCVImageAdapter::*getPixelfptr) (int row, int column, int channel) const;
	void   (OpenCVImageAdapter::*setPixelfptr) (double value, int row, int column, int channel);

	template<typename T> double getPixelT(int row, int column, int channel) const;
	template<typename T> void   setPixelT(double value, int row, int column, int channel);
};

#endif
