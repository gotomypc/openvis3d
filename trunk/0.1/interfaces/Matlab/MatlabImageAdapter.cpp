#include "MatlabImageAdapter.h"

MatlabImageAdapter::MatlabImageAdapter(mxArray*im)
	: OvImageAdapter(), mMatlabImage(im), mImageDataPtr(0)
{
	int tmpNumDims;
	const int *tmpDimSizes;
	mxClassID imageDataType;

	if(mMatlabImage != 0)
	{
		tmpNumDims = mxGetNumberOfDimensions(mMatlabImage);
		tmpDimSizes = mxGetDimensions(mMatlabImage);

		mHeight = tmpDimSizes[0];
		mWidth  = tmpDimSizes[1];
		if(tmpNumDims>2) mChannels = tmpDimSizes[2];
		else mChannels = 1;

		imageDataType = mxGetClassID(mMatlabImage);
		mImageDataPtr = mxGetData(mMatlabImage);

		switch(imageDataType)
		{
			case mxUINT8_CLASS:
				mDataType = OV_DATA_UINT8;
				getPixelfptr = &MatlabImageAdapter::getPixelT<unsigned char>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<unsigned char>;
				break;
			case mxUINT16_CLASS:
				mDataType = OV_DATA_UINT16;
				getPixelfptr = &MatlabImageAdapter::getPixelT<unsigned short>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<unsigned short>;
				break;
			case mxUINT32_CLASS:
				mDataType = OV_DATA_UINT32;
				getPixelfptr = &MatlabImageAdapter::getPixelT<unsigned int>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<unsigned int>;
				break;
			case mxUINT64_CLASS:
				mDataType = OV_DATA_UINT64;
				getPixelfptr = &MatlabImageAdapter::getPixelT<unsigned long long>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<unsigned long long>;
				break;
			case mxINT8_CLASS:
				mDataType = OV_DATA_INT8;
				getPixelfptr = &MatlabImageAdapter::getPixelT<char>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<char>;
				break;
			case mxINT16_CLASS:
				mDataType = OV_DATA_INT16;
				getPixelfptr = &MatlabImageAdapter::getPixelT<short>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<short>;
				break;
			case mxINT32_CLASS:
				mDataType = OV_DATA_INT32;
				getPixelfptr = &MatlabImageAdapter::getPixelT<int>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<int>;
				break;
			case mxINT64_CLASS:
				mDataType = OV_DATA_INT64;
				getPixelfptr = &MatlabImageAdapter::getPixelT<long long>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<long long>;
				break;
			case mxSINGLE_CLASS:
				mDataType = OV_DATA_FLOAT32;
				getPixelfptr = &MatlabImageAdapter::getPixelT<float>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<float>;
				break;
			case mxDOUBLE_CLASS :
				mDataType = OV_DATA_DOUBLE64;
				getPixelfptr = &MatlabImageAdapter::getPixelT<double>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<double>;
				break;
			default:
				mDataType = OV_DATA_UNKNOWN;
				getPixelfptr = &MatlabImageAdapter::getPixelT<unsigned char>; 
				setPixelfptr = &MatlabImageAdapter::setPixelT<unsigned char>;
				break;
		}
	}
	else
	{
		mDataType = OV_DATA_UNKNOWN;
		getPixelfptr = &MatlabImageAdapter::getPixelT<unsigned char>; 
		setPixelfptr = &MatlabImageAdapter::setPixelT<unsigned char>;
	}
}

MatlabImageAdapter::~MatlabImageAdapter()
{
}

void MatlabImageAdapter::getPixel(float * value, int row, int column)
{	
	(this->*getPixelfptr)(value, row, column);	
}

void MatlabImageAdapter::setPixel(float * value, int row, int column)
{	
	(this->*setPixelfptr)(value, row, column);
}

template<class T> void MatlabImageAdapter::getPixelT(float * value, int row, int column)
{
	T*temp = ((T*)mImageDataPtr) + column*mHeight + row;
	for(int i=0;i<mChannels;i++)
	{
		value[i] = (float) *temp; 
		temp+=(mHeight*mWidth);
	}
}

template<class T> void MatlabImageAdapter::setPixelT(float * value, int row, int column)
{
	T*temp = ((T*)mImageDataPtr) + column*mHeight + row;
	for(int i=0;i<mChannels;i++)
	{
		*temp = (T) value[i]; 
		temp+=(mHeight*mWidth);
	}
}

