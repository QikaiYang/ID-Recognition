#include"Rc.h"

void CRc::recognition(IplImage *img)
{
	IplImage  *imadata0_w, *imadata1_w, *imadata2_w, *imadata3_w, *imadata4_w, *imadata5_w, *imadata6_w, *imadata7_w, *imadata8_w, *imadata9_w, *imadataX_w, *image_w;
	image_w = img; //读取测试的数字  
	imadata0_w = cvLoadImage("D://0.jpg", 0);
	imadata1_w = cvLoadImage("D://1.jpg", 0);
	imadata2_w = cvLoadImage("D://2.jpg", 0);
	imadata3_w = cvLoadImage("D://3.jpg", 0);
	imadata4_w = cvLoadImage("D://4.jpg", 0);
	imadata5_w = cvLoadImage("D://5.jpg", 0);
	imadata6_w = cvLoadImage("D://6.jpg", 0);
	imadata7_w = cvLoadImage("D://7.jpg", 0);
	imadata8_w = cvLoadImage("D://8.jpg", 0);
	imadata9_w = cvLoadImage("D://9.jpg", 0);
	imadataX_w = cvLoadImage("D://X.jpg", 0);
	//cvNamedWindow("image", 2);
	//将8*16的图像直接分割为4*4  12*24
	int row_1, col_1;
	unsigned char *ptr, *ptr_1[11];
	//  scanf("%d,%d",row_1,col_1);  
	row_1 = 3, col_1 = 6;
	int stepx = image_w->width / row_1;
	int stepy = image_w->height / col_1;
	//  int count=row_1*col_1;  
	unsigned char counter_w[18] = { 0 };
	unsigned char counter_1_w[11][18] = { 0 };
	for (int row = 0; row<row_1; row++)
	{
		for (int col = 0; col<col_1; col++)
		{
			for (int i = 0 + stepx*row; i<stepx*(row + 1); i++)
			{
				for (int j = 0 + stepy*col; j<stepy*(col + 1); j++)
				{
					ptr = (unsigned char*)image_w->imageData + j*image_w->widthStep + i;
					if (*ptr < 100)
						counter_w[row*col_1 + col] += 1;
					ptr_1[0] = (unsigned char*)imadata0_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[0] < 100)
						counter_1_w[0][row*col_1 + col] += 1;
					ptr_1[1] = (unsigned char*)imadata1_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[1]<100)
						counter_1_w[1][row*col_1 + col] += 1;
					ptr_1[2] = (unsigned char*)imadata2_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[2]<100)
						counter_1_w[2][row*col_1 + col] += 1;
					ptr_1[3] = (unsigned char*)imadata3_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[3]<100)
						counter_1_w[3][row*col_1 + col] += 1;
					ptr_1[4] = (unsigned char*)imadata4_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[4]<100)
						counter_1_w[4][row*col_1 + col] += 1;
					ptr_1[5] = (unsigned char*)imadata5_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[5]<100)
						counter_1_w[5][row*col_1 + col] += 1;
					ptr_1[6] = (unsigned char*)imadata6_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[6]<100)
						counter_1_w[6][row*col_1 + col] += 1;
					ptr_1[7] = (unsigned char*)imadata7_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[7] < 100)
						counter_1_w[7][row*col_1 + col] += 1;
					ptr_1[8] = (unsigned char*)imadata8_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[8]<100)
						counter_1_w[8][row*col_1 + col] += 1;
					ptr_1[9] = (unsigned char*)imadata9_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[9]<100)
						counter_1_w[9][row*col_1 + col] += 1;
					ptr_1[10] = (unsigned char*)imadataX_w->imageData + j*image_w->widthStep + i;
					if (*ptr_1[10]<100)
						counter_1_w[10][row*col_1 + col] += 1;
				}
			}
		}
	}

	//for (int cj = 0; cj<18; cj++) {
	//	/*printf("%d\n", counter_w[cj]);*/
	//}

	////system("pause");

	//for (int ci = 0; ci<11; ci++)
	//{
	//	cout << endl;
	//	for (int cj = 0; cj<18; cj++) {
	//		/*printf("%d\n", counter_1_w[ci][cj]);*/
	//	}
	//}
	//欧氏距离  
	int dis[8];
	dis[0] = distance(counter_w, counter_1_w);
	// for(ci=0;ci<8;ci++)  
	/*printf("%d\n", dis[0]);*/
	/*cvWaitKey(0);*/
}

//欧氏距离,注意不同数据类型间的强制转换~  
double CRc::distance(unsigned char counter[], unsigned char counter_1[][18]) {
	int i, j;
	double sumdis[11], sum;
	sum = 0;
	double sum_1[11] = { 0 };

	for (i = 0; i<11; i++) {
		for (j = 0; j<18; j++) {
			sumdis[i] = (int)(sumdis[i] + (int)pow(((int)counter_1[i][j] - (int)counter[j]), 2));
		}
		/*printf("%f\n", sumdis[i]);*/
	}
	find(sumdis);
	return 0;
}
void CRc::find(double sumdis[]) {
	double temp;
	int c = 0;
	temp = sumdis[0];
	for (int rank = 0; rank<11; rank++)
	{
		if (sumdis[rank]<temp)
		{
			temp = sumdis[rank];
			c = rank;
			//printf("局部最佳匹配数%d\n", rank);
		}
	}
	if (c == 10) cout /*<< "最佳匹配数:"*/  << "X" << endl;
	else
	{
		printf("%d", c);
	}
}