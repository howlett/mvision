#include <cv.h>
#include <stdio.h>
#include <highgui.h>
#include "opencv2/imgproc/imgproc.hpp"

#define CAMERA_0        0
#define KS_WAIT         10

void set_h(int pos);
void main(int argc,char *argv[])
{
	int c;
	IplImage* color_img, * hsv_img, * h_img;
	int flags = CV_WINDOW_AUTOSIZE;
	CvCapture* cv_cap = cvCaptureFromCAM(CAMERA_0);         // Capture from CAMERA 0
	int h = 180;
	CvScalar min = CV_RGB(h-15,100,0);
	CvScalar max = CV_RGB(h+15,256,256);

	/* Create ellipse to despeckle hsv.  */
	IplConvKernel* ellipse = cvCreateStructuringElementEx(6, 6, 1, 1,
			CV_SHAPE_ELLIPSE, NULL);

	if (!cv_cap)
		goto fail;

	cvNamedWindow("Webcam Video", flags);                        // create window
	cvNamedWindow("hsv Video", flags);                        // create window
	cvCreateTrackbar("Hue", "hsv Video", &h, 256, set_h);
	for(;;) {
		color_img = cvQueryFrame(cv_cap);               // get frame
		if(color_img != 0)
			hsv_img = cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 3);
		cvCvtColor(color_img, hsv_img, CV_BGR2HSV);
		h_img = cvCreateImage(cvGetSize(hsv_img), IPL_DEPTH_8U, 1);
		min = CV_RGB(h-20,10,10);
		max = CV_RGB(h+20,256,256);

		/* Remove anything not in the hue range. */
		cvInRangeS(hsv_img, min, max, h_img);
		/* Remove noise, or at least make the blotches bigger? */
		cvErode(h_img, h_img, ellipse,1);
		cvDilate(h_img, h_img, ellipse,1);
		cvShowImage("hsv Video", h_img); // show frame
		cvShowImage("Webcam Video", color_img); // show frame

		c = cvWaitKey(KS_WAIT);                         // wait KS_WAIT ms or for key stroke
		if(c == 27)
			break;                                  // if ESC, break and quit
	}
	/* clean up */
	cvReleaseCapture( &cv_cap );
	cvDestroyWindow("Webcam Video");
	return;
fail:
	printf("capture from cam failed\n");

}

void set_h(int pos) {
}
