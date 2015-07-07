#include <stdio.h>
#include <cv.h>
#include <highgui.h>


#define CAMERA_0        0
#define KS_WAIT         10
void main(int argc,char *argv[])
{
	int c;
	IplImage* color_img, r, g, b, zero;
	int flags = CV_WINDOW_AUTOSIZE;
	CvCapture* cv_cap = cvCaptureFromCAM(CAMERA_0);         // Capture from CAMERA 0

	if (!cv_cap)
		goto fail;

	cvNamedWindow("Webcam Video",flags);                        // create window
	cvNamedWindow("Redcam Video",flags);                        // create window
	cvNamedWindow("Greencam Video",flags);                      // create window
	cvNamedWindow("Bluecam Video",flags);                       // create window
	for(;;) {
		color_img = cvQueryFrame(cv_cap);               // get frame
		if(color_img != 0) {
			cvShowImage("Webcam Video", color_img); // show frame

			zero = *cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 1);
			cvZero(&zero);

			r = *cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 1);
			b = *cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 1);
			g = *cvCreateImage(cvGetSize(color_img), IPL_DEPTH_8U, 1);
			cvSplit(color_img, &b, &g, &r, NULL);


			cvMerge(&zero, &zero, &r, NULL, color_img);
			cvShowImage("Redcam Video", color_img); // show frame
			cvMerge(&zero, &g, &zero, NULL, color_img);
			cvShowImage("Greencam Video", color_img); // show frame
			cvMerge(&b, &zero, &zero, NULL, color_img);
			cvShowImage("Bluecam Video", color_img); // show frame

		}
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
