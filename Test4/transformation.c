#include <cstdlib>
#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;

int main()
{
    IplImage *image = cvLoadImage("airplane.jpg");
    cvNamedWindow("Input", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    cvShowImage("Input", image);
    IplImage* out = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3);
    cvSmooth( image, out, CV_GAUSSIAN, 5, 5 );
    cvShowImage("Output", out);
    cvWaitKey(0);
    cvReleaseImage(&image);
    cvReleaseImage(&out);
    cvDestroyWindow("Input");
    cvDestroyWindow("Output");
    return 0;
}
