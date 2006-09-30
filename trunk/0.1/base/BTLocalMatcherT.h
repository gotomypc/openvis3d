#ifndef __BTLOCALMATCHERT_H
#define __BTLOCALMATCHERT_H

#include "OvLocalMatcherT.h"

///Birchfield-Tomasi sampling-insensitive intensity local matcher.
/** 
* The BTLocalMatcherT class is a subclass of OvLocalMatcherT and implements the Birchfield-Tomasi 
* method for intensity matching.
*
* @author Abhijit Ogale
*/
template<typename T>
class BTLocalMatcherT : public OvLocalMatcherT<T>
{
public:

	BTLocalMatcherT(); /**< Default constructor with no parameters */
	virtual ~BTLocalMatcherT(); /**< Destructor */
	virtual bool setImagePair(const OvImageT<T> & i1, const OvImageT<T> & i2);
	virtual bool setParams(int nparams, double*params);
	virtual const OvImageT<double> getMatch(int shiftx, int shifty);
	virtual const OvImageT<double> getRawMatch(int shiftx, int shifty);

protected:
	OvImageT<double> mImage1; /**< copy of input image 1. */
	OvImageT<double> mImage2; /**< copy of input image 2 */

	OvImageT<double> mMin1; /**< precomputed minimum image from i1 for speeding up Birchfield Tomasi matching. */
	OvImageT<double> mMax1; /**< precomputed maximum image from i1 for speeding up Birchfield Tomasi matching. */
	OvImageT<double> mMin2; /**< precomputed minimum image from i2 for speeding up Birchfield Tomasi matching. */
	OvImageT<double> mMax2; /**< precomputed maximum image from i2 for speeding up Birchfield Tomasi matching. */

		
	/**
	* Alpha is a constant used to convert
	* raw pixel intensity matches dI into the range 0 to 1 using exp(-alpha*dI/255), 
	* where alpha is specified by the user as follows:
	* <pre> 
	*   double params[] = {20.0};
	*	btmatcher.setMatchingParams(1,params);
	* </pre>
	* In this example, alpha is set to 20. Smaller values of alpha cause more smoothing.
	* @see setMatchingParams(int nparams, double*params)
	*/
	double alpha; 

};

#include "BTLocalMatcherT.cpp" //to keep definitions available (needed for template class)

#endif //__BTLOCALMATCHERT_H
