#include "OpenCVImageAdapter.h"

OpenCVImageAdapter::OpenCVImageAdapter(IplImage*im)
	: OvImageAdapter(), mIplImage(im)
{
	if(mIplImage != 0)
	{
		mHeight = mIplImage->height;
		mWidth  = mIplImage->width;
		mChannels = mIplImage->nChannels;

		switch(mIplImage->depth)
		{
			case IPL_DEPTH_8U:
				mDataType = OV_DATA_UINT8;
				getPixelfptr = &OpenCVImageAdapter::getPixelT<unsigned char>; 
				setPixelfptr = &OpenCVImageAdapter::setPixelT<unsigned char>;
				break;
			case IPL_DEPTH_16U:
				mDataType = OV_DATA_UINT16;
				getPixelfptr = &OpenCVImageAdapter::getPixelT<unsigned short>; 
				setPixelfptr = &OpenCVImageAdapter::setPixelT<unsigned short>;
				break;
			case IPL_DEPTH_8S:
				mDataType = OV_DATA_INT8;
				getPixelfptr = &OpenCVImageAdapter::getPixelT<char>; 
				setPixelfptr = &OpenCVImageAdapter::setPixelT<char>;
				break;
			case IPL_DEPTH_16S:
				mDataType = OV_DATA_INT16;
				getPixelfptr = &OpenCVImageAdapter::getPixelT<short>; 
				setPixelfptr = &OpenCVImageAdapter::setPixelT<short>;
				break;
			case IPL_DEPTH_32S:
				mDataType = OV_DATA_INT32;
				getPixelfptr = &OpenCVImageAdapter::getPixelT<int>; 
				setPixelfptr = &OpenCVImageAdapter::setPixelT<int>;
				break;
			case IPL_DEPTH_32F:
				mDataType = OV_DATA_FLOAT32;
				getPixelfptr = &OpenCVImageAdapter::getPixelT<float>; 
				setPixelfptr = &OpenCVImageAdapter::setPixelT<float>;
				break;
			case IPL_DEPTH_64F:
				mDataType = OV_DATA_DOUBLE64;
				getPixelfptr = &OpenCVImageAdapter::getPixelT<double>; 
				setPixelfptr = &OpenCVImageAdapter::setPixelT<double>;
				break;
			default:
				mDataType = OV_DATA_UNKNOWN;
				getPixelfptr = &OpenCVImageAdapter::getPixelT<unsigned char>; 
				setPixelfptr = &OpenCVImageAdapter::setPixelT<unsigned char>;
				break;
		}
	}
	else
	{
		getPixelfptr = &OpenCVImageAdapter::getPixelT<unsigned char>; 
		setPixelfptr = &OpenCVImageAdapter::setPixelT<unsigned char>;
	}
}

OpenCVImageAdapter::~OpenCVImageAdapter()
{
}


double OpenCVImageAdapter::getPixel(int row, int column, int channel) const
{	
	if((row<0)||(row>=mHeight)) return 0;
	if((column<0)||(column>=mWidth)) return 0;
	if((channel<0)||(channel>=mChannels)) return 0;

	return (double) (this->*getPixelfptr)(row, column, channel);	
}

void OpenCVImageAdapter::setPixel(double value, int row, int column, int channel)
{	
	if((row<0)||(row>=mHeight)) return;
	if((column<0)||(column>=mWidth)) return;
	if((channel<0)||(channel>=mChannels)) return;

	(this->*setPixelfptr)(value, row, column, channel);
}

template<class T> double OpenCVImageAdapter::getPixelT(int row, int column, int channel) const
{
	T*temp = &((T*)(mIplImage->imageData + mIplImage->widthStep*row))[column*mChannels];
	return (double) temp[channel];
}

template<class T> void OpenCVImageAdapter::setPixelT(double value, int row, int column, int channel)
{
	T*temp = &((T*)(mIplImage->imageData + mIplImage->widthStep*row))[column*mChannels];
	temp[channel] = (T) value;
}

