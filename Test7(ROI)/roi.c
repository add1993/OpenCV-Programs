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
 IplImage* src;
 src=cvLoadImage("right01.jpg",1);

 int x = 50;
 int y = 100;
 int width = 150;
 int height = 100;
 int add = 150;
 cvSetImageROI(src, cvRect(x,y,width,height));
 cvAddS(src, cvScalar(add),src);
 cvNamedWindow( "Roi_Add", 1 );
 cvShowImage( "Roi_Add", src );
 cvResetImageROI(src);

 cvNamedWindow( "Saturate", 1 );
 saturate_sv(src);
 cvShowImage( "Saturate", src );
 cvWaitKey(0);
 return 0;
}
