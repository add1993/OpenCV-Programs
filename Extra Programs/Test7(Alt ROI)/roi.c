#include <cv.h>
#include <highgui.h>

void saturate_sv( IplImage* img ) {
for( int y=0; y<img->height; y++ ) {
uchar* ptr = (uchar*) (
img->imageData + y * img->widthStep
);
for( int x=0; x<img->width; x++ ) {
ptr[3*x+1] = 255;
ptr[3*x+2] = 255;
}
}
}

int main(int argc, char** argv)
{
IplImage *interest_img = cvLoadImage("airplane.jpg");
CvRect interest_rect= cvRect(100,100, 100, 100);
CvRect interest_rect2= cvRect(200,200, 150, 150);
// Use widthStep to get a region of interest
//
// (Alternate method)
//
IplImage *sub_img = cvCreateImageHeader(cvSize(interest_rect.width,interest_rect.height),interest_img->depth,interest_img->nChannels);
IplImage *sub_img2 = cvCreateImageHeader(cvSize(interest_rect2.width,interest_rect2.height),interest_img->depth,interest_img->nChannels);
cvNamedWindow("ROI1", CV_WINDOW_AUTOSIZE);
cvShowImage("ROI1", interest_img);
cvNamedWindow("ROI2", CV_WINDOW_AUTOSIZE);
cvNamedWindow("ROI3", CV_WINDOW_AUTOSIZE);
sub_img->origin = interest_img->origin;
sub_img->widthStep = interest_img->widthStep;
sub_img->imageData = interest_img->imageData +
interest_rect.y * interest_img->widthStep +
interest_rect.x * interest_img->nChannels;
cvAddS( sub_img, cvScalar(1), sub_img );

sub_img2->origin = interest_img->origin;
sub_img2->widthStep = interest_img->widthStep;
sub_img2->imageData = interest_img->imageData +
interest_rect2.y * interest_img->widthStep +
interest_rect2.x * interest_img->nChannels;
cvAddS( sub_img2, cvScalar(150), sub_img2 );
cvShowImage("ROI2", sub_img);
cvShowImage("ROI3", sub_img2);
cvWaitKey(0);
cvReleaseImageHeader(&sub_img);
cvReleaseImageHeader(&sub_img2);
 
 return 0;
}
