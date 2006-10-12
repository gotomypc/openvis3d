#include "OvImageT.h"

template<>
const OvImageT<bool> OvImageT<bool>::operator ! () const
{
	OvImageT<bool> result(*this); 

	for(int i=0; i<result.mSize; i++)
		result.mData[i] = !result.mData[i];

	return (result); 
}

const OvImageT<bool> operator && (const OvImageT<bool> & i1, const OvImageT<bool> & i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
	{
		for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] && i2.mData[i]);
	}

	return result;
}


const OvImageT<bool> operator || (const OvImageT<bool> & i1, const OvImageT<bool> & i2) 
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
	{
		for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] || i2.mData[i]);
	}

	return result;
}
