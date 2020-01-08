#pragma once
#pragma warning(disable:4996)
#define _USE_MATH_DEFINES
#include"cv.h"
#include"highgui.h"
#include"cvaux.h"
#include"cxcore.h"
#include"opencv2\opencv.hpp"
#include"opencv2/imgproc.hpp"
#include<iostream>
#include<istream>
#include<stdio.h>
#include<string.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cvaux.h>
cv::Mat org_k, dst_k, img_k, tmp_k;
using namespace std;
using namespace cv;
Mat ff[18];

//原来初始化处理
//void pretackle()
//{
//	printf("请输入待测身份证照片路径地址:\n");
//    char path[30];
//	scanf("%s", path);                                       
//	Mat image;//原图像
//	Mat image1;                                               //二值化过后的图像
//	image = imread(path, 0);                                  //读入图片完成！！！！！！！！！！！！！注意是灰度图片！！！！！！！
//    threshold(image, image1, 100, 255, THRESH_BINARY_INV);    //二值化图像并将其储存到image1这个MAT中
//
//	//检测图片专用，误删！！！
//	namedWindow("图", WINDOW_NORMAL);
//	imshow("图", image1);
//	waitKey(0);
//}


////杨雨尧
void deal(cv::Mat& img_y, cv::Mat& out_y)
{
	// Convert to gray  
	/*cv::cvtColor(img_y, out_y, CV_BGR2GRAY);*/
	// Invert the image  
	cv::threshold(out_y, out_y, 128, 255, cv::THRESH_BINARY_INV);
}

Mat CutHight(Mat &Img_y)
{
	bool inBlock = false;//是否遍历到了字符区内
	int startIndex = 0;//记录进入字符区的索引
	int endIndex = 0;//记录进入空白区域的索引
	int width = Img_y.cols;
	int height = Img_y.rows;
	int perPixelValue;//每个像素的值
	int* projectValArry = new int[height];//创建一个用于储存每列白色像素个数的数组
	memset(projectValArry, 0, height * 4);//必须初始化数组
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			perPixelValue = Img_y.at<unsigned char>(row, col);
			if (perPixelValue == 255)//如果是黑底白字
			{
				projectValArry[row]++;
			}
		}
	}
	if (projectValArry[0] == 0 && projectValArry[height - 1] == 0) {
		for (int i = 0; i < height; ++i) {
			if (!inBlock && projectValArry[i] != 0) {
				inBlock = true;
				startIndex = i;
			}
			else if (inBlock && projectValArry[i] == 0) {
				inBlock = false;
				endIndex = i;
			}
		}
	}
	Mat newImg = Img_y(Range(startIndex, endIndex + 1), Range(0, width));
	delete[] projectValArry;
	projectValArry = nullptr;
	return newImg;
}

int CutPerNumber(Mat &Img_y)
{
	string name;
	int num = 1;
	deal(Img_y, Img_y);
	int width = Img_y.cols;
	int height = Img_y.rows;

	int perPixelValue;//每个像素的值
	int* projectValArry = new int[width];//创建一个用于储存每列白色像素个数的数组
	memset(projectValArry, 0, width * 4);//必须初始化数组
										 //遍历每一列的图像灰度值，查找每一行255的值

	for (int col = 0; col < width; ++col)
	{
		for (int row = 0; row < height; ++row)
		{
			perPixelValue = Img_y.at<uchar>(row, col);
			if (perPixelValue == 255)//如果是黑底白字
			{
				projectValArry[col]++;
			}
		}
	}

	/*			画出投影时的投影图			*/
	//Mat verticalProjectionMat(height, width, CV_8UC1);
	//for (int i = 0; i < height; i++)
	//{
	//	for (int j = 0; j < width; j++)
	//	{
	//		perPixelValue = 255;  //背景设置为白色。   
	//		verticalProjectionMat.at<uchar>(i, j) = perPixelValue;
	//	}
	//}

	/*将直方图的曲线设为黑色*/
	//for (int i = 0; i < width; i++)
	//{
	//	for (int j = 0; j < projectValArry[i]; j++)
	//	{
	//		perPixelValue = 0;  //直方图设置为黑色  
	//		verticalProjectionMat.at<uchar>(height - 1 - j, i) = perPixelValue;
	//	}
	//}
	//imshow("【投影】", verticalProjectionMat);

	int startIndex = 0;//记录进入字符区的索引
	int endIndex = 0;//记录进入空白区域的索引
	bool inBlock = false;//是否遍历到了字符区内
	for (int i = 0; i < Img_y.cols; ++i)
	{
		if (!inBlock && projectValArry[i] != 0)//进入字符区了
		{
			inBlock = true;
			startIndex = i;
		}
		else if (projectValArry[i] == 0 && inBlock)//进入空白区了
		{
			endIndex = i;
			inBlock = false;
			Mat roiImg = Img_y(Range(0, Img_y.rows), Range(startIndex, endIndex + 1));
			roiImg = CutHight(roiImg);
			name = to_string(num); //name是图像的名字
			/*imshow(name, roiImg);*////////////////////////////////////////////////调试专用，可注释！！！！！！！！！！！！！！！！！！！！！！！！！！！
			ff[num - 1] = roiImg;
			name += ".jpg";
			imwrite(name, roiImg);
			++num;
		}
	}
	delete[]projectValArry;
	waitKey();
	return 0;
}

//鼠标操作剪切图像
void on_mouse(int event, int x, int y, int flags, void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{
	static Point pre_pt = (-1, -1);//初始坐标  
	static Point cur_pt = (-1, -1);//实时坐标  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		org_k.copyTo(img_k);//将原始图片复制到img中  
		sprintf(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		putText(img_k, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//在窗口上显示坐标  
		circle(img_k, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
		imshow("img", img_k);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{
		img_k.copyTo(tmp_k);//将img复制到临时图像tmp上，用于显示实时坐标  
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp_k, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
		imshow("img", tmp_k);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{
		img_k.copyTo(tmp_k);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp_k, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(tmp_k, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
		imshow("img", tmp_k);
	}
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
	{
		org_k.copyTo(img_k);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(img_k, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		circle(img_k, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		rectangle(img_k, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上  
		imshow("img", img_k);
		img_k.copyTo(tmp_k);
		//截取矩形包围的图像，并保存到dst中  
		int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);
		if (width == 0 || height == 0)
		{
			printf("width == 0 || height == 0");
			return;
		}
		dst_k= org_k(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
		threshold(dst_k, dst_k, 100, 255, THRESH_BINARY_INV);  
		threshold(dst_k, dst_k, 100, 255, THRESH_BINARY_INV);
		/*imshow("jhjh", dst_k);*/
		waitKey(0);
		CutPerNumber(dst_k);
		waitKey(0);
	}
}
//傅里叶变换进行旋转校正
Mat XUANZHUAN(Mat srcImage_k)
{
	Mat srcGray;
	cvtColor(srcImage_k, srcGray, CV_RGB2GRAY);
	const int nRows = srcGray.rows;
	const int nCols = srcGray.cols;
	//计算傅里叶变换尺寸
	int cRows = getOptimalDFTSize(nRows);
	int cCols = getOptimalDFTSize(nCols);
	Mat sizeConvMat;
	copyMakeBorder(srcGray, sizeConvMat, 0, cRows - nRows, 0, cCols - nCols, BORDER_CONSTANT, Scalar::all(0));

	//图像DFT变换
	//通道组建立
	Mat groupMats[] = { Mat_<float>(sizeConvMat), Mat::zeros(sizeConvMat.size(), CV_32F) };
	Mat mergeMat;
	//把两页合成一个2通道的mat  
	merge(groupMats, 2, mergeMat);
	//对上面合成的mat进行离散傅里叶变换，支持原地操作，傅里叶变换结果为复数，通道1存的是实部，通道2存的是虚部。  
	dft(mergeMat, mergeMat);
	//把变换的结果分割到各个数组的两页中，方便后续操作  
	split(mergeMat, groupMats);
	//求傅里叶变化各频率的幅值，幅值放在第一页中  
	magnitude(groupMats[0], groupMats[1], groupMats[0]);
	Mat magnitudeMat = groupMats[0].clone();
	//归一化操作，幅值加1
	magnitudeMat += Scalar::all(1);
	//傅里叶变换的幅度值范围大到不适合在屏幕上显示，高值在屏幕上显示为白点，而低值为黑点，  
	//高低值的变化无法有效分辨，为了在屏幕上凸显出高低的变化得连续性，我们可以用对数尺度来替换线性尺度 
	log(magnitudeMat, magnitudeMat);
	//归一化
	normalize(magnitudeMat, magnitudeMat, 0, 1, CV_MINMAX);
	magnitudeMat.convertTo(magnitudeMat, CV_8UC1, 255, 0);
	/*imshow("magnitudeMat2", magnitudeMat);*/
	//重新分配象限，使(0,0)移动到图像中心，  
	//傅里叶变换之前要对源图像乘以(-1)^(x+y)，进行中心化  
	//这是对傅里叶变换结果进行中心化  
	int cx = magnitudeMat.cols / 2;
	int cy = magnitudeMat.rows / 2;
	Mat tmp;
	//Top-Left--为每一个象限创建ROI  
	Mat q0(magnitudeMat, Rect(0, 0, cx, cy));
	//Top-Right  
	Mat q1(magnitudeMat, Rect(cx, 0, cx, cy));
	//Bottom-Left  
	Mat q2(magnitudeMat, Rect(0, cy, cx, cy));
	//Bottom-Right  
	Mat q3(magnitudeMat, Rect(cx, cy, cx, cy));
	//交换象限，(Top-Left with Bottom-Right)  
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//交换象限，（Top-Right with Bottom-Letf）  
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	Mat binaryMagnMat;
	threshold(magnitudeMat, binaryMagnMat, 155, 255, CV_THRESH_BINARY);
	vector<Vec2f> lines;
	binaryMagnMat.convertTo(binaryMagnMat, CV_8UC1, 255, 0);
	HoughLines(binaryMagnMat, lines, 1, CV_PI / 180, 100, 0, 0);
	cout << "lines.size:  " << lines.size() << endl;
	Mat houghMat(binaryMagnMat.size(), CV_8UC3);
	//绘制检测线
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		//坐标变换生成线表达式
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(houghMat, pt1, pt2, Scalar(0, 0, 255), 1, 8, 0);
	}
	/*imshow("houghMat", houghMat);*/
	float theta = 0;
	//检测线角度判断
	for (size_t i = 0; i < lines.size(); i++)
	{
		float thetaTemp = lines[i][1] * 180 / CV_PI;
		if (thetaTemp > 0 && thetaTemp < 90)
		{
			theta = thetaTemp;
			break;
		}
	}
	//角度转换
	float angelT = nRows*tan(theta / 180 * CV_PI) / nCols;
	theta = atan(angelT) * 180 / CV_PI;
	cout << "theta: " << theta << endl;

	//取图像中心
	Point2f centerPoint = Point2f(nCols / 2, nRows / 2);
	double scale = 1;
	//计算旋转中心
	Mat warpMat = getRotationMatrix2D(centerPoint, theta, scale);
	//仿射变换
	Mat resultImage(srcGray.size(), srcGray.type());
	warpAffine(srcGray, resultImage, warpMat, resultImage.size());
	return resultImage;
}

