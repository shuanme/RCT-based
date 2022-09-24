#include "image_Process.h"

using namespace cv;
using namespace std;

void Convert_to_Bayer(int height, int width, Mat& bayerImg, const Mat& bgrImg, const Mat& CFA)
{
	bayerImg = Mat::zeros(height, width, CV_8UC1);
	int channel;


	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			channel = GetColor(CFA, y, x);

			//cout << y << "," << x << endl;
			if (channel == CFA_B)
				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[0];
			else if (channel == CFA_G)
				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[1];
			else if (channel == CFA_R)
				bayerImg.at<uchar>(y, x) = bgrImg.at<Vec3b>(y, x)[2];
		}
}

Mat Bayer_to_YDgCoCg_Suzuki(Mat& bayerImg, Mat& YDgCoCgImg)
{
	Mat temp_bayerImg;
	Mat signImg;
	temp_bayerImg = bayerImg;

	YDgCoCgImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	signImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	for (int i = 0; i < YDgCoCgImg.rows; i += 2) {
		for (int j = 0; j < YDgCoCgImg.cols; j += 2) {
			//Co
			if ((1 * int(temp_bayerImg.at<uchar>(i, j)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) < 0)
				signImg.at<uchar>(i, j) = 0;
			else 
				signImg.at<uchar>(i, j) = 255;
			YDgCoCgImg.at<uchar>(i, j) = abs(1 * int(temp_bayerImg.at<uchar>(i, j)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))); //Co component
			//Cg
			if (((-0.5) * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + (-0.5) * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) < 0))
				signImg.at<uchar>(i, j + 1) = 0;
			else
				signImg.at<uchar>(i, j + 1) = 255;
			YDgCoCgImg.at<uchar>(i, j + 1) = abs((-0.5) * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + (-0.5) * int(temp_bayerImg.at<uchar>(i + 1, j + 1))); //Cg component
			//Y
			if ((0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) < 0))
				signImg.at<uchar>(i + 1, j + 1) = 0;
			else
				signImg.at<uchar>(i + 1, j + 1) = 255;
			YDgCoCgImg.at<uchar>(i + 1, j + 1) = abs(0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) + 0.5; //Y component
			//Dg
			if (((-1) * int(temp_bayerImg.at<uchar>(i, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				signImg.at<uchar>(i + 1, j) = 0;
			else
				signImg.at<uchar>(i + 1, j) = 255;
			YDgCoCgImg.at<uchar>(i + 1, j) = abs((-1) * int(temp_bayerImg.at<uchar>(i, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j))); //Dg component
		}
	}
	return signImg;
}

Mat Bayer_to_YDgCbCr(Mat& bayerImg, Mat& YDgCbCrImg)
{
	Mat temp_bayerImg;
	Mat signImg;
	temp_bayerImg = bayerImg;
	YDgCbCrImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	signImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	for (int i = 0; i < YDgCbCrImg.rows; i += 2) {
		for (int j = 0; j < YDgCbCrImg.cols; j += 2) {
			if ((1 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				signImg.at<uchar>(i, j) = 0;
			else
				signImg.at<uchar>(i, j) = 255;
			YDgCbCrImg.at<uchar>(i, j) = abs(1 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j))); //Cr component
			/////////////////////////////////////////////////////////
			if ((0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) < 0)
				signImg.at<uchar>(i, j + 1) = 0;
			else
				signImg.at<uchar>(i, j + 1) = 255;
			YDgCbCrImg.at<uchar>(i, j + 1) = abs(0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) + 0.5; //Y component
			/////////////////////////////////////////////////////////
			if (((-0.5) * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) < 0)
				signImg.at<uchar>(i + 1, j + 1) = 0;
			else
				signImg.at<uchar>(i + 1, j + 1) = 255;
			YDgCbCrImg.at<uchar>(i + 1, j + 1) = abs((-0.5) * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))); //Cb component
			/////////////////////////////////////////////////////////
			if (((-1) * int(temp_bayerImg.at<uchar>(i, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				signImg.at<uchar>(i + 1, j) = 0;
			else
				signImg.at<uchar>(i + 1, j) = 255;
			YDgCbCrImg.at<uchar>(i + 1, j) = abs((-1) * int(temp_bayerImg.at<uchar>(i, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j))); //Dg component
		}
	}
	return signImg;
}
void Bayer_to_YDgCoCg_Suzuki1(Mat& bayerImg, Mat& YDgCoCgImg)
{
	Mat temp_bayerImg;
	temp_bayerImg = bayerImg;
	/*補邊，空出邊界，映射邊界前一行或列的邊
	  copyMakeBorder參數分別是input,output,top,bottom,left,right,補邊的方式，其中top,bottom,left,right的單位是幾行或幾列*/
	if (temp_bayerImg.cols % 2 != 0)
		copyMakeBorder(temp_bayerImg, temp_bayerImg, 0, 0, 0, 1, BORDER_REFLECT_101);
	if (temp_bayerImg.rows % 2 != 0)
		copyMakeBorder(temp_bayerImg, temp_bayerImg, 0, 1, 0, 0, BORDER_REFLECT_101);
	YDgCoCgImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	for (int i = 0; i < YDgCoCgImg.rows; i += 2) {
		for (int j = 0; j < YDgCoCgImg.cols; j += 2) {
			if (i + 1 == YDgCoCgImg.rows && YDgCoCgImg.rows % 2 != 0) {
				YDgCoCgImg.at<uchar>(i, j) = 1 * int(temp_bayerImg.at<uchar>(i, j)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Co component
				YDgCoCgImg.at<uchar>(i + 1, j) = (-1) * int(temp_bayerImg.at<uchar>(i, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j)); //Dg component
				continue;
			}
			if (j + 1 == YDgCoCgImg.cols && YDgCoCgImg.cols % 2 != 0) {
				YDgCoCgImg.at<uchar>(i, j) = 1 * int(temp_bayerImg.at<uchar>(i, j)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Co component
				YDgCoCgImg.at<uchar>(i, j + 1) = (-0.5) * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + (-0.5) * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Cg component	
				continue;
			}
			YDgCoCgImg.at<uchar>(i, j) = 1 * int(temp_bayerImg.at<uchar>(i, j)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Co component
			YDgCoCgImg.at<uchar>(i, j + 1) = (-0.5) * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + (-0.5) * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Cg component
			YDgCoCgImg.at<uchar>(i + 1, j + 1) =0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Y component
			YDgCoCgImg.at<uchar>(i + 1, j) = (-1) * int(temp_bayerImg.at<uchar>(i, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j)); //Dg component	
			//cout << (1 * int(temp_bayerImg.at<uchar>(i, j)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) << "***********" << int(YDgCoCgImg.at<uchar>(i, j)) << endl;
		}
	}
}

//void Bayer_to_YDgCbCr(Mat& bayerImg, Mat& YDgCbCrImg)
//{
//	Mat temp_bayerImg;
//	temp_bayerImg = bayerImg;
//	/*補邊，空出邊界，映射邊界前一行或列的邊
//	  copyMakeBorder參數分別是input,output,top,bottom,left,right,補邊的方式，其中top,bottom,left,right的單位是幾行或幾列*/
//	if (temp_bayerImg.cols % 2 != 0) 
//		copyMakeBorder(temp_bayerImg, temp_bayerImg, 0, 0, 0, 1, BORDER_REFLECT_101);
//	if (temp_bayerImg.rows % 2 != 0) 
//		copyMakeBorder(temp_bayerImg, temp_bayerImg, 0, 1, 0, 0, BORDER_REFLECT_101);
//	YDgCbCrImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
//	for (int i = 0; i < YDgCbCrImg.rows; i += 2) {
//		for (int j = 0; j < YDgCbCrImg.cols; j += 2) {
//			if (i + 1 == YDgCbCrImg.rows && YDgCbCrImg.rows % 2 != 0) {
//				YDgCbCrImg.at<uchar>(i, j) = 0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Y component
//				YDgCbCrImg.at<uchar>(i, j + 1) = 0 * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i, j + 1)) + (-1) * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Dg component
//				continue;
//			}
//			if (j + 1 == YDgCbCrImg.cols && YDgCbCrImg.cols % 2 != 0) {
//				YDgCbCrImg.at<uchar>(i, j) = 0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Y component
//				YDgCbCrImg.at<uchar>(i + 1, j) = 1 * int(temp_bayerImg.at<uchar>(i, j)) + (-1 / 2) * int(temp_bayerImg.at<uchar>(i, j + 1)) + (-1 / 2) * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Cr component	
//				continue;
//			}
//			YDgCbCrImg.at<uchar>(i, j) = 0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Y component
//			YDgCbCrImg.at<uchar>(i, j + 1) = 0 * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i, j + 1)) + (-1) * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Dg component
//			YDgCbCrImg.at<uchar>(i + 1, j + 1) = 0 * int(temp_bayerImg.at<uchar>(i, j)) + (-1 / 2) * int(temp_bayerImg.at<uchar>(i, j + 1)) + (-1 / 2) * int(temp_bayerImg.at<uchar>(i + 1, j)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Cb component
//			YDgCbCrImg.at<uchar>(i + 1, j) = 1 * int(temp_bayerImg.at<uchar>(i, j)) + (-1 / 2) * int(temp_bayerImg.at<uchar>(i, j + 1)) + (-1 / 2) * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)); //Cr component	
//		}
//	}
//}

void BGR_to_RGBW(Mat& bgrImg, Mat& rgbwImg) {
	rgbwImg = Mat::zeros(bgrImg.rows, bgrImg.cols, CV_8UC1);
	for (int i = 0; i < bgrImg.rows; i += 4) {
		for (int j = 0; j < bgrImg.cols; j += 4) {
			/*4*4 macropixel中的第一列*/
			rgbwImg.at<uchar>(i, j) = (bgrImg.at<Vec3b>(i, j)[0] + bgrImg.at<Vec3b>(i, j)[1] + bgrImg.at<Vec3b>(i, j)[2]) / 3; //W
			rgbwImg.at<uchar>(i, j + 1) = bgrImg.at<Vec3b>(i, j + 1)[0]; //B
			rgbwImg.at<uchar>(i, j + 2) = (bgrImg.at<Vec3b>(i, j + 2)[0] + bgrImg.at<Vec3b>(i, j + 2)[1] + bgrImg.at<Vec3b>(i, j + 2)[2]) / 3; //W
			rgbwImg.at<uchar>(i, j + 3) = bgrImg.at<Vec3b>(i, j + 3)[1]; //G
			/*4*4 macropixel中的第二列*/
			rgbwImg.at<uchar>(i + 1, j) = bgrImg.at<Vec3b>(i + 1, j)[0]; //B
			rgbwImg.at<uchar>(i + 1, j + 1) = (bgrImg.at<Vec3b>(i + 1, j + 1)[0] + bgrImg.at<Vec3b>(i + 1, j + 1)[1] + bgrImg.at<Vec3b>(i + 1, j + 1)[2]) / 3; //W
			rgbwImg.at<uchar>(i + 1, j + 2) = bgrImg.at<Vec3b>(i + 1, j + 2)[1]; //G
			rgbwImg.at<uchar>(i + 1, j + 3) = (bgrImg.at<Vec3b>(i + 1, j + 3)[0] + bgrImg.at<Vec3b>(i + 1, j + 3)[1] + bgrImg.at<Vec3b>(i + 1, j + 3)[2]) / 3; //W
			/*4*4 macropixel中的第三列*/
			rgbwImg.at<uchar>(i + 2, j) = (bgrImg.at<Vec3b>(i + 2, j)[0] + bgrImg.at<Vec3b>(i + 2, j)[1] + bgrImg.at<Vec3b>(i + 2, j)[2]) / 3; //W
			rgbwImg.at<uchar>(i + 2, j + 1) = bgrImg.at<Vec3b>(i + 2, j + 1)[1]; //G
			rgbwImg.at<uchar>(i + 2, j + 2) = (bgrImg.at<Vec3b>(i + 2, j + 2)[0] + bgrImg.at<Vec3b>(i + 2, j + 2)[1] + bgrImg.at<Vec3b>(i + 2, j + 2)[2]) / 3; //W
			rgbwImg.at<uchar>(i + 2, j + 3) = bgrImg.at<Vec3b>(i + 2, j + 3)[2]; //R
			/*4*4 macropixel中的第四列*/
			rgbwImg.at<uchar>(i + 3, j) = bgrImg.at<Vec3b>(i + 3, j)[1]; //G
			rgbwImg.at<uchar>(i + 3, j + 1) = (bgrImg.at<Vec3b>(i + 3, j + 1)[0] + bgrImg.at<Vec3b>(i + 3, j + 1)[1] + bgrImg.at<Vec3b>(i + 3, j + 1)[2]) / 3; //W
			rgbwImg.at<uchar>(i + 3, j + 2) = bgrImg.at<Vec3b>(i + 3, j + 2)[2]; //R
			rgbwImg.at<uchar>(i + 3, j + 3) = (bgrImg.at<Vec3b>(i + 3, j + 3)[0] + bgrImg.at<Vec3b>(i + 3, j + 3)[1] + bgrImg.at<Vec3b>(i + 3, j + 3)[2]) / 3; //W
		}
	}
}

void RGBW_to_RGBWBandwith(Mat& rgbwImg, Mat& rgbwImg_bandwith) {
	rgbwImg_bandwith = Mat::zeros(rgbwImg.rows, rgbwImg.cols, CV_8UC1);
	for (int i = 0; i < rgbwImg.rows; i += 4) {
		for (int j = 0; j < rgbwImg.cols; j += 2) {
			/*4*2 macropixel中的(0,0)*/
			rgbwImg_bandwith.at<uchar>(i, j) = (int(rgbwImg.at<uchar>(i, j)) + int(rgbwImg.at<uchar>(i + 1, j + 1))) / 2;
			/*4*2 macropixel中的右上pixel(0,1)*/
			rgbwImg_bandwith.at<uchar>(i, j + 1) = (int(rgbwImg.at<uchar>(i, j + 1)) + int(rgbwImg.at<uchar>(i + 1, j))) / 2;
			/*4*2 macropixel中的左下pixel(1,0)*/
			rgbwImg_bandwith.at<uchar>(i + 1, j) = Clip((int(rgbwImg.at<uchar>(i, j + 1)) - int(rgbwImg.at<uchar>(i + 1, j))),0,255) / 2;
			/*4*2 macropixel中的右下pixel(1,1)*/
			rgbwImg_bandwith.at<uchar>(i + 1, j + 1) = Clip((int(rgbwImg.at<uchar>(i, j)) - int(rgbwImg.at<uchar>(i + 1, j + 1))),0,255) / 2;
			/*4*2 macropixel中的右下pixel(2,0)*/
			rgbwImg_bandwith.at<uchar>(i + 2, j) = Clip((int(rgbwImg.at<uchar>(i + 3, j + 1)) - int(rgbwImg.at<uchar>(i + 2, j))),0,255) / 2;
			/*4*2 macropixel中的右下pixel(2,1)*/
			rgbwImg_bandwith.at<uchar>(i + 2, j + 1) = Clip((int(rgbwImg.at<uchar>(i + 3, j)) - int(rgbwImg.at<uchar>(i + 2, j + 1))),0,255) / 2;
			/*4*2 macropixel中的右下pixel(3,0)*/
			rgbwImg_bandwith.at<uchar>(i + 3, j) = (int(rgbwImg.at<uchar>(i + 3, j)) + int(rgbwImg.at<uchar>(i + 2, j + 1))) / 2;
			/*4*2 macropixel中的右下pixel(3,1)*/
			rgbwImg_bandwith.at<uchar>(i + 3, j + 1) = (int(rgbwImg.at<uchar>(i + 3, j + 1)) + int(rgbwImg.at<uchar>(i + 2, j))) / 2;
		}
	}
}

void RGBWBandwith_to_RGBW(Mat& rgbwImg_bandwith, Mat& rgbwImg) {
	rgbwImg = Mat::zeros(rgbwImg_bandwith.rows, rgbwImg_bandwith.cols, CV_8UC1);
	for (int i = 0; i < rgbwImg_bandwith.rows; i += 4) {
		for (int j = 0; j < rgbwImg_bandwith.cols; j += 2) {
			/*4*2 macropixel中的(0,0)*/
			rgbwImg.at<uchar>(i, j) = Clip(int(rgbwImg_bandwith.at<uchar>(i, j)) + int(rgbwImg_bandwith.at<uchar>(i + 1, j + 1)),0,255);
			/*4*2 macropixel中的(0,1)*/
			rgbwImg.at<uchar>(i, j + 1) = Clip(int(rgbwImg_bandwith.at<uchar>(i, j + 1)) + int(rgbwImg_bandwith.at<uchar>(i + 1, j)),0,255);
			/*4*2 macropixel中的(1,0)*/
			rgbwImg.at<uchar>(i + 1, j) = Clip(abs(int(rgbwImg_bandwith.at<uchar>(i, j + 1)) - int(rgbwImg_bandwith.at<uchar>(i + 1, j))),0,255);
			/*4*2 macropixel中的(1,1)*/
			rgbwImg.at<uchar>(i + 1, j + 1) = Clip(abs(int(rgbwImg_bandwith.at<uchar>(i, j)) - int(rgbwImg_bandwith.at<uchar>(i + 1, j + 1))),0,255);
			/*4*2 macropixel中的(2,0)*/
			rgbwImg.at<uchar>(i + 2, j) = Clip(abs(int(rgbwImg_bandwith.at<uchar>(i + 3, j + 1)) - int(rgbwImg_bandwith.at<uchar>(i + 2, j))),0,255);
			/*4*2 macropixel中的(2,1)*/
			rgbwImg.at<uchar>(i + 2, j + 1) = Clip(abs(int(rgbwImg_bandwith.at<uchar>(i + 3, j)) - int(rgbwImg_bandwith.at<uchar>(i + 2, j + 1))),0,255);
			/*4*2 macropixel中的(3,0)*/
			rgbwImg.at<uchar>(i + 3, j) = Clip((int(rgbwImg_bandwith.at<uchar>(i + 3, j)) + int(rgbwImg_bandwith.at<uchar>(i + 2, j + 1))),0,255);
			/*4*2 macropixel中的(3,1)*/
			rgbwImg.at<uchar>(i + 3, j + 1) = Clip((int(rgbwImg_bandwith.at<uchar>(i + 3, j + 1)) + int(rgbwImg_bandwith.at<uchar>(i + 2, j))),0,255);
		}
	}
}

//Mat YDgCbCr_to_Bayer(Mat& YDgCbCrImg) {
//	Mat temp_YDgCbCrImg = YDgCbCrImg;
//	Mat bayerImg = Mat::zeros(YDgCbCrImg.rows, YDgCbCrImg.cols, CV_8UC1);
//	if (temp_YDgCbCrImg.cols % 2 != 0)
//		copyMakeBorder(temp_YDgCbCrImg, temp_YDgCbCrImg, 0, 0, 0, 1, BORDER_REFLECT_101);
//	if (temp_YDgCbCrImg.rows % 2 != 0)
//		copyMakeBorder(temp_YDgCbCrImg, temp_YDgCbCrImg, 0, 1, 0, 0, BORDER_REFLECT_101);
//	for (int i = 0; i < bayerImg.rows; i += 2) {
//		for (int j = 0; j < bayerImg.cols; j += 2) {
//			if (i + 1 == bayerImg.rows && bayerImg.rows % 2 != 0) {
//				bayerImg.at<uchar>(i, j) = 1 * int(temp_YDgCbCrImg.at<uchar>(i, j)) + 0 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) + 0.75 * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) + -0.25 * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)); //R component
//				bayerImg.at<uchar>(i, j + 1) = 1 * int(temp_YDgCbCrImg.at<uchar>(i, j)) + 0.5 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)); //G1 component
//				continue;
//			}
//			if (j + 1 == bayerImg.cols && bayerImg.cols % 2 != 0) {
//				bayerImg.at<uchar>(i, j) = 1 * int(temp_YDgCbCrImg.at<uchar>(i, j)) + 0 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) + 0.75 * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) + -0.25 * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)); //R component
//				bayerImg.at<uchar>(i + 1, j) = 1 * int(temp_YDgCbCrImg.at<uchar>(i, j)) + (-0.5) * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)); //G2 component	
//				continue;
//			}
//			bayerImg.at<uchar>(i, j) = 1 * int(temp_YDgCbCrImg.at<uchar>(i, j)) + 0 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) + 0.75 * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) + -0.25 * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)); //R component
//			bayerImg.at<uchar>(i, j + 1) = 1 * int(temp_YDgCbCrImg.at<uchar>(i, j)) + 0.5 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)); //G1 component
//			bayerImg.at<uchar>(i + 1, j) = 1 * int(temp_YDgCbCrImg.at<uchar>(i, j)) + (-0.5) * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)); //G2 component
//			bayerImg.at<uchar>(i + 1, j + 1) = 1 * int(temp_YDgCbCrImg.at<uchar>(i, j)) + 0 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) + 0.75 * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)); //B component	
//		}
//	}
//	return bayerImg;
//}

void BGR_to_YUV(int height, int width, Mat& YUV, const Mat& BGR)
{
	YUV = Mat::zeros(height, width, CV_64FC3);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			YUV.at<Vec3d>(y, x)[0] = 0.257 * BGR.at<Vec3b>(y, x)[2] + 0.504 * BGR.at<Vec3b>(y, x)[1] + 0.098 * BGR.at<Vec3b>(y, x)[0] + 16;
			YUV.at<Vec3d>(y, x)[1] = -0.148 * BGR.at<Vec3b>(y, x)[2] - 0.291* BGR.at<Vec3b>(y, x)[1] + 0.439 * BGR.at<Vec3b>(y, x)[0] + 128;
			YUV.at<Vec3d>(y, x)[2] = 0.439 * BGR.at<Vec3b>(y, x)[2] - 0.368 * BGR.at<Vec3b>(y, x)[1] - 0.071 * BGR.at<Vec3b>(y, x)[0] + 128;

			/*YUV.at<Vec3d>(y, x)[0] = Y;
			YUV.at<Vec3d>(y, x)[1] = U;
			YUV.at<Vec3d>(y, x)[2] = V;*/
		}
	}
}

void DownSample_420A(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane)
{
	int wid = width / 2;
	int hei = height / 2;
	DS_U_plane = Mat::zeros(hei, wid, CV_64FC1);
	DS_V_plane = Mat::zeros(hei, wid, CV_64FC1);
	int x;
	int y = 0;
	for (int yy = 0; yy < hei; yy++)
	{
		x = 0;
		for (int xx = 0; xx < wid; xx++)
		{

			DS_U_plane.at<double>(yy, xx) += U_plane.at<double>(y, x);
			DS_V_plane.at<double>(yy, xx) += V_plane.at<double>(y, x);
			x++;
			DS_U_plane.at<double>(yy, xx) += U_plane.at<double>(y, x);
			DS_V_plane.at<double>(yy, xx) += V_plane.at<double>(y, x);
			x++;
		}
		y++;
		x = 0;
		for (int xx = 0; xx < wid; xx++)
		{

			DS_U_plane.at<double>(yy, xx) += U_plane.at<double>(y, x);
			DS_V_plane.at<double>(yy, xx) += V_plane.at<double>(y, x);
			x++;
			DS_U_plane.at<double>(yy, xx) = (int)((DS_U_plane.at<double>(yy, xx) + U_plane.at<double>(y, x)) / 4 + 0.5);
			DS_V_plane.at<double>(yy, xx) = (int)((DS_V_plane.at<double>(yy, xx) + V_plane.at<double>(y, x)) / 4 + 0.5);
			x++;
		}
		y++;

	}

	//讀取floor
	/*int yy = 0;
	int xx = 0;
	for (int y = 0; y < height; y += 2)
	{
		xx = 0;
		for (int x = 0; x < width; x += 2)
		{
			DS_U_plane.at<double>(yy, xx) = Clip((U_plane.at<double>(y, x) + U_plane.at<double>(y, x + 1) + U_plane.at<double>(y + 1, x) + U_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);
			DS_V_plane.at<double>(yy, xx) = Clip((V_plane.at<double>(y, x) + V_plane.at<double>(y, x + 1) + V_plane.at<double>(y + 1, x) + V_plane.at<double>(y + 1, x + 1)) / 4, 0, 255);
			DS_U_plane.at<double>(yy, xx) = (int)DS_U_plane.at<double>(yy, xx);
			DS_V_plane.at<double>(yy, xx) = (int)DS_V_plane.at<double>(yy, xx);
			xx++;
		}
		yy++;
	}*/
}

void RGBW_to_RGBWHalfHhalf(Mat& rgbwImg, Mat& rgbwImgHalfHalf) {
	rgbwImgHalfHalf = Mat::zeros(rgbwImg.rows,rgbwImg.cols, CV_8UC1);
	for (int i = 0; i < rgbwImgHalfHalf.rows; i+=2) {
		for (int j = 0; j < rgbwImgHalfHalf.cols; j+=2) {
			rgbwImgHalfHalf.at<uchar>(i/2, j) = (rgbwImg.at<uchar>(i, j) + rgbwImg.at<uchar>(i + 1, j + 1)) / 2;
			rgbwImgHalfHalf.at<uchar>(i/2, j + 1) = (rgbwImg.at<uchar>(i, j + 1) + rgbwImg.at<uchar>(i + 1, j)) / 2;
			rgbwImgHalfHalf.at<uchar>((rgbwImgHalfHalf.rows + i) / 2, j) = Clip((rgbwImg.at<uchar>(i, j) - rgbwImg.at<uchar>(i + 1, j + 1)),0,255) / 2;
			rgbwImgHalfHalf.at<uchar>((rgbwImgHalfHalf.rows + i) / 2, j + 1) = Clip((rgbwImg.at<uchar>(i, j + 1) - rgbwImg.at<uchar>(i + 1, j)),0,255) / 2;
		}
	}
}

void RGBWHalfHhalf_to_RGBW(Mat& rgbwImgHalfHalf, Mat& rgbwImg_re) {
	rgbwImg_re = Mat::zeros(rgbwImgHalfHalf.rows, rgbwImgHalfHalf.cols, CV_8UC1);
	for (int i = 0; i < rgbwImgHalfHalf.rows; i += 2) {
		for (int j = 0; j < rgbwImgHalfHalf.cols; j += 2) {
			rgbwImg_re.at<uchar>(i, j) = rgbwImgHalfHalf.at<uchar>(i, j) + rgbwImgHalfHalf.at<uchar>((rgbwImgHalfHalf.rows + i) / 2, j);
			rgbwImg_re.at<uchar>(i, j + 1) = rgbwImgHalfHalf.at<uchar>(i, j + 1) + rgbwImgHalfHalf.at<uchar>((rgbwImgHalfHalf.rows + i) / 2, j + 1);
			rgbwImg_re.at<uchar>(i + 1, j) = Clip(rgbwImgHalfHalf.at<uchar>(i, j + 1) - rgbwImgHalfHalf.at<uchar>((rgbwImgHalfHalf.rows + i) / 2, j + 1),0,255);
			rgbwImg_re.at<uchar>(i + 1, j + 1) = Clip(rgbwImgHalfHalf.at<uchar>(i, j) - rgbwImgHalfHalf.at<uchar>((rgbwImgHalfHalf.rows + i) / 2, j),0,255);
		}
	}
}



Mat YDgCbCr_to_Bayer(Mat& YDgCbCrImg, Mat& signImg) {

	Mat temp_YDgCbCrImg = YDgCbCrImg;
	Mat bayerImg = Mat::zeros(YDgCbCrImg.rows, YDgCbCrImg.cols, CV_8UC1);
	Mat sign = Mat::zeros(signImg.rows, signImg.cols, CV_64FC1);
	signImg.convertTo(sign, CV_64FC1);


	for (int i = 0; i < signImg.rows; i++)
		for (int j = 0; j < signImg.cols; j++)
		{
			if (sign.at<double>(i, j) == 0)
				sign.at<double>(i, j) = -1;
			else
				sign.at<double>(i, j) = 1;
		}

	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {

			bayerImg.at<uchar>(i, j) = Clip(0.75 * int(temp_YDgCbCrImg.at<uchar>(i, j)) * sign.at<double>(i, j) + 1 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) * sign.at<double>(i, j + 1) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)) * sign.at<double>(i + 1, j + 1) + 0.5, 0, 255); //R component

			bayerImg.at<uchar>(i, j + 1) = Clip((-0.25) * int(temp_YDgCbCrImg.at<uchar>(i, j)) * sign.at<double>(i, j) + (-0.5) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) * sign.at<double>(i + 1, j) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)) * sign.at<double>(i + 1, j + 1) + 1 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) * sign.at<double>(i, j + 1) + 0.5, 0, 255); //G1 component

			bayerImg.at<uchar>(i + 1, j) = Clip(1 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) * sign.at<double>(i, j + 1) + 0.5 * int(temp_YDgCbCrImg.at<uchar>(i + 1, j)) * sign.at<double>(i + 1, j) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)) * sign.at<double>(i + 1, j + 1) + (-0.25) * int(temp_YDgCbCrImg.at<uchar>(i, j)) * sign.at<double>(i, j) + 0.5, 0, 255); //G2 component

			bayerImg.at<uchar>(i + 1, j + 1) = Clip((-0.25) * int(temp_YDgCbCrImg.at<uchar>(i, j)) * sign.at<double>(i, j) + 1 * int(temp_YDgCbCrImg.at<uchar>(i, j + 1)) * sign.at<double>(i, j + 1) + (0.75) * int(temp_YDgCbCrImg.at<uchar>(i + 1, j + 1)) * sign.at<double>(i + 1, j + 1) + 0.5, 0, 255); //B component	
		}
	}
	return bayerImg;
}
void Bayer_to_YYCbCr(Mat& bayerImg, Mat& YYCbCrImg)	//DY
{
	Mat temp_bayerImg;
	bayerImg.convertTo(temp_bayerImg, CV_64FC1);
	YYCbCrImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_64FC1);
	for (int i = 0; i < YYCbCrImg.rows; i += 2) {
		for (int j = 0; j < YYCbCrImg.cols; j += 2) { //bayer: rggb => ggbr	//now GRBG = > GGBR
			//YYCbCrImg.at<double>(i, j + 1) = int(0.504 * temp_bayerImg.at<double>(i, j + 1) + 0 * temp_bayerImg.at<double>(i + 1, j) + 0.098 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.257 * temp_bayerImg.at<double>(i, j)); //Y^{ur} component
			//////////////////////////////////////////////////////

			//YYCbCrImg.at<double>(i + 1, j) = int(0 * temp_bayerImg.at<double>(i, j + 1) + 0.504 * temp_bayerImg.at<double>(i + 1, j) + 0.098 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.257 * temp_bayerImg.at<double>(i, j)); //Y^{ll} component
			///////////////////////////////////////////////////////

			//YYCbCrImg.at<double>(i + 1, j) = int(-0.145 * temp_bayerImg.at<double>(i, j + 1) - 0.145 * temp_bayerImg.at<double>(i + 1, j) + 0.439 * temp_bayerImg.at<double>(i + 1, j + 1) - 0.148 * temp_bayerImg.at<double>(i, j) + 128); //Cb component
			///////////////////////////////////////////////////////

			//YYCbCrImg.at<double>(i, j + 1) = int(-0.184 * temp_bayerImg.at<double>(i, j) - 0.184 * temp_bayerImg.at<double>(i + 1, j + 1) - 0.071 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.439 * temp_bayerImg.at<double>(i, j) + 128); //Cr component
			
			//Y1 Y2 Cb Cr	Y=0.257R 0.504G 0.098B
			//YYCbCrImg.at<double>(i, j) = int(0.504 * temp_bayerImg.at<double>(i, j) + 0 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.098 * temp_bayerImg.at<double>(i + 1, j) + 0.257 * temp_bayerImg.at<double>(i, j + 1) + 16); //Y^{ur} component
			//YYCbCrImg.at<double>(i + 1, j + 1) = int(0 * temp_bayerImg.at<double>(i, j) + 0.504 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.098 * temp_bayerImg.at<double>(i + 1, j) + 0.257 * temp_bayerImg.at<double>(i, j + 1) + 16); //Y^{ll} component
			//YYCbCrImg.at<double>(i + 1, j) = int(-0.145 * temp_bayerImg.at<double>(i, j) - 0.145 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.439 * temp_bayerImg.at<double>(i + 1, j) - 0.148 * temp_bayerImg.at<double>(i, j + 1) + 128); //Cb component
			//YYCbCrImg.at<double>(i, j + 1) = int(-0.184 * temp_bayerImg.at<double>(i, j) - 0.184 * temp_bayerImg.at<double>(i + 1, j + 1) - 0.071 * temp_bayerImg.at<double>(i + 1, j) + 0.439 * temp_bayerImg.at<double>(i, j + 1) + 128); //Cr component

			//Y1 Y2 Cb Cr	Y=0.299R 0.587G 0.114B
			YYCbCrImg.at<double>(i, j) = int(0.587 * temp_bayerImg.at<double>(i, j) + 0 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.114 * temp_bayerImg.at<double>(i + 1, j) + 0.299 * temp_bayerImg.at<double>(i, j + 1)); //Y^{ur} component
			YYCbCrImg.at<double>(i + 1, j + 1) = int(0 * temp_bayerImg.at<double>(i, j) + 0.587 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.114 * temp_bayerImg.at<double>(i + 1, j) + 0.299 * temp_bayerImg.at<double>(i, j + 1)); //Y^{ll} component
			YYCbCrImg.at<double>(i + 1, j) = int(-0.1695 * temp_bayerImg.at<double>(i, j) - 0.1695 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.511 * temp_bayerImg.at<double>(i + 1, j) - 0.172 * temp_bayerImg.at<double>(i, j + 1) + 128); //Cb component
			YYCbCrImg.at<double>(i, j + 1) = int(-0.214 * temp_bayerImg.at<double>(i, j) - 0.214 * temp_bayerImg.at<double>(i + 1, j + 1) - 0.083 * temp_bayerImg.at<double>(i + 1, j) + 0.511 * temp_bayerImg.at<double>(i, j + 1) + 128); //Cr component

		}
	}

	YYCbCrImg.convertTo(YYCbCrImg, CV_8UC1);
}

Mat YYCbCr_to_Bayer(Mat& YYCbCrImg)
{

	Mat temp_YYCbCr;
	YYCbCrImg.convertTo(temp_YYCbCr, CV_64FC1);
	Mat bayerImg = Mat::zeros(YYCbCrImg.rows, YYCbCrImg.cols, CV_64FC1);
	for (int i = 0; i < YYCbCrImg.rows; i += 2) {
		for (int j = 0; j < YYCbCrImg.cols; j += 2) { //YYCbCrImg: CbY_{ur}Y_{ll}Cr => Y_{ur}Y-{ll}CbCr
			//bayerImg.at<double>(i, j + 1) = 1.574 * temp_YYCbCr.at<double>(i, j + 1) - 0.410 * temp_YYCbCr.at<double>(i + 1, j) - 0.392 * (temp_YYCbCr.at<double>(i, j) - 128) - 0.814 * (temp_YYCbCr.at<double>(i + 1, j + 1) - 128); //G^{ur} component
			/////////////////////////////////////////////////////////
			//bayerImg.at<double>(i + 1, j) = -0.410 * temp_YYCbCr.at<double>(i, j + 1) + 1.574 * temp_YYCbCr.at<double>(i + 1, j) - 0.392 * (temp_YYCbCr.at<double>(i, j) - 128) - 0.814 * (temp_YYCbCr.at<double>(i + 1, j + 1) - 128); //G^{ll} component
			/////////////////////////////////////////////////////////
			//bayerImg.at<double>(i + 1, j + 1) = 0.581 * temp_YYCbCr.at<double>(i, j + 1) + 0.581 * temp_YYCbCr.at<double>(i + 1, j) + 2.019 * (temp_YYCbCr.at<double>(i, j) - 128) + 0 * (temp_YYCbCr.at<double>(i + 1, j + 1) - 128); //B component
			/////////////////////////////////////////////////////////
			//bayerImg.at<double>(i, j) = 0.582 * temp_YYCbCr.at<double>(i, j + 1) + 0.582 * temp_YYCbCr.at<double>(i + 1, j) - 0.002 * (temp_YYCbCr.at<double>(i, j) - 128) + 1.596 * (temp_YYCbCr.at<double>(i + 1, j + 1) - 128); //R component
			
			//Y1 Y2 Cb Cr	Y=0.257R 0.504G 0.098B
			//bayerImg.at<double>(i, j) = 1.574 * (temp_YYCbCr.at<double>(i, j) - 16) - 0.410 * (temp_YYCbCr.at<double>(i + 1, j + 1) - 16) - 0.392 * (temp_YYCbCr.at<double>(i + 1, j) - 128) - 0.814 * (temp_YYCbCr.at<double>(i, j + 1) - 128); //G^{ur} component
			//bayerImg.at<double>(i + 1, j + 1) = -0.410 * (temp_YYCbCr.at<double>(i, j) - 16) + 1.574 * (temp_YYCbCr.at<double>(i + 1, j + 1) - 16) - 0.392 * (temp_YYCbCr.at<double>(i + 1, j) - 128) - 0.814 * (temp_YYCbCr.at<double>(i, j + 1) - 128); //G^{ll} component
			//bayerImg.at<double>(i + 1, j) = 0.581 * (temp_YYCbCr.at<double>(i, j) - 16) + 0.581 * (temp_YYCbCr.at<double>(i + 1, j + 1) - 16) + 2.019 * (temp_YYCbCr.at<double>(i + 1, j) - 128) + 0 * (temp_YYCbCr.at<double>(i, j + 1) - 128); //B component
			//bayerImg.at<double>(i, j + 1) = 0.582 * (temp_YYCbCr.at<double>(i, j) - 16) + 0.582 * (temp_YYCbCr.at<double>(i + 1, j + 1) - 16) - 0.002 * (temp_YYCbCr.at<double>(i + 1, j) - 128) + 1.596 * (temp_YYCbCr.at<double>(i, j + 1) - 128); //R component
			
			//Y1 Y2 Cb Cr	Y=0.299R 0.587G 0.114B
			bayerImg.at<double>(i, j) = 1.351 * temp_YYCbCr.at<double>(i, j) - 0.351 * temp_YYCbCr.at<double>(i + 1, j + 1) - 0.336 * (temp_YYCbCr.at<double>(i + 1, j) - 128) - 0.698 * (temp_YYCbCr.at<double>(i, j + 1) - 128); //G^{ur} component
			bayerImg.at<double>(i + 1, j + 1) = -0.351 * temp_YYCbCr.at<double>(i, j) + 1.351 * temp_YYCbCr.at<double>(i + 1, j + 1) - 0.336 * (temp_YYCbCr.at<double>(i + 1, j) - 128) - 0.698 * (temp_YYCbCr.at<double>(i, j + 1) - 128); //G^{ll} component
			bayerImg.at<double>(i + 1, j) = 0.5 * temp_YYCbCr.at<double>(i, j) + 0.5 * temp_YYCbCr.at<double>(i + 1, j + 1) + 1.733 * (temp_YYCbCr.at<double>(i + 1, j) - 128) + 0.0016 * (temp_YYCbCr.at<double>(i, j + 1) - 128); //B component
			bayerImg.at<double>(i, j + 1) = 0.5 * temp_YYCbCr.at<double>(i, j) + 0.5 * temp_YYCbCr.at<double>(i + 1, j + 1) - 0.000287 * (temp_YYCbCr.at<double>(i + 1, j) - 128) + 1.371 * (temp_YYCbCr.at<double>(i, j + 1) - 128); //R component

		}
	}
	bayerImg.convertTo(bayerImg, CV_8UC1);
	return bayerImg;
}


Mat Bayer_to_YDgCoCg4(Mat& bayerImg, Mat& YDgCoCgImg)
{
	Mat temp_bayerImg;
	Mat YImg;
	Mat YsignImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);;
	Mat DgImg;
	Mat DgsignImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);;
	Mat CoImg;
	Mat CosignImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);;
	Mat CgImg;
	Mat CgsignImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);;
	temp_bayerImg = bayerImg;
	YImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	DgImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	CoImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	CgImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	YDgCoCgImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	//signImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	for (int i = 0; i < bayerImg.rows; i += 2) {	//GRBG
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//Co	R-B
			if ((1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				CosignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CosignImg.at<uchar>(i / 2, j / 2) = 255;
			CoImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j))); //Co component
			//Cg	(G1+G2-R-B)*0.5
			if ((0.5 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) < 0))
				CgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CgsignImg.at<uchar>(i / 2, j / 2) = 255;
			CgImg.at<uchar>(i / 2, j / 2) = abs(0.5 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))); //Cg component
			//Y		(G1+G2+R+B)*0.25
			YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs(0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) + 0.5; //Y component
			//Dg	G2-G1
			if ((1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j))) < 0)
				DgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DgsignImg.at<uchar>(i / 2, j / 2) = 255;
			DgImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j))); //Dg component
		}
	}
	Mat htemp;
	Mat htemp2;
	Mat hsigntemp;
	Mat hsigntemp2;
	Mat signImg;
	hconcat(YImg, DgImg, htemp);
	hconcat(CoImg, CgImg, htemp2);
	vconcat(htemp, htemp2, YDgCoCgImg);
	hconcat(YsignImg, DgsignImg, hsigntemp);
	hconcat(CosignImg, CgsignImg, hsigntemp2);
	vconcat(hsigntemp, hsigntemp2, signImg);
	return signImg;
}

Mat Y4_to_Bayer(Mat& YDgCoCgImg, Mat& signImg) {//Y4 = YDgCoCg
	Mat temp_YDgCoCgImg = YDgCoCgImg;
	Mat bayerImg = Mat::zeros(YDgCoCgImg.rows, YDgCoCgImg.cols, CV_8UC1);

	//Mat YImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat DgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CoImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);

	Mat YImg = YDgCoCgImg(Range(0, YDgCoCgImg.rows * 0.5), Range(0, YDgCoCgImg.cols * 0.5)).clone();//Top Left
	Mat DgImg = YDgCoCgImg(Range(0, YDgCoCgImg.rows * 0.5), Range(YDgCoCgImg.cols * 0.5, YDgCoCgImg.cols)).clone();//Top Right
	Mat CoImg = YDgCoCgImg(Range(YDgCoCgImg.rows * 0.5, YDgCoCgImg.rows), Range(0, YDgCoCgImg.cols * 0.5)).clone();//Bottom Left
	Mat CgImg = YDgCoCgImg(Range(YDgCoCgImg.rows * 0.5, YDgCoCgImg.rows), Range(YDgCoCgImg.cols * 0.5, YDgCoCgImg.cols)).clone();//Bottom Right

	Mat YsignImg = signImg(Range(0, signImg.rows * 0.5), Range(0, signImg.cols * 0.5)).clone();
	YsignImg.convertTo(YsignImg, CV_64FC1);

	Mat DgsignImg = signImg(Range(0, signImg.rows * 0.5), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DgsignImg.convertTo(DgsignImg, CV_64FC1);

	Mat CosignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(0, signImg.cols * 0.5)).clone();
	CosignImg.convertTo(CosignImg, CV_64FC1);

	Mat CgsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(signImg.cols * 0.5, signImg.cols)).clone();
	CgsignImg.convertTo(CgsignImg, CV_64FC1);


	for (int i = 0; i < signImg.rows * 0.5; i++)
		for (int j = 0; j < signImg.cols * 0.5; j++)
		{
			if (YsignImg.at<double>(i, j) == 0)
				YsignImg.at<double>(i, j) = -1;
			else
				YsignImg.at<double>(i, j) = 1;

			if (DgsignImg.at<double>(i, j) == 0)
				DgsignImg.at<double>(i, j) = -1;
			else
				DgsignImg.at<double>(i, j) = 1;

			if (CosignImg.at<double>(i, j) == 0)
				CosignImg.at<double>(i, j) = -1;
			else
				CosignImg.at<double>(i, j) = 1;

			if (CgsignImg.at<double>(i, j) == 0)
				CgsignImg.at<double>(i, j) = -1;
			else
				CgsignImg.at<double>(i, j) = 1;
		}


	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//G1
			bayerImg.at<uchar>(i, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (-0.5) * int(DgImg.at<uchar>(i/2, j/2)) * DgsignImg.at<double>(i/2, j/2) + 0.5 * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2) , 0, 255); 
			//R
			bayerImg.at<uchar>(i, j + 1) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(CoImg.at<uchar>(i / 2, j / 2)) * CosignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255); 			
			//B			
			bayerImg.at<uchar>(i + 1, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CoImg.at<uchar>(i / 2, j / 2)) * CosignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255); 			
			//G2			
			bayerImg.at<uchar>(i + 1, j + 1) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + 0.5 * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255); 
		}
	}
	return bayerImg;
}

Mat Bayer_to_YDgCbCr4(Mat& bayerImg, Mat& YDgCbCrImg)
{
	double g=0;
	Mat temp_bayerImg;
	Mat YImg;
	Mat YsignImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	Mat DgImg;
	Mat DgsignImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	Mat CbImg;
	Mat CbsignImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	Mat CrImg;
	Mat CrsignImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	temp_bayerImg = bayerImg;
	YImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	DgImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	CbImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	CrImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	YDgCbCrImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	for (int i = 0; i < YDgCbCrImg.rows; i += 2) {
		for (int j = 0; j < YDgCbCrImg.cols; j += 2) {
			//g'=floor((g1+g2)/2)
			//if ((1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i, j))) < 0)
			//	DgsignImg.at<uchar>(i / 2, j / 2) = 0;
			//else
			//	DgsignImg.at<uchar>(i / 2, j / 2) = 255;
			
			//g'=floor((g1+g2)/2)
			g = floor(0.5 * int(temp_bayerImg.at<uchar>(i , j)) + 0.5 * int(temp_bayerImg.at<uchar>(i+1,j+1))); //g' component

			//Cr component r-g'
			if ((1 * int(temp_bayerImg.at<uchar>(i, j+1)) - g) < 0)
				CrsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CrsignImg.at<uchar>(i / 2, j / 2) = 255;
			CrImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i, j+1)) - g);

			//Y component floor((r+2g'+b)/4)
			YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs(floor(0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.5 * g + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j))));
			
			//Cb component b-g'
			if ((1 * int(temp_bayerImg.at<uchar>(i + 1, j))-g) < 0)
				CbsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CbsignImg.at<uchar>(i / 2, j / 2) = 255;
			CbImg.at<uchar>(i / 2, j / 2) = abs( 1 * int(temp_bayerImg.at<uchar>(i + 1, j))-g);
			
			//Dg component
			if (1 * int(temp_bayerImg.at<uchar>(i, j ) -1 * int(temp_bayerImg.at<uchar>(i+1 , j+1))) < 0)
				DgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DgsignImg.at<uchar>(i / 2, j / 2) = 255;
			DgImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i, j)) -1 * int(temp_bayerImg.at<uchar>(i+1 , j+1)));
		}
	}
	Mat htemp;
	Mat htemp2;
	Mat hsigntemp;
	Mat hsigntemp2;
	Mat signImg;
	hconcat(YImg, DgImg, htemp);
	hconcat(CbImg, CrImg, htemp2);
	vconcat(htemp, htemp2, YDgCbCrImg);
	hconcat(YsignImg, DgsignImg, hsigntemp);
	hconcat(CbsignImg, CrsignImg, hsigntemp2);
	vconcat(hsigntemp, hsigntemp2, signImg);
	return signImg;
}

Mat YDgCbCr4_to_Bayer(Mat& YDgCbCrImg, Mat& signImg) {
	Mat bayerImg = Mat::zeros(YDgCbCrImg.rows, YDgCbCrImg.cols, CV_8UC1);

	//Mat YImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat DgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CoImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);

	Mat YImg = YDgCbCrImg(Range(0, YDgCbCrImg.rows * 0.5), Range(0, YDgCbCrImg.cols * 0.5)).clone();
	Mat DgImg = YDgCbCrImg(Range(0, YDgCbCrImg.rows * 0.5), Range(YDgCbCrImg.cols * 0.5, YDgCbCrImg.cols)).clone();
	Mat CbImg = YDgCbCrImg(Range(YDgCbCrImg.rows * 0.5, YDgCbCrImg.rows), Range(0, YDgCbCrImg.cols * 0.5)).clone();
	Mat CrImg = YDgCbCrImg(Range(YDgCbCrImg.rows * 0.5, YDgCbCrImg.rows), Range(YDgCbCrImg.cols * 0.5, YDgCbCrImg.cols)).clone();

	Mat YsignImg = signImg(Range(0, signImg.rows * 0.5), Range(0, signImg.cols * 0.5)).clone();
	YsignImg.convertTo(YsignImg, CV_64FC1);

	Mat DgsignImg = signImg(Range(0, signImg.rows * 0.5), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DgsignImg.convertTo(DgsignImg, CV_64FC1);

	Mat CbsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(0, signImg.cols * 0.5)).clone();
	CbsignImg.convertTo(CbsignImg, CV_64FC1);

	Mat CrsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(signImg.cols * 0.5, signImg.cols)).clone();
	CrsignImg.convertTo(CrsignImg, CV_64FC1);
	


	for (int i = 0; i < signImg.rows * 0.5; i++)
		for (int j = 0; j < signImg.cols * 0.5; j++)
		{
			if (YsignImg.at<double>(i, j) == 0)
				YsignImg.at<double>(i, j) = -1;
			else
				YsignImg.at<double>(i, j) = 1;

			if (DgsignImg.at<double>(i, j) == 0)
				DgsignImg.at<double>(i, j) = -1;
			else
				DgsignImg.at<double>(i, j) = 1;

			if (CbsignImg.at<double>(i, j) == 0)
				CbsignImg.at<double>(i, j) = -1;
			else
				CbsignImg.at<double>(i, j) = 1;

			if (CrsignImg.at<double>(i, j) == 0)
				CrsignImg.at<double>(i, j) = -1;
			else
				CrsignImg.at<double>(i, j) = 1;
		}

	double temp, temp1, temp2, g1, g2;
	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//g1+g4=2*[Y-(Cb+Cr)/4]
			temp = (2*int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) - 0.5*int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2) -0.5 * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2)) ;
			
			//g1=(Dg+g1+g4)/2
			g1 = (0.5 * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + 0.5 * temp);
			g2 = (temp - g1);
			temp2 = (0.5 * g1 + 0.5 * g2);//temp=g'
			//g1=(Dg+g1+g4)/2
			bayerImg.at<uchar>(i, j) = Clip(g1, 0, 255); 
			//r=Cr+g'
			bayerImg.at<uchar>(i, j + 1) = Clip((1 * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2) + temp2), 0, 255); //G1 component
			//bayerImg.at<uchar>(i, j + 1) = Clip((-0.25) * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2) + (-0.5) * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + (-0.25) * int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2) + 1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5, 0, 255); //G1 component
			//b=Cb+g'
			bayerImg.at<uchar>(i + 1, j) = Clip((1* int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2)+temp2), 0, 255); //G2 component
			//bayerImg.at<uchar>(i + 1, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + (-0.25) * int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2) + (-0.25) * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2) + 0.5, 0, 255); //G2 component

			//bayerImg.at<uchar>(i + 1, j + 1) = Clip((-0.25) * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2) + 1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (0.75) * int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2) + 0.5, 0, 255); //B component

			//g2=g1-Dg
			bayerImg.at<uchar>(i + 1, j + 1) = Clip(g2, 0, 255); //B component
		}
	}
	return bayerImg;
}

void Bayer_to_YYCbCr4(Mat& bayerImg, Mat& YYCbCrImg)
{
	Mat temp_bayerImg;
	Mat Y1Img;
	Mat Y2Img;
	Mat CbImg;
	Mat CrImg;
	temp_bayerImg = bayerImg;
	Y1Img = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	Y2Img = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	CbImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	CrImg = Mat::zeros((bayerImg.rows)*0.5, (bayerImg.cols)*0.5, CV_8UC1);
	YYCbCrImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	for (int i = 0; i < YYCbCrImg.rows; i += 2) {
		for (int j = 0; j < YYCbCrImg.cols; j += 2) { //G1 G2 B R	//bayer: rggb => ggbr
			//cout << double(temp_bayerImg.at<uchar>(0,1));
			//Y1Img.at<uchar>(i / 2, j / 2) = int(0.504 * double(temp_bayerImg.at<uchar>(i, j + 1)) + 0 * double(temp_bayerImg.at<uchar>(i + 1, j)) + 0.098 *double(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 0.257 * double(temp_bayerImg.at<uchar>(i, j))); //Y^{ur} component
			Y1Img.at<uchar>(i / 2, j / 2) = int(0.587 * double(temp_bayerImg.at<uchar>(i, j)) + 0 * double(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 0.114 *double(temp_bayerImg.at<uchar>(i + 1, j)) + 0.299 * double(temp_bayerImg.at<uchar>(i, j + 1))); //Y^{ur} component


			//Y2Img.at<uchar>(i / 2, j / 2) = int(0 * temp_bayerImg.at<double>(i, j + 1) + 0.504 * temp_bayerImg.at<double>(i + 1, j) + 0.098 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.257 * temp_bayerImg.at<double>(i, j)); //Y^{ll} component
			Y2Img.at<uchar>(i / 2, j / 2) = int(0 * double(temp_bayerImg.at<uchar>(i, j)) + 0.587 * double(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 0.114 *double(temp_bayerImg.at<uchar>(i + 1, j)) + 0.299 * double(temp_bayerImg.at<uchar>(i, j + 1))); //Y^{ll} component
			/////////////////////////////////////////////////////////

			//CbImg.at<uchar>(i / 2, j / 2) = int(-0.145 * temp_bayerImg.at<double>(i, j + 1) - 0.145 * temp_bayerImg.at<double>(i + 1, j) + 0.439 * temp_bayerImg.at<double>(i + 1, j + 1) - 0.148 * temp_bayerImg.at<double>(i, j) + 128); //Cb component
			CbImg.at<uchar>(i / 2, j / 2) = int(-0.1695 * double(temp_bayerImg.at<uchar>(i, j)) -0.1695 * double(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 0.511 *double(temp_bayerImg.at<uchar>(i + 1, j)) - 0.172 * double(temp_bayerImg.at<uchar>(i, j + 1)) + 128); //Cb component
			/////////////////////////////////////////////////////////

			//CrImg.at<uchar>(i / 2, j / 2) = int(-0.184 * temp_bayerImg.at<double>(i, j + 1) - 0.184 * temp_bayerImg.at<double>(i + 1, j) - 0.071 * temp_bayerImg.at<double>(i + 1, j + 1) + 0.439 * temp_bayerImg.at<double>(i, j) + 128); //Cr component
			CrImg.at<uchar>(i / 2, j / 2) = int(-0.214 * double(temp_bayerImg.at<uchar>(i, j)) -0.214 * double(temp_bayerImg.at<uchar>(i + 1, j + 1)) -0.083 *double(temp_bayerImg.at<uchar>(i + 1, j)) + 0.511 * double(temp_bayerImg.at<uchar>(i, j + 1)) + 128); //Cr component
		}
	}
	Mat htemp;
	Mat htemp2;
	hconcat(Y1Img, Y2Img, htemp);
	hconcat(CbImg, CrImg, htemp2);
	vconcat(htemp, htemp2, YYCbCrImg);
}

Mat YYCbCr4_to_Bayer(Mat& YYCbCrImg)
{
	Mat bayerImg = Mat::zeros(YYCbCrImg.rows, YYCbCrImg.cols, CV_8UC1);
	Mat Y1Img = YYCbCrImg(Range(0, YYCbCrImg.rows * 0.5), Range(0, YYCbCrImg.cols * 0.5)).clone();
	Mat Y2Img = YYCbCrImg(Range(0, YYCbCrImg.rows * 0.5), Range(YYCbCrImg.cols * 0.5, YYCbCrImg.cols)).clone();
	Mat CbImg = YYCbCrImg(Range(YYCbCrImg.rows * 0.5, YYCbCrImg.rows), Range(0, YYCbCrImg.cols * 0.5)).clone();
	Mat CrImg = YYCbCrImg(Range(YYCbCrImg.rows * 0.5, YYCbCrImg.rows), Range(YYCbCrImg.cols * 0.5, YYCbCrImg.cols)).clone();

	for (int i = 0; i < YYCbCrImg.rows; i += 2) {
		for (int j = 0; j < YYCbCrImg.cols; j += 2) {
			//G1
			//bayerImg.at<uchar>(i, j + 1) = Clip(1.574 * int(Y1Img.at<uchar>(i /2, j / 2)) - 0.410 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.392 * (int(CbImg.at<uchar>(i / 2, j / 2)) - 128) - 0.814 * (int(CrImg.at<uchar>(i / 2, j / 2)) - 128),0,255);
			bayerImg.at<uchar>(i, j) = Clip(1.351 * int(Y1Img.at<uchar>(i / 2, j / 2)) - 0.351 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.336 * (int(CbImg.at<uchar>(i / 2, j / 2)) - 128) - 0.698 * (int(CrImg.at<uchar>(i / 2, j / 2)) - 128), 0, 255);
			//bayerImg.at<uchar>(i, j) = Clip(1.351 * int(Y1Img.at<uchar>(i / 2, j / 2)) - 0.351 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.336 * int(CbImg.at<uchar>(i / 2, j / 2)) - 0.698 * int(CrImg.at<uchar>(i / 2, j / 2)) - 175.488, 0, 255);
			//G2
			//bayerImg.at<uchar>(i + 1, j + 1) = Clip(-0.410 * int(Y1Img.at<uchar>(i/ 2 , j / 2)) + 1.574 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.392 * (int(CbImg.at<uchar>(i / 2, j / 2)) - 128) - 0.814 * (int(CrImg.at<uchar>(i / 2, j / 2)) - 128),0,255);
			bayerImg.at<uchar>(i + 1, j + 1) = Clip(-0.351 * int(Y1Img.at<uchar>(i/ 2 , j / 2)) + 1.351 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.336 * (int(CbImg.at<uchar>(i / 2, j / 2)) - 128) - 0.698 * (int(CrImg.at<uchar>(i / 2, j / 2)) - 128),0,255);
			//bayerImg.at<uchar>(i + 1, j + 1) = Clip(-0.351 * int(Y1Img.at<uchar>(i / 2, j / 2)) + 1.351 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.336 * int(CbImg.at<uchar>(i / 2, j / 2)) - 0.698 * int(CrImg.at<uchar>(i / 2, j / 2)) - 175.488, 0, 255);
			//B
			//bayerImg.at<uchar>(i + 1, j) = Clip(0.581 * int(Y1Img.at<uchar>(i / 2, j / 2)) + 0.581 * int(Y2Img.at<uchar>(i / 2, j / 2)) + 2.019 * (int(CbImg.at<uchar>(i / 2, j / 2)) - 128) + 0 * (int(CrImg.at<uchar>(i / 2, j / 2)) - 128),0,255);
			bayerImg.at<uchar>(i + 1, j) = Clip(0.5 * int(Y1Img.at<uchar>(i / 2, j / 2)) + 0.5 * int(Y2Img.at<uchar>(i / 2, j / 2)) + 1.733 * (int(CbImg.at<uchar>(i / 2, j / 2)) - 128) - 0.0016 * (int(CrImg.at<uchar>(i / 2, j / 2)) - 128),0,255);
			//bayerImg.at<uchar>(i + 1, j) = Clip(0.5 * int(Y1Img.at<uchar>(i / 2, j / 2)) + 0.5 * int(Y2Img.at<uchar>(i / 2, j / 2)) + 1.733 * int(CbImg.at<uchar>(i / 2, j / 2)) - 0.0016 * int(CrImg.at<uchar>(i / 2, j / 2)) - 221.696, 0, 255);
			//R
			//bayerImg.at<uchar>(i, j + 1) = Clip(0.582 * int(Y1Img.at<uchar>(i / 2, j / 2)) + 0.582 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.002 * (int(CbImg.at<uchar>(i / 2, j / 2)) - 128) + 1.596 * (int(CrImg.at<uchar>(i / 2, j / 2)) - 128),0,255);
			bayerImg.at<uchar>(i, j + 1) = Clip(0.5 * int(Y1Img.at<uchar>(i / 2, j / 2)) + 0.5 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.00028763 * (int(CbImg.at<uchar>(i / 2, j / 2)) - 128) - 1.371 * (int(CrImg.at<uchar>(i / 2, j / 2)) - 128),0,255);
			//bayerImg.at<uchar>(i, j + 1) = Clip(0.5 * int(Y1Img.at<uchar>(i / 2, j / 2)) + 0.5 * int(Y2Img.at<uchar>(i / 2, j / 2)) - 0.00028763 * int(CbImg.at<uchar>(i / 2, j / 2)) - 1.371 * int(CrImg.at<uchar>(i / 2, j / 2)) + 132.352, 0, 255);
		}
	}
	return bayerImg;
}

void saveYUV(Mat&Y, Mat&U, Mat&V, fstream& yuvFILE)
{ /**
  * Save Y, U, V to YUV
  *
  * @param[in] Y:
  * @param[in] U:
  * @param[in] V:
  *
  * @param[out] yuvFILE: output YUV file
  */
	Mat Y8U, U8U, V8U;
	Y.convertTo(Y8U, CV_8UC1);
	U.convertTo(U8U, CV_8UC1);
	V.convertTo(V8U, CV_8UC1);

	if (!yuvFILE) {
		cerr << "in \"_saveYUV\" funciotn" << endl;
		cerr << "Can't write yuv file" << endl;
		cerr << "Path : " << yuvFILE._Stdstr << endl;

		system("pause");
		exit(-1);
	}

	for (int y = 0; y < Y8U.rows; ++y)
		for (int x = 0; x < Y8U.cols; ++x)
			yuvFILE.write((char *)Y8U.data + y * Y8U.step1(0) + x * Y8U.step1(1) + 0, 1);

	for (int y = 0; y < U8U.rows; ++y) {
		for (int x = 0; x < U8U.cols; ++x) {
			yuvFILE.write((char *)U8U.data + y * U8U.step1(0) + x * U8U.step1(1) + 0, 1);
		}
	}
	//yuvFILE.write("00000",5);

	for (int y = 0; y < V8U.rows; ++y)
		for (int x = 0; x < V8U.cols; ++x)
			yuvFILE.write((char *)V8U.data + y * V8U.step1(0) + x * V8U.step1(1) + 0, 1);
	yuvFILE.close();
}

void Read_YUV420(int height, int width, fstream& yuv_reader, Mat& Y_plane, Mat& sub_U_plane, Mat& sub_V_plane)
{
	int y, x;
	// Y
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
			yuv_reader.read((char*)Y_plane.data + y * Y_plane.step1(0) + x * Y_plane.step1(1) + 0, 1);
	}
	/*
	// U
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
			yuv_reader.read((char*)sub_U_plane.data + y * sub_U_plane.step1(0) + x * sub_U_plane.step1(1) + 0, 1);
	}

	// V
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
			yuv_reader.read((char*)sub_V_plane.data + y * sub_V_plane.step1(0) + x * sub_V_plane.step1(1) + 0, 1);
	}
	*/
}

double SSIM(Mat& i1, Mat& i2) {
	const double C1 = 6.5025, C2 = 58.5225;
	int d = CV_64F;
	Mat I1, I2;
	i1.convertTo(I1, d);
	i2.convertTo(I2, d);
	Mat I1_2 = I1.mul(I1);
	Mat I2_2 = I2.mul(I2);
	Mat I1_I2 = I1.mul(I2);
	Mat mu1, mu2;
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);
	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);
	Mat sigma1_2, sigam2_2, sigam12;
	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigam2_2, Size(11, 11), 1.5);
	sigam2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigam12, Size(11, 11), 1.5);
	sigam12 -= mu1_mu2;
	Mat t1, t2, t3;
	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigam12 + C2;
	t3 = t1.mul(t2);

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigam2_2 + C2;
	t1 = t1.mul(t2);

	Mat ssim_map;
	divide(t3, t1, ssim_map);
	Scalar mssim = mean(ssim_map);

	double ssim = (mssim.val[0] + mssim.val[1] + mssim.val[2]) / 3;
	//double ssim = mssim.val[0];
	return ssim;
}

double Bayer_SSIM(Mat& i1, Mat& i2) {
	const double C1 = 6.5025, C2 = 58.5225;
	int d = CV_64F;
	Mat I1, I2;
	i1.convertTo(I1, d);
	i2.convertTo(I2, d);
	Mat I1_2 = I1.mul(I1);
	Mat I2_2 = I2.mul(I2);
	Mat I1_I2 = I1.mul(I2);
	Mat mu1, mu2;
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);
	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);
	Mat sigma1_2, sigam2_2, sigam12;
	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigam2_2, Size(11, 11), 1.5);
	sigam2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigam12, Size(11, 11), 1.5);
	sigam12 -= mu1_mu2;
	Mat t1, t2, t3;
	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigam12 + C2;
	t3 = t1.mul(t2);

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigam2_2 + C2;
	t1 = t1.mul(t2);

	Mat ssim_map;
	divide(t3, t1, ssim_map);
	Scalar mssim = mean(ssim_map);

	//double ssim = (mssim.val[0] + mssim.val[1] + mssim.val[2]) / 3;
	double ssim = mssim.val[0];
	return ssim;
}
Mat DWBayer_to_YDgCbCr4(Mat& bayerImg, Mat& YDgCbCrImg)
{
	Mat temp_bayerImg;//i向下j向右
	Mat YImg;
	Mat YsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	Mat DgImg;
	Mat DgsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	Mat CbImg;
	Mat CbsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	Mat CrImg;
	Mat CrsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	temp_bayerImg = bayerImg;
	YImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	DgImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	CbImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	CrImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	YDgCbCrImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	/*
	for (int i = 0; i < YDgCbCrImg.rows; i += 2) {
		for (int j = 0; j < YDgCbCrImg.cols; j += 2) {
			//Cr component
			if ((1 * int(temp_bayerImg.at<uchar>(i, j+1)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j )) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j+1))) < 0)
				CrsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CrsignImg.at<uchar>(i / 2, j / 2) = 255;
			CrImg.at<uchar>(i / 2, j / 2) =  abs(1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)));
			//Y component
			YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs(0.25 * int(temp_bayerImg.at<uchar>(i, j+1)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j )) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j+1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j ))) + 0.5;
			//Cb component
			if (((-0.5) * int(temp_bayerImg.at<uchar>(i, j )) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j+1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j ))) < 0)
				CbsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CbsignImg.at<uchar>(i / 2, j / 2) = 255;
			CbImg.at<uchar>(i / 2, j / 2) = abs((-0.5) * int(temp_bayerImg.at<uchar>(i, j )) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j+1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j )));
			//Dg component
			if (((-1) * int(temp_bayerImg.at<uchar>(i, j )) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j+1))) < 0)
				DgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DgsignImg.at<uchar>(i / 2, j / 2) = 255;
			DgImg.at<uchar>(i / 2, j / 2) = abs((-1) * int(temp_bayerImg.at<uchar>(i, j )) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j+1)));
		}
	}
	*/
	for (int i = 0; i < YDgCbCrImg.rows; i += 2) {
		for (int j = 0; j < YDgCbCrImg.cols; j += 2) {
			//Cr component
			if ((1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - floor(0.5 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)))) < 0)
				CrsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CrsignImg.at<uchar>(i / 2, j / 2) = 255;
			CrImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - floor(0.5 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))));
			//Y component
			YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs(0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.5*(floor(0.5 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)))) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j))) + 0.5;
			//Cb component
			if ((1 * int(temp_bayerImg.at<uchar>(i + 1, j))- floor(0.5 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)))) < 0)
				CbsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CbsignImg.at<uchar>(i / 2, j / 2) = 255;
			CbImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i + 1, j)) - floor(0.5 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))));
			//Dg component
			if (((-1) * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) < 0)
				DgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DgsignImg.at<uchar>(i / 2, j / 2) = 255;
			DgImg.at<uchar>(i / 2, j / 2) = abs((-1) * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)));
		}
	}

	Mat htemp;
	Mat htemp2;
	Mat hsigntemp;
	Mat hsigntemp2;
	Mat signImg;
	hconcat(YImg, DgImg, htemp);
	hconcat(CbImg, CrImg, htemp2);
	vconcat(htemp, htemp2, YDgCbCrImg);
	hconcat(YsignImg, DgsignImg, hsigntemp);
	hconcat(CbsignImg, CrsignImg, hsigntemp2);
	vconcat(hsigntemp, hsigntemp2, signImg);
	return signImg;
}
Mat DWYDgCbCr4_to_Bayer(Mat& YDgCbCrImg, Mat& signImg) {
	Mat bayerImg = Mat::zeros(YDgCbCrImg.rows, YDgCbCrImg.cols, CV_8UC1);

	//Mat YImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat DgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CoImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);

	Mat YImg = YDgCbCrImg(Range(0, YDgCbCrImg.rows * 0.5), Range(0, YDgCbCrImg.cols * 0.5)).clone();
	Mat DgImg = YDgCbCrImg(Range(0, YDgCbCrImg.rows * 0.5), Range(YDgCbCrImg.cols * 0.5, YDgCbCrImg.cols)).clone();
	Mat CbImg = YDgCbCrImg(Range(YDgCbCrImg.rows * 0.5, YDgCbCrImg.rows), Range(0, YDgCbCrImg.cols * 0.5)).clone();
	Mat CrImg = YDgCbCrImg(Range(YDgCbCrImg.rows * 0.5, YDgCbCrImg.rows), Range(YDgCbCrImg.cols * 0.5, YDgCbCrImg.cols)).clone();

	Mat YsignImg = signImg(Range(0, signImg.rows * 0.5), Range(0, signImg.cols * 0.5)).clone();
	YsignImg.convertTo(YsignImg, CV_64FC1);

	Mat DgsignImg = signImg(Range(0, signImg.rows * 0.5), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DgsignImg.convertTo(DgsignImg, CV_64FC1);

	Mat CbsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(0, signImg.cols * 0.5)).clone();
	CbsignImg.convertTo(CbsignImg, CV_64FC1);

	Mat CrsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(signImg.cols * 0.5, signImg.cols)).clone();
	CrsignImg.convertTo(CrsignImg, CV_64FC1);


	for (int i = 0; i < signImg.rows * 0.5; i++)
		for (int j = 0; j < signImg.cols * 0.5; j++)
		{
			if (YsignImg.at<double>(i, j) == 0)
				YsignImg.at<double>(i, j) = -1;
			else
				YsignImg.at<double>(i, j) = 1;

			if (DgsignImg.at<double>(i, j) == 0)
				DgsignImg.at<double>(i, j) = -1;
			else
				DgsignImg.at<double>(i, j) = 1;

			if (CbsignImg.at<double>(i, j) == 0)
				CbsignImg.at<double>(i, j) = -1;
			else
				CbsignImg.at<double>(i, j) = 1;

			if (CrsignImg.at<double>(i, j) == 0)
				CrsignImg.at<double>(i, j) = -1;
			else
				CrsignImg.at<double>(i, j) = 1;
		}


	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//R
			bayerImg.at<uchar>(i, j+1) = Clip(0.75 * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2) + 1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (-0.25) * int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2) , 0, 255); //R component
			//G1
			bayerImg.at<uchar>(i, j ) = Clip((-0.25) * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2) + (-0.5) * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + (-0.25) * int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2) + 1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) , 0, 255); //G1 component
			//G2
			bayerImg.at<uchar>(i + 1, j+1) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + (-0.25) * int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2) + (-0.25) * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2) , 0, 255); //G2 component
			//B
			bayerImg.at<uchar>(i + 1, j ) = Clip((-0.25) * int(CrImg.at<uchar>(i / 2, j / 2)) * CrsignImg.at<double>(i / 2, j / 2) + 1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (0.75) * int(CbImg.at<uchar>(i / 2, j / 2)) * CbsignImg.at<double>(i / 2, j / 2) , 0, 255); //B component
		}
	}
	return bayerImg;
}
 
//穆罕默德
Mat ORCT2017(Mat& bayerImg, Mat& YDrDwDbImg)
{
	Mat temp_bayerImg;

	Mat DrImg;
	Mat DrsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat WrImg;
	Mat WrsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat WbImg;
	Mat WbsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat DbImg;
	Mat DbsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat YImg;
	Mat YsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat DwImg;
	Mat DwsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	


	double DW = 0;




	temp_bayerImg = bayerImg;

	DrImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	WrImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	WbImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	DbImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	YImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	DwImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	YDrDwDbImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);

	double dr = 0,  wr = 0, db = 0 , wb = 0, dw = 0;
	for (int i = 0; i < bayerImg.rows; i += 2)
	{	//GRBG
		for (int j = 0; j < bayerImg.cols; j += 2)
		{
			//Dr	-G1+R
			if (((-1) * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i, j + 1))) < 0)
				DrsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DrsignImg.at<uchar>(i / 2, j / 2) = 255;
			DrImg.at<uchar>(i / 2, j / 2) = abs((-1) * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i, j + 1))); //Dr component
			dr = (-1) * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i, j + 1));
			//printf("dr=%f\n", dr);
		   
			//Wr	G1+0.5*Dr
			if ((1 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5*dr) < 0)
			    WrsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				WrsignImg.at<uchar>(i / 2, j / 2) = 255;
			WrImg.at<uchar>(i / 2, j / 2) = abs((1 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5* dr));
			wr = (1 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5*dr);
			//printf("wr=%f\n", wr);
			
			//Db=G2-B 有待商確認
			if ((1 * int(temp_bayerImg.at<uchar>(i+1, j+1)) - 1 * int(temp_bayerImg.at<uchar>(i+1, j))) < 0)
				DbsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DbsignImg.at<uchar>(i / 2, j / 2) = 255;
			DbImg.at<uchar>(i / 2, j / 2) = abs((1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) -1 * int(temp_bayerImg.at<uchar>(i + 1, j)))); //Db component
			db=(1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) -1 * int(temp_bayerImg.at<uchar>(i + 1, j)));

			//Wb -Gb+0.5Db
			if ((1 * int(temp_bayerImg.at<uchar>(i+1, j+1)) +0.5* db) < 0)
				WbsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				WbsignImg.at<uchar>(i / 2, j / 2) = 255;
			WbImg.at<uchar>(i / 2, j / 2) = (abs(((- 1) * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 0.5 * db)));//floor
			wb = (-1)* int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 0.5 * db;

			//Dw=Wr-Wb
			if ((wr-wb) < 0)
				DwsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DwsignImg.at<uchar>(i / 2, j / 2) = 255;
			DwImg.at<uchar>(i / 2, j / 2) = abs(wr-wb);
			dw = wr - wb;

			//Y=Wb+0.5*Dw
			if ((wb + dw) < 0)
				YsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs((wb + dw));




		}
	}
	Mat htemp;
	Mat htemp2;
	Mat hsigntemp;
	Mat hsigntemp2;
	Mat signImg;

	hconcat(YImg, DrImg, htemp);
	hconcat(DwImg, DbImg, htemp2);
	vconcat(htemp, htemp2, YDrDwDbImg);
	hconcat(YsignImg, DrsignImg, hsigntemp);
	hconcat(DwsignImg, DbsignImg, hsigntemp2);
	vconcat(hsigntemp, hsigntemp2, signImg);
	return signImg;
}
//穆罕默德IRCT
Mat OIRCT2017(Mat& YDrDwDbImg, Mat& signImg) {//Y4 = YDgCoCg
	Mat temp_YDrDwDbImg = YDrDwDbImg;
	Mat bayerImg = Mat::zeros(YDrDwDbImg.rows, YDrDwDbImg.cols, CV_8UC1);
	Mat YImg = YDrDwDbImg(Range(0, YDrDwDbImg.rows * 0.5), Range(0, YDrDwDbImg.cols * 0.5)).clone();//Top Left
	Mat DrImg = YDrDwDbImg(Range(0, YDrDwDbImg.rows * 0.5), Range(YDrDwDbImg.cols * 0.5, YDrDwDbImg.cols)).clone();//Top Right
	Mat DwImg = YDrDwDbImg(Range(YDrDwDbImg.rows * 0.5, YDrDwDbImg.rows), Range(0, YDrDwDbImg.cols * 0.5)).clone();//Bottom Left
	Mat DbImg = YDrDwDbImg(Range(YDrDwDbImg.rows * 0.5, YDrDwDbImg.rows), Range(YDrDwDbImg.cols * 0.5, YDrDwDbImg.cols)).clone();//Bottom Right
	
	Mat YsignImg = signImg(Range(0, signImg.rows * 0.5), Range(0, signImg.cols * 0.5)).clone();
	YsignImg.convertTo(YsignImg, CV_64FC1);

	Mat DrsignImg = signImg(Range(0, signImg.rows * 0.5), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DrsignImg.convertTo(DrsignImg, CV_64FC1);

	Mat DwsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(0, signImg.cols * 0.5)).clone();
	DwsignImg.convertTo(DwsignImg, CV_64FC1);

	Mat DbsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DbsignImg.convertTo(DbsignImg, CV_64FC1);

	for (int i = 0; i < signImg.rows * 0.5; i++)
		for (int j = 0; j < signImg.cols * 0.5; j++)
		{
			if (YsignImg.at<double>(i, j) == 0)
				YsignImg.at<double>(i, j) = -1;
			else
				YsignImg.at<double>(i, j) = 1;

			if (DrsignImg.at<double>(i, j) == 0)
				DrsignImg.at<double>(i, j) = -1;
			else
				DrsignImg.at<double>(i, j) = 1;

			if (DwsignImg.at<double>(i, j) == 0)
				DwsignImg.at<double>(i, j) = -1;
			else
				DwsignImg.at<double>(i, j) = 1;

			if (DbsignImg.at<double>(i, j) == 0)
				DbsignImg.at<double>(i, j) = -1;
			else
				DbsignImg.at<double>(i, j) = 1;
		}

	double WB=0, WR = 0,G2=0,G1=0;
	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {
			
	
			//WB=Y-0.5Dw
			WB = 1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) - 0.5 * int(DwImg.at<uchar>(i / 2, j / 2)) * DwsignImg.at<double>(i / 2, j / 2);
			
			//WR=Wb+Dw
			WR = WB + 1 * int(DwImg.at<uchar>(i / 2, j / 2)) * DwsignImg.at<double>(i / 2, j / 2);
			
			//G2=Wb-0.5*Db
			
			bayerImg.at<uchar>(i + 1, j + 1) = Clip(0.5 * int(DbImg.at<uchar>(i / 2, j / 2)) * DbsignImg.at<double>(i / 2, j / 2)-WB,0,255);
			G2 = 0.5 * int(DbImg.at<uchar>(i / 2, j / 2)) * DbsignImg.at<double>(i / 2, j / 2) - WB;
			
			//B=Db+G2
			bayerImg.at<uchar>(i + 1, j) = Clip(1 * int(DbImg.at<uchar>(i / 2, j / 2)) * DbsignImg.at<double>(i / 2, j / 2) + G2, 0, 255);
			
			//G1 Wr-0.5Dr
			
			bayerImg.at<uchar>(i, j) = Clip(WR - 0.5 * int(DrImg.at<uchar>(i / 2, j / 2)) * DrsignImg.at<double>(i / 2, j / 2), 0, 255);
			G1 = WR - 0.5 * int(DrImg.at<uchar>(i / 2, j / 2)) * DrsignImg.at<double>(i / 2, j / 2);
			
			//R=Dr+G1
			bayerImg.at<uchar>(i, j + 1) = Clip(1 * int(DrImg.at<uchar>(i / 2, j / 2)) * DrsignImg.at<double>(i / 2, j / 2) +G1, 0, 255);

		}
	}

	return bayerImg;
}



//穆罕默德
Mat ORCT2018(Mat& bayerImg, Mat& YDrDwDbImg)
{
	Mat temp_bayerImg;
	/*
	Mat YImg;
	Mat YsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat DgImg;
	Mat DgsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat CoImg;
	Mat CosignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat CgImg;
	Mat CgsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	*/

	Mat DrImg;
	Mat DrsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat WrImg;
	Mat WrsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat WbImg;
	Mat WbsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat DbImg;
	Mat DbsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat YImg;
	Mat YsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat DwImg;
	Mat DwsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;



	double DW = 0;




	temp_bayerImg = bayerImg;
	/*
	YImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	DgImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	CoImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	CgImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	*/
	DrImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	WrImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	WbImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	DbImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	YImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	DwImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	YDrDwDbImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);

	//signImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	/*
	for (int i = 0; i < bayerImg.rows; i += 2) {	//GRBG
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//Co	R-B
			if ((1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				CosignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CosignImg.at<uchar>(i / 2, j / 2) = 255;
			CoImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j))); //Co component
			//Cg	(G1+G2-R-B)*0.5
			if ((0.5 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) < 0))
				CgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CgsignImg.at<uchar>(i / 2, j / 2) = 255;
			CgImg.at<uchar>(i / 2, j / 2) = abs(0.5 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))); //Cg component
			//Y		(G1+G2+R+B)*0.25
			YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs(0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) + 0.5; //Y component
			//Dg	G2-G1
			if ((1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j))) < 0)
				DgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DgsignImg.at<uchar>(i / 2, j / 2) = 255;
			DgImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j))); //Dg component
		}
	}
	*/
	double dr = 0, drf = 0, wr = 0, db = 0, dbf = 0, wb = 0, dw = 0, dwf = 0;
	for (int i = 0; i < bayerImg.rows; i += 2)
	{	//GRBG
		for (int j = 0; j < bayerImg.cols; j += 2)
		{
			//Dr	-G1+R
			if (((-1) * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i, j + 1))) < 0)
				DrsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DrsignImg.at<uchar>(i / 2, j / 2) = 255;
			DrImg.at<uchar>(i / 2, j / 2) = abs((-1) * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i, j + 1))); //Dr component
			dr = (-1) * int(temp_bayerImg.at<uchar>(i, j)) + 1 * int(temp_bayerImg.at<uchar>(i, j + 1));
			//printf("dr=%f\n", dr);

			//Wr	G1+Floor(0.5*Dr)
			drf = floor(0.5 * dr);//floor
			if ((1 * int(temp_bayerImg.at<uchar>(i, j)) + 1 * drf) < 0)
				WrsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				WrsignImg.at<uchar>(i / 2, j / 2) = 255;
			WrImg.at<uchar>(i / 2, j / 2) = abs((1 * int(temp_bayerImg.at<uchar>(i, j)) + 1 * drf));
			wr = (1 * int(temp_bayerImg.at<uchar>(i, j)) + 1 * drf);
			//printf("wr=%f\n", wr);

			//Db=-G2+B
			if (((-1) * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				DbsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DbsignImg.at<uchar>(i / 2, j / 2) = 255;
			DbImg.at<uchar>(i / 2, j / 2) = abs(((-1) * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j)))); //Db component
			db = ((-1) * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 1 * int(temp_bayerImg.at<uchar>(i + 1, j)));

			//Wb Gb+0.5Db
			dbf = floor(0.5 * db);//floor
			if ((1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 1 * dbf) < 0)
				WbsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				WbsignImg.at<uchar>(i / 2, j / 2) = 255;
			WbImg.at<uchar>(i / 2, j / 2) = (abs((1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 1 * dbf)));//floor
			wb = 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 1 * dbf;

			//Dw=Wr-Wb
			if ((wr - wb) < 0)
				DwsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DwsignImg.at<uchar>(i / 2, j / 2) = 255;
			DwImg.at<uchar>(i / 2, j / 2) = abs((wr - wb));
			dw = (wr - wb);

			//Y=Wb+0.5*Dw
			dwf = floor(0.5 * dw);//floor
			if ((wb + dwf) < 0)
				YsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = (abs((wb + dwf)));//floor




		}
	}
	Mat htemp;
	Mat htemp2;
	Mat hsigntemp;
	Mat hsigntemp2;
	Mat signImg;
	/*
	hconcat(YImg, DgImg, htemp);
	hconcat(CoImg, CgImg, htemp2);
	vconcat(htemp, htemp2, YDgCoCgImg);
	hconcat(YsignImg, DgsignImg, hsigntemp);
	hconcat(CosignImg, CgsignImg, hsigntemp2);
	vconcat(hsigntemp, hsigntemp2, signImg);
	*/
	hconcat(YImg, DrImg, htemp);
	hconcat(DwImg, DbImg, htemp2);
	vconcat(htemp, htemp2, YDrDwDbImg);
	hconcat(YsignImg, DrsignImg, hsigntemp);
	hconcat(DwsignImg, DbsignImg, hsigntemp2);
	vconcat(hsigntemp, hsigntemp2, signImg);
	return signImg;
}
//穆罕默德IRCT
Mat OIRCT2018(Mat& YDrDwDbImg, Mat& signImg) {//Y4 = YDgCoCg
	//Mat temp_YDgCoCgImg = YDgCoCgImg;
	//Mat bayerImg = Mat::zeros(YDgCoCgImg.rows, YDgCoCgImg.cols, CV_8UC1);

	Mat temp_YDrDwDbImg = YDrDwDbImg;
	Mat bayerImg = Mat::zeros(YDrDwDbImg.rows, YDrDwDbImg.cols, CV_8UC1);

	//Mat YImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat DgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CoImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);

	//Mat YImg = YDgCoCgImg(Range(0, YDgCoCgImg.rows * 0.5), Range(0, YDgCoCgImg.cols * 0.5)).clone();//Top Left
	//Mat DgImg = YDgCoCgImg(Range(0, YDgCoCgImg.rows * 0.5), Range(YDgCoCgImg.cols * 0.5, YDgCoCgImg.cols)).clone();//Top Right
	//Mat CoImg = YDgCoCgImg(Range(YDgCoCgImg.rows * 0.5, YDgCoCgImg.rows), Range(0, YDgCoCgImg.cols * 0.5)).clone();//Bottom Left
	//Mat CgImg = YDgCoCgImg(Range(YDgCoCgImg.rows * 0.5, YDgCoCgImg.rows), Range(YDgCoCgImg.cols * 0.5, YDgCoCgImg.cols)).clone();//Bottom Right

	Mat YImg = YDrDwDbImg(Range(0, YDrDwDbImg.rows * 0.5), Range(0, YDrDwDbImg.cols * 0.5)).clone();//Top Left
	Mat DrImg = YDrDwDbImg(Range(0, YDrDwDbImg.rows * 0.5), Range(YDrDwDbImg.cols * 0.5, YDrDwDbImg.cols)).clone();//Top Right
	Mat DwImg = YDrDwDbImg(Range(YDrDwDbImg.rows * 0.5, YDrDwDbImg.rows), Range(0, YDrDwDbImg.cols * 0.5)).clone();//Bottom Left
	Mat DbImg = YDrDwDbImg(Range(YDrDwDbImg.rows * 0.5, YDrDwDbImg.rows), Range(YDrDwDbImg.cols * 0.5, YDrDwDbImg.cols)).clone();//Bottom Right
	/*
	Mat YsignImg = signImg(Range(0, signImg.rows * 0.5), Range(0, signImg.cols * 0.5)).clone();
	YsignImg.convertTo(YsignImg, CV_64FC1);

	Mat DgsignImg = signImg(Range(0, signImg.rows * 0.5), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DgsignImg.convertTo(DgsignImg, CV_64FC1);

	Mat CosignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(0, signImg.cols * 0.5)).clone();
	CosignImg.convertTo(CosignImg, CV_64FC1);

	Mat CgsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(signImg.cols * 0.5, signImg.cols)).clone();
	CgsignImg.convertTo(CgsignImg, CV_64FC1);
	*/

	Mat YsignImg = signImg(Range(0, signImg.rows * 0.5), Range(0, signImg.cols * 0.5)).clone();
	YsignImg.convertTo(YsignImg, CV_64FC1);

	Mat DrsignImg = signImg(Range(0, signImg.rows * 0.5), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DrsignImg.convertTo(DrsignImg, CV_64FC1);

	Mat DwsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(0, signImg.cols * 0.5)).clone();
	DwsignImg.convertTo(DwsignImg, CV_64FC1);

	Mat DbsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DbsignImg.convertTo(DbsignImg, CV_64FC1);
	/*
		for (int i = 0; i < signImg.rows * 0.5; i++)
			for (int j = 0; j < signImg.cols * 0.5; j++)
			{
				if (YsignImg.at<double>(i, j) == 0)
					YsignImg.at<double>(i, j) = -1;
				else
					YsignImg.at<double>(i, j) = 1;

				if (DgsignImg.at<double>(i, j) == 0)
					DgsignImg.at<double>(i, j) = -1;
				else
					DgsignImg.at<double>(i, j) = 1;

				if (CosignImg.at<double>(i, j) == 0)
					CosignImg.at<double>(i, j) = -1;
				else
					CosignImg.at<double>(i, j) = 1;

				if (CgsignImg.at<double>(i, j) == 0)
					CgsignImg.at<double>(i, j) = -1;
				else
					CgsignImg.at<double>(i, j) = 1;
			}
		*/
	for (int i = 0; i < signImg.rows * 0.5; i++)
		for (int j = 0; j < signImg.cols * 0.5; j++)
		{
			if (YsignImg.at<double>(i, j) == 0)
				YsignImg.at<double>(i, j) = -1;
			else
				YsignImg.at<double>(i, j) = 1;

			if (DrsignImg.at<double>(i, j) == 0)
				DrsignImg.at<double>(i, j) = -1;
			else
				DrsignImg.at<double>(i, j) = 1;

			if (DwsignImg.at<double>(i, j) == 0)
				DwsignImg.at<double>(i, j) = -1;
			else
				DwsignImg.at<double>(i, j) = 1;

			if (DbsignImg.at<double>(i, j) == 0)
				DbsignImg.at<double>(i, j) = -1;
			else
				DbsignImg.at<double>(i, j) = 1;
		}

	/*

	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//G1
			bayerImg.at<uchar>(i, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (-0.5) * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + 0.5 * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255);
			//R
			bayerImg.at<uchar>(i, j + 1) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(CoImg.at<uchar>(i / 2, j / 2)) * CosignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255);
			//B
			bayerImg.at<uchar>(i + 1, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CoImg.at<uchar>(i / 2, j / 2)) * CosignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255);
			//G2
			bayerImg.at<uchar>(i + 1, j + 1) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + 0.5 * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255);
		}
	}
	*/
	//YDrDwDbImg
	double WB = 0, WR = 0, G21 = 0, G22 = 0, DW = 0, G11 = 0, G12 = 0;
	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {

			DW = floor(0.5 * int(DwImg.at<uchar>(i / 2, j / 2)) * DwsignImg.at<double>(i / 2, j / 2));//floor

			WB = (1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) - DW);

			WR = (WB + 1 * int(DwImg.at<uchar>(i / 2, j / 2)) * DwsignImg.at<double>(i / 2, j / 2));

			//G2=Wb-floor(0.5*Db)
			G21 = floor(0.5 * int(DbImg.at<uchar>(i / 2, j / 2)) * DbsignImg.at<double>(i / 2, j / 2));//floor
			bayerImg.at<uchar>(i + 1, j + 1) = Clip(WB - G21, 0, 255);
			G22 = WB - G21;

			//B
			bayerImg.at<uchar>(i + 1, j) = Clip(1 * int(DbImg.at<uchar>(i / 2, j / 2)) * DbsignImg.at<double>(i / 2, j / 2) + G22, 0, 255);

			//G1 Wr+floor(0.5Dr)
			G11 = floor(0.5 * int(DrImg.at<uchar>(i / 2, j / 2)) * DrsignImg.at<double>(i / 2, j / 2));//floor
			bayerImg.at<uchar>(i, j) = Clip(WR - G11, 0, 255);
			G12 = WR - G11;

			//R Dr+Dg
			bayerImg.at<uchar>(i, j + 1) = Clip(1 * int(DrImg.at<uchar>(i / 2, j / 2)) * DrsignImg.at<double>(i / 2, j / 2) + G12, 0, 255);

		}
	}

	return bayerImg;
}


//試試看無失真malvar
Mat Bayer_to_YDgCoCg4_lossless(Mat& bayerImg, Mat& YDgCoCgImg)
{
	Mat temp_bayerImg;
	Mat YImg;
	Mat YsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat DgImg;
	Mat DgsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat CoImg;
	Mat CosignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat CgImg;
	Mat CgsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	temp_bayerImg = bayerImg;
	YImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	DgImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	CoImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	CgImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	YDgCoCgImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	//signImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);
	/*
	for (int i = 0; i < bayerImg.rows; i += 2) {	//GRBG
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//Co	R-B
			if ((1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				CosignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CosignImg.at<uchar>(i / 2, j / 2) = 255;
			CoImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j))); //Co component
			//Cg	(G1+G2-R-B)*0.5
			if ((0.5 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) < 0))
				CgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CgsignImg.at<uchar>(i / 2, j / 2) = 255;
			CgImg.at<uchar>(i / 2, j / 2) = abs(0.5 * int(temp_bayerImg.at<uchar>(i, j)) - 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))); //Cg component
			//Y		(G1+G2+R+B)*0.25
			YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs(0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1))) + 0.5; //Y component
			//Dg	G2-G1
			if ((1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j))) < 0)
				DgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DgsignImg.at<uchar>(i / 2, j / 2) = 255;
			DgImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j))); //Dg component
		}
	}
	*/
	double CO = 0,COF=0,DG=0,DGF=0,CG=0,CGF=0,u=0,v=0;
	for (int i = 0; i < bayerImg.rows; i += 2) {	//GRBG
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//Co	R-B
			if ((1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				CosignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CosignImg.at<uchar>(i / 2, j / 2) = 255;
			CoImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j))); //Co component
			CO = 1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i + 1, j));
			//Dg	G2-G1
			if ((1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j))) < 0)
				DgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				DgsignImg.at<uchar>(i / 2, j / 2) = 255;
			DgImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j))); //Dg component
			DG = 1 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 1 * int(temp_bayerImg.at<uchar>(i, j));
			
			//u=B+(Co>>1)=B+floor(0.5*Co)
			COF = floor(0.5 * CO);
			u = 1 * int(temp_bayerImg.at<uchar>(i + 1, j)) + COF;

			//v=G1+(Dg>>1)=G1+floor(0.5*Dg)
			DGF = floor(0.5 * DG);
			v = 1 * int(temp_bayerImg.at<uchar>(i , j)) + DGF;

			//Cg=v-u;
			if((v-u)<0)
				CgsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				CgsignImg.at<uchar>(i / 2, j / 2) = 255;
			CgImg.at<uchar>(i / 2, j / 2) = abs(v - u);
			CG = v - u;
			
			//Y=u+(Cg>>1)=u+floor(0.5*Cg)
			CGF = floor(0.5 * CG);
			if ((u+CGF) < 0)
				YsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs(u + CGF);
			
	
			
		}
	}

	Mat htemp;
	Mat htemp2;
	Mat hsigntemp;
	Mat hsigntemp2;
	Mat signImg;
	hconcat(YImg, DgImg, htemp);
	hconcat(CoImg, CgImg, htemp2);
	vconcat(htemp, htemp2, YDgCoCgImg);
	hconcat(YsignImg, DgsignImg, hsigntemp);
	hconcat(CosignImg, CgsignImg, hsigntemp2);
	vconcat(hsigntemp, hsigntemp2, signImg);
	return signImg;
}
Mat YDgCoCg_to_Bayer_lossless(Mat& YDgCoCgImg, Mat& signImg) {//Y4 = YDgCoCg
	Mat temp_YDgCoCgImg = YDgCoCgImg;
	Mat bayerImg = Mat::zeros(YDgCoCgImg.rows, YDgCoCgImg.cols, CV_8UC1);

	//Mat YImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat DgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CoImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);
	//Mat CgImg = Mat::zeros(YDgCoCgImg.rows * 0.5, YDgCoCgImg.cols * 0.5, CV_8UC1);

	Mat YImg = YDgCoCgImg(Range(0, YDgCoCgImg.rows * 0.5), Range(0, YDgCoCgImg.cols * 0.5)).clone();//Top Left
	Mat DgImg = YDgCoCgImg(Range(0, YDgCoCgImg.rows * 0.5), Range(YDgCoCgImg.cols * 0.5, YDgCoCgImg.cols)).clone();//Top Right
	Mat CoImg = YDgCoCgImg(Range(YDgCoCgImg.rows * 0.5, YDgCoCgImg.rows), Range(0, YDgCoCgImg.cols * 0.5)).clone();//Bottom Left
	Mat CgImg = YDgCoCgImg(Range(YDgCoCgImg.rows * 0.5, YDgCoCgImg.rows), Range(YDgCoCgImg.cols * 0.5, YDgCoCgImg.cols)).clone();//Bottom Right

	Mat YsignImg = signImg(Range(0, signImg.rows * 0.5), Range(0, signImg.cols * 0.5)).clone();
	YsignImg.convertTo(YsignImg, CV_64FC1);

	Mat DgsignImg = signImg(Range(0, signImg.rows * 0.5), Range(signImg.cols * 0.5, signImg.cols)).clone();
	DgsignImg.convertTo(DgsignImg, CV_64FC1);

	Mat CosignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(0, signImg.cols * 0.5)).clone();
	CosignImg.convertTo(CosignImg, CV_64FC1);

	Mat CgsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(signImg.cols * 0.5, signImg.cols)).clone();
	CgsignImg.convertTo(CgsignImg, CV_64FC1);


	for (int i = 0; i < signImg.rows * 0.5; i++)
		for (int j = 0; j < signImg.cols * 0.5; j++)
		{
			if (YsignImg.at<double>(i, j) == 0)
				YsignImg.at<double>(i, j) = -1;
			else
				YsignImg.at<double>(i, j) = 1;

			if (DgsignImg.at<double>(i, j) == 0)
				DgsignImg.at<double>(i, j) = -1;
			else
				DgsignImg.at<double>(i, j) = 1;

			if (CosignImg.at<double>(i, j) == 0)
				CosignImg.at<double>(i, j) = -1;
			else
				CosignImg.at<double>(i, j) = 1;

			if (CgsignImg.at<double>(i, j) == 0)
				CgsignImg.at<double>(i, j) = -1;
			else
				CgsignImg.at<double>(i, j) = 1;
		}

	/*
	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//G1
			bayerImg.at<uchar>(i, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (-0.5) * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + 0.5 * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255);
			//R
			bayerImg.at<uchar>(i, j + 1) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(CoImg.at<uchar>(i / 2, j / 2)) * CosignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255);
			//B			
			bayerImg.at<uchar>(i + 1, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CoImg.at<uchar>(i / 2, j / 2)) * CosignImg.at<double>(i / 2, j / 2) + (-0.5) * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255);
			//G2			
			bayerImg.at<uchar>(i + 1, j + 1) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2) + 0.5 * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2), 0, 255);
		}
	}
	*/
	double u = 0, v = 0, CG = 0, CGF = 0, DG = 0, DGF = 0, CO = 0, COF = 0;

	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {
			//u=Y-(Cg>>1)=Y-floor(0.5*Cg);
			CG = 1 * int(CgImg.at<uchar>(i / 2, j / 2)) * CgsignImg.at<double>(i / 2, j / 2);
			CGF = floor(0.5 * CG);
			u = 1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) - CGF;

			//v=u+Cg
			v = u + CG;

			//G1=v-(Dg>>1)=v-floor(0.5*Dg)
			DG = 1 * int(DgImg.at<uchar>(i / 2, j / 2)) * DgsignImg.at<double>(i / 2, j / 2);
			DGF = floor(0.5 * DG);
			bayerImg.at<uchar>(i, j) = Clip(v - DGF, 0, 255);

			//B=u-(Co>>1)=u-floor(0.5*Co)
			CO= 1 * int(CoImg.at<uchar>(i / 2, j / 2)) * CosignImg.at<double>(i / 2, j / 2);
			COF = floor(0.5 * CO);
			bayerImg.at<uchar>(i+1, j) = Clip(u - COF, 0, 255);

            //G2=G1+Dg
			bayerImg.at<uchar>(i + 1, j+1) = Clip(v - DGF+DG, 0, 255);

			//R=B+Co
			bayerImg.at<uchar>(i , j + 1) = Clip(u - COF+CO, 0, 255);
		}
	}

	return bayerImg;
}




//穆罕默德
Mat NEWORCT2017(Mat& bayerImg, Mat& YLMNImg)
{
	Mat temp_bayerImg;

	Mat YImg;
	Mat YsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat LImg;
	Mat LsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat MImg;
	Mat MsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;
	Mat NImg;
	Mat NsignImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);;

	temp_bayerImg = bayerImg;

	
	YImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	LImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	MImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	NImg = Mat::zeros((bayerImg.rows) * 0.5, (bayerImg.cols) * 0.5, CV_8UC1);
	YLMNImg = Mat::zeros(bayerImg.rows, bayerImg.cols, CV_8UC1);

	

	//ORCT1+ORCT2
	for (int i = 0; i < bayerImg.rows; i += 2)
	{	//GRBG
		for (int j = 0; j < bayerImg.cols; j += 2)
		{

			//M=R-*G1
			if ((1 * int(temp_bayerImg.at<uchar>(i , j + 1)) - 1* int(temp_bayerImg.at<uchar>(i , j))) < 0)
				MsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				MsignImg.at<uchar>(i / 2, j / 2) = 255;
			MImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 1* int(temp_bayerImg.at<uchar>(i, j)));

			//L=WRr-Wb=(G1+0.5*M)-(G2+0.5*N)=(G1+0.5*R-0.5*G1)-(G2+0.5*B-0.5*G2)==0.5*G1+0.5*R-0.5*G2-0.5*B
			if ((0.5* int(temp_bayerImg.at<uchar>(i, j ))+ 0.5 * int(temp_bayerImg.at<uchar>(i, j+1))- 0.5 * int(temp_bayerImg.at<uchar>(i+1, j+1)) - 0.5 * int(temp_bayerImg.at<uchar>(i+1, j))) < 0)
				LsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				LsignImg.at<uchar>(i / 2, j / 2) = 255;
			LImg.at<uchar>(i / 2, j / 2) = abs(0.5 * int(temp_bayerImg.at<uchar>(i, j)) + 0.5 * int(temp_bayerImg.at<uchar>(i, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) - 0.5 * int(temp_bayerImg.at<uchar>(i + 1, j)));
		
			//Y=0.5*L+WB=0.5*L+G2+0.5*N=0.5*(0.5*G1+0.5*R-0.5*G2-0.5*B)+G2+0.5*B-0.5*G2==0.25*G1+0.25*R+0.25*G2+0.25*B
			if ((0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j))) < 0)
				YsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				YsignImg.at<uchar>(i / 2, j / 2) = 255;
			YImg.at<uchar>(i / 2, j / 2) = abs(0.25 * int(temp_bayerImg.at<uchar>(i, j)) + 0.25 * int(temp_bayerImg.at<uchar>(i, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j + 1)) + 0.25 * int(temp_bayerImg.at<uchar>(i + 1, j)));
			
			//N=B-*G2
			if ((1 * int(temp_bayerImg.at<uchar>(i+1, j)) - 1 * int(temp_bayerImg.at<uchar>(i+1, j+1))) < 0)
				NsignImg.at<uchar>(i / 2, j / 2) = 0;
			else
				NsignImg.at<uchar>(i / 2, j / 2) = 255;
			NImg.at<uchar>(i / 2, j / 2) = abs(1 * int(temp_bayerImg.at<uchar>(i + 1, j)) - 1* int(temp_bayerImg.at<uchar>(i + 1, j + 1)));


		}
	}
	Mat htemp;
	Mat htemp2;
	Mat hsigntemp;
	Mat hsigntemp2;
	Mat signImg;

	hconcat(YImg, LImg, htemp);
	hconcat(MImg, NImg, htemp2);
	vconcat(htemp, htemp2, YLMNImg);
	hconcat(YsignImg, LsignImg, hsigntemp);
	hconcat(MsignImg, NsignImg, hsigntemp2);
	vconcat(hsigntemp, hsigntemp2, signImg);
	return signImg;
}
//穆罕默德IRCT
Mat NEWOIRCT2017(Mat& YLMNImg, Mat& signImg) {//Y4 = YDgCoCg
	Mat temp_YLMNImg = YLMNImg;
	Mat bayerImg = Mat::zeros(YLMNImg.rows, YLMNImg.cols, CV_8UC1);
	Mat YImg = YLMNImg(Range(0, YLMNImg.rows * 0.5), Range(0, YLMNImg.cols * 0.5)).clone();//Top Left
	Mat LImg = YLMNImg(Range(0, YLMNImg.rows * 0.5), Range(YLMNImg.cols * 0.5, YLMNImg.cols)).clone();//Top Right
	Mat MImg = YLMNImg(Range(YLMNImg.rows * 0.5, YLMNImg.rows), Range(0, YLMNImg.cols * 0.5)).clone();//Bottom Left
	Mat NImg = YLMNImg(Range(YLMNImg.rows * 0.5, YLMNImg.rows), Range(YLMNImg.cols * 0.5, YLMNImg.cols)).clone();//Bottom Right

	Mat YsignImg = signImg(Range(0, signImg.rows * 0.5), Range(0, signImg.cols * 0.5)).clone();
	YsignImg.convertTo(YsignImg, CV_64FC1);

	Mat LsignImg = signImg(Range(0, signImg.rows * 0.5), Range(signImg.cols * 0.5, signImg.cols)).clone();
	LsignImg.convertTo(LsignImg, CV_64FC1);

	Mat MsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(0, signImg.cols * 0.5)).clone();
	MsignImg.convertTo(MsignImg, CV_64FC1);

	Mat NsignImg = signImg(Range(signImg.rows * 0.5, signImg.rows), Range(signImg.cols * 0.5, signImg.cols)).clone();
	NsignImg.convertTo(NsignImg, CV_64FC1);

	for (int i = 0; i < signImg.rows * 0.5; i++)
		for (int j = 0; j < signImg.cols * 0.5; j++)
		{
			if (YsignImg.at<double>(i, j) == 0)
				YsignImg.at<double>(i, j) = -1;
			else
				YsignImg.at<double>(i, j) = 1;

			if (LsignImg.at<double>(i, j) == 0)
				LsignImg.at<double>(i, j) = -1;
			else
				LsignImg.at<double>(i, j) = 1;

			if (MsignImg.at<double>(i, j) == 0)
				MsignImg.at<double>(i, j) = -1;
			else
				MsignImg.at<double>(i, j) = 1;

			if (NsignImg.at<double>(i, j) == 0)
				NsignImg.at<double>(i, j) = -1;
			else
				NsignImg.at<double>(i, j) = 1;
		}

	double WB = 0, WR = 0, G2 = 0, G1 = 0;
	for (int i = 0; i < bayerImg.rows; i += 2) {
		for (int j = 0; j < bayerImg.cols; j += 2) {

			//Wb=Y-0.5*Dw
			//Wr=Wb+Dw=Y-0.5*Dw+Dw==Y+0.5Dw
			//G2=Wb-0.5Db=Y-0.5*Dw-0.5*Db===Y-0.5*L-0.5*N
			bayerImg.at<uchar>(i + 1, j + 1) = Clip(1*int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) -0.5* int(LImg.at<uchar>(i / 2, j / 2)) * LsignImg.at<double>(i / 2, j / 2)-0.5* int(NImg.at<uchar>(i / 2, j / 2)) * NsignImg.at<double>(i / 2, j / 2), 0, 255);

			//B=Db+G2=Db+(Y-0.5*Dw-0.5*Db)=Y-0.5*Dw+0.5*Db=Y-0.5*L+0.5*N
			bayerImg.at<uchar>(i + 1, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) - 0.5 * int(LImg.at<uchar>(i / 2, j / 2)) * LsignImg.at<double>(i / 2, j / 2) + 0.5 * int(NImg.at<uchar>(i / 2, j / 2)) * NsignImg.at<double>(i / 2, j / 2), 0, 255);

			//G1=Wr-0.5*Dr=Y+0.5*Dw-0.5*Dr===Y+0.5*L-0.5*M
			bayerImg.at<uchar>(i, j) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(LImg.at<uchar>(i / 2, j / 2)) * LsignImg.at<double>(i / 2, j / 2) - 0.5 * int(MImg.at<uchar>(i / 2, j / 2)) * MsignImg.at<double>(i / 2, j / 2), 0, 255);

			//R=Dr+G1=Dr+Y+0.5*Dw-0.5*Dr=Y+0.5*Dw+0.5*Dr===Y+0.5*L+0.5*M
			bayerImg.at<uchar>(i, j+1) = Clip(1 * int(YImg.at<uchar>(i / 2, j / 2)) * YsignImg.at<double>(i / 2, j / 2) + 0.5 * int(LImg.at<uchar>(i / 2, j / 2)) * LsignImg.at<double>(i / 2, j / 2) + 0.5 * int(MImg.at<uchar>(i / 2, j / 2)) * MsignImg.at<double>(i / 2, j / 2), 0, 255);

		}
	}

	return bayerImg;
}


