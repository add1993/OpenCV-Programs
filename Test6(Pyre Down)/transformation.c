#include <cstdlib>
#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;

IplImage* doPyredown(IplImage* in, int filter = IPL_GAUSSIAN_5x5)
{
    assert( in->width%2 == 0 && in->height%2 == 0 );
    IplImage* out = cvCreateImage(
    cvSize( in->width/2, in->height/2 ), in->depth, in->nChannels);
    cvPyrDown( in, out );
    return( out );
};
 
int main()
{
    IplImage *image = cvLoadImage("airplane.jpg");
    cvNamedWindow("Input", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    cvShowImage("Input", image);
    IplImage* out = doPyredown(image, IPL_GAUSSIAN_5x5);
    //cvSmooth( image, out, CV_GAUSSIAN, 3, 3 );
    cvShowImage("Output", out);
    cvWaitKey(0);
    cvReleaseImage(&image);
    cvReleaseImage(&out);
    cvDestroyWindow("Input");
    cvDestroyWindow("Output");
    return 0;
}
