#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;

Mat img1=imread("/home/satvikb/Downloads/samps/fruits.png",1);
Mat img2(img1.rows,img1.cols,CV_8UC1,Scalar(0));
Mat img3(img1.rows,img1.cols,CV_8UC1,Scalar(0));

int main()
{
	int i,j,x,y,z;
	for(i=0;i<img1.rows;i++)
	{
		for(j=0;j<img1.cols;j++)
		{
			x=img1.at<Vec3b>(i,j)[2];
			y=img1.at<Vec3b>(i,j)[1];
			z=img1.at<Vec3b>(i,j)[0];
			if(x>=125&&x<=220&&y>=19&&y<=54&&z>=21&&z<=59) img2.at<uchar>(i,j)=255;
			else if(x>=223&&x<=253&&y>=134&&y<=204&&z>=2&&z<=74) img3.at<uchar>(i,j)=255;
		}
	}
	namedWindow("image1",1);
	namedWindow("image2",1);
	namedWindow("original",1);
	imwrite("apple.jpg",img2);
	imwrite("mango.jpg",img3);
	imshow("original",img1);
	imshow("image1",img2);
	imshow("image2",img3);
	waitKey(0);
	return 0; 
}
		

