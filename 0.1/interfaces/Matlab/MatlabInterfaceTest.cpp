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

	printf("----- Image info -----\n");
	printf("Height: %d, Width: %d, Ncolors: %d\n", height, width, ncolors);
	printf("DataType: %d\n", dataType);

	double temp1, temp2;

	for(int i=0; i<height/2; i++)
	for(int j=0; j<width; j++)
	for(int k=0; k<ncolors; k++)
	{
		temp1 = image.getPixel(height-i-1,j,k);
		temp2 = image.getPixel(i,j,k);
		image.setPixel(temp2,height-i-1,j,k);
		image.setPixel(temp1,i,j,k);		
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


