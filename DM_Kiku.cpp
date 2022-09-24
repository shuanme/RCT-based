#include "image_Process.h"
inline int Round(double val)
{
	return (int)(val + 0.5);
}


inline double Clip(double val, int min, int max)
{
	double dst = val;
	if (dst < min) dst = min;
	if (dst > max) dst = max;
	return dst;
}

int GetColor(const Mat& CFA, int y, int x)
{

	if (!CFA.data || CFA.rows <= 0 || CFA.cols <= 0)
	{
		cerr << "getcolor cannot work" << endl;
		cerr << "y,x=" << y << "," << x << endl;
		exit(-1);
	}

	int x_in_cfa, y_in_cfa;

	if (CFA.cols == 1)
		x_in_cfa = 0;
	else
	{
		if (x < 0)
			x = -x;
		x_in_cfa = x % CFA.cols;
	}

	if (CFA.rows == 1)
		y_in_cfa = 0;
	else
	{
		if (y < 0)
			y = -y;
		y_in_cfa = y % CFA.rows;
	}

	return CFA.at<uchar>(y_in_cfa, x_in_cfa);

}
void impsnr(Mat& X, Mat& Y, int peak, int b, double *psnr) {
	/// 注意：X和Y 必須為有號數格式(Ex: CV_64F)
	if (b > 0) {
		X = X(Rect(b, b, X.rows - 2 * b, X.cols - 2 * b));
		Y = Y(Rect(b, b, Y.rows - 2 * b, Y.cols - 2 * b));
	}

	Mat dif = X - Y;
	dif = dif.mul(dif);
	Scalar Sum = cv::sum(dif);

	double d, mse;
	for (int k = 0; k < 3; k++) {
		d = Sum[k];
		mse = d / ((X.rows * X.cols) + 1e-32);
		psnr[k] = 10 * log10(peak * peak / mse);
	}
}


double imcpsnr(Mat& X, Mat& Y, int peak, int b) {
	/// 注意：X和Y 必須為有號數格式(Ex: CV_64F)
	double cpsnr = 0.0;
	if (b > 0) {
		X = X(Rect(b, b, X.rows - 2 * b, X.cols - 2 * b));
		Y = Y(Rect(b, b, Y.rows - 2 * b, Y.cols - 2 * b));
	}

	Mat dif = X - Y;
	dif = dif.mul(dif);
	Scalar Sum = cv::sum(dif);

	double d = Sum[0] + Sum[1] + Sum[2];
	double mse = d / ((X.rows * X.cols * 3) + 1e-32);
	cpsnr = 10 * log10(peak * peak / mse);

	return cpsnr;
}


Mat boxfilter(Mat& imSrc, int h, int v) {
	Mat imDst(imSrc.rows, imSrc.cols, CV_64FC1, Scalar::all(0));
	Mat imCum(imSrc.rows, imSrc.cols, CV_64FC1, Scalar::all(0));

	if (v != 0) {
		/// cumulative sum over Y axis
		imCum = Scalar::all(0);
		for (int i = 0; i < imSrc.rows; i++)
			for (int j = 0; j < imSrc.cols; j++) {
				if (i == 0)
					imCum.at<double>(i, j) = imSrc.at<double>(i, j);
				else
					imCum.at<double>(i, j) = imCum.at<double>(i - 1, j) + imSrc.at<double>(i, j);
			}

		/// difference over Y axis
		for (int i = 0; i < v + 1; i++)
			for (int j = 0; j < imDst.cols; j++) {
				int k = i + v;
				imDst.at<double>(i, j) = imCum.at<double>(k, j);
			}
		for (int i = v + 1; i < imDst.rows - v; i++)
			for (int j = 0; j < imDst.cols; j++) {
				int k = i + v;
				int l = i - v - 1;
				imDst.at<double>(i, j) = imCum.at<double>(k, j) - imCum.at<double>(l, j);
			}
		for (int i = imDst.rows - v; i < imDst.rows; i++)
			for (int j = 0; j < imDst.cols; j++) {
				int k = i - v - 1;
				imDst.at<double>(i, j) = imCum.at<double>(imCum.rows - 1, j) - imCum.at<double>(k, j);
			}
	}

	if (h != 0) {
		if (v != 0) {
			/// cumulative sum over X axis
			imCum = Scalar::all(0);
			for (int i = 0; i < imDst.rows; i++)
				for (int j = 0; j < imDst.cols; j++) {
					if (j == 0)
						imCum.at<double>(i, j) = imDst.at<double>(i, j);
					else
						imCum.at<double>(i, j) = imCum.at<double>(i, j - 1) + imDst.at<double>(i, j);
				}
		}
		else {
			/// cumulative sum over X axis
			imCum = Scalar::all(0);
			for (int i = 0; i < imSrc.rows; i++)
				for (int j = 0; j < imSrc.cols; j++) {
					if (j == 0)
						imCum.at<double>(i, j) = imSrc.at<double>(i, j);
					else
						imCum.at<double>(i, j) = imCum.at<double>(i, j - 1) + imSrc.at<double>(i, j);
				}
		}

		/// difference over X axis
		for (int i = 0; i < imDst.rows; i++)
			for (int j = 0; j < h + 1; j++) {
				int k = j + h;
				imDst.at<double>(i, j) = imCum.at<double>(i, k);
			}
		for (int i = 0; i < imDst.rows; i++)
			for (int j = h + 1; j < imDst.cols - h; j++) {
				int k = j + h;
				int l = j - h - 1;
				imDst.at<double>(i, j) = imCum.at<double>(i, k) - imCum.at<double>(i, l);
			}
		for (int i = 0; i < imDst.rows; i++)
			for (int j = imDst.cols - h; j < imDst.cols; j++) {
				int k = j - h - 1;
				imDst.at<double>(i, j) = imCum.at<double>(i, imCum.cols - 1) - imCum.at<double>(i, k);
			}
	}

	/// output result
	return imDst;
}


Mat guidedfilter(Mat& I, Mat& p, Mat& M, int h, int v, int eps) {
	/// threshold parameter
	double th = 0.00001 * 255 * 255;

	/// The number of the sammpled pixels in each local patch
	Mat N = boxfilter(M, h, v); //每個local patch有幾個被採樣的點
	for (int i = 0; i < N.rows; i++)
		for (int j = 0; j < N.cols; j++)
			if (N.at<double>(i, j) == 0)
				N.at<double>(i, j) = 1;

	/// The size of each local patch; N=(2h+1)*(2v+1) except for boundary pixels.
	Mat one = Mat::ones(I.rows, I.cols, CV_64FC1);
	Mat N2 = boxfilter(one, h, v); //每個local patch的大小

	Mat IxM = I.mul(M);
	Mat mean_I = boxfilter(IxM, h, v); //每個local patch中 被採樣Guide點的平均
	divide(mean_I, N, mean_I);

	Mat mean_p = boxfilter(p, h, v); //每個local patch中 被採樣BeGuide點的平均
	divide(mean_p, N, mean_p);

	Mat Ixp = I.mul(p);
	Mat mean_Ip = boxfilter(Ixp, h, v); //每個local patch中 被採樣Guide x BeGuide點的平均
	divide(mean_Ip, N, mean_Ip);

	/// The covariance of(I, p) in each local patch.
	Mat cov_Ip = mean_Ip - mean_I.mul(mean_p); //共變異數 = Guide x BeGuide點的平均 - 每點的Guide x BeGuide

	Mat IxIxM = I.mul(I.mul(M));
	Mat mean_II = boxfilter(IxIxM, h, v); //每個local patch中 被採樣Guide的平方 的平均
	divide(mean_II, N, mean_II);

	Mat var_I = mean_II - mean_I.mul(mean_I); //變異數 = 被採樣Guide的平方 的平均 - 每點被採樣Guide點平均的平方
	for (int i = 0; i < var_I.rows; i++)
		for (int j = 0; j < var_I.cols; j++)
			if (var_I.at<double>(i, j) < th)
				var_I.at<double>(i, j) = th;

	/// linear coefficients
	Mat a, b;
	divide(cov_Ip, (var_I + eps), a);
	b = mean_p - a.mul(mean_I);

	Mat mean_a = boxfilter(a, h, v); //每個local patch中 平均的a
	divide(mean_a, N2, mean_a);

	Mat mean_b = boxfilter(b, h, v); //每個local patch中 平均的b
	divide(mean_b, N2, mean_b);

	/// output
	Mat q = mean_a.mul(I) + mean_b;

	return q;
}

Mat green_interpolation(Mat& mosaicImage, Mat& mask,const Mat& CFA, double sigma) {
	/// mask
	Mat imask(mask.rows, mask.cols, CV_64FC3, Scalar::all(0)); //需要補顏色的地方
	for (int i = 0; i < mask.rows; i++)
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<Vec3d>(i, j)[0] == 0)
				imask.at<Vec3d>(i, j)[0] = 1;
			if (mask.at<Vec3d>(i, j)[1] == 0)
				imask.at<Vec3d>(i, j)[1] = 1;
			if (mask.at<Vec3d>(i, j)[2] == 0)
				imask.at<Vec3d>(i, j)[2] = 1;
		}

	vector<Mat> imask_1channel;
	split(imask, imask_1channel);

	/// raw CFA data
	Mat rawq(mosaicImage.rows, mosaicImage.cols, CV_64FC1, Scalar::all(0)); //單通道馬賽克圖
	for (int i = 0; i < mosaicImage.rows; i++)
		for (int j = 0; j < mosaicImage.cols; j++) {
			if (GetColor(CFA, i, j) == CFA_B)
				rawq.at<double>(i, j) = mosaicImage.at<Vec3d>(i, j)[0];
			else if (GetColor(CFA, i, j) == CFA_G)
				rawq.at<double>(i, j) = mosaicImage.at<Vec3d>(i, j)[1];
			else if (GetColor(CFA, i, j) == CFA_R)
				rawq.at<double>(i, j) = mosaicImage.at<Vec3d>(i, j)[2];
		}

	////// Calculate Horizontal and Vertical Color Differences //////
	///mask
	Mat maskGr(rawq.rows, rawq.cols, CV_64FC1, Scalar::all(0)); //水平GR的"G"位置，也可以是垂直GB的"G"位置
	Mat maskGb(rawq.rows, rawq.cols, CV_64FC1, Scalar::all(0)); //水平GB的"G"位置，也可以是垂直GR的"G"位置
	
	if ((GetColor(CFA, 0, 0) == CFA_G && GetColor(CFA, 0, 1) == CFA_B) || (GetColor(CFA, 0, 0) == CFA_B && GetColor(CFA, 0, 1) == CFA_G)) {
		for (int i = 0; i < rawq.rows; i++)
			for (int j = 0; j < rawq.cols; j++) {
				if (i % 2 == 0 && GetColor(CFA, i, j) == CFA_G)
					maskGb.at<double>(i, j) = 1;
				if (i % 2 == 1 && GetColor(CFA, i, j) == CFA_G)
					maskGr.at<double>(i, j) = 1;
			}
	}
	else if ((GetColor(CFA, 0, 0) == CFA_G && GetColor(CFA, 0, 1) == CFA_R) || (GetColor(CFA, 0, 0) == CFA_R && GetColor(CFA, 0, 1) == CFA_G)) {

		for (int i = 0; i < rawq.rows; i++)
			for (int j = 0; j < rawq.cols; j++) {
				if (i % 2 == 0 && GetColor(CFA, i, j) == CFA_G)
					maskGr.at<double>(i, j) = 1;
				if (i % 2 == 1 && GetColor(CFA, i, j) == CFA_G)
					maskGb.at<double>(i, j) = 1;
			}
	}
	else {}

	/// guide image
	double KhData[] = { 0.5, 0, 0.5 };
	Mat Kh = Mat(1, 3, CV_64FC1, KhData).clone();
	Mat Kv = Kh.t();
	Mat rawh, rawv;  //用Bilinear內插水平及垂直方向的圖
	filter2D(rawq, rawh, rawq.depth(), Kh, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(rawq, rawv, rawq.depth(), Kv, Point(-1, -1), 0, BORDER_REPLICATE);

	vector<Mat> mosaic_1channel, mask_1channel;
	split(mosaicImage, mosaic_1channel);
	split(mask, mask_1channel);
	Mat Guidegh = mosaic_1channel[1] + rawh.mul(mask_1channel[2]) + rawh.mul(mask_1channel[0]); //Groundtruth G + 水平內插出來的G
	Mat Guiderh = mosaic_1channel[2] + rawh.mul(maskGr); //Groundtruth R + 水平內插出來的R
	Mat Guidebh = mosaic_1channel[0] + rawh.mul(maskGb); //Groundtruth B + 水平內插出來的B
	Mat Guidegv = mosaic_1channel[1] + rawv.mul(mask_1channel[2]) + rawv.mul(mask_1channel[0]); //Groundtruth G + 垂直內插出來的G
	Mat Guiderv = mosaic_1channel[2] + rawv.mul(maskGb); //Groundtruth R + 垂直內插出來的R
	Mat Guidebv = mosaic_1channel[0] + rawv.mul(maskGr); //Groundtruth B + 垂直內插出來的B

														 /// tentative image
	int h = 5;
	int v = 0;
	int eps = 0;
	Mat m1GxmGr = mosaic_1channel[1].mul(maskGr); //水平R的Groundtruth G, 或垂直B的Groundtruth G
	Mat m1GxmGb = mosaic_1channel[1].mul(maskGb); //水平B的Groundtruth G, 或垂直R的Groundtruth G
	Mat tentativeRh = guidedfilter(Guidegh, mosaic_1channel[2], mask_1channel[2], h, v, eps); //用水平內插的G 預估的水平R
	Mat tentativeGrh = guidedfilter(Guiderh, m1GxmGr, maskGr, h, v, eps); //用水平內插的R 預估的水平GR的G
	Mat tentativeGbh = guidedfilter(Guidebh, m1GxmGb, maskGb, h, v, eps); //用水平內插的B 預估的水平GB的G
	Mat tentativeBh = guidedfilter(Guidegh, mosaic_1channel[0], mask_1channel[0], h, v, eps); //用水平內插的G 預估的水平B

	Mat tentativeRv = guidedfilter(Guidegv, mosaic_1channel[2], mask_1channel[2], v, h, eps); //用垂直內插的G 預估的垂直R
	Mat tentativeGrv = guidedfilter(Guiderv, m1GxmGb, maskGb, v, h, eps); //用垂直內插的R 預估的垂直GR的G
	Mat tentativeGbv = guidedfilter(Guidebv, m1GxmGr, maskGr, v, h, eps); //用垂直內插的B 預估的垂直GB的G
	Mat tentativeBv = guidedfilter(Guidegv, mosaic_1channel[0], mask_1channel[0], v, h, eps); //用垂直內插的G 預估的垂直B

																							  /// residual (Groundtruth - 預估 = 殘差)
	Mat residualGrh = (mosaic_1channel[1] - tentativeGrh).mul(maskGr);
	Mat residualGbh = (mosaic_1channel[1] - tentativeGbh).mul(maskGb);
	Mat residualRh = (mosaic_1channel[2] - tentativeRh).mul(mask_1channel[2]);
	Mat residualBh = (mosaic_1channel[0] - tentativeBh).mul(mask_1channel[0]);

	Mat residualGrv = (mosaic_1channel[1] - tentativeGrv).mul(maskGb);
	Mat residualGbv = (mosaic_1channel[1] - tentativeGbv).mul(maskGr);
	Mat residualRv = (mosaic_1channel[2] - tentativeRv).mul(mask_1channel[2]);
	Mat residualBv = (mosaic_1channel[0] - tentativeBv).mul(mask_1channel[0]);

	/// residual interpolation (內插殘差平面)
	filter2D(residualGrh, residualGrh, residualGrh.depth(), Kh, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(residualGbh, residualGbh, residualGbh.depth(), Kh, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(residualRh, residualRh, residualRh.depth(), Kh, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(residualBh, residualBh, residualBh.depth(), Kh, Point(-1, -1), 0, BORDER_REPLICATE);

	filter2D(residualGrv, residualGrv, residualGrv.depth(), Kv, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(residualGbv, residualGbv, residualGbv.depth(), Kv, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(residualRv, residualRv, residualRv.depth(), Kv, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(residualBv, residualBv, residualBv.depth(), Kv, Point(-1, -1), 0, BORDER_REPLICATE);

	/// add tentative image (預估 + 殘差)
	Mat Grh = (tentativeGrh + residualGrh).mul(mask_1channel[2]);
	Mat Gbh = (tentativeGbh + residualGbh).mul(mask_1channel[0]);
	Mat Rh = (tentativeRh + residualRh).mul(maskGr);
	Mat Bh = (tentativeBh + residualBh).mul(maskGb);
	Mat Grv = (tentativeGrv + residualGrv).mul(mask_1channel[2]);
	Mat Gbv = (tentativeGbv + residualGbv).mul(mask_1channel[0]);
	Mat Rv = (tentativeRv + residualRv).mul(maskGb);
	Mat Bv = (tentativeBv + residualBv).mul(maskGr);


	/// vertical and horizontal color difference
	Mat difh = mosaic_1channel[1] + Grh + Gbh - mosaic_1channel[2] - mosaic_1channel[0] - Rh - Bh; //G-R-B
	Mat difv = mosaic_1channel[1] + Grv + Gbv - mosaic_1channel[2] - mosaic_1channel[0] - Rv - Bv;

	////// Combine Vertical and Horizontal Color Differences //////
	/// color difference gradient
	double Kh2Data[] = { 1.0, 0.0, -1.0 };
	Mat Kh2 = Mat(1, 3, CV_64FC1, Kh2Data).clone();
	Mat Kv2 = Kh2.t();
	Mat difh2, difv2;
	filter2D(difh, difh2, difh.depth(), Kh2, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(difv, difv2, difv.depth(), Kv2, Point(-1, -1), 0, BORDER_REPLICATE);
	difh2 = abs(difh2);
	difv2 = abs(difv2);

	/// directional weight
	Mat K = Mat::ones(5, 5, CV_64FC1);
	Mat wh, wv;
	filter2D(difh2, wh, difh2.depth(), K, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(difv2, wv, difv2.depth(), K, Point(-1, -1), 0, BORDER_REPLICATE);

	double KwData[] = { 1.0, 0.0, 0.0, 0.0, 0.0 };
	double KeData[] = { 0.0, 0.0, 0.0, 0.0, 1.0 };
	Mat Kw = Mat(1, 5, CV_64FC1, KwData).clone();
	Mat Ke = Mat(1, 5, CV_64FC1, KeData).clone();
	Mat Ks = Ke.t();
	Mat Kn = Kw.t();
	Mat Ww, We, Wn, Ws;
	filter2D(wh, Ww, wh.depth(), Kw, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(wh, We, wh.depth(), Ke, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(wv, Wn, wv.depth(), Kn, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(wv, Ws, wv.depth(), Ks, Point(-1, -1), 0, BORDER_REPLICATE);
	for (int i = 0; i < Ww.rows; i++)
		for (int j = 0; j < Ww.cols; j++) {
			Ww.at<double>(i, j) = 1 / (Ww.at<double>(i, j) * Ww.at<double>(i, j) + (1e-32));
			We.at<double>(i, j) = 1 / (We.at<double>(i, j) * We.at<double>(i, j) + (1e-32));
			Ws.at<double>(i, j) = 1 / (Ws.at<double>(i, j) * Ws.at<double>(i, j) + (1e-32));
			Wn.at<double>(i, j) = 1 / (Wn.at<double>(i, j) * Wn.at<double>(i, j) + (1e-32));
		}

	/// combine directional color differences
	double GaussianData[] = { 1.338306246147418e-04, 0.004431861620031, 0.053991127420704, 0.241971445656601, 0.398943469356098, 0.241971445656601, 0.053991127420704, 0.004431861620031, 1.338306246147418e-04 };
	double Ke2Data[] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	double Kw2Data[] = { 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
	Mat Gaussain = Mat(1, 9, CV_64FC1, GaussianData).clone();
	Mat Ke2 = Mat(1, 9, CV_64FC1, Ke2Data).clone();
	Mat Kw2 = Mat(1, 9, CV_64FC1, Kw2Data).clone();
	Ke2 = Ke2.mul(Gaussain);
	Kw2 = Kw2.mul(Gaussain);
	Ke2 = Ke2 / sum(Ke2)[0];
	Kw2 = Kw2 / sum(Kw2)[0];

	Mat Ks2 = Ke2.t();
	Mat Kn2 = Kw2.t();
	Mat difn, difs, difw, dife;
	filter2D(difv, difn, difv.depth(), Kn2, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(difv, difs, difv.depth(), Ks2, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(difh, difw, difh.depth(), Kw2, Point(-1, -1), 0, BORDER_REPLICATE);
	filter2D(difh, dife, difh.depth(), Ke2, Point(-1, -1), 0, BORDER_REPLICATE);

	Mat Wt = Ww + We + Wn + Ws;
	Mat dif = Wn.mul(difn) + Ws.mul(difs) + Ww.mul(difw) + We.mul(dife);
	divide(dif, Wt, dif);

	////// Calculate Green by adding bayer raw data //////
	Mat green = dif + rawq;
	green = green.mul(imask_1channel[1]) + rawq.mul(mask_1channel[1]);

	/// clip to 0-255
	for (int i = 0; i < green.rows; i++)
		for (int j = 0; j < green.cols; j++)
			green.at<double>(i, j) = Clip(green.at<double>(i, j), 0, 255);

	/// output
	return green;
}


Mat red_interpolation(Mat& green, Mat& mosaicImage, Mat& mask, int h, int v, int eps) {
	vector<Mat> mosaic_1channel, mask_1channel;
	split(mosaicImage, mosaic_1channel);
	split(mask, mask_1channel);

	/// R interpolation
	Mat tentitiveR = guidedfilter(green, mosaic_1channel[2], mask_1channel[2], h, v, eps);
	Mat residualR = (mosaic_1channel[2] - tentitiveR).mul(mask_1channel[2]);

	double HData[] = { 0.25, 0.5, 0.25, 0.5, 1.0, 0.5, 0.25, 0.5, 0.25 };
	Mat H(3, 3, CV_64FC1);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			H.at<double>(i, j) = HData[i * 3 + j];

	filter2D(residualR, residualR, residualR.depth(), H, Point(-1, -1), 0, BORDER_REPLICATE);
	Mat red = residualR + tentitiveR;

	/// output
	return red;
}


Mat blue_interpolation(Mat& green, Mat& mosaicImage, Mat& mask, int h, int v, int eps) {
	vector<Mat> mosaic_1channel, mask_1channel;
	split(mosaicImage, mosaic_1channel);
	split(mask, mask_1channel);

	/// R interpolation
	Mat tentitiveB = guidedfilter(green, mosaic_1channel[0], mask_1channel[0], h, v, eps);
	Mat residualB = (mosaic_1channel[0] - tentitiveB).mul(mask_1channel[0]);

	double HData[] = { 0.25, 0.5, 0.25, 0.5, 1.0, 0.5, 0.25, 0.5, 0.25 };
	Mat H(3, 3, CV_64FC1);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			H.at<double>(i, j) = HData[i * 3 + j];

	filter2D(residualB, residualB, residualB.depth(), H, Point(-1, -1), 0, BORDER_REPLICATE);
	Mat blue = residualB + tentitiveB;

	/// output
	return blue;
}


Mat demosaic(Mat& InputImage, const Mat& CFA, double sigma) {
	/// mosaic and mask (mosaic是將全彩圖抽成馬賽克的圖，mask是指整張的CFA)
	Mat mosaicImage = Mat(InputImage.rows, InputImage.cols, CV_64FC3, Scalar::all(0));
	Mat mask = Mat(InputImage.rows, InputImage.cols, CV_64FC3, Scalar::all(0));
	for (int i = 0; i < InputImage.rows; i++)
		for (int j = 0; j < InputImage.cols; j++) {
			if (GetColor(CFA, i, j) == CFA_B) {
				mosaicImage.at<Vec3d>(i, j)[0] = InputImage.at<double>(i, j);
				mask.at<Vec3d>(i, j).val[0] = 1;
			}
			else if (GetColor(CFA, i, j) == CFA_G) {
				mosaicImage.at<Vec3d>(i, j)[1] = InputImage.at<double>(i, j);
				mask.at<Vec3d>(i, j).val[1] = 1;
			}
			else if (GetColor(CFA, i, j) == CFA_R) {
				mosaicImage.at<Vec3d>(i, j)[2] = InputImage.at<double>(i, j);
				mask.at<Vec3d>(i, j).val[2] = 1;
			}
		}

	/// green interpolation
	Mat green = green_interpolation(mosaicImage, mask, CFA, sigma);

	/// parameters for guided upsampling
	int h = 5;
	int v = 5;
	int eps = 0;

	/// Red and Blue demosaicking
	Mat red = red_interpolation(green, mosaicImage, mask, h, v, eps);
	Mat blue = blue_interpolation(green, mosaicImage, mask, h, v, eps);

	/// result image
	Mat demosaicedImage;
	vector<Mat> result;
	result.push_back(blue);
	result.push_back(green);
	result.push_back(red);
	merge(result, demosaicedImage);

	/// output
	return demosaicedImage;
}


void Kiku(int height, int width, Mat& dst, const Mat& src, const Mat& CFA) {
	
	Mat InputImage;

	/// cast to double
	src.convertTo(InputImage, CV_64FC1);


	/// demosaicking
	double sigma = 1.0;
	double eps = 1e-32;
	Mat DemImage = demosaic(InputImage, CFA, sigma);

	/// save image
	DemImage.convertTo(dst, CV_8UC3);
	
}