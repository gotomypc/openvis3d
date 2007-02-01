#include "Openvis3d.h"
#include <cv.h>
#include <highgui.h>
#include <cstdio>
#include <ctime>
#include <iostream>

void testStereo(char*imgfilename1, char*imgfilename2, double minshift, double maxshift)
{
	IplImage*img1 = 0, *img2 = 0, *imgLeftDisparity = 0, *imgRightDisparity = 0, *imgLeftOcclusions = 0, *imgRightOcclusions = 0;
	OpenCVImageAdapter*ovaImg1=0, *ovaImg2=0, *ovaLeftDisp=0, *ovaRightDisp=0, *ovaLeftOcc=0, *ovaRightOcc=0;
	OvImageT<double> ovtleftDisp, ovtrightDisp;

	BTLocalMatcherT<double> btmatcher;
	OvStereoDiffuseMatcherT<double> stereoDiffuseMatcher;
	OvStereoT<double> stereoManager;

	CvSize sz;

	img1 = cvLoadImage(imgfilename1);
	img2 = cvLoadImage(imgfilename2); 	

	sz.height = img1->height;
	sz.width  = img1->width;
	imgLeftDisparity = cvCreateImage(sz,IPL_DEPTH_64F,1);
	imgRightDisparity = cvCreateImage(sz,IPL_DEPTH_64F,1);
	imgLeftOcclusions = cvCreateImage(sz,IPL_DEPTH_64F,1);
	imgRightOcclusions = cvCreateImage(sz,IPL_DEPTH_64F,1);

	if(img1 && img2 && imgLeftDisparity && imgRightDisparity && imgLeftOcclusions && imgRightOcclusions)
	{
		ovaImg1 = new OpenCVImageAdapter(img1);
		ovaImg2 = new OpenCVImageAdapter(img2);
		ovaLeftDisp = new OpenCVImageAdapter(imgLeftDisparity);
		ovaRightDisp = new OpenCVImageAdapter(imgRightDisparity);
		ovaLeftOcc = new OpenCVImageAdapter(imgLeftOcclusions);
		ovaRightOcc = new OpenCVImageAdapter(imgRightOcclusions);

		double alpha[] = {20.0};
		btmatcher.setParams(1,alpha);

		stereoManager.setLocalImageMatcher(btmatcher);
		stereoManager.setGlobalMatcher(stereoDiffuseMatcher);

		clock_t clockstart,clockfinish;
		double  duration;
		clockstart = clock();

		stereoManager.doStereoMatching(*ovaImg1, *ovaImg2, minshift, maxshift, *ovaLeftDisp, *ovaRightDisp, *ovaLeftOcc, *ovaRightOcc);

		clockfinish = clock();
		duration = (double)(clockfinish - clockstart) / CLOCKS_PER_SEC;
		printf( "%f seconds\n", duration );

		//BEGIN: rescale disparity maps to range (0,1) so that they can be displayed by OpenCV
		ovtleftDisp.copyFromAdapter(*ovaLeftDisp);
		ovtrightDisp.copyFromAdapter(*ovaRightDisp);

		ovtleftDisp = (ovtleftDisp-minshift)/(maxshift-minshift);
		ovtrightDisp = (ovtrightDisp+maxshift)/(-minshift+maxshift);

		ovtleftDisp.copyToAdapter(*ovaLeftDisp);
		ovtrightDisp.copyToAdapter(*ovaRightDisp);
		//END: rescale disparity maps to range (0,1) so that they can be displayed by OpenCV

		cvNamedWindow("Left Disparity", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("Left Occlusions", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("Right Disparity", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("Right Occlusions", CV_WINDOW_AUTOSIZE);

		cvShowImage("Left Disparity", imgLeftDisparity);		
		cvShowImage("Left Occlusions", imgLeftOcclusions);		
		cvShowImage("Right Disparity", imgRightDisparity);	
		cvShowImage("Right Occlusions", imgRightOcclusions);		

		cvWaitKey(0);
	}
	
	//release adaptors
	if(ovaImg1) delete ovaImg1;
	if(ovaImg2) delete ovaImg2;
	if(ovaLeftDisp) delete ovaLeftDisp;
	if(ovaRightDisp) delete ovaRightDisp;
	if(ovaLeftOcc) delete ovaLeftOcc;
	if(ovaRightOcc) delete ovaRightOcc;

	//release opencv images
	if(img1) cvReleaseImage(&img1);
	if(img2) cvReleaseImage(&img2);
	if(imgLeftDisparity) cvReleaseImage(&imgLeftDisparity);
	if(imgRightDisparity) cvReleaseImage(&imgRightDisparity);
	if(imgLeftOcclusions) cvReleaseImage(&imgLeftOcclusions);
	if(imgRightOcclusions) cvReleaseImage(&imgRightOcclusions);
}

void testOpticalFlow(char*imgfilename1, char*imgfilename2, double minshiftX, double maxshiftX, double minshiftY, double maxshiftY)
{
	IplImage*img1 = 0, *img2 = 0, *imgU1 = 0, *imgV1 = 0, *imgO1 = 0, *imgU2 = 0, *imgV2 = 0, *imgO2 = 0;
	OpenCVImageAdapter*ovaImg1 = 0, *ovaImg2 = 0, *ovaImgU1 = 0, *ovaImgV1 = 0, *ovaImgO1 = 0, *ovaImgU2 = 0, *ovaImgV2 = 0, *ovaImgO2 = 0;
	OvImageT<double> ovtU1, ovtV1, ovtU2, ovtV2;

	BTLocalMatcherT<double> btmatcher;
	OvFlowDiffuseMatcherT<double> flowDiffuseMatcher;
	OvFlowT<double> flowManager;

	CvSize sz;

	img1 = cvLoadImage(imgfilename1);
	img2 = cvLoadImage(imgfilename2); 	

	sz.height = img1->height;
	sz.width  = img1->width;
	imgU1 = cvCreateImage(sz,IPL_DEPTH_64F,1);
	imgV1 = cvCreateImage(sz,IPL_DEPTH_64F,1);
	imgO1 = cvCreateImage(sz,IPL_DEPTH_64F,1);
	imgU2 = cvCreateImage(sz,IPL_DEPTH_64F,1);
	imgV2 = cvCreateImage(sz,IPL_DEPTH_64F,1);
	imgO2 = cvCreateImage(sz,IPL_DEPTH_64F,1);

	if(img1 && img2 && imgU1 && imgV1 && imgO1 && imgU2 && imgV2 && imgO2)
	{
		ovaImg1 = new OpenCVImageAdapter(img1);
		ovaImg2 = new OpenCVImageAdapter(img2);
		ovaImgU1 = new OpenCVImageAdapter(imgU1);
		ovaImgV1 = new OpenCVImageAdapter(imgV1);
		ovaImgO1 = new OpenCVImageAdapter(imgO1);
		ovaImgU2 = new OpenCVImageAdapter(imgU2);
		ovaImgV2 = new OpenCVImageAdapter(imgV2);
		ovaImgO2 = new OpenCVImageAdapter(imgO2);

		double alpha[] = {20.0};
		btmatcher.setParams(1,alpha);

		flowManager.setLocalImageMatcher(btmatcher);
		flowManager.setGlobalMatcher(flowDiffuseMatcher);

		clock_t clockstart,clockfinish;
		double  duration;
		clockstart = clock();

		flowManager.doOpticalFlow(*ovaImg1, *ovaImg2, minshiftX, maxshiftX, minshiftY, maxshiftY, *ovaImgU1, *ovaImgV1, *ovaImgO1, *ovaImgU2, *ovaImgV2, *ovaImgO2);

		clockfinish = clock();
		duration = (double)(clockfinish - clockstart) / CLOCKS_PER_SEC;
		printf( "%f seconds\n", duration );

		//BEGIN: rescale flow maps to range (0,1) so that they can be displayed by OpenCV
		ovtU1.copyFromAdapter(*ovaImgU1);
		ovtV1.copyFromAdapter(*ovaImgV1);
		ovtU2.copyFromAdapter(*ovaImgU2);
		ovtV2.copyFromAdapter(*ovaImgV2);

		ovtU1 = (ovtU1-minshiftX)/(maxshiftX-minshiftX);
		ovtV1 = (ovtV1-minshiftY)/(maxshiftY-minshiftY);
		ovtU2 = (ovtU2+maxshiftX)/(-minshiftX+maxshiftX);
		ovtV2 = (ovtV2+maxshiftY)/(-minshiftY+maxshiftY);

		ovtU1.copyToAdapter(*ovaImgU1);
		ovtV1.copyToAdapter(*ovaImgV1);
		ovtU2.copyToAdapter(*ovaImgU2);
		ovtV2.copyToAdapter(*ovaImgV2);
		//END: rescale flow maps to range (0,1) so that they can be displayed by OpenCV

		cvNamedWindow("U1", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("V1", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("O1", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("U2", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("V2", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("O2", CV_WINDOW_AUTOSIZE);

		cvShowImage("U1", imgU1);	
		cvShowImage("V1", imgV1);	
		cvShowImage("O1", imgO1);	
		cvShowImage("U2", imgU2);	
		cvShowImage("V2", imgV2);	
		cvShowImage("O2", imgO2);	

		cvWaitKey(0);
	}
	
	//release adaptors
	if(ovaImg1) delete ovaImg1;
	if(ovaImg2) delete ovaImg2;
	if(ovaImgU1) delete ovaImgU1;
	if(ovaImgV1) delete ovaImgV1;
	if(ovaImgO1) delete ovaImgO1;
	if(ovaImgU2) delete ovaImgU2;
	if(ovaImgV2) delete ovaImgV2;
	if(ovaImgO2) delete ovaImgO2;

	//release opencv images
	if(img1) cvReleaseImage(&img1);
	if(img2) cvReleaseImage(&img2);
	if(imgU1) cvReleaseImage(&imgU1);
	if(imgV1) cvReleaseImage(&imgV1);
	if(imgO1) cvReleaseImage(&imgO1);
	if(imgU2) cvReleaseImage(&imgU2);
	if(imgV2) cvReleaseImage(&imgV2);
	if(imgO2) cvReleaseImage(&imgO2);
}


int main()
{
	printf("\nRunning stereo ...\n");
	testStereo("tsukuba1color.png", "tsukuba2color.png", 5, 15);
	printf("\nRunning optical flow ...\n");
	testOpticalFlow("car1.png", "car0.png", 0, 18, -3, 0);

	return 0;
}

