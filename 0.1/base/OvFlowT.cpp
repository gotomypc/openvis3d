#include "OvImageAdapter.h"
#include "OvImageT.h"
#include "OvImagePairPreprocessorT.h"
#include "OvLocalMatcherT.h"
#include "OvFlowGlobalMatcherT.h"
#include "OvFlowPostProcessor.h"

template<typename T>
OvFlowT<T>::OvFlowT()
: mImagePairPreprocessor(0), mLocalImageMatcher(0), mFlowGlobalMatcher(0), mFlowPostprocessor(0), 
isImagePairPreprocessorInternallyAllocated(false), isLocalImageMatcherInternallyAllocated(false), isFlowGlobalMatcherInternallyAllocated(false), isFlowPostprocessorInternallyAllocated(false)
{
}

template<typename T>
OvFlowT<T>::~OvFlowT()
{
	if(isImagePairPreprocessorInternallyAllocated && (mImagePairPreprocessor!=0)) delete mImagePairPreprocessor;
	if(isLocalImageMatcherInternallyAllocated && (mLocalImageMatcher!=0)) delete mLocalImageMatcher;
	if(isFlowGlobalMatcherInternallyAllocated && (mFlowGlobalMatcher!=0)) delete mFlowGlobalMatcher;
	if(isFlowPostprocessorInternallyAllocated && (mFlowPostprocessor!=0)) delete mFlowPostprocessor;
}

template<typename T>
void OvFlowT<T>::setImagePairPreprocessor(OvImagePairPreprocessorT<T> & imagePairPreprocessor)
{
	mImagePairPreprocessor = & imagePairPreprocessor;
}

template<typename T>
void OvFlowT<T>::setImagePairPreprocessorParams(int nparams, double*params)
{
	if(mImagePairPreprocessor)mImagePairPreprocessor->setParams(nparams, params);
}

template<typename T>
void OvFlowT<T>::setLocalImageMatcher(OvLocalMatcherT<T> & localImageMatcher)
{
	mLocalImageMatcher = &localImageMatcher;
}

template<typename T>
void OvFlowT<T>::setLocalImageMatcherParams(int nparams, double*params)
{
	if(mLocalImageMatcher)mLocalImageMatcher->setParams(nparams, params);
}

template<typename T>
void OvFlowT<T>::setGlobalMatcher(OvFlowGlobalMatcherT<T> & flowGlobalMatcher)
{
	mFlowGlobalMatcher = &flowGlobalMatcher;
}

template<typename T>
void OvFlowT<T>::setGlobalMatcherParams(int nparams, double*params)
{
	if(mFlowGlobalMatcher)mFlowGlobalMatcher->setParams(nparams, params);
}

template<typename T>
void OvFlowT<T>::setFlowPostprocessor(OvFlowPostprocessor & flowPostProcessor)
{
	mFlowPostprocessor = &flowPostProcessor;
}

template<typename T>
void OvFlowT<T>::setFlowPostprocessorParams(int nparams, double*params)
{
	if(mFlowPostprocessor)mFlowPostprocessor->setParams(nparams, params);
}

template<typename T>
bool OvFlowT<T>::doOpticalFlow(const OvImageAdapter & i1, const OvImageAdapter & i2, double minshiftX, double maxshiftX, double minshiftY, double maxshiftY, OvImageAdapter & u1, OvImageAdapter & v1, OvImageAdapter & o1, OvImageAdapter & u2, OvImageAdapter & v2,  OvImageAdapter & o2)
{
	OvImageT<T> mImage1, mImage2;
	OvImageT<double> mU1, mV1, mO1, mU2, mV2, mO2;

	mImage1.copyFromAdapter(i1);
	mImage2.copyFromAdapter(i2);
	mU1.copyFromAdapter(u1);
	mV1.copyFromAdapter(v1);
	mO1.copyFromAdapter(o1);
	mU2.copyFromAdapter(u2);
	mV2.copyFromAdapter(v2);
	mO2.copyFromAdapter(o2);

	if(!haveEqualDimensions(mImage1, mImage2)) return false; //return if images have different dimensions	

	//output images must have the same height and width as input, but at least one channel
	if(!haveEqualHeightWidth(mImage1, mU1)) return false; 
	if(mU1.getChannels()<=0) return false;
	if(!haveEqualHeightWidth(mImage1, mV1)) return false; 
	if(mV1.getChannels()<=0) return false;
	if(!haveEqualHeightWidth(mImage1, mO1)) return false; 
	if(mV1.getChannels()<=0) return false;
	if(!haveEqualHeightWidth(mImage1, mU2)) return false; 
	if(mU2.getChannels()<=0) return false;
	if(!haveEqualHeightWidth(mImage1, mV2)) return false; 
	if(mV2.getChannels()<=0) return false;
	if(!haveEqualHeightWidth(mImage1, mO2)) return false; 
	if(mV2.getChannels()<=0) return false;

	if(mLocalImageMatcher==0) return false; //must select a local matcher
	if(mFlowGlobalMatcher==0) return false; //must select a global matcher
	
	mFlowGlobalMatcher->setLocalImageMatcher(*mLocalImageMatcher); //tell global matcher about local matcher

	if(mImagePairPreprocessor)mImagePairPreprocessor->preProcessImagePair(mImage1, mImage2);
	mFlowGlobalMatcher->doMatching(mImage1, mImage2, minshiftX, maxshiftX, minshiftY, maxshiftY, mU1, mV1, mO1, mU2, mV2, mO2);
	if(mFlowPostprocessor)mFlowPostprocessor->postProcessFlow(mU1, mV1, mO1, mU2, mV2, mO2);

	mU1.copyToAdapter(u1);
	mV1.copyToAdapter(v1);
	mO1.copyToAdapter(o1);
	mU2.copyToAdapter(u2);
	mV2.copyToAdapter(v2);
	mO2.copyToAdapter(o2);

	return true;
}

