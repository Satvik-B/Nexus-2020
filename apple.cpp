#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;

Mat img1;

int ru=65,rl=12,bu=82,bl=0,gu=118,gl=18;

void showimage(int ru,int rl,int bu,int bl, int gu,int gl)
{
	Mat img2(img1.rows,img1.cols,CV_8UC1,Scalar(0));
	int x,y,z,i,j;
	for(i=0;i<img1.rows;i++)
	{
		for(j=0;j<img1.cols;j++)
		{
			x=img1.at<Vec3b>(i,j)[2];
			y=img1.at<Vec3b>(i,j)[1];
			z=img1.at<Vec3b>(i,j)[0];
			if(x>=rl&&x<=ru&&y>=bl&&y<=bu&&z>=gl&&z<=gu) img2.at<uchar>(i,j)=255;
			else img2.at<uchar>(i,j)=0;
		}
	}
	Canny(img2,img2,50,150);
	imshow("TrackBar",img2);
	imwrite("seperated.jpg",img2);
}

void callback(int a, void* c)
{
	showimage(ru,rl,bu,bl,gu,gl);
}

int main()
{
	 VideoCapture capture(1);
  
    // mat container to receive images
	    Mat source, destination;
	    
	    // check if capture was successful
	    if( !capture.isOpened()) throw "Error reading video";
	
	namedWindow("TrackBar",1);
	namedWindow("original",0);
	
	createTrackbar("redUpper","TrackBar",&ru,255);
	createTrackbar("redLower","TrackBar",&rl,255);
	createTrackbar("blueUpper","TrackBar",&bu,255);
	createTrackbar("blueLower","TrackBar",&bl,255);
	createTrackbar("greenUpper","TrackBar",&gu,255);
	createTrackbar("greenLower","TrackBar",&gl,255);
	
	while(1)
	{
		capture >> img1;
		imshow("original",img1);
		Mat img2(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		int x,y,z,i,j;
		for(i=0;i<img1.rows;i++)
		{
			for(j=0;j<img1.cols;j++)
			{
				x=img1.at<Vec3b>(i,j)[2];
				y=img1.at<Vec3b>(i,j)[1];
				z=img1.at<Vec3b>(i,j)[0];
				if(x>=rl&&x<=ru&&y>=bl&&y<=bu&&z>=gl&&z<=gu) img2.at<uchar>(i,j)=255;
				else img2.at<uchar>(i,j)=0;
			}
		}
		imshow("TrackBar",img2);
		waitKey(50);
	}
	return 0;
}

///home/satvikb/Downloads/samps/fruits.png
///home/satvikb/Downloads/nexus/2020-01-17-011226.jpg
