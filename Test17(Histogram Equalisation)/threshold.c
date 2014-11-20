#include <cv.h>
#include <highgui.h>


int main(int argc, char* argv[])
{
	// Set up images
	const char* name = "Histogram Equalization";
	IplImage *img = cvLoadImage("airplane.jpg", 0);
	IplImage* out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );

	// Show original
	cvNamedWindow( "Original", 1) ;
	cvShowImage( "Original", img );

	// Perform histogram equalization
	cvEqualizeHist( img, out );

	// Show histogram equalized
	cvNamedWindow( name, 1) ;
	cvShowImage( name, out );

	cvWaitKey();

	cvReleaseImage( &img );
	cvReleaseImage( &out );
	

	return 0;
}
