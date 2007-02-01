#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "OpenCVImageAdapter.h"
#include <time.h>

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

int main (int argc, char*argv [])
{
	IplImage*img = 0;
	img = cvLoadImage("..\\..\\..\\..\\testdata\\test.jpg");
	
	if(!img)
	{
		printf("Error\n");
		exit(0);
	}

	OpenCVImageAdapter*opencvAdaptor;
	opencvAdaptor = new OpenCVImageAdapter(img);

	//time_t start,end;
	//double dif;
	//time (&start);

	//for(int i=0; i<1000; i++)
		flipImage(*opencvAdaptor);

	//time (&end);
	//dif = difftime (end,start);
	//printf ("You have taken %.2lf seconds to type your name.\n", dif );

	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvShowImage("mainWin", img);

	cvWaitKey(0);
	
	if(opencvAdaptor) delete opencvAdaptor;
	cvReleaseImage(&img);
	
	return 0;
}