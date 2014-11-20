#include <cstdlib>
#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;

IplImage* doPyredown(IplImage* in, int filter = IPL_GAUSSIAN_5x5)
{
    assert( in->width%2 == 0 && in->height%2 == 0 );
    IplImage* out = cvCreateImage(cvSize( in->width/2, in->height/2 ), in->depth, in->nChannels);
    cvPyrDown( in, out );
    return( out );
};

IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, double aperture) 
{
   if(in->nChannels != 1) {
        printf("No Image\n");
        return(0);
   } //Canny only handles gray scale images
   IplImage* out = cvCreateImage(cvSize( in->width/2, in->height/2 ), IPL_DEPTH_8U, 1);
   cvCanny( in, out, lowThresh, highThresh, aperture );
   return( out );
};
 
/*int main()
{
    IplImage *image = cvLoadImage("right01.jpg");
    cvNamedWindow("Input", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    cvShowImage("Input", image);
    IplImage* out = doCanny(image, 10, 100, 3);
    //cvSmooth( image, out, CV_GAUSSIAN, 3, 3 );
    cvShowImage("Output", out);
    cvWaitKey(0);
    cvReleaseImage(&image);
    cvReleaseImage(&out);
    cvDestroyWindow("Input");
    cvDestroyWindow("Output");
    return 0;
}*/
int main()
{
	IplImage* img = cvLoadImage( "airplane.jpg", 0);
	cvNamedWindow( "Example4-in" );
	cvNamedWindow( "Example4-out" );

	// Show the original image
	cvShowImage("Example4-in", img);

	// Make sure image is divisible by 2
	//assert( img->width%2 == 0 && img->height%2 == 0);

	// Create an image for the output
	IplImage* out = cvCreateImage( cvSize(img->width,img->height), img->depth, img->nChannels );

	// Reduce the image by 2
	//cvPyrDown( img, out );
	
	// Perform canny edge detection
	cvCanny( img, out, 10, 100, 3 );

	// Show the processed image
	cvShowImage("Example4-out", out);

	cvWaitKey(0);
	cvReleaseImage( &img );
	cvReleaseImage( &out );
	cvDestroyWindow( "Example4-in" );
	cvDestroyWindow( "Example4-out" );

	return 0;
}
