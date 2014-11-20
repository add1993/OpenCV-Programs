#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <cstdlib>

void sum_rgb( IplImage* src, IplImage* dst ){
	
	// Allocate image planes
	IplImage* r = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* g = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* b = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );

	// Split image onto the color planes
	cvSplit( src, r, g, b, NULL );

	IplImage* s = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	
	// Add equally weighted rgb values
	cvAddWeighted( r, 1./3., g, 1./3., 0.0, s );
	cvAddWeighted( s, 2./3., b, 1./3., 0.0, s );

	// Truncate values over 100
	cvThreshold( s, dst, 100, 100, CV_THRESH_TRUNC );

	cvReleaseImage( &r );
	cvReleaseImage( &g );
	cvReleaseImage( &b );
	cvReleaseImage( &s );
}

int main(int argc, char* argv[])
{
	const char* name = "Thresholding";
	cvNamedWindow( name, 1 );

	IplImage* src = cvLoadImage("airplane.jpg");
	IplImage* dst = cvCreateImage( cvGetSize(src), src->depth, 1 );
	sum_rgb( src, dst);

	cvShowImage( name, dst);

	while( 1 ){
		if( (cvWaitKey(10)&0x7f) == 27 )
			break;
	}

	cvDestroyWindow( name );
	cvReleaseImage( &src );
	cvReleaseImage( &dst );

	return 0;
}
