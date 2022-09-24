#include "image_Process.h"

inline double Clip(double val, int min, int max)
{
	double dst = val;
	if (dst < min) dst = min;
	if (dst > max) dst = max;
	return dst;
}

void _dirExists(string path)
{    /**
	 * Create folders
	 *
	 *  This function creates folders in the path
	 *
	 * @param[in] path:
	 *
	 * To do: If path has "\\", this function won't work
	 */

	for (int i = 0; i < strlen(path.c_str()); i++) //c_str回傳const char*指標，指向以空字元終止的陣列。
	{
		if (path[i] != '/') continue;
		path[i] = 0;
		_mkdir(path.c_str());
		path[i] = '/';
	}
}

void write_YDgCoCg(int height, int width, fstream& yuv_writer, const Mat& YDgCoCgImg)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			yuv_writer.write((char*)YDgCoCgImg.data + y * YDgCoCgImg.step1(0) + x * YDgCoCgImg.step1(1) + 0, 1);
	}
}

void Write_YUV420(int height, int width, fstream& yuv_writer, const Mat& Y_plane, const Mat& sub_U_plane, const Mat& sub_V_plane)
{
	int y, x;
	// Y
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
			yuv_writer.write((char*)Y_plane.data + y * Y_plane.step1(0) + x * Y_plane.step1(1) + 0, 1);
	}

	// U
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
			yuv_writer.write((char*)sub_U_plane.data + y * sub_U_plane.step1(0) + x * sub_U_plane.step1(1) + 0, 1);
	}

	// V
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
			yuv_writer.write((char*)sub_V_plane.data + y * sub_V_plane.step1(0) + x * sub_V_plane.step1(1) + 0, 1);
	}
	//cout << "end" << endl;
}



void ReadCFA(istream& in, Mat& CFA)
{
	int cfa_h, cfa_w, color;

	in >> cfa_h >> cfa_w;

	if (!in || cfa_h <= 0 || cfa_w <= 0)
	{
		cerr << "Cannot process the cfa file!" << endl;
		exit(-1);
	}

	CFA.create(cfa_h, cfa_w, CV_8U);

	for (int h = 0; h < cfa_h; ++h)
		for (int w = 0; w < cfa_w; ++w)
		{
			if (in >> color)
			{
				uchar& val = CFA.at<uchar>(h, w);
				val = (color % 10) ? CFA_R : CFA_None;
				val |= ((color / 10) % 10) ? CFA_G : CFA_None;
				val |= ((color / 100) % 10) ? CFA_B : CFA_None;
				val |= (color / 1000) ? CFA_W : CFA_None;
			}
			else
			{
				cerr << "Cannot read the cfa data!" << endl;
				cerr << "h,w=" << h << "," << w << endl;
				exit(-1);
			}
		}
}