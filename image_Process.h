#include <opencv2/opencv.hpp>
#include <direct.h>
//#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <typeinfo>
//#pragma warning(disable:4996)

using namespace std;
using namespace cv;

/* CFA */
const int CFA_R = 0x01;
const int CFA_G = 0x02;
const int CFA_B = 0x04;
const int CFA_W = 0x08;
const int CFA_None = 0x00;

double Clip(double val, int min, int max);
int GetColor(const Mat& CFA, int y, int x);
void _dirExists(string path);
void ReadCFA(istream& in, Mat& CFA);
void Write_YUV420(int height, int width, fstream& yuv_writer, const Mat& Y_plane, const Mat& sub_U_plane, const Mat& sub_V_plane);
void write_YDgCoCg(int height, int width, fstream& yuv_writer, const Mat& YDgCoCgImg);
void Convert_to_Bayer(int height, int width, Mat& bayerImg, const Mat& bgrImg, const Mat& CFA);
void BGR_to_YUV(int height, int width, Mat& YUV, const Mat& BGR);
void DownSample_420A(int height, int width, Mat& DS_U_plane, Mat& DS_V_plane, const Mat& BGR, const Mat &Y_plane, const Mat& U_plane, const Mat& V_plane);
void Kiku(int height, int width, Mat& dst, const Mat& src, const Mat& CFA);
Mat Bayer_to_YDgCbCr(Mat& bayerImg, Mat& YDgCbCrImg);
Mat Bayer_to_YDgCoCg_Suzuki(Mat& bayerImg, Mat& YDgCoCgImg);
void BGR_to_RGBW(Mat& bgrImg, Mat& rgbwImg);
void RGBW_to_RGBWBandwith(Mat& rgbwImg, Mat& rgbwImg_bandwith);
void RGBWBandwith_to_RGBW(Mat& rgbwImg_bandwith, Mat& rgbwImg);
void RGBW_to_RGBWHalfHhalf(Mat& rgbwImg, Mat& rgbwImgHalfHalf);
void RGBWHalfHhalf_to_RGBW(Mat& rgbwImgHalfHalf, Mat& rgbwImg_re);
Mat YDgCbCr_to_Bayer(Mat& YDgCbCrImg, Mat& signImg);
void Bayer_to_YDgCoCg_Suzuki1(Mat& bayerImg, Mat& YDgCoCgImg);
Mat YYCbCr_to_Bayer(Mat& YYCbCrImg);
Mat Bayer_to_YDgCoCg4(Mat& bayerImg, Mat& YDgCoCgImg);
Mat Y4_to_Bayer(Mat& YDgCoCgImg, Mat& signImg);
Mat Bayer_to_YDgCbCr4(Mat& bayerImg, Mat& YDgCbCrImg);
Mat YDgCbCr4_to_Bayer(Mat& YDgCbCrImg, Mat& signImg);
void Bayer_to_YYCbCr4(Mat& bayerImg, Mat& YYCbCrImg);
Mat YYCbCr4_to_Bayer(Mat& YYCbCrImg);
void saveYUV(Mat&Y, Mat&U, Mat&V, fstream& yuvFILE);
void Read_YUV420(int height, int width, fstream& yuv_reader, Mat& Y_plane, Mat& sub_U_plane, Mat& sub_V_plane);
double SSIM(Mat& i1, Mat& i2);
double Bayer_SSIM(Mat& i1, Mat& i2);
Mat DWYDgCbCr4_to_Bayer(Mat& YDgCbCrImg, Mat& signImg);
Mat DWBayer_to_YDgCbCr4(Mat& bayerImg, Mat& YDgCbCrImg);
Mat ORCT2017(Mat& bayerImg, Mat& DrWrWbDbImg);
Mat OIRCT2017(Mat& YDgCoCgImg, Mat& signImg);
Mat ORCT2018(Mat& bayerImg, Mat& DrWrWbDbImg);
Mat OIRCT2018(Mat& YDgCoCgImg, Mat& signImg);
Mat Bayer_to_YDgCoCg4_lossless(Mat& bayerImg, Mat& YDgCoCgImg);
Mat YDgCoCg_to_Bayer_lossless(Mat& YDgCoCgImg, Mat& signImg);

Mat NEWORCT2017(Mat& bayerImg, Mat& DrWrWbDbImg);
Mat NEWOIRCT2017(Mat& YDgCoCgImg, Mat& signImg);
