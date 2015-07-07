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
	IplImage* color_img;
	IplImage* hsv_img;
	IplImage* h_img;
	IplImage* gray;
	int flags = CV_WINDOW_AUTOSIZE;
	CvCapture* cv_cap = cvCaptureFromCAM(CAMERA_0);         // Capture from CAMERA 0
	int h = 180;
	int t1 = 3, t2 = 5;
	CvScalar min = CV_RGB(h-15,100,0);
	CvScalar max = CV_RGB(h+15,256,256);

	/* Create ellipse to despeckle hsv.  */
	IplConvKernel* ellipse = cvCreateStructuringElementEx(10, 10, 1, 1,
			CV_SHAPE_ELLIPSE, NULL);
	/* For X, Y, And Area */
	CvMoments moments;

	/* For contours  */
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;

	double area, m01, m10;
	if (!cv_cap)
		goto fail;

	cvNamedWindow("Webcam Video", flags);                        // create window
	cvNamedWindow("hsv Video", flags);                        // create window
	cvNamedWindow("Contour Video", flags);                        // create window
	cvCreateTrackbar("Hue", "hsv Video", &h, 256, set_h);
	cvCreateTrackbar("countour1", "Contour Video", &t1, 256, set_h);
	cvCreateTrackbar("countour2", "Contour Video", &t2, 256, set_h);
	for(;;) {
		color_img = cvQueryFrame(cv_cap);               // get frame
		if(color_img != 0) {
			hsv_img = cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 3);
			gray = cvCreateImage(cvGetSize(hsv_img), IPL_DEPTH_8U, 1);
		}
		cvCvtColor(color_img, gray, CV_RGB2GRAY);
		cvCvtColor(color_img, hsv_img, CV_BGR2HSV);
		h_img = cvCreateImage(cvGetSize(hsv_img), IPL_DEPTH_8U, 1);


		/* HSV */
		min = CV_RGB(h-20,10,10);
		max = CV_RGB(h+20,256,256);

		/* Remove anything not in the hue range. */
		cvInRangeS(hsv_img, min, max, h_img);
		/* Remove noise, or at least make the blotches bigger? */
		cvErode(h_img, h_img, ellipse,1);
		cvDilate(h_img, h_img, ellipse,1);

		/* Calculate moments to figure out if the object is present */
		cvMoments(h_img, &moments, 1);

		area = cvGetSpatialMoment(&moments, 0,0);
		m01 = cvGetSpatialMoment(&moments, 0,1);
		m10 = cvGetSpatialMoment(&moments, 1,0);

		if (area > 17000) {
			int x = m10/area;
			int y = m01/area;
			printf("x = %d, y = %d (area = %f)\n", x, y, area);
		}

		/* Draw contours */
		cvCanny(gray, gray, (double)t1, (double)t2, 3);
		//cvDilate(gray, gray, 0, 1);
		cvDilate(gray, gray, ellipse, 1);
		cvFindContours(gray, storage, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0,0));
		cvDrawContours(color_img, contours, CV_RGB(254,0,0), CV_RGB(0,255,0), 10, 1, CV_AA, cvPoint(0,0));
		/* Display images */
		cvShowImage("hsv Video", h_img); // show frame
		cvShowImage("Contour Video", gray); // show frame
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
