#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
vector<vector<Point> > contoursa;
vector<Vec4i> hierarchya;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

int fd;
void settings(const char *abc)
{
      fd = open(abc,O_RDWR | O_NOCTTY); /* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
      usleep(3500000);
                                    /* O_RDWR Read/Write access to serial port           */
                                    /* O_NOCTTY - No terminal will control the process   */
                                    /* O_NDELAY -Non Blocking Mode,Does not care about-  */
                                    /* -the status of DCD line,Open() returns immediatly */                                        
                                    
            if(fd == -1)                        /* Error Checking */
                   printf("\n  Error! in Opening ttyUSB0  ");
            else
                   printf("\n  ttyUSB0 Opened Successfully ");
       struct termios toptions;         /* get current serial port settings */
       tcgetattr(fd, &toptions);        /* set 9600 baud both ways */
       cfsetispeed(&toptions, B9600);
       cfsetospeed(&toptions, B9600);   /* 8 bits, no parity, no stop bits */
       toptions.c_cflag &= ~PARENB;
       toptions.c_cflag &= ~CSTOPB;
       toptions.c_cflag &= ~CSIZE;
       toptions.c_cflag |= CS8;         /* Canonical mode */
       toptions.c_lflag |= ICANON;       /* commit the serial port settings */
       tcsetattr(fd, TCSANOW, &toptions);
}
void sendCommand(const char *abc)
{
   write(fd, abc, 1);
}




#define PI 3.1415926

int frameWidth = 640;
int frameHeight = 480;

int flagimsep=0,flagled=1;

Mat img1,img2,img5,img3,img4,img7;

Mat source, destination;

void erosion(Mat imgin,Mat imgout);
void dilation(Mat imgin,Mat imgout);
int checkerosion(int i,int j,Mat img);
int checkdilation(int i,int j,Mat img);

int detectshape();

void findcentre();

int main()
{
	char c;
	settings("/dev/ttyACM0");
	int flagshape=0;
	VideoCapture capture(1);//
	if( !capture.isOpened()) throw "Error reading video";
	namedWindow("original", 0);
	namedWindow("circledetect", 0);
	namedWindow("circlecentre",0);
	namedWindow("triangle",1);
	while( true ) 
	{
		
		capture >> source;

		resize(source, destination,Size(frameWidth, frameHeight));
		
		int i,j,x,y,z;
		Mat img1=destination;
		Mat img2(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		Mat img3(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		Mat img6(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		Mat img7(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		for(i=0;i<img1.rows;i++)
		{
			for(j=0;j<img1.cols;j++)
			{
				x=img1.at<Vec3b>(i,j)[2];
				y=img1.at<Vec3b>(i,j)[1];
				z=img1.at<Vec3b>(i,j)[0];
				if(x>=13&&x<=215&&y>=28&&y<97&&z>=24&&z<=135) img2.at<uchar>(i,j)=255;
				else if(x>=81&&x<=186&&y>=11&&y<=89&&z>=4&&z<=112) 
				{
					img3.at<uchar>(i,j)=255;
					flagimsep=1;
				}
			}
		}
		erosion(img3,img6);
		dilation(img6,img7);
		Canny(img7,img7, 50, 150);
		
		
		findContours( img2, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
		vector<Moments> mu(contours.size());
		for( int i = 0; i<contours.size(); i++ )
		{ mu[i] = moments( contours[i], false ); }
		 
		// get the centroid of figures.
		vector<Point2f> mc(contours.size());
		for( int i = 0; i<contours.size(); i++)
		{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
		
		Mat drawing(img2.size(), CV_8UC3, Scalar(255,255,255));
		for( int i = 0; i<contours.size(); i++ )
		{
		Scalar color = Scalar(167,151,0); // B G R values
		Scalar color1 = Scalar(0,0,255);
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle( drawing, mc[i], 4, color1, -1, 8, 0 );
		}
		
 
		if(flagimsep) 
		{
			flagshape=detectshape();
		}
		flagimsep=0;
		Canny(img2,img4, 50, 150);
		
		if(flagshape==1)
		{
			
			if(flagled==1)
			{
				c='B';
				for(int j=0;j<1;j++) sendCommand(&c);
				flagled=0;
				
			}
			
			for( int i = 0; i<contours.size(); i++ )
			{
				if(mc[i].x<=img2.cols/2) break;
			}
			if(mc[i].x<img2.cols/6) c='A';      //we have to give command to arduino
			else if(mc[i].x>img2.cols/3) c='D';
			else c='W';
			sendCommand(&c);
		}
		else if(flagshape==2)
		{
			if(flagled==1)
			{
				c='B';
				for(int j=0;j<2;j++) sendCommand(&c);
				flagled=0;
				
			}
			
			for( int i = 0; i<contours.size(); i++ )
			{
				if(mc[i].x>=img2.cols/2) break;
			}
			if(mc[i].x<2*img2.cols/3) c='A';     //we have to give command to arduino
			else if(mc[i].x>5*img2.cols/6) c='D';
			else c='W';
			sendCommand(&c);
			
		}
		else
		{
			flagled=1;
			if(mc[0].x<img2.cols/3) c='A';      //we have to give command to arduino
			else if(mc[0].x>2*img2.cols/3) c='D';
			else c='W';
			sendCommand(&c);
		}
		imshow("original",destination);
		imshow("circledetect",img2);
		imshow("circlecentre",drawing);
		imshow("triangle",img3);
		waitKey(1);
	}
	return 0;
}

int detectshape()
{
	
	findContours(img7,contoursa,hierarchya,RETR_LIST,CHAIN_APPROX_NONE );
	
	std::vector<cv::Point> approx;

	for (int i = 0; i < contoursa.size(); i++)
	{
		
		cv::approxPolyDP(cv::Mat(contoursa[i]), approx, cv::arcLength(cv::Mat(contoursa[i]), true)*0.02, true);

		if (std::fabs(cv::contourArea(contoursa[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		if (approx.size() == 3)
		{
			return 1;
		}
		else if (approx.size() == 4 )
		{
			return 2;
		}
	}
	return 5;
			 
}

void erosion(Mat imgin,Mat imgout)
{
	int i,j;
	for(i=0;i<imgout.rows;i++)
	{
		for(j=0;j<imgout.cols;j++)
		{
			imgout.at<uchar>(i,j)=checkerosion(i,j,imgin);
		}
	}
}

int checkerosion(int i,int j,Mat img)
{
	int m,n;
	for(m=-1;m<=1;m++)
	{
		for(n=-1;n<=1;n++)
		{
			if(img.at<uchar>(i+m,j+n)==0) return 0;
		}
	}
	return 255;
}

void dilation(Mat imgin,Mat imgout)
{
	int i,j;
	for(i=0;i<imgout.rows;i++)
	{
		for(j=0;j<imgout.cols;j++)
		{
			imgout.at<uchar>(i,j)=checkdilation(i,j,imgin);
		}
	}
}

int checkdilation(int i,int j,Mat img)
{
	int m,n;
	for(m=-1;m<=1;m++)
	{
		for(n=-1;n<=1;n++)
		{
			if(img.at<uchar>(i+m,j+n)==255) return 255;
		}
	}
	return 0;
}
