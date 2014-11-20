#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    CvCapture* capture = 0;
capture = cvCreateFileCapture("tree.avi");
if(!capture){
return -1;
}
IplImage *bgr_frame=cvQueryFrame(capture);//Init the video read
double fps = cvGetCaptureProperty (
capture,
CV_CAP_PROP_FPS
);
CvSize size = cvSize(
(int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH),
(int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT)
);
CvVideoWriter *writer = cvCreateVideoWriter("new.avi",CV_FOURCC('M','J','P','G'),fps,size);
IplImage* logpolar_frame = cvCreateImage(
size,
IPL_DEPTH_8U,
3
);
while( (bgr_frame=cvQueryFrame(capture)) != NULL ) {
cvLogPolar( bgr_frame, logpolar_frame,
cvPoint2D32f(bgr_frame->width/2,
bgr_frame->height/2),
40,
CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS );
cvWriteFrame( writer, logpolar_frame );
}
cvReleaseVideoWriter( &writer );
cvReleaseImage( &logpolar_frame );
cvReleaseCapture( &capture );
return(0);
}
