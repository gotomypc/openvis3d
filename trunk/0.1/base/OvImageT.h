#ifndef __OVIMAGET_H
#define __OVIMAGET_H

template<typename T>
class OvImageT
{
public:

	OvImageT();
	OvImageT(int height, int width, int nColorChannels);
	OvImageT(const OvImageT<T>& srcImage, bool CopyData=true);

	template<typename C>
	OvImageT(const OvImageT<C>& srcImage, bool CopyData=true);

	virtual ~OvImageT();

	//methods
	void getDimensions(int & height, int & width, int & nColorChannels) const;
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
	template <typename C>
	OvImageT<T>& operator = (const OvImageT<C> & rhsImage); //assignment
	
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

	template<typename C> friend const OvImageT<C> operator + (const OvImageT<C> & i1, const OvImageT<C> & i2); 
	template<typename C> friend const OvImageT<C> operator + (const double i1, const OvImageT<C> & i2); 
	template<typename C> friend const OvImageT<C> operator + (const OvImageT<C> & i1, const double i2); 
	template<typename C> friend const OvImageT<C> operator - (const OvImageT<C> & i1, const OvImageT<C> & i2); 
	template<typename C> friend const OvImageT<C> operator - (const double i1, const OvImageT<C> & i2); 
	template<typename C> friend const OvImageT<C> operator - (const OvImageT<C> & i1, const double i2); 
	template<typename C> friend const OvImageT<C> operator * (const OvImageT<C> & i1, const OvImageT<C> & i2); 
	template<typename C> friend const OvImageT<C> operator * (const double i1, const OvImageT<C> & i2); 
	template<typename C> friend const OvImageT<C> operator * (const OvImageT<C> & i1, const double i2); 
	template<typename C> friend const OvImageT<C> operator / (const OvImageT<C> & i1, const OvImageT<C> & i2); 
	template<typename C> friend const OvImageT<C> operator / (const double i1, const OvImageT<C> & i2); 
	template<typename C> friend const OvImageT<C> operator / (const OvImageT<C> & i1, const double i2); 

	template<typename C> friend const OvImageT<C> cos (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> sin (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> tan (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> acos (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> asin (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> atan (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> atan2 (const OvImageT<C> & iy, const OvImageT<C> & ix);
	template<typename C> friend const OvImageT<C> cosh (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> sinh (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> tanh (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> exp (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> log (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> log10 (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> abs (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> ceil (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> floor (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> round (const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> mod (const OvImageT<C> & i1, double d);
	template<typename C> friend const OvImageT<C> pow (const OvImageT<C> & i1, double p);
	template<typename C> friend const OvImageT<C> pow (double p, const OvImageT<C> & i1);
	template<typename C> friend const OvImageT<C> sqrt (const OvImageT<C> & i1);

	template<typename C> friend const OvImageT<C> convolve2D (const OvImageT<C> & ikernel, const OvImageT<C> & input);
	template<typename C> friend const OvImageT<C> filter2D (const OvImageT<C> & ikernel, const OvImageT<C> & input);
	template<typename C> friend const OvImageT<C> medianFilter2D (const OvImageT<C> & input, int filterHeight, int filterWidth);
	
	template<typename C> friend const OvImageT<C> sum(const OvImageT<C> & input, int dimension);
	template<typename C> friend const OvImageT<C> min(const OvImageT<C> & input, int dimension);
	template<typename C> friend const OvImageT<C> max(const OvImageT<C> & input, int dimension);
	template<typename C> friend const OvImageT<C> mean(const OvImageT<C> & input, int dimension);

	template<typename C> friend const OvImageT<C> transpose(const OvImageT<C> & input);
	template<typename C> friend const OvImageT<C> flipLR(const OvImageT<C> & input);
	template<typename C> friend const OvImageT<C> flipUD(const OvImageT<C> & input);
	template<typename C> friend const OvImageT<C> rgb2gray(const OvImageT<C> & input);

	template<typename C> friend const OvImageT<C> repmat (const OvImageT<C> & input, int height, int width, int channels);

	//template<typename T> friend const OvImageT<T> resizeNearestNbr(const OvImageT<T> & input, double scale);	
	//template<typename T> friend const OvImageT<T> resizeBilinear(const OvImageT<T> & input, double scale);	

	//desired functionality
	//gabor
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

