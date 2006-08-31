#ifndef __OVIMAGET_H
#define __OVIMAGET_H

#include <string>

template<class T>
class OvImageT
{
public:

	OvImageT();
	OvImageT(int height, int width, int nColorChannels);
	OvImageT(const OvImageT<T>& srcImage, bool CopyData=true);

	virtual ~OvImageT();

	//methods
	void getDimensions(int & height, int & width, int & nColorChannels);
	void resetDimensions(int height, int width, int nColorChannels = 1);
	void reshape(int height, int width, int nColorChannels = 1); 
	void fillWithRandomNumbers(double multiplier = 1.0);
	void setToMeshgridX (T x1, T x2, T y1, T y2, T dx = 1, T dy = 1);
	void setToMeshgridY (T x1, T x2, T y1, T y2, T dx = 1, T dy = 1);
	void setToGaussian(int size, float sigma);	

	T sumRegion(int rowLo=-1, int rowHi=-1, int columnLo=-1, int columnHi=-1, int channelLo=-1, int channelHi=-1);
	T sumSingleChannel(int channel);
	T sumAll(void);
	T L1Norm(void);
	T L2Norm(void);

	//for debugging
	void print(void);

	//special operators	
	inline T& operator() (int row, int column = 0, int channel = 0); //allows easy indexing into the image
	inline T& operator() (int row, int column = 0, int channel = 0) const; //allows easy indexing into the image
	OvImageT<T>& operator = (const OvImageT<T> & rhsImage); //assignment
	OvImageT<T>& operator = (const T & rhs); //assignment
	
	//arithmetic operators	
	OvImageT<T> & operator += (const OvImageT<T> & rhs); 
	OvImageT<T> & operator += (const T & rhs); 
	OvImageT<T> & operator -= (const OvImageT<T> & rhs); 
	OvImageT<T> & operator -= (const T & rhs); 
	OvImageT<T> & operator *= (const OvImageT<T> & rhs); 
	OvImageT<T> & operator *= (const T & rhs); 
	OvImageT<T> & operator /= (const OvImageT<T> & rhs); 
	OvImageT<T> & operator /= (const T & rhs); 

	OvImageT<T> & operator ++ (); 	
	OvImageT<T> & operator -- (); 
	const OvImageT<T> operator ++ (int); 
	const OvImageT<T> operator -- (int); 
	
	const OvImageT<T> operator - (); 
	const OvImageT<T> operator + ();

	template<class T> friend const OvImageT<T> operator + (const OvImageT<T> & i1, const OvImageT<T> & i2); 
	template<class T> friend const OvImageT<T> operator + (const double i1, const OvImageT<T> & i2); 
	template<class T> friend const OvImageT<T> operator + (const OvImageT<T> & i1, const double i2); 
	template<class T> friend const OvImageT<T> operator - (const OvImageT<T> & i1, const OvImageT<T> & i2); 
	template<class T> friend const OvImageT<T> operator - (const double i1, const OvImageT<T> & i2); 
	template<class T> friend const OvImageT<T> operator - (const OvImageT<T> & i1, const double i2); 
	template<class T> friend const OvImageT<T> operator * (const OvImageT<T> & i1, const OvImageT<T> & i2); 
	template<class T> friend const OvImageT<T> operator * (const double i1, const OvImageT<T> & i2); 
	template<class T> friend const OvImageT<T> operator * (const OvImageT<T> & i1, const double i2); 
	template<class T> friend const OvImageT<T> operator / (const OvImageT<T> & i1, const OvImageT<T> & i2); 
	template<class T> friend const OvImageT<T> operator / (const double i1, const OvImageT<T> & i2); 
	template<class T> friend const OvImageT<T> operator / (const OvImageT<T> & i1, const double i2); 

	template<class T> friend const OvImageT<T> cos (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> sin (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> tan (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> acos (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> asin (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> atan (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> atan2 (const OvImageT<T> & iy, const OvImageT<T> & ix);
	template<class T> friend const OvImageT<T> cosh (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> sinh (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> tanh (const OvImageT<T> & i1);	template<class T> friend const OvImageT<T> exp (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> log (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> log10 (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> abs (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> ceil (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> floor (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> round (const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> mod (const OvImageT<T> & i1, double d);
	template<class T> friend const OvImageT<T> pow (const OvImageT<T> & i1, double p);
	template<class T> friend const OvImageT<T> pow (double p, const OvImageT<T> & i1);
	template<class T> friend const OvImageT<T> sqrt (const OvImageT<T> & i1);

	template<class T> friend const OvImageT<T> convolve2D (const OvImageT<T> & ikernel, const OvImageT<T> & input);
	template<class T> friend const OvImageT<T> filter2D (const OvImageT<T> & ikernel, const OvImageT<T> & input);
	template<class T> friend const OvImageT<T> medianFilter2D (const OvImageT<T> & input, int filterHeight, int filterWidth);
	
	template<class T> friend const OvImageT<T> sum(const OvImageT<T> & input, int dimension = 3);
	template<class T> friend const OvImageT<T> min(const OvImageT<T> & input, int dimension = 3);
	template<class T> friend const OvImageT<T> max(const OvImageT<T> & input, int dimension = 3);
	template<class T> friend const OvImageT<T> mean(const OvImageT<T> & input, int dimension = 3);

	template<class T> friend const OvImageT<T> transpose(const OvImageT<T> & input);
	template<class T> friend const OvImageT<T> flipLR(const OvImageT<T> & input);
	template<class T> friend const OvImageT<T> flipUD(const OvImageT<T> & input);
	template<class T> friend const OvImageT<T> rgb2gray(const OvImageT<T> & input);

	template<class T> friend const OvImageT<T> repmat (const OvImageT<T> & input, int height=1, int width=1, int channels=1);

	//template<class T> friend const OvImageT<T> resize(const OvImageT<T> & input, int height, int width, string interpolation_method);
	//template<class T> friend const OvImageT<T> resize(const OvImageT<T> & input, double scale, string interpolation_method);
	//template<class T> friend const OvImageT<T> resize(const OvImageT<T> & input, int height, int width, string interpolation_method);

	//desired functionality
	//gaussian, gabor
	//resize (resample), rotate, transform
	//generalized nonlinear functional filter	


protected:
	int  mHeight;			//height of the image
	int  mWidth;			//width of the image
	int  mChannels;			//number of color channels or dimensions (e.g., 1 for grayscale, 3 for RGB)

	//for convenience
	int  mHeightTimesWidth;	//mWidth*mHeight
	int  mSize;				//mWidth*mHeight*mChannels

	T   *mData;				//image data
};

#include "OvImageT.cpp" //for definitions

#endif //__OVIMAGET_H

