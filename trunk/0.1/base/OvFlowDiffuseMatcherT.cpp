#include "OvImageT.h"

template<typename T>
OvFlowDiffuseMatcherT<T>::OvFlowDiffuseMatcherT()
{
}

template<typename T>
OvFlowDiffuseMatcherT<T>::~OvFlowDiffuseMatcherT()
{
}

template<typename T>
bool OvFlowDiffuseMatcherT<T>::doMatching(OvImageT<T> & i1, OvImageT<T> & i2, double minshiftX, double maxshiftX, double minshiftY, double maxshiftY, OvImageT<double> & u1, OvImageT<double> & v1, OvImageT<double> & o1, OvImageT<double> & u2, OvImageT<double> & v2,  OvImageT<double> & o2)
{
	int height, width, channels;
	int i,j,iproj,jproj;

	OvImageT<double> iMatch, iGoodnessVert, iGoodnessHoriz, iGoodnessL, iGoodnessR, bestprobabL, bestprobabR, bestprobabRshifted;
	OvImageT<bool> maskL, maskR;

	i1.getDimensions(height,width,channels);
	iMatch.resetDimensions(height,width);
	iGoodnessHoriz.resetDimensions(height,width);
	iGoodnessVert.resetDimensions(height,width);
	iGoodnessL.resetDimensions(height,width);
	iGoodnessR.resetDimensions(height,width);
	bestprobabL.resetDimensions(height,width);
	bestprobabR.resetDimensions(height,width);
	bestprobabRshifted.resetDimensions(height,width);

	mLocalMatcher->setImagePair(i1, i2);

	u1 = minshiftX;
	v1 = minshiftY;
	o1 = 0;
	u2 = -minshiftX;
	v2 = -minshiftY;
	o2 = 0;

	for(double shiftX=minshiftX; shiftX<=maxshiftX; shiftX++)
	{
		for(double shiftY=minshiftY; shiftY<=maxshiftY; shiftY++)
		{
			iMatch = mLocalMatcher->getMatch(shiftX,shiftY);
			computeGoodnessHoriz(iMatch, iMatch, iGoodnessHoriz);
			computeGoodnessVert(iMatch, iMatch, iGoodnessVert);

			iGoodnessL = iGoodnessHoriz*iGoodnessVert;
			iGoodnessR = shiftImageXY(iGoodnessL, shiftX, shiftY); 

			bestprobabRshifted = shiftImageXY(bestprobabR, -shiftX, -shiftY);
			maskL = (iGoodnessL>bestprobabL) && (iGoodnessL>bestprobabRshifted);
			maskR = shiftImageXY(maskL, shiftX, shiftY); 

			u1.copyMasked(maskL, shiftX);
			v1.copyMasked(maskL, shiftY);
			bestprobabL.copyMasked(maskL, iGoodnessL);

			u2.copyMasked(maskR, -shiftX);
			v2.copyMasked(maskR, -shiftY);
			bestprobabR.copyMasked(maskR, iGoodnessR);
		}
	}

	//find left occlusions using LR consistency
	for(i=0; i<height; i++)
		for(j=0; j<width; j++)
		{
			iproj = i + v1(i,j);
			jproj = j + u1(i,j);
			if((iproj>=0)&&(iproj<height)&&(jproj>=0)&&(jproj<width))
			{
				iproj = iproj + v2(iproj,jproj);
				jproj = jproj + u2(iproj,jproj);
				if((iproj!=i)&&(jproj!=j)) o1(i,j) = 1;
			}
			else o1(i,j) = 1;
		}

	//find right occlusions using LR consistency
	for(i=0; i<height; i++)
		for(j=0; j<width; j++)
		{
			iproj = i + v2(i,j);
			jproj = j + u2(i,j);
			if((iproj>=0)&&(iproj<height)&&(jproj>=0)&&(jproj<width))
			{
				iproj = iproj + v1(iproj,jproj);
				jproj = jproj + u1(iproj,jproj);
				if((iproj!=i)&&(jproj!=j)) o2(i,j) = 1;
			}
			else o2(i,j) = 1;
		}

	o1 = medianFilter2D(o1,3,3);
	o2 = medianFilter2D(o2,3,3);

	return true;
}

template<typename T>
bool OvFlowDiffuseMatcherT<T>::setParams(int nparams, double*params)
{
	return true;
}

template<typename T>
void OvFlowDiffuseMatcherT<T>::setLocalImageMatcher(OvLocalMatcherT<T> & localImageMatcher)
{
	mLocalMatcher = &localImageMatcher;
}

template<typename T>
void OvFlowDiffuseMatcherT<T>::computeGoodnessHoriz(const OvImageT<double> & iMatch, const OvImageT<double> & iConductivity, OvImageT<double> & iGoodness)
{
	int i, j, height, width, channels;
	double temp, leftval, rightval;

	iMatch.getDimensions(height, width, channels);

	//left to right
	for(i=0;i<height;i++)
	{
		leftval = 0;
		for(j=0;j<width;j++)
		{
			temp = iMatch(i,j) + leftval*iConductivity(i,j);
			iGoodness(i,j) = temp;
			leftval = temp;
		}
	}	

	//now right to left
	for(i=0;i<height;i++)
	{
		rightval = 0;
		for(j=(width-1);j>=0;j--)
		{
			temp = rightval*iConductivity(i,j);
			iGoodness(i,j) += temp; //we are not including iMatch here since we would only subtract it later
			rightval = temp + iMatch(i,j); //but we have to include it here before propagation
		}
	}		
}

template<typename T>
void OvFlowDiffuseMatcherT<T>::computeGoodnessVert(const OvImageT<double> & iMatch, const OvImageT<double> & iConductivity, OvImageT<double> & iGoodness)
{
	int i, j, height, width, channels;
	double temp, topval, bottomval;

	iMatch.getDimensions(height, width, channels);

	//top to bottom
	for(j=0;j<width;j++)
	{
		topval = 0;
		for(i=0;i<height;i++)	
		{
			temp = iMatch(i,j) + topval*iConductivity(i,j);
			iGoodness(i,j) = temp;
			topval = temp;
		}
	}	

	//now bottom to top
	for(j=0;j<width;j++)
	{
		bottomval = 0;
		for(i=(height-1);i>=0;i--)	
		{
			temp = bottomval*iConductivity(i,j);
			iGoodness(i,j) += temp; //we are not including iMatch here since we would only subtract it later
			bottomval = temp + iMatch(i,j); //but we have to include it here before propagation
		}
	}		
}
