#include "image_Process.h"
#include "ctime"
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <sstream>
#include<stack>
using namespace cv;
using namespace std;

int main()
{
	double result_PSNR;
	double result_CPSNR;
	double result_PSNR2;
	double result_SSIM;
	double result_CSSIM;
	double a, b;
	int option,pick,pick2;
	int image_page;
	bool yuv_write = true;	//------------ save YUV file for VTM
	double total_time = 0;
	int evalute = 0;
	int compressor;
	String Data ;
	String method;
	
	cout << "1:RCT 2:IRCT 3:Kiku 4:Caculate PSNR&SSIM 5:Bitrate" << endl;
	cin >> option; //------------ 1  RCT後跑J2K	// 2	J2K完做IRCT
	cout << "Chose dataset 1:IMAX		2:Video		3:Kodak		4:SCI		5:classical  0:other" << endl;
	cin >> pick2;
	switch (pick2)
	{
	case 1:
	{
		Data = "IMAX";
		image_page = 18;
	}
	break;
	case 2:
	{
		Data = "Video";
		image_page = 200;
	}
	break;
	case 3:
	{
		Data = "Kodak";
		image_page = 24;
	}
	break;
	case 4:
	{
		Data = "SCI";
		image_page = 18;
	}
	break;
	case 5:
	{
		Data = "classical";
		image_page = 8;
	}
	break;
	}
	
	

	if (option == 1)//convert Bayer ->  YDgCoCg or YDgCbCr
	{
		cout << "1:JPEG2000   2:VVC" << endl;
		cin >> compressor;
		cout << "Chose RCT method    0:Baseline   1:YDgCoCgLossy   2:YDgCbCr    3:YLMN2017  " << endl;
		cin >> pick;
		switch (pick)
		{
		case 1:
		{
			method = "YDgCoCgLossy";
		}
		break;
		case 2:
		{
			method = "YDgCbCr";
		}
		break;
		case 3:
		{
			method = "YLMN2017";
		}
		break;
		case 0:
		{
			method = "Baseline";
		}
		break;
		}

		for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
		{
			String RCT = method;	//------------YDgCoCg	//YDgCbCr
			String dataset = Data;	//------------IMAX		//Video		//Kodak		//SCI		//classical
			stringstream frameNum;
			frameNum.clear();
			frameNum.str("");
			frameNum << i;
			cout << i << endl;
			Mat bgrImg;
			if (dataset == "Kodak")
				bgrImg = imread(dataset + "/" + frameNum.str() + ".tif");//.tif	//.bmp
			else
				bgrImg = imread(dataset + "/" + frameNum.str() + ".bmp");//.tif	//.bmp

		    
				
			//YDgCoCgLossy
			Mat YDgCoCgLossyImg;	
			Mat YDgCoCgLossyImg_sign;
			//YDgCbCr
			Mat YDgCbCrImg;
			Mat YDgCbCrImg_sign;
			//YLMNM2017
			Mat YLMN2017Img;
			Mat YLMN2017Img_sign;
			
			
			//cout << "bgrImg size=" << bgrImg.size() << endl;//顯示原圖大小
			Mat bayerImg;

			bayerImg = imread(dataset + "_Bayer_GRBG/" + frameNum.str() + ".bmp",0);//讀bayer
			
			/*讀取不同的bayer pattern(CFA)*///------------全彩套CFA轉bayer
			Mat CFA;
			fstream cfa;
			cfa.open("GRBG.cfa", ios::in);	
			ReadCFA(cfa, CFA);//store bayer pattern in CFA based on cfa file
			cfa.close();
			/*讀取不同的bayer pattern(CFA)*/


			/*儲存該CFA的Ground Truth*////------------全彩套CFA轉bayer
			//Convert_to_Bayer(bgrImg.rows, bgrImg.cols, bayerImg, bgrImg, CFA);//turn RGB to bayer pattern
			//_dirExists(dataset + "_Bayer_GRBG/");
			//imwrite(dataset+"_Bayer_GRBG/" + frameNum.str() + ".bmp", bayerImg);
			
			
			
			


			/*將bayerImg做RCT 有的需要額外建立"sign"*/

		    if (RCT == "YDgCoCgLossy")
			{
				a = clock();//計時
			    YDgCoCgLossyImg_sign = Bayer_to_YDgCoCg4(bayerImg, YDgCoCgLossyImg);
			}
			else if (RCT == "YDgCbCr")
			{
				a = clock();//計時
				YDgCbCrImg_sign = DWBayer_to_YDgCbCr4(bayerImg, YDgCbCrImg);
			}
			else if (RCT == "YLMN2017")
			{
				a = clock();//計時
				YLMN2017Img_sign = NEWORCT2017(bayerImg, YLMN2017Img);
				
			}
			b = clock();//計時
			total_time += (b - a) / CLOCKS_PER_SEC;
			cout << "time: "<< setprecision(16) << (b - a) / CLOCKS_PER_SEC << endl;

			_dirExists("4_" + RCT + "/" + dataset + "/");
			_dirExists("4_" + RCT + "/" + dataset + "/GRBG/");
		// Store RCT_IMAGE&sign_img
		    if (RCT == "YDgCoCgLossy")
			{
				imwrite("4_" + RCT + "/" + dataset + "/GRBG/" + frameNum.str() + ".bmp", YDgCoCgLossyImg);
				imwrite("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", YDgCoCgLossyImg_sign);
			}
			else if (RCT == "YDgCbCr")
			{
				imwrite("4_" + RCT + "/" + dataset + "/GRBG/" + frameNum.str() + ".bmp", YDgCbCrImg);
					imwrite("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", YDgCbCrImg_sign);
			}
				else if (RCT == "YLMN2017")
			{
				imwrite("4_" + RCT + "/" + dataset + "/GRBG/" + frameNum.str() + ".bmp", YLMN2017Img);
				imwrite("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", YLMN2017Img_sign);
			}
				else if (RCT == "Baseline")
			{
				imwrite("4_" + RCT + "/" + dataset + "/GRBG/" + frameNum.str() + ".bmp", bayerImg);
				imwrite("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", bayerImg);
			}
				
			//cout << "YDgCoCgImg_sign size=" << YDgCoCgImg_sign.size() << endl;
			//cout << "YDgCoCgImg size=" << YDgCoCgImg.size() << endl;
			//延伸為8的倍數
			int height = bgrImg.rows;
			int width = bgrImg.cols;
			//cout << bgrImg.size() << endl;
			int nh = height + (8 - height % 8) % 8;
			int nw = width + (8 - width % 8) % 8;

			Mat new_Y_plane = Mat::zeros(height, width, CV_8UC1);;//新Y			Mat DS_U_plane, DS_V_plane;//抽樣UV
			
		    if (RCT == "YDgCoCgLossy")
			{
				YDgCoCgLossyImg.convertTo(new_Y_plane, CV_8UC1);
			}
			else if (RCT == "YDgCbCr")
			{
				YDgCbCrImg.convertTo(new_Y_plane, CV_8UC1);
			}
			else if (RCT == "YLMN2017")
			{
				YLMN2017Img.convertTo(new_Y_plane, CV_8UC1);
			}
		
			else if (RCT == "Baseline")
			{
				bayerImg.convertTo(new_Y_plane, CV_8UC1);
			}
			
			Mat DS_U_plane = Mat::zeros(height / 2, width / 2, CV_8UC1);//420
			Mat DS_V_plane = Mat::zeros(height / 2, width / 2, CV_8UC1);//420

			copyMakeBorder(new_Y_plane, new_Y_plane, 0, nh - height, 0, nw - width, BORDER_REPLICATE);
			copyMakeBorder(DS_U_plane, DS_U_plane, 0, (nh - height) / 2, 0, (nw - width) / 2, BORDER_REPLICATE);
			copyMakeBorder(DS_V_plane, DS_V_plane, 0, (nh - height) / 2, 0, (nw - width) / 2, BORDER_REPLICATE);

			//-----------------------------------寫YUV-----------------------------------
			fstream yuvBegin;
			stringstream ssim_yuvImgNameOctober;
			if (yuv_write == true)
			{
				ssim_yuvImgNameOctober.str("");
				ssim_yuvImgNameOctober.clear();
				_dirExists("Bayer YUV/");
				_dirExists("Bayer YUV/"+RCT+"/");
				_dirExists("Bayer YUV/" +RCT + "/"+ dataset + "/source/");
				ssim_yuvImgNameOctober << "Bayer YUV/"<< RCT<< "/" << dataset << "/source/" << RCT << "_" << i << "L.yuv";
				yuvBegin.open(ssim_yuvImgNameOctober.str(), ios::out | ios::binary);//in or out
				saveYUV(new_Y_plane, DS_U_plane, DS_V_plane, yuvBegin);
				yuvBegin.close();
			}

		}
		total_time = total_time / image_page;
		total_time = round(total_time * 10000) / 10000;
		cout << "Avg time: " << total_time << endl;
	}

	else if (option == 2)//convert YYCbCr or YDgCoCg or YDgCbCr -> Bayer 
	{
	cout << "1:JPEG2000   2:VVC" << endl;
	cin >> compressor;
	cout << "Chose RCT method   0:Baseline   1:YDgCoCgLossy   2:YDgCbCr    3:YLMN2017" << endl;
	cin >> pick;
	switch (pick)
	{

	case 1:
	{
		method = "YDgCoCgLossy";
	}
	break;
	case 2:
	{
		method = "YDgCbCr";
	}
	break;
	case 3:
	{
		method = "YLMN2017";
	}
	break;
	case 0:
	{
		method = "Baseline";
	}
	break;
	}
		double t = 0;
		double ave_PSNR = 0, ave_SSIM = 0, ave_CPSNR = 0, ave_CSSIM = 0;
		String RCT = method;//------------YDgCoCg	//YDgCbCr
		String dataset = Data;//------------IMAX		//Video		//Kodak		//SCI		//classical
		string file[10] = { "1", "5", "10", "15", "20", "25", "30", "35", "40", "45" };//compression factor, 1->non compression forJPEG2000
		string qparray[14] = { "0","4","8", "12","16", "20","24", "28","32", "36","40", "44","48","51"};//for vvc
		
		switch (compressor)
		{
		case 1://For JPEG2000
		{
			for (int f = 0; f <10; f++) //------------//10	//6 //c < file array size	or c < qp array size
			{
				cout << "CR=" << file[f]<<"   ";
				for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
				{
					stringstream frameNum;
					frameNum.clear();
					frameNum.str("");
					frameNum << i;;
					
					Mat YDgCoCgLossyImg = imread("E:/Paperwork/J2K/4_" + RCT + "_" + dataset + "_J2K/" + file[f] + "/" + frameNum.str() + ".bmp", 0);
					Mat YDgCbCrImg = imread("E:/Paperwork/J2K/4_" + RCT + "_" + dataset +"_J2K/"+file[f]+"/" + frameNum.str() + ".bmp",0);
					Mat YLMN2017Img = imread("E:/Paperwork/J2K/4_" + RCT + "_" + dataset + "_J2K/" + file[f] + "/" + frameNum.str() + ".bmp", 0);
					Mat bayerImg;
					
					
				    if(RCT == "YDgCoCgLossy")
					{
						Mat signImg = imread("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", 0);
						a = clock();
						bayerImg = Y4_to_Bayer(YDgCoCgLossyImg, signImg);	//------------1 Y4 = YDgCoCg IRCT
					}
					else if (RCT == "YDgCbCr")
					{
						Mat signImg = imread("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", 0);
						a = clock();
						bayerImg= DWYDgCbCr4_to_Bayer(YDgCbCrImg, signImg);
						
					}
					else if (RCT == "YLMN2017")
					{
						Mat signImg = imread("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", 0);
						a = clock();
						bayerImg = NEWOIRCT2017(YLMN2017Img, signImg);

					}
					
					else if (RCT == "Baseline")
					{
						a = clock();
						bayerImg= imread("E:/Paperwork/J2K/4_" + RCT + "_" + dataset + "_J2K/" + file[f] + "/" + frameNum.str() + ".bmp", 0);
					}
				
					b = clock();
					t += (b - a) / CLOCKS_PER_SEC;

					_dirExists(dataset + "_Bayer_" + RCT + "_J2K_GRBG/");
					_dirExists(dataset + "_Bayer_" + RCT + "_J2K_GRBG/" + file[f] + "/");
					imwrite(dataset + "_Bayer_" + RCT + "_J2K_GRBG/" + file[f] + "/" + frameNum.str() + ".bmp", bayerImg);//存bayerImg
					
					cout << "CR=" << file[f] << "  page" << frameNum.str() << "  finish!" << endl;
				}
			}
		}
		break;
		case 2://For VVC
		{
			for (int c = 0; c < 14; c++) //------------//10	//6 //c < file array size	or c < qp array size
			{
				cout << "QP=" << qparray[c]<<"  ";
				for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
				{
					//cout << i<<endl;
					//cout << file[c] << endl;
					stringstream frameNum;
					frameNum.clear();
					frameNum.str("");
					frameNum << i;
				
					Mat YDgCoCgImg;
					
					YDgCoCgImg = imread("C:/JP/JPEG_image_process/JPEG_image_process/4_" + RCT + "/" + dataset + "/GRBG/" + frameNum.str() + ".bmp", 0);
			
					
					
					Mat bayerImg;
					int height = YDgCoCgImg.rows;
					int width = YDgCoCgImg.cols;
					int nh = height + (8 - height % 8) % 8;
					int nw = width + (8 - width % 8) % 8;

					Mat new_Y_plane = Mat::zeros(height, width, CV_8UC1);//新Y			Mat DS_U_plane, DS_V_plane;//抽樣UV
					Mat DS_U_plane = Mat::zeros(height / 2, width / 2, CV_8UC1);//420
					Mat DS_V_plane = Mat::zeros(height / 2, width / 2, CV_8UC1);//420
			
					copyMakeBorder(new_Y_plane, new_Y_plane, 0, nh - height, 0, nw - width, BORDER_REPLICATE);
					copyMakeBorder(DS_U_plane, DS_U_plane, 0, (nh - height) / 2, 0, (nw - width) / 2, BORDER_REPLICATE);
					copyMakeBorder(DS_V_plane, DS_V_plane, 0, (nh - height) / 2, 0, (nw - width) / 2, BORDER_REPLICATE);
						//-----------------------------------讀YUV-----------------------------------
					fstream yuvBegin;
					stringstream ssim_yuvImgNameOctober2;
					ssim_yuvImgNameOctober2.str("");
					ssim_yuvImgNameOctober2.clear();
					
						ssim_yuvImgNameOctober2 << "E:/Paperwork/VVC/BAYER_" + RCT + "_" + dataset + "/result/" + qparray[c] + "/" + RCT + "_" + frameNum.str() + "_L_QP" + qparray[c] + ".yuv";
						
					fstream yuvEnd;
					yuvEnd.open(ssim_yuvImgNameOctober2.str(), ios::in | ios::binary);//in or out		
					Read_YUV420(nh, nw, yuvEnd, new_Y_plane, DS_U_plane, DS_V_plane);
					yuvEnd.close();

					//cout << "new_Y_plane_forIMAX=" << new_Y_plane.size() << endl;
					new_Y_plane = new_Y_plane(Rect(0, 0, width, height));//給IMAX轉回500*500
					//cout << "CHnew_Y_plane_forIMAX=" << new_Y_plane.size() << endl;//檢查轉換後大小
				
		
					a = clock();
	
			
					if (RCT == "YDgCoCgLossy")
					{
						Mat signImg = imread("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", 0);	//------------做YYCbCr要註解掉
						bayerImg = Y4_to_Bayer(new_Y_plane, signImg);	//------------1 Y4 = YDgCoCg IRCT
					}
					else if (RCT == "YDgCbCr")
					{
						Mat signImg = imread("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", 0);	//------------做YYCbCr要註解掉
						bayerImg = DWYDgCbCr4_to_Bayer(new_Y_plane, signImg);

					}
					else if (RCT == "YLMN2017")
					{
						Mat signImg = imread("4_" + RCT + "/" + dataset + "/GRBG/sign" + frameNum.str() + ".bmp", 0);	//------------做YYCbCr要註解掉
						bayerImg = NEWOIRCT2017(new_Y_plane, signImg);

					}
					
					else if (RCT == "Baseline")
					{
						bayerImg = new_Y_plane;
					}
					
					b = clock();
					t += (b - a) / CLOCKS_PER_SEC;
			
				

					//For VVC
					
					_dirExists(dataset + "_Bayer_" + RCT + "_VVC_GRBG/");
					_dirExists(dataset + "_Bayer_" + RCT + "_VVC_GRBG/" + qparray[c] + "/");
					imwrite(dataset + "_Bayer_" + RCT + "_VVC_GRBG/" + qparray[c] + "/" + frameNum.str() + ".bmp", bayerImg);//存bayerImg
					cout << "QP=" << qparray[c] << "  page" << frameNum.str() << "  finish!" << endl;
				}
			}
		}
		}
		
		
		cout << "Avg Time: " << t / 240.0 << endl;
	}
	else if (option == 3)//bayer重建全彩 KIKU Demosaicing
	{
	cout << "1:JPEG2000   2:VVC" << endl;
	cin >> compressor;
	cout << "Chose RCT method   0:Baseline   1:YDgCoCgLossy   2:YDgCbCr    3:YLMN2017  " << endl;
	cin >> pick;
	switch (pick)
	{
	case 2:
	{
		method = "YDgCoCgLossy";
	}
	break;
	case 3:
	{
		method = "YDgCbCr";
	}
	break;
	case 4:
	{
		method = "YLMN2017";
	}
	break;
	case 0:
	{
		method = "Baseline";
	}
	break;
	}
		
		int i;
		String RCT =method;//------------YDgCoCg	//YDgCbCr
		String dataset = Data;//------------IMAX		//Video		//Kodak		//SCI		//classical
		string qparray[14] = { "0","4","8", "12","16", "20","24", "28","32", "36","40", "44","48","51" };//for vvc
		string file[10] = { "1", "5", "10", "15", "20", "25", "30", "35", "40", "45" };//compression factor, 1->non compression forJPEG2000
		Mat CFA;
		fstream cfa;
		cfa.open("GRBG.cfa", ios::in);
		ReadCFA(cfa, CFA);
		cfa.close();

		switch (compressor)
		{
		case 1://For JPEG2000
		{
			for (int f = 0; f < 10; f++) //------------//10	//6 //c < file array size	or c < qp array size
			{
				cout << "CR=" << file[f] << endl;
				for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
				{
					cout << i << endl;
					stringstream frameNum;
					frameNum.clear();
					frameNum.str("");
					frameNum << i;
					Mat bayer_to_RGB = imread(dataset + "_Bayer_" + RCT + "_J2K_GRBG/" + file[f] + "/" + frameNum.str() + ".bmp",0);
					Mat destination;
					int height = bayer_to_RGB.rows;
					int width = bayer_to_RGB.cols;
					Kiku(height, width, destination, bayer_to_RGB, CFA);
					_dirExists(dataset + "_Reconstruction_" + RCT + "_J2K/");
					_dirExists(dataset + "_Reconstruction_" + RCT + "_J2K/" + file[f] + "/");
					if (dataset == "Kodak")
					{
						imwrite(dataset + "_Reconstruction_" + RCT + "_J2K/" + file[f] + "/" + frameNum.str() + ".tif", destination);
					}
					else
					{
						imwrite(dataset + "_Reconstruction_" + RCT + "_J2K/" + file[f] + "/" + frameNum.str() + ".bmp", destination);//存解馬賽克的全彩
						//cout << dataset + "_Reconstruction_" + RCT + "_J2K/" + file[f] + "/" + frameNum.str() + ".bmp" << endl;
					}
					cout << "finish" << endl;
				}
			}
		}
		break;
		case 2://For VVC
		{
			for (int c = 0; c < 14; c++) //------------//10	//6 //c < file array size	or c < qp array size
			{
				cout << "QP=" << qparray[c] << endl;
				for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
				{
					cout << i << endl;
					stringstream frameNum;
					frameNum.clear();
					frameNum.str("");
					frameNum << i;
					//outcsv << qparray[c] + "," + frameNum.str() + ",";

					Mat bayer_to_RGB = imread(dataset + "_Bayer_" + RCT + "_VVC_GRBG/" + qparray[c] + "/" + frameNum.str() + ".bmp",0);
					Mat destination;
					int height = bayer_to_RGB.rows;
					int width = bayer_to_RGB.cols;
					Kiku(height, width, destination, bayer_to_RGB, CFA);
					_dirExists(dataset + "_Reconstruction_" + RCT + "_VVC/");
					_dirExists(dataset + "_Reconstruction_" + RCT + "_VVC/" + qparray[c] + "/");
					if (dataset == "Kodak")
					{
						imwrite(dataset + "_Reconstruction_" + RCT + "_VVC/" + qparray[c] + "/" + frameNum.str() + ".tif", destination);
					}
					else
					{
						imwrite(dataset + "_Reconstruction_" + RCT + "_VVC/" + qparray[c] + "/" + frameNum.str() + ".bmp", destination);//存解馬賽克的全彩
					}
					cout << "finish" << endl;
				}
			}
			
		}
		break;
		}
		//outcsv.close();

	}
	
	
	else if (option == 4)//Caclulate PSNR&SSIM
	{
	cout << "1:JPEG2000   2:VVC" << endl;
	cin >> compressor;
	cout << "Chose RCT method   0:Baseline   1:YDgCoCgLossy   2:YDgCbCr    3:YLMN2017" << endl;
	cin >> pick;
	switch (pick)
	{
	case 1:
	{
		method = "YDgCoCgLossy";
	}
	break;
	case 2:
	{
		method = "YDgCbCr";
	}
	break;
	case 3:
	{
		method = "YLMN2017";
	}
	break;
	case 5:
	{
		method = "YLMN2018";
	}
	break;
	case 0:
	{
		method = "Baseline";
	}
	break;
	}

	double t = 0;
	double ave_PSNR = 0, ave_SSIM = 0, ave_CPSNR = 0, ave_CSSIM = 0;
	double temp = 0;
	String RCT = method;//------------YDgCoCg	//YDgCbCr
	String dataset = Data;//------------IMAX		//Video		//Kodak		//SCI		//classical
	string file[10] = { "1", "5", "10", "15", "20", "25", "30", "35", "40", "45" };//compression factor, 1->non compression forJPEG2000
	string qparray[14] = { "0","4","8", "12","16", "20","24", "28","32", "36","40", "44","48","51" };//for vvc
	ofstream outcsv;
	
	switch (compressor)
	{
	case 1://For JPEG2000
	{
		outcsv.open(dataset + "_bayer_J2K_" + RCT + ".csv", ios::out);//bayer_YDgCbCr		//IMAX_bayer_YDgCbCr
		
		for (int f = 0; f < 10; f++) //------------//10	//6 //c < file array size	or c < qp array size
		{
			outcsv << "CR"<< file[f]<<",";
			
			cout << "CR=" << file[f] << "\n";
			for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
			{
				stringstream frameNum;
				frameNum.clear();
				frameNum.str("");
				frameNum << i;
				
				
				Mat bayerImg=imread(dataset+ "_Bayer_" + RCT + "_J2K_GRBG/" + file[f] + "/" + frameNum.str() + ".bmp", 0);
				Mat bayerOrigin = imread(dataset + "_Bayer_GRBG/" + frameNum.str() + ".bmp", 0);
				Mat bayerColor;//直接bayer解馬賽克的groundtruth
				Mat aftdemosacing;//讀RCT後的bayer解馬賽克

				if (dataset == "Kodak")
				{
					bayerColor = imread(dataset + "_bayerdirect/" + frameNum.str() + ".tif");
				}
				else
				{
					bayerColor = imread(dataset + "_bayerdirect/" + frameNum.str() + ".bmp");
				}

				//bayerColor.convertTo(bayerColor, CV_8UC3);
				if (dataset == "Kodak")
				{
					aftdemosacing = imread(dataset + "_Reconstruction_" + RCT + "_J2K/" + file[f] + "/" + frameNum.str() + ".tif");
				}
				else
				{
					aftdemosacing = imread(dataset + "_Reconstruction_" + RCT + "_J2K/" + file[f] + "/" + frameNum.str() + ".bmp");
				}

				//測量PSNR&SSIM
				
					//For PSNR																							 
				
					result_PSNR = PSNR(bayerOrigin, bayerImg);
					
					ave_PSNR += result_PSNR;
					//cout << result_PSNR << endl;
 

					//For SSIM
					result_SSIM = Bayer_SSIM(bayerImg, bayerOrigin);
					
					ave_SSIM += result_SSIM;
					//cout << result_SSIM<<endl;
				
					//For CPSNR
					result_CPSNR = PSNR(aftdemosacing, bayerColor);
					
					ave_CPSNR += result_CPSNR;
				
					//For CSSIM
					result_CSSIM = SSIM(aftdemosacing, bayerColor);
					
					ave_CSSIM += result_CSSIM;
					//cout << result_CSSIM<<endl;
				
			}

			//測量PSNR&SSIM
				//For PSNR
				ave_PSNR = ave_PSNR / image_page;
				ave_PSNR = round(ave_PSNR * 10000) / 10000;
				

				cout << "Avg PSNR: " << ave_PSNR << endl;
				outcsv << "PSNR"<<","<<std::to_string(ave_PSNR) + ",\n";
				ave_PSNR = 0;
				
				outcsv << "," << "SSIM" << ",";
				
				//For SSIM
				ave_SSIM = ave_SSIM / image_page;
				cout << "Avg SSIM: " << ave_SSIM << endl;
				outcsv << std::to_string(ave_SSIM) + ",\n";
				ave_SSIM = 0;
			 
				outcsv <<",\n";
				outcsv << "," << "CPSNR" << ",";
				
				//For CPSNR
				ave_CPSNR = ave_CPSNR / image_page;
				ave_CPSNR = round(ave_CPSNR * 10000) / 10000;
				cout << "Avg CPSNR: " << ave_CPSNR << endl;
				outcsv << std::to_string(ave_CPSNR) + ",\n";
				ave_CPSNR = 0;
			
				outcsv << "," << "CSSIM" << ",";
				//For CSSIM
				ave_CSSIM = ave_CSSIM / image_page;

				cout << "Avg CSSIM: " << ave_CSSIM << endl;
				outcsv << std::to_string(ave_CSSIM) + ",\n" + ",\n" + ",\n" + ",\n";
				ave_CSSIM = 0;
			
		}
	}
	break;
	case 2://For VVC
	{
		
		outcsv.open(dataset + "_bayer_VVC_" + RCT + ".csv", ios::out);//bayer_YDgCbCr		//IMAX_bayer_YDgCbCr
		
		for (int c = 0; c < 14; c++) //------------//10	//6 //c < file array size	or c < qp array size
		{
			outcsv << "QP" << qparray[c] << ",";
			cout << "QP=" << qparray[c] << "\n";
			for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
			{
				stringstream frameNum;
				frameNum.clear();
				frameNum.str("");
				frameNum << i;
				
				
				Mat bayerImg=imread(dataset + "_Bayer_" + RCT + "_VVC_GRBG/" + qparray[c] + "/" + frameNum.str() + ".bmp",0);
				Mat bayerOrigin = imread(dataset + "_Bayer_GRBG/" + frameNum.str() + ".bmp",0);
				Mat aftdemosacing;//讀RCT後的bayer解馬賽克
				Mat bayerColor;//直接bayer解馬賽克的groundtruth

				if (dataset == "Kodak")
				{
					bayerColor = imread(dataset + "_bayerdirect/" + frameNum.str() + ".tif");
				}
				else
				{
					bayerColor = imread(dataset + "_bayerdirect/" + frameNum.str() + ".bmp");
				}

				//bayerColor.convertTo(bayerColor, CV_8UC3);


				if (dataset == "Kodak")
				{
					aftdemosacing = imread(dataset + "_Reconstruction_" + RCT + "_VVC/" + qparray[c] + "/" + frameNum.str() + ".tif");
				}
				else
				{
					aftdemosacing = imread(dataset + "_Reconstruction_" + RCT + "_VVC/" + qparray[c] + "/" + frameNum.str() + ".bmp");
				}
				
				
				
				//測量PSNR&SSIM
				
					result_PSNR = PSNR(bayerImg, bayerOrigin);
					
					ave_PSNR += result_PSNR;

				
					//For SSIM
					result_SSIM = Bayer_SSIM(bayerImg, bayerOrigin);
					
					ave_SSIM += result_SSIM;
					//cout << result_SSIM<<endl;
				
					//For CPSNR
					result_CPSNR = PSNR(aftdemosacing, bayerColor);
					
					ave_CPSNR += result_CPSNR;
				
					//For CSSIM
					result_CSSIM = SSIM(aftdemosacing, bayerColor);
	
					ave_CSSIM += result_CSSIM;
					//cout << result_CSSIM<<endl;
				
			}

			//測量PSNR&SSIM
			
				//For PSNR
				ave_PSNR = ave_PSNR / image_page;
				ave_PSNR = round(ave_PSNR * 10000) / 10000;
				cout << "Avg PSNR: " << ave_PSNR << endl;
				outcsv << "PSNR" << "," << std::to_string(ave_PSNR) + ",\n";
				ave_PSNR = 0;
			
				//For SSIM
				outcsv << "," << "SSIM" << ",";
				ave_SSIM = ave_SSIM / image_page;
				cout << "Avg SSIM: " << ave_SSIM << endl;
				outcsv << std::to_string(ave_SSIM) + ",\n";
				ave_SSIM = 0;
			
				outcsv << ",\n";
				outcsv << "," << "CPSNR" << ",";

				//For PSNR
				ave_CPSNR = ave_CPSNR / image_page;
				ave_CPSNR = round(ave_CPSNR * 10000) / 10000;
				cout << "Avg CPSNR: " << ave_CPSNR << endl;
				outcsv << std::to_string(ave_CPSNR) + ",\n";
				ave_CPSNR = 0;
			
				outcsv << "," << "CSSIM" << ",";
				//For CSSIM

				ave_CSSIM = ave_CSSIM / image_page;
				cout << "Avg CSSIM: " << ave_CSSIM << endl;
				outcsv << std::to_string(ave_CSSIM) + ",\n" + ",\n" + ",\n" + ",\n";
				ave_CSSIM = 0;
			


				


		}
	}
	}

	outcsv.close();
	cout << "Avg Time: " << t / 240.0 << endl;
 }
	else if (option == 5)//caculate jp2 bitrate
	{
	cout << "Chose RCT method  0:Baseline   1:YDgCoCgLossy   2:YDgCbCr    3:YLMN2017"<< endl;
	cin >> pick;
	switch (pick)
	{
	
	case 1:
	{
		method = "YDgCoCgLossy";
	}
	break;
	case 2:
	{
		method = "YDgCbCr";
	}
	break;
	case 3:
	{
		method = "YLMN2017";
	}
	break;
	case 0:
	{
		method = "baseline";
	}
	break;
	}

		String RCT=method;
		String dataset[5]={"IMAX","Video","Kodak","SCI","classical"};	//------------IMAX		//Video		//Kodak		//SCI		//classical
		stringstream frameNum;
		long double bitrate = 0, sum = 0;
		long double ave_PSNR, result_PSNR = 0;
		string file[10] = { "1", "5", "10", "15", "20", "25", "30", "35", "40", "45" };//compression factor, 1->non compression forJPEG2000
		ofstream outcsv;
		outcsv.open( RCT+"_bitrate" + ".csv", ios::out);//bayer_YDgCbCr		//IMAX_bayer_YDgCbCr
		
		for (int j = 0; j < 5; j++)		
		{
			outcsv << RCT << ",Quantify,Bitrate,PSNR\n";
			
			for (int f = 0; f < 10; f++)
			{
				
				if (dataset[j] == "IMAX")
				{
					image_page = 18;
				}
				else if (dataset[j] == "Video")
				{
					image_page = 200;
				}
				else if (dataset[j] == "Kodak")
				{
					image_page = 24;
				}
				else if (dataset[j] == "SCI")
				{
					image_page = 20;
				}
				else if (dataset[j] == "classical")
				{
					image_page = 8;
				}

				for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
				{
					frameNum.clear();
					frameNum.str("");
					frameNum << i;
					Mat Imgjp2 = imread("E:/Paperwork/J2K/4_" + RCT + "_" + dataset[j] + "_J2K/" + file[f] + "/" + frameNum.str() + ".jp2", 0);//jp2位置
					long double height = Imgjp2.rows;
					long double  width = Imgjp2.cols;
					long double sizejp2 = height * width;//jp2pixil數量			
					stringstream readjp2;
					fstream fin;
					readjp2 << "E:/Paperwork/J2K/4_" + RCT + "_" + dataset[j] + "_J2K/" + file[f] + "/" + frameNum.str() + ".jp2";//位置
					// 1. 開檔
					fin.open(readjp2.str(), ifstream::in | ifstream::binary);
					// 2. Seek 檔案頭到尾
					fin.seekg(0, ios::end);
						// 3. tell 位置 
						long double length = fin.tellg();//jp2檔案大小
						// 4. 關檔
						fin.close();
						bitrate = (length*8)/ sizejp2;
						sum = sum + bitrate;
						Mat bayerImg = imread(dataset[j] + "_Bayer_" + RCT + "_J2K_GRBG/" + file[f] + "/" + frameNum.str() + ".bmp", 0);
						Mat bayerOrigin = imread(dataset[j] + "_Bayer_GRBG/" + frameNum.str() + ".bmp", 0);
						result_PSNR = PSNR(bayerImg, bayerOrigin);
						ave_PSNR += result_PSNR;
				}

					sum = sum / image_page;
					ave_PSNR = ave_PSNR / image_page;
					cout << RCT << " " << dataset[j] << " CR" << file[f] << " bitrate=" << sum  << " Avg PSNR=" << ave_PSNR << endl;
					outcsv  << dataset[j] << ",CR" << file[f] << "," << sum << "," << std::to_string(ave_PSNR) + ",\n";;
					ave_PSNR = 0;
					sum = 0;
			}
			cout<<endl;
			outcsv << "\n";
		}
	}
	
	else if (option == 6)//caculate jp2 bitrate
	{

	String RCT[5] = { "Baseline","YDgCoCgLossy","YDgCbCr","YLMN2017","ZhangOnce"};
	String dataset[5] = { "IMAX","Video","Kodak","SCI","classical" };	//------------IMAX		//Video		//Kodak		//SCI		//classical
	stringstream frameNum;
	long double bitrate = 0, sum = 0;
	long double ave_PSNR, result_PSNR = 0;
	string file[9] = { "5", "10", "15", "20", "25", "30", "35", "40", "45" };//compression factor, 1->non compression forJPEG2000
	ofstream outcsv;
	outcsv.open("NOCR1Curvebitrate.csv", ios::out);//bayer_YDgCbCr		//IMAX_bayer_YDgCbCr
	outcsv << "YB,Quantify,RCT,Bitrate,PSNR\n";
	for (int j = 0; j < 5; j++)
	{
		for (int r = 0; r < 5; r++)
		{
			for (int f = 0; f < 9; f++)

			{

				if (dataset[j] == "IMAX")
				{
					image_page = 18;
				}
				else if (dataset[j] == "Video")
				{
					image_page = 200;
				}
				else if (dataset[j] == "Kodak")
				{
					image_page = 24;
				}
				else if (dataset[j] == "SCI")
				{
					image_page = 18;
				}
				else if (dataset[j] == "classical")
				{
					image_page = 8;
				}

				for (int i = 1; i <= image_page; ++i)	//------------18		//200		//24		//20		//8
				{
					frameNum.clear();
					frameNum.str("");
					frameNum << i;
					Mat Imgjp2 = imread("E:/Paperwork/J2K/4_" + RCT[r] + "_" + dataset[j] + "_J2K/" + file[f] + "/" + frameNum.str() + ".jp2", 0);//jp2位置
					long double height = Imgjp2.rows;
					long double  width = Imgjp2.cols;
					long double sizejp2 = height * width;//jp2pixil數量			
					stringstream readjp2;
					fstream fin;
					readjp2 << "E:/Paperwork/J2K/4_" + RCT[r] + "_" + dataset[j] + "_J2K/" + file[f] + "/" + frameNum.str() + ".jp2";//位置
					// 1. 開檔
					fin.open(readjp2.str(), ifstream::in | ifstream::binary);
					// 2. Seek 檔案頭到尾
					fin.seekg(0, ios::end);
					// 3. tell 位置 
					long double length = fin.tellg();//jp2檔案大小
					// 4. 關檔
					fin.close();
					bitrate = (length * 8) / sizejp2;
					sum = sum + bitrate;
					Mat bayerImg = imread(dataset[j] + "_Bayer_" + RCT[r] + "_J2K_GRBG/" + file[f] + "/" + frameNum.str() + ".bmp", 0);
					Mat bayerOrigin = imread(dataset[j] + "_Bayer_GRBG/" + frameNum.str() + ".bmp", 0);
					result_PSNR = PSNR(bayerImg, bayerOrigin);
					ave_PSNR += result_PSNR;
				}
				sum = sum / image_page;
				ave_PSNR = ave_PSNR / image_page;
				ave_PSNR = round(ave_PSNR * 10000) / 10000;
				cout << dataset[j] << " " << RCT[r] << " CR" << file[f] << " bitrate=" << sum << " Avg PSNR=" << ave_PSNR << endl;
				outcsv << dataset[j] << ",CR" << file[f] << "," << RCT[r] << "," << sum << "," << std::to_string(ave_PSNR) + ",\n";
				ave_PSNR = 0;
				sum = 0;
			}
		}
		cout << endl;
	}
	}


	system("pause");

	
}