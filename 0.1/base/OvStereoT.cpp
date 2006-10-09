#include "OvImageAdapter.h"
#include "OvImageT.h"
#include "OvImagePairPreprocessorT.h"
#include "OvLocalMatcherT.h"
#include "OvStereoGlobalMatcherT.h"
#include "OvDisparityPostprocessor.h"

template<typename T>
OvStereoT<T>::OvStereoT()
: mImagePairPreprocessor(0), mLocalImageMatcher(0), mStereoGlobalMatcher(0), mDisparityPostprocessor(0), 
isImagePairPreprocessorInternallyAllocated(false), isLocalImageMatcherInternallyAllocated(false), isStereoGlobalMatcherInternallyAllocated(false), isDisparityPostprocessorInternallyAllocated(false)
{
}

template<typename T>
OvStereoT<T>::~OvStereoT()
{
	if(isImagePairPreprocessorInternallyAllocated && (mImagePairPreprocessor!=0)) delete mImagePairPreprocessor;
	if(isLocalImageMatcherInternallyAllocated && (mLocalImageMatcher!=0)) delete mLocalImageMatcher;
	if(isStereoGlobalMatcherInternallyAllocated && (mStereoGlobalMatcher!=0)) delete mStereoGlobalMatcher;
	if(isDisparityPostprocessorInternallyAllocated && (mDisparityPostprocessor!=0)) delete mDisparityPostprocessor;
}

template<typename T>
void OvStereoT<T>::setImagePairPreprocessor(OvImagePairPreprocessorT<T> & imagePairPreprocessor)
{
	mImagePairPreprocessor = & imagePairPreprocessor;
}

template<typename T>
void OvStereoT<T>::setImagePairPreprocessorParams(int nparams, double*params)
{
	if(mImagePairPreprocessor)mImagePairPreprocessor->setParams(nparams, params);
}

template<typename T>
void OvStereoT<T>::setLocalImageMatcher(OvLocalMatcherT<T> & localImageMatcher)
{
	mLocalImageMatcher = &localImageMatcher;
}

template<typename T>
void OvStereoT<T>::setLocalImageMatcherParams(int nparams, double*params)
{
	if(mLocalImageMatcher)mLocalImageMatcher->setParams(nparams, params);
}

template<typename T>
void OvStereoT<T>::setGlobalMatcher(OvStereoGlobalMatcherT<T> & stereoGlobalMatcher)
{
	mStereoGlobalMatcher = &stereoGlobalMatcher;
}

template<typename T>
void OvStereoT<T>::setGlobalMatcherParams(int nparams, double*params)
{
	if(mStereoGlobalMatcher)mStereoGlobalMatcher->setParams(nparams, params);
}

template<typename T>
void OvStereoT<T>::setDisparityPostprocessor(OvDisparityPostprocessor & disparityPostProcessor)
{
	mDisparityPostprocessor = &disparityPostProcessor;
}

template<typename T>
void OvStereoT<T>::setDisparityPostprocessorParams(int nparams, double*params)
{
	if(mDisparityPostprocessor)mDisparityPostprocessor->setParams(nparams, params);
}

template<typename T>
bool OvStereoT<T>::doStereoMatching(const OvImageAdapter & i1, const OvImageAdapter & i2, double minshift, double maxshift, OvImageAdapter & leftDisparityMap, OvImageAdapter & rightDisparityMap, OvImageAdapter & leftOcclusions, OvImageAdapter & rightOcclusions)
{
	OvImageT<T> mImage1, mImage2;
	OvImageT<double> mLeftDisparityMap, mRightDisparityMap, mLeftOcclusions, mRightOcclusions;

	mImage1.copyFromAdapter(i1);
	mImage2.copyFromAdapter(i2);
	mLeftDisparityMap.copyFromAdapter(leftDisparityMap);
	mRightDisparityMap.copyFromAdapter(rightDisparityMap);
	mLeftOcclusions.copyFromAdapter(leftOcclusions);
	mRightOcclusions.copyFromAdapter(rightOcclusions);

	if(!haveEqualDimensions(mImage1, mImage2)) return false; //return if images have different dimensions	

	//output images must have the same height and width as input, but at least one channel
	if(!haveEqualHeightWidth(mImage1, mLeftDisparityMap)) return false; 
	if(mLeftDisparityMap.getChannels()<=0) return false;
	if(!haveEqualHeightWidth(mImage1, mRightDisparityMap)) return false;
	if(mRightDisparityMap.getChannels()<=0) return false;
	if(!haveEqualHeightWidth(mImage1, mLeftOcclusions)) return false;
	if(mLeftOcclusions.getChannels()<=0) return false;
	if(!haveEqualHeightWidth(mImage1, mRightOcclusions)) return false;
	if(mRightOcclusions.getChannels()<=0) return false;

	if(mLocalImageMatcher==0) return false; //must select a local matcher
	if(mStereoGlobalMatcher==0) return false; //must select a global matcher
	
	mStereoGlobalMatcher->setLocalImageMatcher(*mLocalImageMatcher); //tell global matcher about local matcher

	if(mImagePairPreprocessor)mImagePairPreprocessor->preProcessImagePair(mImage1, mImage2);
	mStereoGlobalMatcher->doMatching(mImage1, mImage2, minshift, maxshift, mLeftDisparityMap, mRightDisparityMap, mLeftOcclusions, mRightOcclusions);
	if(mDisparityPostprocessor)mDisparityPostprocessor->postProcessDisparity(mLeftDisparityMap, mRightDisparityMap, mLeftOcclusions, mRightOcclusions);

	mLeftDisparityMap.copyToAdapter(leftDisparityMap);
	mRightDisparityMap.copyToAdapter(rightDisparityMap);
	mLeftOcclusions.copyToAdapter(leftOcclusions);
	mRightOcclusions.copyToAdapter(rightOcclusions);

	return true;
}

