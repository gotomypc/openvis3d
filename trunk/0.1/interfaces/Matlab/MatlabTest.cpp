#include "MatlabImageAdapter.h"

extern "C" {
#include "mex.h"
}


void flipImage(OvImageAdapter & image)
{
	int width, height, ncolors;	
	OvImageAdapter::OvDataType dataType;

	image.getSize(height,width,ncolors);
	image.getDataType(dataType);

	mexPrintf("----- Image info -----\n");
	mexPrintf("Height: %d, Width: %d, Ncolors: %d\n", height, width, ncolors);
	printf("DataType: %d\n", dataType);

	float temp1[4], temp2[4];

	for(int i=0; i<height/2; i++)
	for(int j=0; j<width; j++)
	{
		image.getPixel(temp1,height-i-1,j);
		image.getPixel(temp2,i,j);
		image.setPixel(temp2,height-i-1,j);
		image.setPixel(temp1,i,j);		
	}

}


void openvis3dmatlabtest1( int nlhs, mxArray *plhs[], int nrhs, const mxArray  *prhs[] )
{
	if(nrhs<1) mexErrMsgTxt("Usage:\ni2 = openvis3dmatlabtest1(i1);");
	if(nlhs<1) mexErrMsgTxt("Usage:\ni2 = openvis3dmatlabtest1(i1);");

	plhs[0] = mxDuplicateArray(prhs[0]);

	MatlabImageAdapter*matlabAdaptor;
	matlabAdaptor = new MatlabImageAdapter(plhs[0]);

	flipImage(*matlabAdaptor);

	if(matlabAdaptor) delete matlabAdaptor;
}

extern "C" {
  void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray  *prhs[] )
  {
    openvis3dmatlabtest1(nlhs, plhs, nrhs, prhs);
  }
}


