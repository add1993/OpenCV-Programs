#include <cv.h>
#include <highgui.h>

using namespace std;

int main()
{
    cvNamedWindow("Video", CV_WINDOW_AUTOSIZE);
    CvCapture* capture = cvCreateFileCapture("tree.avi");
    IplImage* frame;
    
    while (1) {
          frame = cvQueryFrame(capture);
          if (!frame) {
             break;
          }
          cvShowImage("Video", frame);
          char c = cvWaitKey(33);
          if (c == 27) {
             break;
          }    
    }
    cvReleaseCapture(&capture);
    cvDestroyWindow("Video");
    return 0;
}
    
    

