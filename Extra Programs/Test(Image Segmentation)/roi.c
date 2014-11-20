#include <cv.h>
#include <highgui.h>
// STORE TO DISK A LINE SEGMENT OF BGR PIXELS FROM pt1 to pt2.
//
int main()
{
CvCapture* capture = cvCreateFileCapture( "tree.avi" );
CvPoint pt1, pt2;
int max_buffer;
IplImage* rawImage;
int r[10000],g[10000],b[10000];
CvLineIterator iterator;
FILE *fptrb = fopen("blines.csv","w"); // Store the data here
FILE *fptrg = fopen("glines.csv","w"); // for each color channel
FILE *fptrr = fopen("rlines.csv","w");
// MAIN PROCESSING LOOP:
//
for(;;){
if( !cvGrabFrame( capture ))
break;
rawImage = cvRetrieveFrame( capture );
max_buffer = cvInitLineIterator(rawImage,pt1,pt2,&iterator,8,0);
for(int j=0; j<max_buffer; j++){
fprintf(fptrb,"%d,", iterator.ptr[0]); //Write blue value
fprintf(fptrg,"%d,", iterator.ptr[1]); //green
fprintf(fptrr,"%d,", iterator.ptr[2]); //red
iterator.ptr[2] = 255; //Mark this sample in red
CV_NEXT_LINE_POINT(iterator); //Step to the next pixel
}
// OUTPUT THE DATA IN ROWS:
//
fprintf(fptrb,"/n");fprintf(fptrg,"/n");fprintf(fptrr,"/n");
}
// CLEAN UP:
//
fclose(fptrb); fclose(fptrg); fclose(fptrr);
cvReleaseCapture( &capture );
return 0;
}
