#include "OvImageT.h"

template<typename T>
OvStereoDiffuseMatcherT<T>::OvStereoDiffuseMatcherT()
{
}

template<typename T>
OvStereoDiffuseMatcherT<T>::~OvStereoDiffuseMatcherT()
{
}

template<typename T>
bool OvStereoDiffuseMatcherT<T>::doMatching(OvImageT<T> & i1, OvImageT<T> & i2, double minshift, double maxshift, OvImageT<double> & leftDisparityMap, OvImageT<double> & rightDisparityMap, OvImageT<double> & leftOcclusions, OvImageT<double> & rightOcclusions)
{
	int height, width, channels;
	int i,j,jproj;

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

	leftDisparityMap = minshift;
	rightDisparityMap = -minshift;
	leftOcclusions = 0;
	rightOcclusions = 0;

	for(double shift=minshift; shift<=maxshift; shift++)
	{
		iMatch = mLocalMatcher->getMatch(shift,0);
		computeGoodnessHoriz(iMatch, iMatch, iGoodnessHoriz);
		computeGoodnessVert(iMatch, iMatch, iGoodnessVert);

		iGoodnessL = iGoodnessHoriz*iGoodnessVert;
		iGoodnessR = shiftImageXY(iGoodnessL, shift, 0); 

		bestprobabRshifted = shiftImageXY(bestprobabR, -shift, 0);
		maskL = (iGoodnessL>bestprobabL) && (iGoodnessL>bestprobabRshifted);
		maskR = shiftImageXY(maskL, shift, 0); 

		leftDisparityMap.copyMasked(maskL,shift);
		bestprobabL.copyMasked(maskL, iGoodnessL);
		rightDisparityMap.copyMasked(maskR,-shift);
		bestprobabR.copyMasked(maskR, iGoodnessR);
	}

	leftDisparityMap  = medianFilter2D(leftDisparityMap,3,3);
	rightDisparityMap = medianFilter2D(rightDisparityMap,3,3);

	//find left occlusions using LR consistency
	for(i=0; i<height; i++)
		for(j=0; j<width; j++)
		{
			jproj = j + leftDisparityMap(i,j);
			if((jproj>=0)&&(jproj<width))
			{
				jproj = jproj + rightDisparityMap(i,jproj);
				if(jproj!=j) leftOcclusions(i,j) = 1;
			}
			else leftOcclusions(i,j) = 1;
		}

	//find right occlusions using LR consistency
	for(i=0; i<height; i++)
		for(j=0; j<width; j++)
		{
			jproj = j + rightDisparityMap(i,j);
			if((jproj>=0)&&(jproj<width))
			{
				jproj = jproj + leftDisparityMap(i,jproj);
				if(jproj!=j) rightOcclusions(i,j) = 1;
			}
			else rightOcclusions(i,j) = 1;
		}

	leftOcclusions = medianFilter2D(leftOcclusions,3,3);
	rightOcclusions = medianFilter2D(rightOcclusions,3,3);

	return true;
}

template<typename T>
bool OvStereoDiffuseMatcherT<T>::setParams(int nparams, double*params)
{
	return true;
}

template<typename T>
void OvStereoDiffuseMatcherT<T>::setLocalImageMatcher(OvLocalMatcherT<T> & localImageMatcher)
{
	mLocalMatcher = &localImageMatcher;
}

template<typename T>
void OvStereoDiffuseMatcherT<T>::computeGoodnessHoriz(const OvImageT<double> & iMatch, const OvImageT<double> & iConductivity, OvImageT<double> & iGoodness)
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
void OvStereoDiffuseMatcherT<T>::computeGoodnessVert(const OvImageT<double> & iMatch, const OvImageT<double> & iConductivity, OvImageT<double> & iGoodness)
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
