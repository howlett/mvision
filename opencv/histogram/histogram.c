#include <stdio.h>
#include <cv.h>
#include <highgui.h>


#define CAMERA_0        0
#define KS_WAIT         10
void main(int argc,char *argv[])
{
	int c;
	IplImage* color_img, r, g, b;
	int flags = CV_WINDOW_AUTOSIZE;
	CvCapture* cv_cap = cvCaptureFromCAM(CAMERA_0);         // Capture from CAMERA 0

	if (!cv_cap)
		goto fail;

	cvNamedWindow("Webcam Video", flags);                        // create window
	for(;;) {
		color_img = cvQueryFrame(cv_cap);               // get frame
		if(color_img != 0)
			r = *cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 1);
			b = *cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 1);
			g = *cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 1);
			cvSplit(color_img, &b, &g, &r, NULL);

			cvEqualizeHist(&r, &r);
			cvEqualizeHist(&b, &b);
			cvEqualizeHist(&g, &g);

			cvMerge(&b, &g, &r, NULL, color_img);
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
