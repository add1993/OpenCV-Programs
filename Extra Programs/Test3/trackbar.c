#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <cstdlib>

using namespace std;

int slider_pos = 0;
CvCapture* g_cap = NULL;

void onTrackbarSlide(int pos)
{
     cvSetCaptureProperty(g_cap, CV_CAP_PROP_POS_FRAMES, pos);
}

int main()
{
    cvNamedWindow("Slider", CV_WINDOW_AUTOSIZE);
    g_cap = cvCreateFileCapture("tree.avi");
    int frames = (int) cvGetCaptureProperty(g_cap, CV_CAP_PROP_FRAME_COUNT); //Total frames
    printf("Frames = %d\n", frames);
    if (frames != 0) {
       cvCreateTrackbar("Pos","Slider",&slider_pos, frames, onTrackbarSlide);
    }
    IplImage* frame;
    
    while (1) {
          frame = cvQueryFrame( g_cap );
          if( !frame ) {
              break;
          }
          slider_pos+=6;
          
          cvCreateTrackbar("Pos","Slider",&slider_pos, frames, onTrackbarSlide);
          cvShowImage( "Slider", frame );
          char c = cvWaitKey(33);
                  
          if( c == 27 ) {
              break;
          }
    }
    cvReleaseCapture( &g_cap );
    cvDestroyWindow( "Slider" );
    return 0;
}
    
    
