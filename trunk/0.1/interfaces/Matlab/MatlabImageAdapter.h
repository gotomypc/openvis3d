#ifndef __MATLABIMAGEADAPTER_H
#define __MATLABIMAGEADAPTER_H

#include "OvImageAdapter.h"

extern "C" {
#include "mex.h"
}

class MatlabImageAdapter :	public OvImageAdapter
{
public:
	MatlabImageAdapter(mxArray*im = 0);
	virtual ~MatlabImageAdapter();

	virtual void getPixel(float * value, int row, int column);
	virtual void setPixel(float * value, int row, int column);	

protected:
	mxArray* mMatlabImage;	
	void * mImageDataPtr;

	void (MatlabImageAdapter::*getPixelfptr) (float * value, int row, int column);
	void (MatlabImageAdapter::*setPixelfptr) (float * value, int row, int column);

	template<class T> void getPixelT(float * value, int row, int column);
	template<class T> void setPixelT(float * value, int row, int column);
};

#endif
