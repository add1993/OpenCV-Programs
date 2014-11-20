#include <cstdlib>
#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;

IplImage* doPyredown(IplImage* in, int filter = IPL_GAUSSIAN_5x5)
{
    assert( in->width%2 == 0 && in->height%2 == 0 );
    IplImage* out = cvCreateImage(
    cvSize( in->width/4, in->height/4), in->depth, in->nChannels);
    cvPyrDown( in, out );
    return( out );
};

void f(IplImage* src,IplImage* dst) {
CvMemStorage* storage = cvCreateMemStorage(0);
CvSeq* comp = NULL;
cvPyrSegmentation( src, dst, storage, &comp, 4, 200, 50 );
int n_comp = comp->total;
//for( int i=0; i<n_comp; i++ ) {
//vConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
//do_something_with( cc );
//}
cvReleaseMemStorage( &storage );
} 
int main()
{
    IplImage *image = cvLoadImage("airplane.jpg");
    IplImage *dst = cvLoadImage("airplane.jpg");
    cvNamedWindow("Input", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    cvShowImage("Input", image);
    //IplImage* out = doPyredown(image, IPL_GAUSSIAN_5x5);
    //cvSmooth( image, out, CV_GAUSSIAN, 3, 3 );
    f(image, dst);
    cvShowImage("Output", dst);
    cvWaitKey(0);
    cvReleaseImage(&image);
    cvReleaseImage(&dst);
    cvDestroyWindow("Input");
    cvDestroyWindow("Output");
    return 0;
}
