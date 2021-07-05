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

#define PI 3.1415926


int frameWidth = 640;
int frameHeight = 480;

int main()
{
    VideoCapture capture(0);
    Mat source, destination;
    if( !capture.isOpened()) throw "Error reading video";


	int alpha_ = 90, beta_ = 90, gamma_ = 90;
	int f_ = 500, dist_ = 500;
	//change the above values to the values obtained from trackbar

	namedWindow("Result", 1);
	while( true ) 
	{
		capture >> source;
		resize(source, source,Size(frameWidth, frameHeight));
		double focalLength, dist, alpha, beta, gamma; 
		alpha =((double)alpha_ -90) * PI/180;
		beta =((double)beta_ -90) * PI/180;
		gamma =((double)gamma_ -90) * PI/180;
		focalLength = (double)f_;
		dist = (double)dist_;

		Size image_size = source.size();
		double w = (double)image_size.width, h = (double)image_size.height;
		Mat A1 = (Mat_<float>(4, 3)<< 
			1, 0, -w/2,
			0, 1, -h/2,
			0, 0, 0,
			0, 0, 1 );
		Mat RX = (Mat_<float>(4, 4) << 
			1, 0, 0, 0,
			0, cos(alpha), -sin(alpha), 0,
			0, sin(alpha), cos(alpha), 0,
			0, 0, 0, 1 );
		Mat RY = (Mat_<float>(4, 4) << 
			cos(beta), 0, -sin(beta), 0,
			0, 1, 0, 0,
			sin(beta), 0, cos(beta), 0,
			0, 0, 0, 1	);
		Mat RZ = (Mat_<float>(4, 4) << 
			cos(gamma), -sin(gamma), 0, 0,
			sin(gamma), cos(gamma), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1	);
		Mat R = RX * RY * RZ;
		Mat T = (Mat_<float>(4, 4) << 
			1, 0, 0, 0,  
			0, 1, 0, 0,  
			0, 0, 1, dist,  
			0, 0, 0, 1); 
		Mat K = (Mat_<float>(3, 4) << 
			focalLength, 0, w/2, 0,
			0, focalLength, h/2, 0,
			0, 0, 1, 0
			); 
		Mat transformationMat = K * (T * (R * A1));
		warpPerspective(source, destination, transformationMat, image_size, INTER_CUBIC | WARP_INVERSE_MAP);
		int i,j,x,y,z;
		Mat img1=destination;
		Mat img2(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		Mat img3(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		for(i=0;i<img1.rows;i++)
		{
			for(j=0;j<img1.cols;j++)
			{
				x=img1.at<Vec3b>(i,j)[2];
				y=img1.at<Vec3b>(i,j)[1];
				z=img1.at<Vec3b>(i,j)[0];
				if(x>=125&&x<=220&&y>=19&&y<=54&&z>=21&&z<=59) img2.at<uchar>(i,j)=255;
				/*else if(x>=223&&x<=253&&y>=134&&y<=204&&z>=2&&z<=74) img3.at<uchar>(i,j)=255;*/
				//this is the code to seperate images so add or change accordiingly
			}
		}
		Canny(img2,img3, 50, 150);
		findContours(img3,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE );
		vector<Moments> mu(contours.size());
		mu[0] = moments( contours[0], false );
		waitKey(10000);
		vector<Point2f> mc(contours.size());
		mc[0] = Point2f( mu[0].m10/mu[0].m00 , mu[0].m01/mu[0].m00 ); 
		cout<<mc[0]<<endl;/**/
		imshow("Result", img3);
		waitKey(100);
		destroyAllWindows();
	}


	return 0;
}
