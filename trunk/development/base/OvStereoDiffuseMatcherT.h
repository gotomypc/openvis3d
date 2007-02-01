#ifndef __OVSTEREODIFFUSEMATCHERT_H
#define __OVSTEREODIFFUSEMATCHERT_H

#include "OvImageT.h"
#include "OvStereoGlobalMatcherT.h"
#include "OvLocalMatcherT.h"

///Global stereo matching algorithm based on fast diffusion (see Ogale et al. ICRA April 05, IJCV July 06).
/** 
* The OvStereoDiffuseMatcherT implements the fast diffusion based stereo matching discussed in
* Ogale and Aloimonos, ICRA April 2005 and IJCV July 2006.
* @see http://www.cs.umd.edu/users/ogale/publist.htm
*
* @author Abhijit Ogale
*/
template<typename T>
class OvStereoDiffuseMatcherT : public OvStereoGlobalMatcherT<T>
{
public:

	OvStereoDiffuseMatcherT(); /**< Default constructor with no parameters */

	virtual ~OvStereoDiffuseMatcherT(); /**< Destructor */

	/**
	* Main method for stereo matching an image pair.
	* Note: This method modifies the input images, so be careful.
	* @param i1 the first image
	* @param i2 the second image
	* @param minshift method searches for disparities from minshift to maxshift
	* @param maxshift method searches for disparities from minshift to maxshift
	* @param leftDisparityMap the disparity map for the left image. (method sets this).
	* @param rightDisparityMap the disparity map for the right image. (method sets this).
	* @param leftOcclusions the occlusion map for the left image. (method sets this).
	* @param rightOcclusions the occlusion map for the right image. (method sets this).	
	* @return true if successful.
	*/
	virtual bool doMatching(OvImageT<T> & i1, OvImageT<T> & i2, double minshift, double maxshift, OvImageT<double> & leftDisparityMap, OvImageT<double> & rightDisparityMap, OvImageT<double> & leftOcclusions, OvImageT<double> & rightOcclusions);

	/**
	* Used for specifying any parameters required.
	* @param nparams number of parameters which are being passed
	* @param params the values of the parameters
	* @return true if successful.
	*/
	virtual bool setParams(int nparams, double*params);

	/**
	* Specifies the local image matcher to be used by the algorithm.
	* @param localImageMatcher this is an OvLocalMatcherT<T> object which matches a pair of images.
	*/
	virtual void setLocalImageMatcher(OvLocalMatcherT<T> & localImageMatcher);

protected:
	OvLocalMatcherT<T> * mLocalMatcher;

	virtual void computeGoodnessHoriz (const OvImageT<double> & iMatch, const OvImageT<double> & iConductivity, OvImageT<double> & iGoodness); 
	virtual void computeGoodnessVert  (const OvImageT<double> & iMatch, const OvImageT<double> & iConductivity, OvImageT<double> & iGoodness); 

};


#include "OvStereoDiffuseMatcherT.cpp" //to keep definitions available (needed for template class)


#endif //__OVSTEREODIFFUSEMATCHERT_H
