/*
 * From https://stackoverflow.com/questions/2649421/displaying-video-from-webcam-in-opencv
 */


#include <stdio.h>
#include <cv.h>
#include <highgui.h>


#define CAMERA_0        0
#define KS_WAIT         10
void main(int argc,char *argv[])
{
	int c;
	IplImage* color_img;
	CvCapture* cv_cap = cvCaptureFromCAM(CAMERA_0);         // Capture from CAMERA 0

	if (!cv_cap)
		goto fail;

	cvNamedWindow("Webcam Video",0);                        // create window
	for(;;) {
		color_img = cvQueryFrame(cv_cap);               // get frame
		if(color_img != 0)
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
