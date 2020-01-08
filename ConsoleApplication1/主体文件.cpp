#include"Rc.h"
#include"图像预处理.h"
#include"图像预处理1.h"  

void main()
{
	CRc tt[18];
    printf("请输入待测身份证照片路径地址:\n");
	char path[30];
	scanf("%s", path);
	                                                //添加写入路径,为了提升兼容性，直接统一改文件名字母最后一个，防止混乱
	char path1[30];
	int i;
	for (i = 0; i < 30; i++)
	{
		path1[i] = path[i];
	}
	for (i = 0; i < 30; i++)
	{
		if (path1[i] == '.')
		{
			path1[i - 1] = '|';
		}
	}
	int a;
	printf("1人工输入，2标准输入, 3自动切割\n");
	scanf("%d",&a);
	Mat image_k;                                                                //储存原图像的MAT
    Mat image1_k;                                                               //储存原图像的另一个MAT
	switch (a)
	{
	case 1:
		image_k = imread(path);                                                 //读图，将目标路径的图像存到image中
		if (image_k.empty())
		{
			printf("您输入的图像有误，请重新输入\n");
			system("pause");
			return;
		}                   
		image_k = XUANZHUAN(image_k);                                           //傅里叶旋转进行调整
		org_k = image_k;
		resize(org_k, org_k, Size(640, 410), 0, 0, CV_INTER_LINEAR);              //变换尺度，便于输出
		org_k.copyTo(img_k);
		org_k.copyTo(tmp_k);
		namedWindow("img", 1);                                                  //定义一个img窗口  
		imshow("img", img_k);
		setMouseCallback("img", on_mouse, 0);                                   //调用回调函数  
		waitKey(0);
		for (i = 0; i < 18; i++)
		{
			if (ff[i].empty())
			{
				break;
			}
			resize(ff[i], ff[i], Size(12, 24), CV_INTER_LINEAR);
			imwrite("testt.jpg", ff[i]);
			tt[0].recognition(cvLoadImage("testt.jpg", 0));
		}
		system("pause");
		break;
	case 2:
		SmallBmp(path, 1, 1);
		if (SmallBmp(path, 1, 1) == 10)
		{
			printf("您输入的图像有误，请重新输入\n");
			system("pause");
			return;
		}
		CutBmp();
		Binary(path1);
		CutPerNumber(process3);
		for (i = 0; i < 18; i++)
		{
			if (ff[i].empty())
			{
				break;
			}
			resize(ff[i], ff[i], Size(12, 24), CV_INTER_LINEAR);
			imwrite("testt.jpg", ff[i]);
			tt[0].recognition(cvLoadImage("testt.jpg", 0));
		}
		system("pause");
		break;
	case 3:
		Mat original_x = imread(path);
		RNG &rng = theRNG();
		resize(original_x, original_x, Size(640, 410), 0, 0, CV_INTER_LINEAR);
		namedWindow("My original");
		imshow("My original", original_x);
		Mat gray = original_x;
		cv::cvtColor(gray, gray, CV_RGB2GRAY);                                                                                  //灰度化
		int thresh_size = (100 / 4) * 2 + 1;                                                                                    //自适应二值化阈值
		adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, thresh_size, thresh_size / 3);
		morphologyEx(gray, gray, MORPH_OPEN, Mat());                                                                            //形态学开运算去噪点

		vector<vector<Point> > contours;
		cv::findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);                                               //找轮廓
		vector<vector<Point>> contours1;
		for (int i = 0; i < contours.size(); ++i)
		{
			if (contours[i].size() > 10)                                                                                        //将比较小的轮廓剔除掉
			{
				contours1.push_back(contours[i]);
			}
		}
		vector<Point> points;                                                                                                   //找最大矩形
		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = 0; j < contours[i].size(); j++)
			{
				Point midpoint;
				midpoint = contours[i][j];
				points.push_back(midpoint);
			}
		}
		Rect rect = boundingRect(points);
		rectangle(original_x, rect, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1);                  //画矩形
		Point A(contours[0][0].x, contours[0][0].y); Point B(contours[0][0].x, contours[0][0].y);
		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = 0; j < contours[i].size(); j++)
			{
				if (contours[i][j].x<A.x)
				{
					A.x = contours[i][j].x;
				}
				if (contours[i][j].y<A.y)
				{
					A.y = contours[i][j].y;
				}
				if (contours[i][j].y > B.y)
				{
					B.y = contours[i][j].y;
				}
				if (contours[i][j].x > B.x)
				{
					B.x = contours[i][j].x;
				}
			}
		}
		int width = abs(A.x - B.x);
		int height = abs(A.y - B.y);
		original_x = original_x(Rect(min(A.x, B.x), min(A.y, B.y), width, height));                                     //剪矩形
		//调试专用
		/*namedWindow("My son");
		imshow("My son", original_x);
		waitKey(0);*/
		blur(original_x, original_x, Size (2,2));
		SmallBmp1(original_x, 1, 1);
		if (SmallBmp1(original_x, 1, 1) == 10)
		{
			printf("您输入的图像有误，请重新输入\n");
			system("pause");
			return;
		}
		CutBmp1();
		Binary(path1);
		CutPerNumber(process3);
		for (i = 0; i < 18; i++)
		{
			if (ff[i].empty())
			{
				break;
			}
			resize(ff[i], ff[i], Size(12, 24), CV_INTER_LINEAR);
			imwrite("testt.jpg", ff[i]);
			tt[0].recognition(cvLoadImage("testt.jpg", 0));
		}
		system("pause");
	}
}






    //鼠标切割函数运用示例
    //org = imread("D://timg.jpg");
	//org.copyTo(img);
	//org.copyTo(tmp);
	//namedWindow("img",WINDOW_NORMAL);//定义一个img窗口  
	//setMouseCallback("img", on_mouse, 0);//调用回调函数  
	//imshow("img", img);
	//cv::waitKey(0);	

     