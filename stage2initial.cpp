#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
vector<vector<Point> > contoursred;
vector<Vec4i> hierarchyred;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
vector<vector<Point> > contoursgreen;
vector<Vec4i> hierarchygreen;
vector<vector<Point> > contoursblue;
vector<Vec4i> hierarchyblue;

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

#define PI 3.1415926

int frameWidth = 640;
int frameHeight = 480;

int flagimsep=0,barflag=0,barflagread=0,count1=0,num,sum1=0;

Mat img1,imgcirc,imgredcirc,imggreencirc,imgbluecirc,barcode;

Mat source, destination;

int readbarcode();

int main()
{
	int flagshape=0;
	VideoCapture capture("/home/satvikb/Downloads/video2.webm");//
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
		Mat imgcirc(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		Mat imgredcirc(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		Mat imgbluecirc(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		Mat imggreencirc(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		Mat barcode(img1.rows,img1.cols,CV_8UC1,Scalar(0));
		for(i=0;i<img1.rows;i++)
		{
			for(j=0;j<img1.cols;j++)
			{
				x=img1.at<Vec3b>(i,j)[2];
				y=img1.at<Vec3b>(i,j)[1];
				z=img1.at<Vec3b>(i,j)[0];
				if(x>=10&&x<=84&&y>=67&&y<=153&&z>=47&&z<=132)//for red circle
				{
					imgcirc.at<uchar>(i,j)=255;
					imgredcirc.at<uchar>(i,j)=255;
				}
				else if(x>=89&&x<=231&&y>=11&&y<=67&&z>=00&&z<=152)//for green circle
				{
					imgcirc.at<uchar>(i,j)=255;
					imggreencirc.at<uchar>(i,j)=255;
				}
				else if(x>=10&&x<=84&&y>=67&&y<=153&&z>=47&&z<=132)//for blue circle
				{
					imgcirc.at<uchar>(i,j)=255;
					imgbluecirc.at<uchar>(i,j)=255;
				}
				else if(x>=10&&x<=84&&y>=67&&y<=153&&z>=47&&z<=132)
				{
					//for barcode segmentation
					barflag=1;
				}
			}
		}
		
		
		
		
		
		
		
		
		
		
		if(barflag)
		{
			if(!barflagread)
			{
				//num=readbarcode();
				sum1+=num;
				count1++;
				barflagread=1;
			}
			if(num==0||num==3||num==6||num==9||num==12||num==15)
			{
				//finding centre of green image
		
				findContours( imggreencirc, contoursgreen, hierarchygreen, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
				vector<Moments> mugreen(contoursgreen.size());
				for( int i = 0; i<contoursgreen.size(); i++ )
				{ mugreen[i] = moments( contoursgreen[i], false ); }
				vector<Point2f> mcgreen(contoursgreen.size());
				for( int i = 0; i<contoursgreen.size(); i++)
				{ mcgreen[i] = Point2f( mugreen[i].m10/mugreen[i].m00 , mugreen[i].m01/mugreen[i].m00 ); }
				
				
				//commanding movement
				if(mcgreen[0].x<imggreencirc.cols/3) cout<<"A"<<endl;      //we have to give command to arduino
				else if(mcgreen[0].x>2*imggreencirc.cols/3) cout<<"D"<<endl;
				else cout<<"W"<<endl;
			
			}
			else if(num==1||num==4||num==7||num==10||num==13)
			{
				//finding centres of red image
				findContours( imgredcirc, contoursred, hierarchyred, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
				vector<Moments> mured(contoursred.size());
				for( int i = 0; i<contoursred.size(); i++ )
				{ mured[i] = moments( contoursred[i], false ); }
				vector<Point2f> mcred(contoursred.size());
				for( int i = 0; i<contoursred.size(); i++)
				{ mcred[i] = Point2f( mured[i].m10/mured[i].m00 , mured[i].m01/mured[i].m00 ); }
				
				//commanding movement
				if(mcred[0].x<imgredcirc.cols/3) cout<<"A"<<endl;      //we have to give command to arduino
				else if(mcred[0].x>2*imgredcirc.cols/3) cout<<"D"<<endl;
				else cout<<"W"<<endl;
		
			}
			else if(num==2||num==5||num==8||num==11||num==14)
			{
				
				//finding centre of blue image
		
				findContours( imgbluecirc, contoursblue, hierarchyblue, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
				vector<Moments> mublue(contoursblue.size());
				for( int i = 0; i<contoursblue.size(); i++ )
				{ mublue[i] = moments( contoursblue[i], false ); }
				vector<Point2f> mcblue(contoursblue.size());
				for( int i = 0; i<contoursblue.size(); i++)
				{ mcblue[i] = Point2f( mublue[i].m10/mublue[i].m00 , mublue[i].m01/mublue[i].m00 ); }
				
				//commanding movement
				if(mcblue[0].x<imgbluecirc.cols/3) cout<<"A"<<endl;      //we have to give command to arduino
				else if(mcblue[0].x>2*imgbluecirc.cols/3) cout<<"D"<<endl;
				else cout<<"W"<<endl;
		
			}
		}
		else
		{
			//finding centre
			findContours( imgcirc, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
			vector<Moments> mu(contours.size());
			for( int i = 0; i<contours.size(); i++ )
			{ mu[i] = moments( contours[i], false ); }
			vector<Point2f> mc(contours.size());
			for( int i = 0; i<contours.size(); i++)
			{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
		
			//commanding movement
			if(mc[0].x<imgcirc.cols/3) cout<<"A"<<endl;      //we have to give command to arduino
			else if(mc[0].x>2*imgcirc.cols/3) cout<<"D"<<endl;
			else cout<<"W"<<endl;
			
			barflagread=0;
		}
	}
		
	return 0;		
}		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
