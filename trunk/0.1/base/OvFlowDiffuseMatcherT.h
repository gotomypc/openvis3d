#ifndef __OVFLOWDIFFUSEMATCHERT_H
#define __OVFLOWDIFFUSEMATCHERT_H

#include "OvImageT.h"
#include "OvFlowGlobalMatcherT.h"
#include "OvLocalMatcherT.h"

///Global optical flow algorithm based on fast diffusion (see Ogale et al. ICRA April 05, IJCV July 06).
/** 
* The OvFlowDiffuseMatcherT implements the fast diffusion based optical flow based on
* Ogale and Aloimonos, ICRA April 2005 and IJCV July 2006.
* @see http://www.cs.umd.edu/users/ogale/publist.htm
*
* @author Abhijit Ogale
*/
template<typename T>
class OvFlowDiffuseMatcherT : public OvFlowGlobalMatcherT<T>
{
public:

	OvFlowDiffuseMatcherT(); /**< Default constructor with no parameters */

	virtual ~OvFlowDiffuseMatcherT(); /**< Destructor */

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
	virtual bool doMatching(OvImageT<T> & i1, OvImageT<T> & i2, double minshiftX, double maxshiftX, double minshiftY, double maxshiftY, OvImageT<double> & u1, OvImageT<double> & v1, OvImageT<double> & o1, OvImageT<double> & u2, OvImageT<double> & v2,  OvImageT<double> & o2);

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


#include "OvFlowDiffuseMatcherT.cpp" //to keep definitions available (needed for template class)


#endif //__OVFLOWDIFFUSEMATCHERT_H
