#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

Mat img1=imread("mango.jpg",0);
Mat img2(img1.rows,img1.cols,CV_8UC3,Scalar(0,0,0)),img3=img2;

int main()
{
	Canny(img1,img2, 50, 150);
	
	findContours(img2,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE );
	
	vector<Moments> mu(contours.size());
	
	
	mu[0] = moments( contours[0], false );
	vector<Point2f> mc(contours.size());
	mc[0] = Point2f( mu[0].m10/mu[0].m00 , mu[0].m01/mu[0].m00 ); 
	cout<<mc[0]<<endl;
	return 0;
}

