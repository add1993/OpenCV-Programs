#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <highgui.h>
using namespace std;

int main(int argc, char *argv[])
{
    IplImage* img = cvLoadImage("airplane.jpg", 1);
    cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
    cvShowImage("Example1", img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvDestroyWindow("Example1");
    return 0;
}
