#include <cv.h>
#include <highgui.h>


int main(int argc, char* argv[])
{
	IplImage *src, *templ, *ftmp[6]; // ftmp will hold results
	int i;

	// Read in the source image to be searched
	src = cvLoadImage("airplane.jpg");

	// Read in the template to be used for matching:
	templ = cvLoadImage("airtemplate.jpg");

	// Allocate Output Images:
	int iwidth = src->width - templ->width + 1;
	int iheight = src->height - templ->height + 1;
	for(i = 0; i < 6; ++i){
		ftmp[i]= cvCreateImage( cvSize( iwidth, iheight ), 32, 1 );
	}

	// Do the matching of the template with the image
	for( i = 0; i < 6; ++i ){
		cvMatchTemplate( src, templ, ftmp[i], i );
		cvNormalize( ftmp[i], ftmp[i], 1, 0, CV_MINMAX );
	}

	// DISPLAY
	cvNamedWindow( "Template", 0 );
	cvShowImage( "Template", templ );
	cvNamedWindow( "Image", 0 );
	cvShowImage( "Image", src );
	cvNamedWindow( "SQDIFF", 0 );
	cvShowImage( "SQDIFF", ftmp[0] );
	cvNamedWindow( "SQDIFF_NORMED", 0 );
	cvShowImage( "SQDIFF_NORMED", ftmp[1] );
	cvNamedWindow( "CCORR", 0 );
	cvShowImage( "CCORR", ftmp[2] );
	cvNamedWindow( "CCORR_NORMED", 0 );
	cvShowImage( "CCORR_NORMED", ftmp[3] );
	cvNamedWindow( "COEFF", 0 );
	cvShowImage( "COEFF", ftmp[4] );
	cvNamedWindow( "COEFF_NORMED", 0 );
	cvShowImage( "COEFF_NORMED", ftmp[5] );

	cvWaitKey(0);

	return 0;
}
