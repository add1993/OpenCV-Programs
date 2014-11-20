#include <cv.h>
#include <highgui.h>
int main(int argc, char** argv)
{
 IplImage *src1, *src2;
 src1=cvLoadImage("airplane.jpg",1);
 src2=cvLoadImage("right01.jpg",1);
 {
 int x = 100;
 int y = 100;
 int width = 200;

 int height = 200;
 double alpha = .1;
 double beta = .9;
 cvSetImageROI(src1, cvRect(x,y,width,height));
 cvSetImageROI(src2, cvRect(0,0,width,height));
 cvAddWeighted(src1, alpha, src2, beta,0.0,src1);
 cvResetImageROI(src1);
 cvNamedWindow( "Alpha_blend", 1 );
 cvShowImage( "Alpha_blend", src1 );
 cvWaitKey(0);
}
return 0;
}
