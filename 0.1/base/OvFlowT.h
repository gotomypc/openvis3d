#ifndef __OVFLOWT_H
#define __OVFLOWT_H

#include "OvImageAdapter.h"
#include "OvImageT.h"
#include "OvImagePairPreprocessorT.h"
#include "OvLocalMatcherT.h"
#include "OvFlowGlobalMatcherT.h"
#include "OvFlowPostProcessor.h"


///Class for managing the execution of an optical flow algorithm.
/** 
* The OvFlowT class manages the execution of an optical flow algorithm, allowing the user to
* select any image preprocessors, local matching methods, global flow algorithm, and
* optical flow and occlusion postprocessors.
*
* @author Abhijit Ogale
*/
template<typename T>
class OvFlowT
{
public:

	OvFlowT(); /**< Default constructor with no parameters */

	virtual ~OvFlowT(); /**< Destructor */

	/**
	* Specifies the image pair preprocessor.
	* @param imagePairPreprocessor this specifies the preprocessor to be used.
	*/
	virtual void setImagePairPreprocessor(OvImagePairPreprocessorT<T> & imagePairPreprocessor);

	/**
	* Useful to set any parameters of the image pair preprocessor.
	* @param nparams number of parameters which are being passed
	* @param params the values of the parameters
	* @return true if successful.
	*/
	virtual void setImagePairPreprocessorParams(int nparams, double*params);


	/**
	* Specifies the local image matcher to be used.
	* @param localImageMatcher this is an OvLocalMatcherT<T> object which matches a pair of images.
	*/
	virtual void setLocalImageMatcher(OvLocalMatcherT<T> & localImageMatcher);

	/**
	* Useful to set parameters of the local image matcher.
	* @param nparams number of parameters which are being passed
	* @param params the values of the parameters
	* @return true if successful.
	*/
	virtual void setLocalImageMatcherParams(int nparams, double*params);

	/**
	* Specifies the global optical flow matcher (algorithm).
	* @param flowGlobalMatcher this specifies the global optical flow algorithm to be used.
	*/
	virtual void setGlobalMatcher(OvFlowGlobalMatcherT<T> & flowGlobalMatcher);

	/**
	* Useful to set any parameters of the global flow algorithm.
	* @param nparams number of parameters which are being passed
	* @param params the values of the parameters
	* @return true if successful.
	*/
	virtual void setGlobalMatcherParams(int nparams, double*params);

	/**
	* Specifies the optical flow and occlusion postprocessor to be used.
	* @param flowPostProcessor this object specifies the flow postprocessor to be used.
	*/
	virtual void setFlowPostprocessor(OvFlowPostprocessor & flowPostProcessor);

	/**
	* Useful to set any parameters of the flow postprocessor.
	* @param nparams number of parameters which are being passed
	* @param params the values of the parameters
	* @return true if successful.
	*/
	virtual void setFlowPostprocessorParams(int nparams, double*params);

	/**
	* Main method for computing optical flow on an image pair.
	* Note: This method modifies the input images, so be careful.
	* @param i1 the first image
	* @param i2 the second image
	* @param minshiftX method searches for horizontal flow values from minshiftX to maxshiftX
	* @param maxshiftX method searches for horizontal flow values from minshiftX to maxshiftX
	* @param minshiftY method searches for vertical flow values from minshiftY to maxshiftY
	* @param maxshiftY method searches for vertical flow values from minshiftY to maxshiftY
	* @param u1 the horizontal flow for image 1. (method sets this).
	* @param v1 the vertical flow for image 1. (method sets this).
	* @param o1 the occlusion map for image 1. (method sets this).
	* @param u2 the horizontal flow for image 2. (method sets this).
	* @param v2 the vertical flow for image 2. (method sets this).
	* @param o2 the occlusion map for image 2. (method sets this).
	* @return true if successful.
	*/
	virtual bool doOpticalFlow(const OvImageAdapter & i1, const OvImageAdapter & i2, double minshiftX, double maxshiftX, double minshiftY, double maxshiftY, OvImageAdapter & u1, OvImageAdapter & v1, OvImageAdapter & o1, OvImageAdapter & u2, OvImageAdapter & v2,  OvImageAdapter & o2);


protected:

	OvImagePairPreprocessorT<T>		*mImagePairPreprocessor;	/**< Image pair preprocessor */
	OvLocalMatcherT<T>				*mLocalImageMatcher;		/**< Local image pair matcher */
	OvFlowGlobalMatcherT<T>			*mFlowGlobalMatcher;		/**< Global optical flow algorithm */
	OvFlowPostprocessor				*mFlowPostprocessor;	/**< Flow post processor */
		
	// The flags below are for marking whether the above variables are internally allocated.
	// In case they are, we are responsible for releasing them at destruction. 
	// If not, then we do not release them.
	bool isImagePairPreprocessorInternallyAllocated;
	bool isLocalImageMatcherInternallyAllocated;
	bool isFlowGlobalMatcherInternallyAllocated;
	bool isFlowPostprocessorInternallyAllocated;
};

#include "OvFlowT.cpp" //for definitions (since this is a template class)

#endif //__OVFLOWT_H
