#pragma once
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>  
#include <iostream>  
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "process.h"
#include "math.h"
#include "Windows.h"
#include <string>
#include<opencv2\highgui\highgui.hpp>
using namespace std;
using namespace cv;
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
void SmallBmp(char*Path, double sma_x, double sma_y)
{
	Mat org = imread(Path, 1);  //图片路径
	Size dsize = Size(org.cols*sma_x, org.rows*sma_y);
	Mat image = Mat(dsize, CV_32S);
	resize(org, image, dsize);
	//imshow("SmallBmp", image);
	image.copyTo(process1);

}
void Binary()
{
	cvtColor(process2, process4, CV_BGR2GRAY);
	//imshow("Gray", process4);
	threshold(process4, process3, 120, 255, CV_THRESH_BINARY);
	imshow("Binary", process3);
	imwrite("E://test//yqz.bmp", process3);
}
