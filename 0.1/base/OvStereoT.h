#ifndef __OVSTEREOT_H
#define __OVSTEREOT_H

#include "OvImageAdapter.h"
#include "OvImageT.h"
#include "OvImagePairPreprocessorT.h"
#include "OvLocalMatcherT.h"
#include "OvStereoGlobalMatcherT.h"
#include "OvDisparityPostprocessor.h"


///Class for managing the execution of a stereo algorithm.
/** 
* The OvStereoT class manages the execution of a stereo algorithm, allowing the user to
* select any image preprocessors, local matching methods, global stereo algorithm, and
* disparity and occlusion postprocessors.
*
* @author Abhijit Ogale
*/
template<typename T>
class OvStereoT
{
public:

	OvStereoT(); /**< Default constructor with no parameters */

	virtual ~OvStereoT(); /**< Destructor */

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
	* Specifies the global stereo matcher (algorithm).
	* @param stereoGlobalMatcher this specifies the global stereo algorithm to be used.
	*/
	virtual void setGlobalMatcher(OvStereoGlobalMatcherT<T> & stereoGlobalMatcher);

	/**
	* Useful to set any parameters of the global stereo algorithm.
	* @param nparams number of parameters which are being passed
	* @param params the values of the parameters
	* @return true if successful.
	*/
	virtual void setGlobalMatcherParams(int nparams, double*params);

	/**
	* Specifies the disparity postprocessor to be used.
	* @param disparityPostProcessor this OvDisparityPostprocessor object specifies the postprocessor to be used.
	*/
	virtual void setDisparityPostprocessor(OvDisparityPostprocessor & disparityPostProcessor);

	/**
	* Useful to set any parameters of the disparity postprocessor.
	* @param nparams number of parameters which are being passed
	* @param params the values of the parameters
	* @return true if successful.
	*/
	virtual void setDisparityPostprocessorParams(int nparams, double*params);

	/**
	* Main method for executing stereo matching on an image pair.
	* Note: This method modifies the input images, so be careful.
	* @param i1 the first image
	* @param i2 the second image
	* @param minshift method searches for disparities from minshift to maxshift
	* @param maxshift method searches for disparities from minshift to maxshift
	* @param leftDisparityMap the disparity map for the left image. (method returns this).
	* @param rightDisparityMap the disparity map for the right image. (method returns this).
	* @param leftOcclusions the occlusion map for the left image. (method returns this).
	* @param rightOcclusions the occlusion map for the right image. (method returns this).	
	* @return true if successful.
	*/
	virtual bool doStereoMatching(const OvImageAdapter & i1, const OvImageAdapter & i2, double minshift, double maxshift, OvImageAdapter & leftDisparityMap, OvImageAdapter & rightDisparityMap, OvImageAdapter & leftOcclusions, OvImageAdapter & rightOcclusions);


protected:

	OvImagePairPreprocessorT<T>		*mImagePairPreprocessor;	/**< Image pair preprocessor */
	OvLocalMatcherT<T>				*mLocalImageMatcher;		/**< Local image pair matcher */
	OvStereoGlobalMatcherT<T>		*mStereoGlobalMatcher;		/**< Global stereo algorithm */
	OvDisparityPostprocessor		*mDisparityPostprocessor;	/**< Disparity map post processor */
		
	// The flags below are for marking whether the above variables are internally allocated.
	// In case they are, we are responsible for releasing them at destruction. 
	// If not, then we do not release them.
	bool isImagePairPreprocessorInternallyAllocated;
	bool isLocalImageMatcherInternallyAllocated;
	bool isStereoGlobalMatcherInternallyAllocated;
	bool isDisparityPostprocessorInternallyAllocated;
};

#include "OvStereoT.cpp" //for definitions (since this is a template class)

#endif //__OVSTEREOT_H
