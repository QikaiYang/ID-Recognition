#pragma once
#include"图像预处理.h"

Mat process1;//图像放缩后的临时存放
Mat process2;//图像剪切后的临时存放
Mat process3;//二值化处理后临时存放
Mat process4;//灰度化处理后临时存放
void CutBmp()
{
	Mat imageROI, Templmg;
	int nr = process1.rows * 3 / 4 + 6;
	int nc = process1.cols / 3 + 2;
	int heigth = process1.rows * 7 / 32;
	int width = process1.cols * 3 / 5;
	cout << nr << nc << endl;
	imageROI = process1(Rect(nc, nr, width, heigth));
	imageROI.convertTo(Templmg, Templmg.type());
	Templmg.copyTo(process2);
	//imshow("CutBmp", Templmg);
}
void CutBmp1()
{
	Mat imageROI, Templmg;
	int nr = process1.rows * 3 / 4 + 6;
	int nc = process1.cols / 3 + 2;
	int heigth = process1.rows * 5 / 32;
	int width = process1.cols * 3 / 5;
	cout << nr << nc << endl;
	imageROI = process1(Rect(nc, nr, width, heigth));
	imageROI.convertTo(Templmg, Templmg.type());
	Templmg.copyTo(process2);
	//imshow("CutBmp", Templmg);
}
int SmallBmp(char*Path, double sma_x, double sma_y)
{
	Mat org = imread(Path, 1);  //图片路径
	if (org.empty())
	{
		return 10;
	}
	Size dsize = Size(org.cols*sma_x, org.rows*sma_y);
	Mat image = Mat(dsize, CV_32S);
	resize(org, image, dsize);
	//imshow("SmallBmp", image);
	image.copyTo(process1);
}
int SmallBmp1(Mat org, double sma_x, double sma_y)
{  
	if (org.empty())
	{
		return 10;
	}
	Size dsize = Size(org.cols*sma_x, org.rows*sma_y);
	Mat image = Mat(dsize, CV_32S);
	resize(org, image, dsize);
	//imshow("SmallBmp", image);
	image.copyTo(process1);
}
void Binary(char path[])
{
	cvtColor(process2, process4, CV_BGR2GRAY);
	//imshow("Gray", process4);
	threshold(process4, process3, 120, 255, CV_THRESH_BINARY);
	imshow("Binary", process3);
	imwrite(path, process3);
}
