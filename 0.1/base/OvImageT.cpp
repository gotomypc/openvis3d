#ifndef __OVIMAGET_CPP
#define __OVIMAGET_CPP

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

/**  Rounds to nearest integer
* 
* 
* @param value	input value
* @return the rounded value
*/
inline int round(double value)
{
	return int(value + 0.5);
}

/** 
* Constructor with no parameters to create empty image.
* @see OvImageT(int height, int width, int nColorChannels)
* @see OvImageT(const OvImageT<T>& srcImage, bool copyData)
* @see OvImageT(const OvImageT<C>& srcImage, bool copyData)
*/
template<typename T>
OvImageT<T>::OvImageT()
: mHeight(0), mWidth(0), mChannels(0), mSize(0), mHeightTimesWidth(0), mData(0)
{
}

/** 
* Constructor specifying height, width and number of color channels.
* 
* @param height	desired height of the image
* @param width desired width of image
* @param nColorChannels desired number of color channels
* @see OvImageT()
* @see OvImageT(const OvImageT<T>& srcImage, bool copyData)
* @see OvImageT(const OvImageT<C>& srcImage, bool copyData)
*/
template<typename T>
OvImageT<T>::OvImageT(int height, int width, int nColorChannels)
: mHeight(0), mWidth(0), mChannels(0), mSize(0), mHeightTimesWidth(0), mData(0)
{
	if(height>0) mHeight = height;
	if(width>0) mWidth = width;
	if(nColorChannels>0) mChannels = nColorChannels;
	mSize = mWidth*mHeight*mChannels;	

	if(mSize>0) 
	{
		mData = new T[mSize];
		if(mData == 0)
		{
			mHeight = 0;
			mWidth = 0;
			mChannels = 0;
			mSize = 0;
		}
		else
		{
			for(int i=0; i<mSize; i++) mData[i] = (T) 0;
		}
	}

	mHeightTimesWidth = mWidth*mHeight;
}

/** 
* Copy constructor creates image with same size as the input image, with the option of copying data.
* 
* @param srcImage source image whose dimensions are to be copied
* @param copyData copies source data if set to true (default), otherwise returns zeroed image.
* @see OvImageT()
* @see OvImageT(int height, int width, int nColorChannels)
* @see OvImageT(const OvImageT<C>& srcImage, bool copyData)
*/
template<typename T>
OvImageT<T>::OvImageT(const OvImageT<T>& srcImage, bool copyData)
: mHeight(srcImage.mHeight), mWidth(srcImage.mWidth), mChannels(srcImage.mChannels), mSize(srcImage.mSize), mHeightTimesWidth(srcImage.mHeightTimesWidth)
{
	if((srcImage.mSize>0) && (srcImage.mData != 0))
	{
		mData = new T[mSize];
		if(mData == 0)
		{
			mHeight = 0;
			mWidth = 0;
			mChannels = 0;
			mSize = 0;
			mHeightTimesWidth = 0;
		}
		else
		{
			if(copyData)
			{
				for(int i=0;i<mSize; i++) mData[i] = srcImage.mData[i];
			}
			else
			{
				for(int i=0;i<mSize; i++) mData[i] = 0;
			}
		}
	}
	else
	{
		mHeight = 0;
		mWidth = 0;
		mChannels = 0;
		mSize = 0;
		mHeightTimesWidth = 0;
		mData = 0;
	}
}

/** 
* Copy constructor to copy between two different image template types T and C.
* <p>e.g., 
* <br>OvImageT<int> i1(2,3,1);
* <br>OvImageT<float> i2(i1);
* </p>
*
* @param srcImage source image whose dimensions are to be copied (of a different template type)
* @param copyData copies source data if set to true (default), otherwise returns zeroed image.
* @see OvImageT()
* @see OvImageT(int height, int width, int nColorChannels)
* @see OvImageT(const OvImageT<T>& srcImage, bool copyData)
*/
template<typename T>
template<typename C>
OvImageT<T>::OvImageT(const OvImageT<C>& srcImage, bool copyData)
{
	int height,width,ncolors,srcSize;

	srcImage.getDimensions(height,width,ncolors);
	srcSize = height*width*ncolors;
	
	mHeight = height;
	mWidth = width;
	mChannels= ncolors;
	mSize = srcSize;
	mHeightTimesWidth = mHeight*mWidth;
	
	if(srcSize!=0)
	{
		mData = new T[mSize];
		if(mData == 0)
		{
			mHeight = 0;
			mWidth = 0;
			mChannels = 0;
			mSize = 0;
			mHeightTimesWidth = 0;
		}
		else
		{
			if(copyData)
			{
				for(int k=0;k<mChannels;k++)
					for(int j=0;j<mWidth;j++)
						for(int i=0;i<mHeight;i++)
							(*this)(i,j,k) = (T) srcImage(i,j,k); 
			}
			else
			{
				for(int i=0;i<mSize; i++) mData[i] = 0;
			}
		}
	}
	else
	{
		mHeight = 0;
		mWidth = 0;
		mChannels = 0;
		mSize = 0;
		mHeightTimesWidth = 0;
		mData = 0;
	}
}

/** 
* Destructor of OvImageT.
*/

template<typename T>
OvImageT<T>::~OvImageT()
{
	if(mData!=0) delete [] mData;
}


template<typename T>
bool OvImageT<T>::copyFromAdapter(OvImageAdapter & iadapter)
{
	int i,j,k;
	int height, width, ncolors;
	iadapter.getSize(height, width, ncolors);
	resetDimensions(height,width,ncolors);
	
	if((height!=mHeight)||(width!=mWidth)||(ncolors!=mChannels)) return false;

	for(k=0; k<mChannels;k++)
		for(j=0; j<mWidth;j++)
			for(i=0; i<mHeight;i++)
			{
				(*this)(i,j,k) = (T) iadapter.getPixel(i,j,k); 
			}
	
	return true;
}

template<typename T>
bool OvImageT<T>::copyToAdapter(OvImageAdapter & iadapter)
{
	int i,j,k;
	int height, width, ncolors;
	iadapter.getSize(height, width, ncolors);
	
	if((height!=mHeight)||(width!=mWidth)||(ncolors!=mChannels)) return false;

	for(k=0; k<mChannels;k++) 
		for(j=0; j<mWidth;j++)
			for(i=0; i<mHeight;i++)
			{
				iadapter.setPixel((double)(*this)(i,j,k),i,j,k);
			}
	
	return true;
}



template<typename T>
void OvImageT<T>::getDimensions(int & height, int & width, int & nColorChannels) const
{
	height = mHeight;
	width = mWidth;
	nColorChannels = mChannels;
}

template<typename T>
void OvImageT<T>::resetDimensions(int height, int width, int nColorChannels)
{
	if(mData!=0) {delete [] mData; mData = 0;}

	if(height>0) mHeight = height; else height = 0;
	if(width>0) mWidth = width; else width = 0;
	if(nColorChannels>0) mChannels = nColorChannels; else nColorChannels = 0;
	mSize = mWidth*mHeight*mChannels;	

	if(mSize>0) 
	{
		mData = new T[mSize];
		if(mData == 0)
		{
			mHeight = 0;
			mWidth = 0;
			mChannels = 0;
			mSize = 0;
		}
		else
		{
			for(int i=0; i<mSize; i++) mData[i] = (T) 0;
		}
	}

	mHeightTimesWidth = mWidth*mHeight;
}

template<typename T>
void OvImageT<T>::reshape(int height, int width, int nColorChannels)
{
	if((height*width*nColorChannels)==mSize)
	{
		mHeight = height;
		mWidth = width;
		mChannels = nColorChannels;
		mHeightTimesWidth = mWidth*mHeight;
	}
}


template<typename T>
void OvImageT<T>::setToRandom(double multiplier)
{
	multiplier = multiplier/RAND_MAX;
	for(int i=0; i<mSize; i++) mData[i] = (T) multiplier*rand();
}

template<typename T>
inline T& OvImageT<T>::operator() (int row, int column, int channel)
{
	return *(mData + channel*mHeightTimesWidth + column*mHeight + row); 
}

template<typename T>
inline T& OvImageT<T>::operator() (int row, int column, int channel) const
{
	return *(mData + channel*mHeightTimesWidth + column*mHeight + row); 
}

template<typename T>
OvImageT<T>& OvImageT<T>::operator = (const OvImageT<T> & rhsImage)
{
	if(this == &rhsImage) return (*this); // for a case where lhsimage == rhsimage

	if((rhsImage.mSize>0) && (rhsImage.mData != 0))
	{
		if(mSize != rhsImage.mSize) //reallocate memory only if size is different, else just use what is already there
		{
			if(mData!=0) {delete [] mData; mData = 0;}
			mSize = rhsImage.mSize;
			mData = new T[mSize];
		}
		
		if(mData == 0)
		{
			mHeight = 0;
			mWidth = 0;
			mChannels = 0;
			mSize = 0;
			mHeightTimesWidth = 0;
		}
		else
		{
			mHeight = rhsImage.mHeight;
			mWidth = rhsImage.mWidth;
			mChannels = rhsImage.mChannels; 
			mHeightTimesWidth = rhsImage.mHeightTimesWidth;

			for(int i=0;i<mSize; i++) mData[i] = rhsImage.mData[i];
		}
	}
	return (*this);
}

template<typename T>
template<typename C>
OvImageT<T>& OvImageT<T>::operator = (const OvImageT<C> & rhsImage)
{
	//self-assignment check not necessary since T and C are different, hence not used below

	int height,width,ncolors,rhssize;

	rhsImage.getDimensions(height,width,ncolors);
	rhssize = height*width*ncolors;

	if(rhssize!=0)
	{
		if(mSize != rhssize) //reallocate memory only if size is different, else just use what is already there
		{
			if(mData!=0) {delete [] mData; mData = 0;}
			mSize = rhssize;
			mData = new T[mSize];
		}
		
		if(mData == 0)
		{
			mHeight = 0;
			mWidth = 0;
			mChannels = 0;
			mSize = 0;
			mHeightTimesWidth = 0;
		}
		else
		{
			mHeight = height;
			mWidth = width;
			mChannels = ncolors; 
			mHeightTimesWidth = mHeight*mWidth;

			for(int k=0;k<mChannels;k++)
				for(int j=0;j<mWidth;j++)
					for(int i=0;i<mHeight;i++)
						(*this)(i,j,k) = (T) rhsImage(i,j,k); 
		}
	}
	return (*this);
}


template<typename T>
OvImageT<T>& OvImageT<T>::operator = (const T & rhs)
{
	for(int i=0;i<mSize; i++) mData[i] = rhs;
	return (*this);
}

template<typename T>
bool OvImageT<T>::copyMasked(const OvImageT<bool> & mask, const OvImageT<T> & srcImage)
{
	int maskHeight, maskWidth, maskChannels, srcHeight, srcWidth, srcChannels;
	
	mask.getDimensions(maskHeight,maskWidth,maskChannels);
	srcImage.getDimensions(srcHeight,srcWidth,srcChannels);

	if((mHeight!=maskHeight)||(mWidth!=maskWidth)||(mChannels!=maskChannels)) return false;		
	if((mHeight!=srcHeight)||(mWidth!=srcWidth)||(mChannels!=srcChannels)) return false;		

	for(int k=0; k<mChannels;k++)
		for(int j=0; j<mWidth;j++)
			for(int i=0; i<mHeight;i++)
			{
				if(mask(i,j,k)) (*this)(i,j,k) = srcImage(i,j,k);
			}

	return true;
}

template<typename T>
bool OvImageT<T>::copyMasked(const OvImageT<bool> & mask, const T & value)
{
	int maskHeight, maskWidth, maskChannels;
	mask.getDimensions(maskHeight,maskWidth,maskChannels);
	if((mHeight!=maskHeight)||(mWidth!=maskWidth)||(mChannels!=maskChannels)) return false;		

	for(int k=0; k<mChannels;k++)
		for(int j=0; j<mWidth;j++)
			for(int i=0; i<mHeight;i++)
			{
				if(mask(i,j,k)) (*this)(i,j,k) = value;
			}
	return true;
}

template<typename T>
const OvImageT<T> OvImageT<T>::getSubImage(int rowLo, int rowHi, int columnLo, int columnHi, int channelLo, int channelHi)
{
	int i,j,k,width,height,nchannels;
	OvImageT<T> result;

	if(rowLo<0) rowLo = 0; if(rowLo>=mHeight) rowLo = mHeight-1;
	if(rowHi<0) rowHi = mHeight-1; if(rowHi>=mHeight) rowHi = mHeight-1;	
	if(columnLo<0) columnLo = 0; if(columnLo>=mWidth) columnLo = mWidth-1;
	if(columnHi<0) columnHi = mWidth-1; if(columnHi>=mWidth) columnHi = mWidth-1;	
	if(channelLo<0) channelLo = 0; if(channelLo>=mChannels) channelLo = mChannels-1;
	if(channelHi<0) channelHi = mChannels-1; if(channelHi>=mChannels) channelHi = mChannels-1;	
	
	height = rowHi-rowLo+1;
	width  = columnHi-columnLo+1;
	nchannels = channelHi-channelLo+1;

	if(height<=0) return result;
	if(width<=0) return result;
	if(nchannels<=0) return result;

	result.resetDimensions(height,width,nchannels);

	for(k=channelLo;k<=channelHi;k++)
		for(j=columnLo;j<=columnHi;j++)
			for(i=rowLo;i<=rowHi;i++)
				result(i-rowLo,j-columnLo,k-channelLo) = (*this)(i,j,k);
	
	return result;
}


template<typename T>
void OvImageT<T>::print(void)
{
	using namespace std;
	int i,j,k;

	cout << "Height: " << mHeight << ",Width:" << mWidth << ",Channels:" << mChannels << endl;

	for(k=0;k<mChannels;k++)
	{
		cout << "Channel " << k << endl;
		for(i=0;i<mHeight;i++)
		{
			for(j=0;j<mWidth;j++)
			{
				cout << (*this)(i,j,k) << "\t";
			}
			cout << endl;
		}
	}
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator += (const OvImageT<T> & rhs)
{
	if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
	{
		for(int i=0; i<mSize; i++)
			mData[i] += rhs.mData[i];
	}
	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator += (const T & rhs)
{
	for(int i=0; i<mSize; i++)
		mData[i] += rhs;

	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator -= (const OvImageT<T> & rhs)
{
	if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
	{
		for(int i=0; i<mSize; i++)
			mData[i] -= rhs.mData[i];
	}
	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator -= (const T & rhs)
{
	for(int i=0; i<mSize; i++)
		mData[i] -= rhs;

	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator *= (const OvImageT<T> & rhs)
{
	if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
	{
		for(int i=0; i<mSize; i++)
			mData[i] *= rhs.mData[i];
	}
	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator *= (const T & rhs)
{
	for(int i=0; i<mSize; i++)
		mData[i] *= rhs;

	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator /= (const OvImageT<T> & rhs)
{
	if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
	{
		for(int i=0; i<mSize; i++)
			mData[i] /= rhs.mData[i];
	}
	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator /= (const T & rhs)
{
	for(int i=0; i<mSize; i++)
		mData[i] /= rhs;

	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator ++ ()
{
	for(int i=0; i<mSize; i++)
		mData[i]++;

	return (*this);
}

template<typename T>
OvImageT<T> & OvImageT<T>::operator -- ()
{
	for(int i=0; i<mSize; i++)
		mData[i]--;

	return (*this);
}

template<typename T>
const OvImageT<T> OvImageT<T>::operator ++ (int)
{
	OvImageT<T> oldImage(*this); //make a copy
	for(int i=0; i<mSize; i++) 
		++mData[i];				//increment original

	return (oldImage); //return the copy
}

template<typename T>
const OvImageT<T> OvImageT<T>::operator -- (int)
{
	OvImageT<T> oldImage(*this); //make a copy
	for(int i=0; i<mSize; i++)
		--mData[i];			//decrement original

	return (oldImage); //return the copy
}

template<typename T>
const OvImageT<T> OvImageT<T>::operator - ()
{
	OvImageT<T> result(*this); 

	for(int i=0; i<mSize; i++)
		result.mData[i] = -result.mData[i];

	return (result); 
}

template<typename T>
const OvImageT<T> OvImageT<T>::operator + ()
{
	OvImageT<T> result(*this); 
	return (result); 
}


template<typename T>
const OvImageT<T> operator + (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i1;
	result += i2;
	return result;
}

template<typename T>
const OvImageT<T> operator + (const double i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i2;
	result += (T)i1;
	return result;
}

template<typename T>
const OvImageT<T> operator + (const OvImageT<T> & i1, const double i2)
{
	OvImageT<T> result;
	result = i1;
	result += (T)i2;
	return result;
}

template<typename T>
const OvImageT<T> operator - (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i1;
	result -= i2;
	return result;
}

template<typename T>
const OvImageT<T> operator - (const double i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i2;
	result -= (T)i1;
	return result;
}

template<typename T>
const OvImageT<T> operator - (const OvImageT<T> & i1, const double i2)
{
	OvImageT<T> result;
	result = i1;
	result -= (T)i2;
	return result;
}

template<typename T>
const OvImageT<T> operator * (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i1;
	result *= i2;
	return result;
}

template<typename T>
const OvImageT<T> operator * (const double i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i2;
	result *= (T)i1;
	return result;
}

template<typename T>
const OvImageT<T> operator * (const OvImageT<T> & i1, const double i2)
{
	OvImageT<T> result;
	result = i1;
	result *= (T)i2;
	return result;
}

template<typename T>
const OvImageT<T> operator / (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i1;
	result /= i2;
	return result;
}

template<typename T>
const OvImageT<T> operator / (const double i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i2;
	result /= (T)i1;
	return result;
}

template<typename T>
const OvImageT<T> operator / (const OvImageT<T> & i1, const double i2)
{
	OvImageT<T> result;
	result = i1;
	result /= (T)i2;
	return result;
}

template<typename T>
const OvImageT<bool> operator < (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
	{
		for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] < i2.mData[i]);
	}

	return result;
}

template<typename T>
const OvImageT<bool> operator < (const double i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 < i2.mData[i]);

	return result;
}

template<typename T>
const OvImageT<bool> operator < (const OvImageT<T> & i1, const double i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] < i2);

	return result;
}


template<typename T>
const OvImageT<bool> operator <= (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
	{
		for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] <= i2.mData[i]);
	}

	return result;
}

template<typename T>
const OvImageT<bool> operator <= (const double i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 <= i2.mData[i]);

	return result;
}

template<typename T>
const OvImageT<bool> operator <= (const OvImageT<T> & i1, const double i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] <= i2);

	return result;
}


template<typename T>
const OvImageT<bool> operator > (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
	{
		for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] > i2.mData[i]);
	}

	return result;
}

template<typename T>
const OvImageT<bool> operator > (const double i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 > i2.mData[i]);

	return result;
}

template<typename T>
const OvImageT<bool> operator > (const OvImageT<T> & i1, const double i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] > i2);

	return result;
}


template<typename T>
const OvImageT<bool> operator >= (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
	{
		for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] >= i2.mData[i]);
	}

	return result;
}

template<typename T>
const OvImageT<bool> operator >= (const double i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 >= i2.mData[i]);

	return result;
}

template<typename T>
const OvImageT<bool> operator >= (const OvImageT<T> & i1, const double i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] >= i2);

	return result;
}

template<typename T>
const OvImageT<bool> operator == (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
	{
		for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] == i2.mData[i]);
	}

	return result;
}

template<typename T>
const OvImageT<bool> operator == (const double i1, const OvImageT<T> & i2)
{
	OvImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 == i2.mData[i]);

	return result;
}

template<typename T>
const OvImageT<bool> operator == (const OvImageT<T> & i1, const double i2)
{
	OvImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

	for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] == i2);

	return result;
}

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


template<typename T>
T OvImageT<T>::sumRegion(int rowLo, int rowHi, int columnLo, int columnHi, int channelLo, int channelHi)
{
	int i,j,k;
	T result;

	if(rowLo<0) rowLo = 0; if(rowLo>=mHeight) rowLo = mHeight-1;
	if(rowHi<0) rowHi = mHeight-1; if(rowHi>=mHeight) rowHi = mHeight-1;	
	if(columnLo<0) columnLo = 0; if(columnLo>=mWidth) columnLo = mWidth-1;
	if(columnHi<0) columnHi = mWidth-1; if(columnHi>=mWidth) columnHi = mWidth-1;	
	if(channelLo<0) channelLo = 0; if(channelLo>=mChannels) channelLo = mChannels-1;
	if(channelHi<0) channelHi = mChannels-1; if(channelHi>=mChannels) channelHi = mChannels-1;	
	
	result = 0;

	for(k=channelLo;k<=channelHi;k++)
		for(j=columnLo;j<=columnHi;j++)
			for(i=rowLo;i<=rowHi;i++)
				result += (*this)(i,j,k);
	
	return result;
}

template<typename T>
T OvImageT<T>::sumSingleChannel(int channel)
{
	if((channel<0)||(channel>=mChannels)) return 0;
	return sumRegion(-1,-1,-1,-1,channel,channel);
}

template<typename T>
T OvImageT<T>::sumAll(void)
{
	return sumRegion(-1,-1,-1,-1,-1,-1);
}


template<typename T>
T OvImageT<T>::L1Norm(void)
{
	T result = 0;
	for(int i=0; i<mSize; i++) result += (T) fabs(mData[i]);
	return result;
}

template<typename T>
T OvImageT<T>::L2Norm(void)
{
	T result = 0;
	for(int i=0; i<mSize; i++) result += (mData[i]*mData[i]);
	result = sqrt(result);
	return result;
}

template<typename T> 
const OvImageT<T> cos (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) cos(i1.mData[i]);
	return result;
}

template<typename T> 
const OvImageT<T> sin (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sin(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> tan (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) tan(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> acos (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) acos(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> asin (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) asin(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> atan (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) atan(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> atan2 (const OvImageT<T> & iy, const OvImageT<T> & ix)
{
	OvImageT<T> result(iy,false); //create same-sized copy without copying contents
	if(iy.mSize != ix.mSize) return result; //unequal sized arrays
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) atan2(iy.mData[i],ix.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> cosh (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) cosh(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> sinh (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sinh(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> tanh (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) tanh(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> exp (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) exp(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> log (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) log(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> log10 (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) log10(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> abs (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) fabs(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> ceil (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) ceil(i1.mData[i]);
	return result;
}


template<typename T> 
const OvImageT<T> floor (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) floor(i1.mData[i]);
	return result;
}

template<typename T> 
const OvImageT<T> round (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	T tempValue;
	for(int i=0; i<result.mSize; i++) 
	{
		tempValue = floor(i1.mData[i]);
		result.mData[i] = (T) ((i1.mData[i]-tempValue)<=0.5)?tempValue:ceil(i1.mData[i]);
	}
	return result;
}

template<typename T> 
const OvImageT<T> mod (const OvImageT<T> & i1, double d)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) fmod((double)i1.mData[i],(double)d);
	return result;
}


template<typename T> 
const OvImageT<T> pow (const OvImageT<T> & i1, double p)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) pow((double)i1.mData[i],p);
	return result;
}

template<typename T> 
const OvImageT<T> pow (double p, const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) pow(p,(double)i1.mData[i]);
	return result;
}

template<typename T> 
const OvImageT<T> sqrt (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sqrt(i1.mData[i]);
	return result;
}


/** 
* @relates OvImageT
* Performs 2D convolution on the input image with a given kernel.
* 
* @param kernel a 2D image used as a kernel for convolution
* @param input the input image
* @return convolved image
* @see filter2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
*/
template<typename T> 
const OvImageT<T> convolve2D (const OvImageT<T> & kernel, const OvImageT<T> & input)
{
	int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
	T tempValue;

	OvImageT<T> result(input,false); //create same-sized copy without copying contents

	if((kernel.mSize==0)||(input.mSize==0)) return result;

	iKernelMidpoint = kernel.mHeight/2;
	jKernelMidpoint = kernel.mWidth/2;

	for(k=0;k<result.mChannels;k++)
		for(jResult=0;jResult<result.mWidth;jResult++)
			for(iResult=0;iResult<result.mHeight;iResult++)
			{
				tempValue = 0;

				for(jKernel=0; jKernel<kernel.mWidth; jKernel++)
				{
					jInput = jResult - (jKernel-jKernelMidpoint);
					if((jInput<0)||(jInput>=input.mWidth)) continue;

					for(iKernel=0; iKernel<kernel.mHeight; iKernel++)
					{
						iInput = iResult - (iKernel-iKernelMidpoint);
						if((iInput<0)||(iInput>=input.mHeight)) continue;
						tempValue += (kernel(iKernel,jKernel)*input(iInput,jInput,k));
					}
				}

				result(iResult,jResult,k) = tempValue;
			}

	return result;
}


/** 
* @relates OvImageT
* Performs 2D filtering on the input image with a given kernel.
* 
* @param kernel a 2D image used as a kernel for filtering
* @param input the input image
* @return filtered image
* @see convolve2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
*/
template<typename T> 
const OvImageT<T> filter2D (const OvImageT<T> & kernel, const OvImageT<T> & input)
{
	int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
	T tempValue;

	OvImageT<T> result(input,false); //create same-sized copy without copying contents

	if((kernel.mSize==0)||(input.mSize==0)) return result;

	iKernelMidpoint = kernel.mHeight/2;
	jKernelMidpoint = kernel.mWidth/2;

	for(k=0;k<result.mChannels;k++)
		for(jResult=0;jResult<result.mWidth;jResult++)
			for(iResult=0;iResult<result.mHeight;iResult++)
			{
				tempValue = 0;

				for(jKernel=0; jKernel<kernel.mWidth; jKernel++)
				{
					jInput = jResult + (jKernel-jKernelMidpoint); //only difference from convolve2D is the plus sign
					if((jInput<0)||(jInput>=input.mWidth)) continue;

					for(iKernel=0; iKernel<kernel.mHeight; iKernel++)
					{
						iInput = iResult + (iKernel-iKernelMidpoint); //only difference from convolve2D is the plus sign
						if((iInput<0)||(iInput>=input.mHeight)) continue;
						tempValue += (kernel(iKernel,jKernel)*input(iInput,jInput,k));
					}
				}

				result(iResult,jResult,k) = tempValue;
			}

	return result;
}

template<typename T> 
int medianFilter2DHelperFunc_Compare( const void *arg1, const void *arg2 )
{
	if((*(T*)arg1)<(*(T*)arg2)) return -1;
	else if ((*(T*)arg1)==(*(T*)arg2)) return 0;
	else return 1;
}

template<typename T> 
T medianFilter2DHelperFunc_FindMedian(int n, T*elements)
{
	qsort( (void *) elements, (size_t)n, sizeof(T), medianFilter2DHelperFunc_Compare<T>);

	if(n==0) return 0; 
	else if(n%2) /*odd*/
		return elements[(n-1)/2];
	else
		return elements[n/2];

}


/** 
* @relates OvImageT
* Performs 2D median filtering on the input image, i.e., every output pixel is set to the median value within a rectangular block of pixels around it.
* 
* <p> e.g., 
* <br> i2 = medianFilter2D(i1, 5, 5); 
* </p>
* @param input the input image
* @param filterHeight height of the median filter
* @param filterWidth width of the median filter
* @return filtered image
* @see minFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see maxFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see meanFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see filter2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
* @see convolve2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
*/
template<typename T> 
const OvImageT<T> medianFilter2D (const OvImageT<T> & input, int filterHeight, int filterWidth)
{
	int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
	T*listOfElements, tempValue;
	int count;

	OvImageT<T> result(input,false); //create same-sized copy without copying contents

	if(input.mSize==0) return result;
	if((filterHeight<1)||(filterWidth<1)) return result;
	
	listOfElements = new T[filterHeight*filterWidth];
	if(listOfElements==0) return result; //no memory

	iKernelMidpoint = filterHeight/2;
	jKernelMidpoint = filterWidth/2;

	for(k=0;k<result.mChannels;k++)
		for(jResult=0;jResult<result.mWidth;jResult++)
			for(iResult=0;iResult<result.mHeight;iResult++)
			{
				count = 0;
				for(jKernel=0; jKernel<filterWidth; jKernel++)
				{
					jInput = jResult + (jKernel-jKernelMidpoint); 
					if((jInput<0)||(jInput>=input.mWidth)) continue;

					for(iKernel=0; iKernel<filterHeight; iKernel++)
					{
						iInput = iResult + (iKernel-iKernelMidpoint); 
						if((iInput<0)||(iInput>=input.mHeight)) continue;

						tempValue = input(iInput,jInput,k);
						if(tempValue!=tempValue) continue; //check if value is NaN
						listOfElements[count] = tempValue;
						count++;						
					}
				}				

				result(iResult,jResult,k) = medianFilter2DHelperFunc_FindMedian<T>(count, listOfElements);
			}

	
	if(listOfElements!=0) delete [] listOfElements;
	return result;
}

/** 
* @relates OvImageT
* Performs 2D minimum filtering on the input image, i.e., every output pixel is set to the minimum value within a rectangular block of pixels around it.
* 
* <p> e.g., 
* <br> i2 = minFilter2D(i1, 5, 5); 
* </p>
* @param input the input image
* @param filterHeight height of the filter
* @param filterWidth width of the filter
* @return filtered image
* @see medianFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see maxFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see meanFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see filter2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
* @see convolve2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
*/
template<typename T> 
const OvImageT<T> minFilter2D (const OvImageT<T> & input, int filterHeight, int filterWidth)
{
	int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
	T resultValue, tempValue;

	OvImageT<T> result(input,false); //create same-sized copy without copying contents

	if(input.mSize==0) return result;
	if((filterHeight<1)||(filterWidth<1)) return result;
	
	iKernelMidpoint = filterHeight/2;
	jKernelMidpoint = filterWidth/2;

	for(k=0;k<result.mChannels;k++)
		for(jResult=0;jResult<result.mWidth;jResult++)
			for(iResult=0;iResult<result.mHeight;iResult++)
			{
				resultValue = input(iResult,jResult,k);

				for(jKernel=0; jKernel<filterWidth; jKernel++)
				{
					jInput = jResult + (jKernel-jKernelMidpoint); 
					if((jInput<0)||(jInput>=input.mWidth)) continue;

					for(iKernel=0; iKernel<filterHeight; iKernel++)
					{
						iInput = iResult + (iKernel-iKernelMidpoint); 
						if((iInput<0)||(iInput>=input.mHeight)) continue;

						tempValue = input(iInput,jInput,k);
						if(tempValue!=tempValue) continue; //check if value is NaN

						if(resultValue>tempValue) resultValue = tempValue;
					}
				}				

				result(iResult,jResult,k) = resultValue;
			}
	
	return result;
}


/** 
* @relates OvImageT
* Performs 2D maximum filtering on the input image, i.e., every output pixel is set to the maximum value within a rectangular block of pixels around it.
* 
* <p> e.g., 
* <br> i2 = maxFilter2D(i1, 5, 5); 
* </p>
* @param input the input image
* @param filterHeight height of the filter
* @param filterWidth width of the filter
* @return filtered image
* @see medianFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see minFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see meanFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see filter2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
* @see convolve2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
*/
template<typename T> 
const OvImageT<T> maxFilter2D (const OvImageT<T> & input, int filterHeight, int filterWidth)
{
	int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
	T resultValue, tempValue;

	OvImageT<T> result(input,false); //create same-sized copy without copying contents

	if(input.mSize==0) return result;
	if((filterHeight<1)||(filterWidth<1)) return result;
	
	iKernelMidpoint = filterHeight/2;
	jKernelMidpoint = filterWidth/2;

	for(k=0;k<result.mChannels;k++)
		for(jResult=0;jResult<result.mWidth;jResult++)
			for(iResult=0;iResult<result.mHeight;iResult++)
			{
				resultValue = input(iResult,jResult,k);

				for(jKernel=0; jKernel<filterWidth; jKernel++)
				{
					jInput = jResult + (jKernel-jKernelMidpoint); 
					if((jInput<0)||(jInput>=input.mWidth)) continue;

					for(iKernel=0; iKernel<filterHeight; iKernel++)
					{
						iInput = iResult + (iKernel-iKernelMidpoint); 
						if((iInput<0)||(iInput>=input.mHeight)) continue;

						tempValue = input(iInput,jInput,k);
						if(tempValue!=tempValue) continue; //check if value is NaN

						if(resultValue<tempValue) resultValue = tempValue;
					}
				}				

				result(iResult,jResult,k) = resultValue;
			}
	
	return result;
}

/** 
* @relates OvImageT
* Performs 2D mean filtering on the input image, i.e., every output pixel is set to the mean value of a rectangular block of pixels around it.
* 
* <p> e.g., 
* <br> i2 = meanFilter2D(i1, 5, 5); 
* </p>
* @param input the input image
* @param filterHeight height of the filter
* @param filterWidth width of the filter
* @return filtered image
* @see medianFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see minFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see maxFilter2D(const OvImageT<T> & input, int filterHeight, int filterWidth)
* @see filter2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
* @see convolve2D(const OvImageT<T> & kernel, const OvImageT<T> & input)
*/
template<typename T> 
const OvImageT<T> meanFilter2D (const OvImageT<T> & input, int filterHeight, int filterWidth)
{
	OvImageT<T> result; 
	OvImageT<T> kernel;

	if(input.mSize==0) return result;
	if((filterHeight<1)||(filterWidth<1)) return result;
	
	kernel.resetDimensions(filterHeight, filterWidth);
	kernel = 1;
	kernel /= kernel.L1Norm();

	result = filter2D(kernel,input);
	
	return result;
}

/** 
* @relates OvImageT
* Sums image pixels along a particular dimension (1 = sum rows, 2 = sum columns, 3 = sum channels)
* 
* <p> e.g., 
* <br> i2 = sum(i1,3); 
* <br> Here, i2 has the same height and width as i1, but only one color channel (since we have summed all color channels together)
* <br>
* <br> i2 = sum(i1,1); 
* <br> Here, i2 has the same width and channels as i1, but only one row (since we have summed all rows together)
* </p>
* @param input the input image
* @param dimension the dimension to sum along [1 = sum rows, 2 = sum columns, 3 = sum channels(default)]
* @return summed image
*/
template<typename T> 
const OvImageT<T> sum(const OvImageT<T> & input, int dimension = 3)
{
	OvImageT<T> result;
	int i,j,k;
	T tempVal;

	if((dimension<1)||(dimension>3)) dimension = 3;
	switch(dimension)
	{
		case 1:
			result.resetDimensions(1, input.mWidth, input.mChannels);
			for(k=0; k<input.mChannels;k++)
				for(j=0; j<input.mWidth;j++)
				{
					tempVal = 0;
					for(i=0; i<input.mHeight;i++) tempVal+=input(i,j,k);
					result(0,j,k) = tempVal;
				}					
			break;
		case 2:
			result.resetDimensions(input.mHeight, 1, input.mChannels);
			for(k=0; k<input.mChannels;k++)
				for(i=0; i<input.mHeight;i++)
				{
					tempVal = 0;
					for(j=0; j<input.mWidth;j++) tempVal+=input(i,j,k);
					result(i,0,k) = tempVal;
				}					
			break;
		case 3:
			result.resetDimensions(input.mHeight, input.mWidth, 1);
			for(j=0; j<input.mWidth;j++)
				for(i=0; i<input.mHeight;i++)
				{
					tempVal = 0;
					for(k=0; k<input.mChannels;k++) tempVal+=input(i,j,k);
					result(i,j,0) = tempVal;
				}					
			break;
	}
	return result;
}

/** 
* @relates OvImageT
* Computes mean of image pixels along a particular dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* 
* <p> e.g., 
* <br> i2 = mean(i1,3); 
* <br> Here, i2 has the same height and width as i1, but only one color channel (since we have averaged all color channels together)
* <br>
* <br> i2 = mean(i1,1); 
* <br> Here, i2 has the same width and channels as i1, but only one row (since we have averaged all rows together)
* </p>
* @param input the input image
* @param dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* @return image
*/
template<typename T> 
const OvImageT<T> mean(const OvImageT<T> & input, int dimension = 3)
{
	OvImageT<T> result;
	int i,j,k;
	T tempVal;

	if((dimension<1)||(dimension>3)) dimension = 3;
	switch(dimension)
	{
		case 1:
			result.resetDimensions(1, input.mWidth, input.mChannels);
			for(k=0; k<input.mChannels;k++)
				for(j=0; j<input.mWidth;j++)
				{
					tempVal = 0;
					for(i=0; i<input.mHeight;i++) tempVal+=input(i,j,k);
					result(0,j,k) = tempVal/input.mHeight;
				}					
			break;
		case 2:
			result.resetDimensions(input.mHeight, 1, input.mChannels);
			for(k=0; k<input.mChannels;k++)
				for(i=0; i<input.mHeight;i++)
				{
					tempVal = 0;
					for(j=0; j<input.mWidth;j++) tempVal+=input(i,j,k);
					result(i,0,k) = tempVal/input.mWidth;
				}					
			break;
		case 3:
			result.resetDimensions(input.mHeight, input.mWidth, 1);
			for(j=0; j<input.mWidth;j++)
				for(i=0; i<input.mHeight;i++)
				{
					tempVal = 0;
					for(k=0; k<input.mChannels;k++) tempVal+=input(i,j,k);
					result(i,j,0) = tempVal/input.mChannels;
				}					
			break;
	}
	return result;
}

/** 
* @relates OvImageT
* Computes minimum of image pixels along a particular dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* 
* <p> e.g., 
* <br> i2 = min(i1,3); 
* <br> Here, i2 has the same height and width as i1, but only one color channel 
* <br>
* <br> i2 = min(i1,1); 
* <br> Here, i2 has the same width and channels as i1, but only one row 
* </p>
* @param input the input image
* @param dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* @return image
*/
template<typename T> 
const OvImageT<T> min(const OvImageT<T> & input, int dimension = 3)
{
	OvImageT<T> result;
	int i,j,k;
	T tempVal;

	if((dimension<1)||(dimension>3)) dimension = 3;
	switch(dimension)
	{
		case 1:
			result.resetDimensions(1, input.mWidth, input.mChannels);
			for(k=0; k<input.mChannels;k++)
				for(j=0; j<input.mWidth;j++)
				{
					tempVal = input(0,j,k);
					for(i=1; i<input.mHeight;i++) tempVal=(tempVal<input(i,j,k))?tempVal:input(i,j,k);
					result(0,j,k) = tempVal;
				}					
			break;
		case 2:
			result.resetDimensions(input.mHeight, 1, input.mChannels);
			for(k=0; k<input.mChannels;k++)
				for(i=0; i<input.mHeight;i++)
				{
					tempVal = input(i,0,k);
					for(j=1; j<input.mWidth;j++) tempVal=(tempVal<input(i,j,k))?tempVal:input(i,j,k);
					result(i,0,k) = tempVal;
				}					
			break;
		case 3:
			result.resetDimensions(input.mHeight, input.mWidth, 1);
			for(j=0; j<input.mWidth;j++)
				for(i=0; i<input.mHeight;i++)
				{
					tempVal = input(i,j,0);
					for(k=1; k<input.mChannels;k++) tempVal=(tempVal<input(i,j,k))?tempVal:input(i,j,k);
					result(i,j,0) = tempVal;
				}					
			break;
	}
	return result;
}

/** 
* @relates OvImageT
* Computes maximum of image pixels along a particular dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* 
* <p> e.g., 
* <br> i2 = max(i1,3); 
* <br> Here, i2 has the same height and width as i1, but only one color channel 
* <br>
* <br> i2 = max(i1,1); 
* <br> Here, i2 has the same width and channels as i1, but only one row 
* </p>
* @param input the input image
* @param dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* @return image
*/
template<typename T> 
const OvImageT<T> max(const OvImageT<T> & input, int dimension = 3)
{
	OvImageT<T> result;
	int i,j,k;
	T tempVal;

	if((dimension<1)||(dimension>3)) dimension = 3;
	switch(dimension)
	{
		case 1:
			result.resetDimensions(1, input.mWidth, input.mChannels);
			for(k=0; k<input.mChannels;k++)
				for(j=0; j<input.mWidth;j++)
				{
					tempVal = input(0,j,k);
					for(i=1; i<input.mHeight;i++) tempVal=(tempVal>input(i,j,k))?tempVal:input(i,j,k);
					result(0,j,k) = tempVal;
				}					
			break;
		case 2:
			result.resetDimensions(input.mHeight, 1, input.mChannels);
			for(k=0; k<input.mChannels;k++)
				for(i=0; i<input.mHeight;i++)
				{
					tempVal = input(i,0,k);
					for(j=1; j<input.mWidth;j++) tempVal=(tempVal>input(i,j,k))?tempVal:input(i,j,k);
					result(i,0,k) = tempVal;
				}					
			break;
		case 3:
			result.resetDimensions(input.mHeight, input.mWidth, 1);
			for(j=0; j<input.mWidth;j++)
				for(i=0; i<input.mHeight;i++)
				{
					tempVal = input(i,j,0);
					for(k=1; k<input.mChannels;k++) tempVal=(tempVal>input(i,j,k))?tempVal:input(i,j,k);
					result(i,j,0) = tempVal;
				}					
			break;
	}
	return result;
}


/** 
* @relates OvImageT
* Transposes the image. Each color channel is independently transposed.
* 
* <p> e.g., 
* <br> i2 = transpose(i1); 
* </p>
* @param input the input image
* @return transposed image
*/
template<typename T> 
const OvImageT<T> transpose(const OvImageT<T> & input)
{
	OvImageT<T> result;
	int i,j,k;

	result.resetDimensions(input.mWidth, input.mHeight, input.mChannels); //transpose dimensions

	//fill transpose
	for(k=0; k<result.mChannels;k++)
		for(j=0; j<result.mWidth;j++)
			for(i=0; i<result.mHeight;i++)
			{
				result(i,j,k) = input(j,i,k);
			}

	return result;
	
}

/** 
* @relates OvImageT
* Flips image left-to-right (i.e., about a vertical axis).
* 
* <p> e.g., 
* <br> i2 = flipLR(i1); 
* </p>
* @param input the input image
* @return flipped image
*/
template<typename T> 
const OvImageT<T> flipLR(const OvImageT<T> & input)
{
	OvImageT<T> result(input,false);
	int i,j,k;

	//fill transpose
	for(k=0; k<result.mChannels;k++)
		for(j=0; j<result.mWidth;j++)
			for(i=0; i<result.mHeight;i++)
			{
				result(i,j,k) = input(i,(result.mWidth-1)-j,k);
			}

	return result;	
}

/** 
* @relates OvImageT
* Flips image upside-down (i.e., about a horizontal axis).
* 
* <p> e.g., 
* <br> i2 = flipUD(i1); 
* </p>
* @param input the input image
* @return flipped image
*/
template<typename T> 
const OvImageT<T> flipUD(const OvImageT<T> & input)
{
	OvImageT<T> result(input,false);
	int i,j,k;

	//fill transpose
	for(k=0; k<result.mChannels;k++)
		for(j=0; j<result.mWidth;j++)
			for(i=0; i<result.mHeight;i++)
			{
				result(i,j,k) = input((result.mHeight-1)-i,j,k);
			}

	return result;	
}

/** 
* @relates OvImageT
* Converts multi-channel color image to single-channel gray image by averaging channels.
* <p> e.g., 
* <br> i2 = rgb2gray(i1); 
* </p>
* @param input the input image
* @return gray image
*/
template<typename T> 
const OvImageT<T> rgb2gray(const OvImageT<T> & input)
{
	OvImageT<T> result(input,false);

	result = mean(input,3);	

	return result;	
}

/** 
* @relates OvImageT
* Tiles input image <b>height</b> times along rows, <b>width</b> times along columns and <b>channels</b> times along color channels.
* <p> e.g., 
* <br> i2 = repmat(i1,2,2,1);
* <br> Here, i2 is twice as large as i1, and is basically a 2x2 tiled copy of i1.
* </p>
* @param input the input image
* @param height number of vertical tiles (default = 1)
* @param width number of horizontal tiles (default = 1)
* @param channels number of color channel tiles (default = 1)
* @return image
*/
template<typename T> 
const OvImageT<T> repmat (const OvImageT<T> & input, int height=1, int width=1, int channels=1)
{
	OvImageT<T> result;
	int i,j,k;

	result.resetDimensions(input.mHeight*height, input.mWidth*width, input.mChannels*channels); 

	for(k=0; k<result.mChannels;k++)
		for(j=0; j<result.mWidth;j++)
			for(i=0; i<result.mHeight;i++)
			{
				result(i,j,k) = input(i%input.mHeight,j%input.mWidth,k%input.mChannels);
			}

	return result;
}

/** 
* @relates OvImageT
* Creates a translated copy of an image.
* <p> e.g., 
* <br> i2 = shiftImageXY(i1,10,20);
* <br> Here, i2 is i1 shifted vertically down by 10 pixels and to the right by 20 pixels.
* <br> Note: i1 and i2 have the same dimensions.
* </p>
* @param input the input image
* @param rows vertical translation in pixels (default = 0)
* @param columns horizontal translation in pixels (default = 0)
* @return translated image
*/
template<typename T> 
const OvImageT<T> shiftImageXY (const OvImageT<T> & input, int rows=0, int columns=0)
{	
	OvImageT<T> result(input,false);
	int i,j,k, iLow, iHigh, jLow, jHigh;

	if(rows>=0) {iLow = rows; iHigh = result.mHeight;}
	else {iLow = 0; iHigh = result.mHeight-rows;}

	if(columns>=0) {jLow = columns; jHigh = result.mWidth;}
	else {jLow = 0; jHigh = result.mWidth-columns;}

	for(k=0; k<result.mChannels;k++)
		for(j=jLow; j<jHigh;j++)
			for(i=iLow; i<iHigh;i++)
			{
				result(i,j,k) = input(i-rows,j-columns,k); //translate
			}

	return result;	
}

/** 
* @relates OvImageT
* Rescales the input image by a certain scale using the nearest-neighbor method and optional pre-smoothing.
* <p> e.g., 
* <br> i2 = resizeNearestNbr(i1, 0.5, true);
* <br> Here, i2 is half the size of i1 and presmoothing is set to true.
* <br> Pre-smoothing performs appropriate gaussian smoothing before rescaling and gives better results but is slower.
* </p>
* @param input the input image
* @param scale the magnification factor (in the range 0.01 to 100)
* @param preSmooth if true performs gaussian pre-smoothing. if false (default), no smoothing is performed.
* @return image
* @see resizeBilinear(const OvImageT<T> & input, double scale, bool preSmooth)
*/
template<typename T> 
const OvImageT<T> resizeNearestNbr(const OvImageT<T> & input, double scale, bool preSmooth=false)
{
	int i,j,k;
	OvImageT<T> result, intermediate, kernel;
	if((scale<0.01)||(scale>100)) return result; //enforce scaling limits: pretty large limits :)

	result.resetDimensions((int)floor(input.mHeight*scale),(int)floor(input.mWidth*scale),input.mChannels);

	if(preSmooth && (scale<1)) //smooth if presmooth is set and we are shrinking only; no need to smooth when magnifying image
	{
		kernel.setToGaussian((int)ceil((1/scale)*3),1/scale);
		intermediate = filter2D(kernel,input);		

		for(k=0; k<result.mChannels;k++)
			for(j=0; j<result.mWidth;j++)
				for(i=0; i<result.mHeight;i++)
				{
					result(i,j,k) = (T) intermediate(round(i/scale),round(j/scale),k); //scale
				}		
	}
	else
	{
		for(k=0; k<result.mChannels;k++)
			for(j=0; j<result.mWidth;j++)
				for(i=0; i<result.mHeight;i++)
				{
					result(i,j,k) = (T) input(round(i/scale),round(j/scale),k); //scale
				}		
	}

	return result;
}

/** 
* @relates OvImageT
* Rescales the input image by a certain scale using bilinear interpolation and optional pre-smoothing.
* <p> e.g., 
* <br> i2 = resizeBilinear(i1, 0.5, true);
* <br> Here, i2 is half the size of i1 and presmoothing is set to true.
* <br> Pre-smoothing performs appropriate gaussian smoothing before rescaling and gives better results but is slower.
* </p>
* @param input the input image
* @param scale the magnification factor (in the range 0.01 to 100)
* @param preSmooth if true performs gaussian pre-smoothing. if false (default), no smoothing is performed.
* @return image
* @see resizeNearestNbr(const OvImageT<T> & input, double scale, bool preSmooth)
*/
template<typename T> 
const OvImageT<T> resizeBilinear(const OvImageT<T> & input, double scale, bool preSmooth=false)
{	
	int i,j,k;
	double iInput,jInput;
	int iInputLo,jInputLo,iInputHi,jInputHi;
	T interp1, interp2;

	OvImageT<T> result, intermediate, kernel;
	if((scale<0.01)||(scale>100)) return result; //enforce scaling limits: pretty large limits :)

	result.resetDimensions((int)floor(input.mHeight*scale),(int)floor(input.mWidth*scale),input.mChannels);

	if(preSmooth && (scale<1)) //smooth if presmooth is set and we are shrinking only; no need to smooth when magnifying image
	{
		kernel.setToGaussian((int)ceil((1/scale)*3),1/scale);
		intermediate = filter2D(kernel,input);		

		for(k=0; k<result.mChannels;k++)
			for(j=0; j<result.mWidth;j++)
				for(i=0; i<result.mHeight;i++)
				{
					iInput = i/scale; iInputLo = (int)iInput; iInputHi = (int)(iInput+1);
					jInput = j/scale; jInputLo = (int)jInput; jInputHi = (int)(jInput+1);

					interp1 = (T) ((iInputHi-iInput)*intermediate(iInputLo,jInputLo,k) + (iInput-iInputLo)*intermediate(iInputHi,jInputLo,k));
					interp2 = (T) ((iInputHi-iInput)*intermediate(iInputLo,jInputHi,k) + (iInput-iInputLo)*intermediate(iInputHi,jInputHi,k));

					result(i,j,k) = (T) ((jInputHi-jInput)*interp1+(jInput-jInputLo)*interp2);
				}		
	}
	else
	{
		for(k=0; k<result.mChannels;k++)
			for(j=0; j<result.mWidth;j++)
				for(i=0; i<result.mHeight;i++)
				{
					iInput = i/scale; iInputLo = (int)iInput; iInputHi = (int)(iInput+1);
					jInput = j/scale; jInputLo = (int)jInput; jInputHi = (int)(jInput+1);

					interp1 = (T) ((iInputHi-iInput)*input(iInputLo,jInputLo,k) + (iInput-iInputLo)*input(iInputHi,jInputLo,k));
					interp2 = (T) ((iInputHi-iInput)*input(iInputLo,jInputHi,k) + (iInput-iInputLo)*input(iInputHi,jInputHi,k));

					result(i,j,k) = (T) ((jInputHi-jInput)*interp1+(jInput-jInputLo)*interp2);
				}		
	}

	return result;
}


template<typename T> 
void OvImageT<T> ::setToMeshgridX (T x1, T x2, T y1, T y2, T dx, T dy)
{
	int i,j,height,width;

	width = 1 + (int) floor((double)(x2-x1)/dx);
	height  = 1 + (int) floor((double)(y2-y1)/dy);

	printf("Yes: %d,%d\n", height,width);

	this->resetDimensions(height, width, 1); 

	for(j=0; j<width; j++)
		for(i=0; i<height; i++)
		{
			(*this)(i,j) = x1 + j*dx;
		}
}

template<typename T> 
void OvImageT<T> ::setToMeshgridY (T x1, T x2, T y1, T y2, T dx, T dy)
{
	int i,j,height,width;

	width = 1 + (int) floor((double)(x2-x1)/dx);
	height  = 1 + (int) floor((double)(y2-y1)/dy);

	printf("Yes: %f,%f,%f\n", x1,x2,dx);

	this->resetDimensions(height, width, 1); 

	for(j=0; j<mWidth; j++)
		for(i=0; i<mHeight; i++)
		{
			(*this)(i,j) = y1 + i*dy;
		}
}

template<typename T> 
void OvImageT<T> ::setToGaussian(int size, double sigma)
{
	double x,y;
	double halfsize;

	this->resetDimensions(size, size, 1); 
	halfsize = (size-1)/2.0;	

	for(int j=0; j<mWidth; j++)
		for(int i=0; i<mHeight; i++)
		{
			x = j-halfsize;
			y = i-halfsize;
			(*this)(i,j) = (T) exp(-0.5*(x*x+y*y)/(sigma*sigma));
		}	
	(*this) /= this->L1Norm();
}

template<typename T> 
void OvImageT<T> ::setToGaborX(int size, double sigma, double period, double phaseshift=0)
{
	double x,y;
	double halfsize, tempVal, normalizer;
	double pi = 2 * acos(0.0);

	if(size<=0) return;
	if(sigma<=0) return;
	if(period<=0) return;

	phaseshift = phaseshift*pi/180; //convert phaseshift from degrees to radians

	this->resetDimensions(size, size, 1); 
	halfsize = (size-1)/2.0;	

	normalizer = 0;
	for(int j=0; j<mWidth; j++)
		for(int i=0; i<mHeight; i++)
		{
			x = j-halfsize;
			y = i-halfsize;
		
			tempVal = exp(-0.5*(x*x+y*y)/(sigma*sigma)); //gaussian envelope
			normalizer += tempVal; //normalise using gaussian envelope only
			
			//now multiply by sinusoid to get gabor function
			(*this)(i,j) = (T) (tempVal*sin(2.0*pi*x/period + phaseshift));
		}	

	(*this) /= (T) normalizer;
}

template<typename T> 
void OvImageT<T> ::setToGaborY(int size, double sigma, double period, double phaseshift=0)
{
	double x,y;
	double halfsize, tempVal, normalizer;
	double pi = 2 * acos(0.0);

	if(size<=0) return;
	if(sigma<=0) return;
	if(period<=0) return;

	phaseshift = phaseshift*pi/180; //convert phaseshift from degrees to radians

	this->resetDimensions(size, size, 1); 
	halfsize = (size-1)/2.0;	

	normalizer = 0;
	for(int j=0; j<mWidth; j++)
		for(int i=0; i<mHeight; i++)
		{
			x = j-halfsize;
			y = i-halfsize;
		
			tempVal = exp(-0.5*(x*x+y*y)/(sigma*sigma)); //gaussian envelope
			normalizer += tempVal; //normalise using gaussian envelope only
			
			//now multiply by sinusoid to get gabor function
			(*this)(i,j) = (T) (tempVal*sin(2.0*pi*y/period + phaseshift));
		}	

	(*this) /= (T) normalizer;
}

#endif //__OVIMAGET_CPP