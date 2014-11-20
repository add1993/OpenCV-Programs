#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <cstdlib>

int g_switch_value = 0;
int colorInt = 0;

// Trackbar/switch callback
void switch_callback( int position ){
	if( position == 0 ){
		 colorInt = 0;
	}else{
		 colorInt = 1;
	}
}

int main(int argc, char* argv[])
{
	const char* name = "Demo Window";
	int radius = 30;
	int thickness = 2;
	int connectivity = 8;
	CvScalar green = CV_RGB(0,250,0);
	CvScalar orange = CV_RGB(250,150,0);

	IplImage* src1 = cvLoadImage( "airplane.jpg" );
	CvPoint pt2 = cvPoint(405,195);

	cvNamedWindow( name, 1 );
	cvShowImage(name, src1);
	

	// Create trackbar
	cvCreateTrackbar( "Switch", name, &g_switch_value, 1, switch_callback );

	// Loop to update the circle color
	while( 1 ) {
		if( colorInt == 0)
			cvCircle(src1,pt2,radius,green,thickness,connectivity);
		else
			cvCircle(src1,pt2,radius,orange,thickness,connectivity);
		cvShowImage(name, src1);
		if( cvWaitKey( 15 ) == 27 ) 
			break;
	}

	cvReleaseImage( &src1 );
	cvDestroyWindow( name );

	return 0;
}
