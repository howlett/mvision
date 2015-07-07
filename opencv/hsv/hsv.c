#include <stdio.h>
#include <cv.h>
#include <highgui.h>


#define CAMERA_0        0
#define KS_WAIT         10

void set_h(int pos);
void main(int argc,char *argv[])
{
	int c;
	IplImage* color_img, hsv_img, h_img;
	int flags = CV_WINDOW_AUTOSIZE;
	CvCapture* cv_cap = cvCaptureFromCAM(CAMERA_0);         // Capture from CAMERA 0
	int h = 50;
	CvScalar min = CV_RGB(h-20,100,0);
	CvScalar max = CV_RGB(h+20,256,256);

	if (!cv_cap)
		goto fail;

	cvNamedWindow("Webcam Video", flags);                        // create window
	cvNamedWindow("hsv Video", flags);                        // create window
	cvCreateTrackbar("Hue", "hsv Video", &h, 256, set_h);
	for(;;) {
		color_img = cvQueryFrame(cv_cap);               // get frame
		if(color_img != 0)
			hsv_img = *cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 3);
			cvCvtColor(color_img, &hsv_img, CV_BGR2HSV);
			h_img = *cvCreateImage(cvGetSize(&hsv_img), IPL_DEPTH_8U, 1);
			min = CV_RGB(h-20,0,0);
			max = CV_RGB(h+20,256,256);
			cvInRangeS(&hsv_img, min, max, &h_img);
			cvShowImage("hsv Video", &h_img); // show frame
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
