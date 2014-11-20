#include <cv.h>
#include <highgui.h>

int angle_switch_value = 0;
int angleInt = 0;
int scale_switch_value = 0;
int scaleInt = 0;

void switch_callback_a( int position ){
	angleInt = position;
}
void switch_callback_s( int position ){
	scaleInt = position;
}

int main(int argc, char* argv[])
{
	// Set up variables
	CvPoint2D32f srcTri[3], dstTri[3];
	CvMat* rot_mat = cvCreateMat(2,3,CV_32FC1);
	CvMat* warp_mat = cvCreateMat(2,3,CV_32FC1);
	IplImage *src, *dst;
	const char* name = "Affine_Transform";

	// Load image
	src=cvLoadImage("airplane.jpg");
	dst = cvCloneImage( src );
	dst->origin = src->origin;
	cvZero( dst );
	cvNamedWindow( name, 1 );

	// Create angle and scale
	double angle = 0.0;
	double scale = 1.0;

	// Create trackbars
	cvCreateTrackbar( "Angle", name, &angle_switch_value, 4, switch_callback_a );
	cvCreateTrackbar( "Scale", name, &scale_switch_value, 4, switch_callback_s );

	// Compute warp matrix
	srcTri[0].x = 0;
	srcTri[0].y = 0;
	srcTri[1].x = src->width - 1;
	srcTri[1].y = 0;
	srcTri[2].x = 0;
	srcTri[2].y = src->height - 1;

	dstTri[0].x = src->width*0.0;
	dstTri[0].y = src->height*0.25;
	dstTri[1].x = src->width*0.90;
	dstTri[1].y = src->height*0.15;
	dstTri[2].x = src->width*0.10;
	dstTri[2].y = src->height*0.75;

	cvGetAffineTransform( srcTri, dstTri, warp_mat );
	cvWarpAffine( src, dst, warp_mat );
	cvCopy ( dst, src );

	while( 1 ) {
		switch( angleInt ){
			case 0:
				angle = 0.0;
				break;
			case 1:
				angle = 20.0;
				break;
			case 2:
				angle = 40.0;
				break;
			case 3:
				angle = 60.0;
				break;
			case 4:
				angle = 90.0;
				break;
		}
		switch( scaleInt ){
			case 0:
				scale = 1.0;
				break;
			case 1:
				scale = 0.8;
				break;
			case 2:
				scale = 0.6;
				break;
			case 3:
				scale = 0.4;
				break;
			case 4:
				scale = 0.2;
				break;
		}

		// Compute rotation matrix
		CvPoint2D32f center = cvPoint2D32f( src->width/2, src->height/2 );
		cv2DRotationMatrix( center, angle, scale, rot_mat );

		// Do the transformation
		cvWarpAffine( src, dst, rot_mat );

		cvShowImage( name, dst );

		if( cvWaitKey( 15 ) == 27 )
			break;
	}

	cvReleaseImage( &dst );
	cvReleaseMat( &rot_mat );
	cvReleaseMat( &warp_mat );

	return 0;
}
