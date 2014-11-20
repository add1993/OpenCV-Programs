#include <cv.h>
#include <highgui.h>

using namespace std;

int main()
{
    IplImage* image = cvLoadImage("b.jpg", 1);
    cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
    cvShowImage("Image", image);
    cvWaitKey(0);
    cvReleaseImage(&image);
    cvDestroyWindow("Image");
    return 0;
}
    
    

