#include <cv.h>
#include <highgui.h>

int main(int argc, _TCHAR* argv[])
{
	IplImage* src1 = cvLoadImage( "airplane.jpg" );
	
	// Line variables
	CvPoint pt1 = cvPoint(250,60);
	CvPoint pt2 = cvPoint(405,195);
	CvScalar red = CV_RGB(250,0,0);
	int thickness = 2;
	int connectivity = 8;

	// Circle variables
	int radius = 30;
	CvScalar blue = CV_RGB(0,0,250);

	// Text variables
	const char* text = "testing";
	double hscale = 1.0;
	double vscale = 0.8;
	double shear = 1;
	int thickness2 = 1;
	int line_type = 8;

	CvFont font1;
	cvInitFont(&font1,CV_FONT_HERSHEY_DUPLEX,hscale,vscale,shear,thickness,line_type);

	cvLine(src1,pt1,pt2,red,thickness,connectivity);
	cvCircle(src1,pt2,radius,blue,thickness,connectivity);
	cvPutText(src1,text,pt1,&font1,blue);
	
	cvNamedWindow("Drawing_and_Text", 1);
	cvShowImage("Drawing_and_Text", src1);
	cvWaitKey();

	return 0;
}
