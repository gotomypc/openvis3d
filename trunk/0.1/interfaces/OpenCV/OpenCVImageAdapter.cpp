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

void OpenCVImageAdapter::getPixel(float * value, int row, int column)
{	
	(this->*getPixelfptr)(value, row, column);	
}

void OpenCVImageAdapter::setPixel(float * value, int row, int column)
{	
	(this->*setPixelfptr)(value, row, column);
}

template<class T> void OpenCVImageAdapter::getPixelT(float * value, int row, int column)
{
	T*temp = &((T*)(mIplImage->imageData + mIplImage->widthStep*row))[column*mChannels];
	for(int i=0;i<mChannels;i++)
	{
		value[i] = (float) *temp; 
		temp++;
	}
}

template<class T> void OpenCVImageAdapter::setPixelT(float * value, int row, int column)
{
	T*temp = &((T*)(mIplImage->imageData + mIplImage->widthStep*row))[column*mChannels];
	for(int i=0;i<mChannels;i++)
	{
		*temp = (T) value[i]; 
		temp++;
	}
}

