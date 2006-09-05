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

	virtual double getPixel(int row, int column, int channel) const;
	virtual void   setPixel(double value, int row, int column, int channel);

protected:
	mxArray* mMatlabImage;	
	void * mImageDataPtr;

	double (MatlabImageAdapter::*getPixelfptr) (int row, int column, int channel) const;
	void (MatlabImageAdapter::*setPixelfptr) (double value, int row, int column, int channel);

	template<class T> double getPixelT(int row, int column, int channel) const;
	template<class T> void setPixelT(double value, int row, int column, int channel);
};

#endif
