#include <iostream> 
#include<cv.h>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

class CRc
{
public:
	void recognition(IplImage *img);
	double distance(unsigned char counter[], unsigned char counter_1[][18]);
	void find(double sumdis[]);
};
