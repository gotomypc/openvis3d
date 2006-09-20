#include "OvImageAdapter.h"
#include "OvImageT.h"
#include "OvImagePairPreprocessorT.h"
#include "OvLocalMatcherT.h"
#include "BTLocalMatcherT.h"
#include "OvStereoGlobalMatcherT.h"
#include "OvDisparityPostProcessor.h"

template<typename T>
OvStereoT<T>::OvStereoT()
{
}

template<typename T>
OvStereoT<T>::~OvStereoT()
{
}

template<typename T>
void OvStereoT<T>::setImagePairPreprocessor(OvImagePairPreprocessorT<T> & imagePairPreprocessor)
{
}

template<typename T>
void OvStereoT<T>::setImagePairPreprocessorParams(int nparams, double*params)
{
	if(mImagePairPreprocessor)mImagePairPreprocessor->setParams(nparams, params);
}

template<typename T>
void OvStereoT<T>::setLocalImageMatcher(OvImageMatcherT<T> & localImageMatcher)
{
}

template<typename T>
void OvStereoT<T>::setLocalImageMatcherParams(int nparams, double*params)
{
	if(mLocalImageMatcher)mLocalImageMatcher->setParams(nparams, params);
}

template<typename T>
void OvStereoT<T>::setGlobalMatcher(OvStereoGlobalMatcherT<T> & stereoGlobalMatcher)
{
}

template<typename T>
void OvStereoT<T>::setGlobalMatcherParams(int nparams, double*params)
{
	if(mStereoGlobalMatcher)mStereoGlobalMatcher->setParams(nparams, params);
}

template<typename T>
void OvStereoT<T>::setDisparityPostprocessor(OvDisparityPostprocessor & disparityPostProcessor)
{
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

	image1.copyFromAdapter(i1);
	image2.copyFromAdapter(i2);

	if(!haveEqualDimensions(image1, image2)) return false; //return if images have different dimensions	


	mLeftDisparityMap.copyToAdapter(leftDisparityMap);
	mRightDisparityMap.copyToAdapter(rightDisparityMap);
	mLeftOcclusions.copyToAdapter(leftOcclusions);
	mRightOcclusions.copyToAdapter(rightOcclusions);
}

