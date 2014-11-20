//#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <highgui.h>
using namespace std;

int main(int argc, char *argv[])
{

	IplImage  *img = cvLoadImage( "mty.jpg",1 );

	cvNamedWindow("ventana", CV_WINDOW_AUTOSIZE );

	cvShowImage("ventana", img );
    printf("\nUPIITA");
	cvWaitKey(0);

	cvReleaseImage( &img );

	cvDestroyWindow("ventana");



    system("PAUSE");
    return EXIT_SUCCESS;
}
