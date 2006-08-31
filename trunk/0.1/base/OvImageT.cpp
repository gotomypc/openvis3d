#include <math.h>
#include <stdlib.h>

template<class T>
OvImageT<T>::OvImageT()
: mHeight(0), mWidth(0), mChannels(0), mSize(0), mHeightTimesWidth(0), mData(0)
{
}

template<class T>
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

template<class T>
OvImageT<T>::OvImageT(const OvImageT<T>& srcImage, bool copyData=true)
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

template<class T>
OvImageT<T>::~OvImageT()
{
	if(mData!=0) delete [] mData;
}

template<class T>
void OvImageT<T>::getDimensions(int & height, int & width, int & nColorChannels)
{
	height = mHeight;
	width = mWidth;
	nColorChannels = mChannels;
}

template<class T>
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

template<class T>
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


template<class T>
void OvImageT<T>::fillWithRandomNumbers(double multiplier)
{
	multiplier = multiplier/RAND_MAX;
	for(int i=0; i<mSize; i++) mData[i] = (T) multiplier*rand();
}

template<class T>
inline T& OvImageT<T>::operator() (int row, int column, int channel)
{
	return *(mData + channel*mHeightTimesWidth + column*mHeight + row); 
}

template<class T>
inline T& OvImageT<T>::operator() (int row, int column, int channel) const
{
	return *(mData + channel*mHeightTimesWidth + column*mHeight + row); 
}

template<class T>
OvImageT<T>& OvImageT<T>::operator = (const OvImageT<T> & rhsImage)
{
	if(this == &rhsImage) return (*this); // for a case where lhsimage == rhsimage

	if((rhsImage.mSize>0) && (rhsImage.mData != 0))
	{
		if(mSize != rhsImage.mSize) //reallocate memory only if size is different, else just use what is already there
		{
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

template<class T>
OvImageT<T>& OvImageT<T>::operator = (const T & rhs)
{
	for(int i=0;i<mSize; i++) mData[i] = rhs;
	return (*this);
}


template<class T>
void OvImageT<T>::print(void)
{
	int i,j,k;

	printf("\nHeight:%d,Width:%d,Ncolors:%d\n",mHeight,mWidth,mChannels);

	for(k=0;k<mChannels;k++)
	{
		printf("Channel %d\n", k);
		for(i=0;i<mHeight;i++)
		{
			for(j=0;j<mWidth;j++)
			{
				printf("%f\t", (*this)(i,j,k));
			}
			printf("\n");
		}
	}
}

template<class T>
OvImageT<T> & OvImageT<T>::operator += (const OvImageT<T> & rhs)
{
	if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
	{
		for(int i=0; i<mSize; i++)
			mData[i] += rhs.mData[i];
	}
	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator += (const T & rhs)
{
	for(int i=0; i<mSize; i++)
		mData[i] += rhs;

	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator -= (const OvImageT<T> & rhs)
{
	if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
	{
		for(int i=0; i<mSize; i++)
			mData[i] -= rhs.mData[i];
	}
	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator -= (const T & rhs)
{
	for(int i=0; i<mSize; i++)
		mData[i] -= rhs;

	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator *= (const OvImageT<T> & rhs)
{
	if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
	{
		for(int i=0; i<mSize; i++)
			mData[i] *= rhs.mData[i];
	}
	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator *= (const T & rhs)
{
	for(int i=0; i<mSize; i++)
		mData[i] *= rhs;

	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator /= (const OvImageT<T> & rhs)
{
	if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
	{
		for(int i=0; i<mSize; i++)
			mData[i] /= rhs.mData[i];
	}
	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator /= (const T & rhs)
{
	for(int i=0; i<mSize; i++)
		mData[i] /= rhs;

	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator ++ ()
{
	for(int i=0; i<mSize; i++)
		mData[i]++;

	return (*this);
}

template<class T>
OvImageT<T> & OvImageT<T>::operator -- ()
{
	for(int i=0; i<mSize; i++)
		mData[i]--;

	return (*this);
}

template<class T>
const OvImageT<T> OvImageT<T>::operator ++ (int)
{
	OvImageT<T> oldImage(*this); //make a copy
	for(int i=0; i<mSize; i++) 
		++mData[i];				//increment original

	return (oldImage); //return the copy
}

template<class T>
const OvImageT<T> OvImageT<T>::operator -- (int)
{
	OvImageT<T> oldImage(*this); //make a copy
	for(int i=0; i<mSize; i++)
		--mData[i];			//decrement original

	return (oldImage); //return the copy
}

template<class T>
const OvImageT<T> OvImageT<T>::operator - ()
{
	OvImageT<T> result(*this); 

	for(int i=0; i<mSize; i++)
		result.mData[i] = -result.mData[i];

	return (result); 
}

template<class T>
const OvImageT<T> OvImageT<T>::operator + ()
{
	OvImageT<T> result(*this); 
	return (result); 
}


template<class T>
const OvImageT<T> operator + (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i1;
	result += i2;
	return result;
}

template<class T>
const OvImageT<T> operator + (const double i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i2;
	result += (T)i1;
	return result;
}

template<class T>
const OvImageT<T> operator + (const OvImageT<T> & i1, const double i2)
{
	OvImageT<T> result;
	result = i1;
	result += (T)i2;
	return result;
}

template<class T>
const OvImageT<T> operator - (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i1;
	result -= i2;
	return result;
}

template<class T>
const OvImageT<T> operator - (const double i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i2;
	result -= (T)i1;
	return result;
}

template<class T>
const OvImageT<T> operator - (const OvImageT<T> & i1, const double i2)
{
	OvImageT<T> result;
	result = i1;
	result -= (T)i2;
	return result;
}

template<class T>
const OvImageT<T> operator * (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i1;
	result *= i2;
	return result;
}

template<class T>
const OvImageT<T> operator * (const double i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i2;
	result *= (T)i1;
	return result;
}

template<class T>
const OvImageT<T> operator * (const OvImageT<T> & i1, const double i2)
{
	OvImageT<T> result;
	result = i1;
	result *= (T)i2;
	return result;
}

template<class T>
const OvImageT<T> operator / (const OvImageT<T> & i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i1;
	result /= i2;
	return result;
}

template<class T>
const OvImageT<T> operator / (const double i1, const OvImageT<T> & i2)
{
	OvImageT<T> result;
	result = i2;
	result /= (T)i1;
	return result;
}

template<class T>
const OvImageT<T> operator / (const OvImageT<T> & i1, const double i2)
{
	OvImageT<T> result;
	result = i1;
	result /= (T)i2;
	return result;
}

template<class T>
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

template<class T>
T OvImageT<T>::sumSingleChannel(int channel)
{
	if((channel<0)||(channel>=mChannels)) return 0;
	return sumRegion(-1,-1,-1,-1,channel,channel);
}

template<class T>
T OvImageT<T>::sumAll(void)
{
	return sumRegion(-1,-1,-1,-1,-1,-1);
}


template<class T>
T OvImageT<T>::L1Norm(void)
{
	T result = 0;
	for(int i=0; i<mSize; i++) result += (T) fabs(mData[i]);
	return result;
}

template<class T>
T OvImageT<T>::L2Norm(void)
{
	T result = 0;
	for(int i=0; i<mSize; i++) result += (mData[i]*mData[i]);
	result = sqrt(result);
	return result;
}

template<class T> 
const OvImageT<T> cos (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) cos(i1.mData[i]);
	return result;
}

template<class T> 
const OvImageT<T> sin (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sin(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> tan (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) tan(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> acos (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) acos(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> asin (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) asin(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> atan (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) atan(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> atan2 (const OvImageT<T> & iy, const OvImageT<T> & ix)
{
	OvImageT<T> result(iy,false); //create same-sized copy without copying contents
	if(iy.mSize != ix.mSize) return result; //unequal sized arrays
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) atan2(iy.mData[i],ix.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> cosh (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) cosh(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> sinh (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sinh(i1.mData[i]);
	return result;
}


template<class T> const OvImageT<T> tanh (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) tanh(i1.mData[i]);
	return result;
}

template<class T> 
const OvImageT<T> exp (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) exp(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> log (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) log(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> log10 (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) log10(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> abs (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) fabs(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> ceil (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) ceil(i1.mData[i]);
	return result;
}


template<class T> 
const OvImageT<T> floor (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) floor(i1.mData[i]);
	return result;
}

template<class T> 
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

template<class T> 
const OvImageT<T> mod (const OvImageT<T> & i1, double d)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) fmod((double)i1.mData[i],(double)d);
	return result;
}


template<class T> 
const OvImageT<T> pow (const OvImageT<T> & i1, double p)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) pow((double)i1.mData[i],p);
	return result;
}

template<class T> 
const OvImageT<T> pow (double p, const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) pow(p,(double)i1.mData[i]);
	return result;
}

template<class T> 
const OvImageT<T> sqrt (const OvImageT<T> & i1)
{
	OvImageT<T> result(i1,false); //create same-sized copy without copying contents
	for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sqrt(i1.mData[i]);
	return result;
}


template<class T> 
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


template<class T> 
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

template<class T> 
int medianFilter2DHelperFunc_Compare( const void *arg1, const void *arg2 )
{
	if((*(T*)arg1)<(*(T*)arg2)) return -1;
	else if ((*(T*)arg1)==(*(T*)arg2)) return 0;
	else return 1;
}

template<class T> 
T medianFilter2DHelperFunc_FindMedian(int n, T*elements)
{
	qsort( (void *) elements, (size_t)n, sizeof(T), medianFilter2DHelperFunc_Compare<T>);

	if(n==0) return 0; 
	else if(n%2) /*odd*/
		return elements[(n-1)/2];
	else
		return elements[n/2];

}


template<class T> 
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

	
	if(listOfElements==0) delete [] listOfElements;
	return result;
}

template<class T> 
const OvImageT<T> sum(const OvImageT<T> & input, int dimension)
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

template<class T> 
const OvImageT<T> mean(const OvImageT<T> & input, int dimension)
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

template<class T> 
const OvImageT<T> min(const OvImageT<T> & input, int dimension)
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

template<class T> 
const OvImageT<T> max(const OvImageT<T> & input, int dimension)
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



template<class T> 
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

template<class T> 
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

template<class T> 
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

template<class T> 
const OvImageT<T> rgb2gray(const OvImageT<T> & input)
{
	OvImageT<T> result(input,false);

	result = mean(input,3);	

	return result;	
}

template<class T> 
const OvImageT<T> repmat (const OvImageT<T> & input, int height, int width, int channels)
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

template<class T> 
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

template<class T> 
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

template<class T> 
void OvImageT<T> ::setToGaussian(int size, float sigma)
{
	float x,y;
	float halfsize;

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

