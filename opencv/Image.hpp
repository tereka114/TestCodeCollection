#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include "./ConvertMatAndUblas.hpp"

#define N_BIN 9
#define THETA 180 / N_BIN
#define CELL_SIZE 20
#define BLOCK_SIZE 2
#define R (CELL_SIZE * (BLOCK_SIZE) * 0.5)

using namespace cv;
using namespace std;
using namespace boost::numeric;


//Hogについてはhttp://gori-naru.blogspot.jp/2012/11/hog.html　を参考に。
class Image{
public:
	Mat image; //普通の画像
	Mat glayimage; //グレースケール画像
	vector<MatND> colorhistrogram; //カラーヒストグラム
	MatND same_color_histrogram; //同じカラーヒストグラム（三食合体）
	map<string,vector<KeyPoint> > keypoints; //キーポイント
	map<string,Mat > feature_descriptors; //特徴量（SIFTなど）
	vector<Mat> hog; //hog特徴量

	void ImageRead(string path);
	void MatRead(Mat &mat);
	void FeatureExtract(string algorithm);
	void TransFormAllHistrogram();
	void MakeColorHistrogram();
	void Color2Glay();
	void Hog();
	void GetExtractFeature(string algorithm,vector<ublas::vector<double> > &result);
};

void Image::ImageRead(string path){
	image = imread(path);
}

void Image::MatRead(Mat &mat){
	image = mat.clone();
}

void Image::Color2Glay(){
	cvtColor(image, glayimage,CV_RGB2GRAY);
}

std::vector<Mat> CalculateIntegralHOG(Mat& glayimage){
	Mat xsobel,ysobel;
	Sobel(glayimage,xsobel,CV_32F,1,0); //xの微分
	Sobel(glayimage,ysobel,CV_32F,0,1); //yの微分

	std::vector<Mat> bins(N_BIN);
	for(int i = 0; i < N_BIN; i++){
		bins[i] = Mat::zeros(glayimage.size(),CV_32F);
	}

	Mat Imag,Iang;
	cartToPolar(xsobel,ysobel,Imag,Iang,true);

	add(Iang,Scalar(180),Iang,Iang<0);
	add(Iang,Scalar(-180),Iang,Iang>=180);
	Iang /= THETA;

	for(int y = 0; y <glayimage.rows;y++){
		for(int x = 0; x < glayimage.cols;x++){
			int ind = Iang.at<float>(y,x);
			bins[ind].at<float>(y,x) += Imag.at<float>(y,x);
		}
	}

	std::vector<Mat> integrals(N_BIN);
	for(int i = 0; i < N_BIN; i++){
		integral(bins[i],integrals[i]);
	}

	return integrals;
}


//セルごとのHog。セルはピクセルの塊
void calculateHOGinCell(Mat& hog_cell,Rect roi,std::vector<Mat>& integrals){
	int x0 = roi.x,y0 = roi.y;
	int x1 = x0 + roi.width,y1 = y0 + roi.height;

	for(int i = 0; i < N_BIN; i++){
		Mat integral = integrals[i];
		float a = integral.at<double>(y0, x0);
        float b = integral.at<double>(y1, x1);
        float c = integral.at<double>(y0, x1);
        float d = integral.at<double>(y1, x0);
		hog_cell.at<float>(0,i) = (a + b) - (c + d);
	}
}

Mat getHog(Point pt,std::vector<Mat> &integrals){
	if(pt.x - R < 0 ||
		pt.y - R < 0 ||
		pt.x + R >= integrals[0].cols ||
		pt.y + R >= integrals[0].rows
		){
		return Mat();
	}

	//リターン条件
	Mat hist(Size(N_BIN*BLOCK_SIZE*BLOCK_SIZE,1),CV_32F);

	//ポイントごとにがんばろう
	Point tl(0,pt.y-R);
	int c = 0;
	for(int i = 0; i < BLOCK_SIZE;i++){
		tl.x = pt.x - R;
		for(int j = 0; j < BLOCK_SIZE; j++){
			Rect roi(tl,tl+Point(CELL_SIZE,CELL_SIZE));
			Mat hist_temp = hist.colRange(c,c+N_BIN);

			calculateHOGinCell(hist_temp,roi,integrals);
			//cout << hist << endl;
			tl.x += CELL_SIZE;
			c += N_BIN;
		}
		tl.y = CELL_SIZE;
	}
	normalize(hist,hist,1,0,NORM_L2);
	return hist;
}

void Image::Hog(){
	//積分画像の生成
	std::vector<Mat> integrals = CalculateIntegralHOG(glayimage);

	Mat image = glayimage.clone();
	image *= 0.5;

	Mat mean_hog_in_block(Size(N_BIN,1),CV_32F);
	for(int y = CELL_SIZE/2; y < glayimage.rows; y += CELL_SIZE){
		for(int x = CELL_SIZE / 2; x < glayimage.cols; x += CELL_SIZE){
			Mat hist = getHog(Point(x,y),integrals);

			if (hist.empty()) continue;

			mean_hog_in_block = Scalar(0);
			for(int i = 0; i < N_BIN; i++){
				for(int j = 0; j < BLOCK_SIZE; j++){
					mean_hog_in_block.at<float>(0,i) += hist.at<float>(0,i+j*N_BIN);
				}
			}
			normalize(mean_hog_in_block,mean_hog_in_block,1,0,CV_L2);
			hog.push_back(mean_hog_in_block);
			
			Point center(x, y);
			for (int i = 0; i < N_BIN; i++) {
			    double theta = (i * THETA + 90.0 ) * CV_PI / 180.0;
			    Point rd(CELL_SIZE*0.5*cos(theta), CELL_SIZE*0.5*sin(theta));
			    Point rp = center -  rd;
			    Point lp = center -  -rd;
			    line(image, rp, lp, Scalar(255*mean_hog_in_block.at<float>(0, i), 255, 255));
			}
		}
	}
	
	//imshow("out",image);
	//waitKey(0);
}

void Image::FeatureExtract(string algorithm){
	initModule_nonfree();

	Ptr<FeatureDetector> detector = FeatureDetector::create(algorithm);
	std::vector<KeyPoint> keypoint;
	detector->detect(image,keypoint);

	keypoints[algorithm] = keypoint;

	Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create(algorithm);
	Mat descriptor;
	extractor->compute(image,keypoint,descriptor);
	//cout << descriptor.cols << " "<< descriptor.rows << endl;
	for(int i = 0; i < descriptor.rows; i++){
		//Mat d(descriptor, Rect(0,i,descriptor.cols,1));
		//cout << i << ":" << d << endl;
	}
	feature_descriptors[algorithm] = descriptor;
}

void transformHistrogram(std::vector<MatND> &hist,MatND &return_hist){
	std::vector<double> temp_histrogram;
	for(int i = 0; i < 3; i++){
		std::vector<double> temp_color;
		hist[i].copyTo(temp_color);
		for(int i = 0; i < temp_color.size(); i++){
			temp_histrogram.push_back(temp_color[i]);
		}
	}
	MatND temp_result(temp_histrogram,CV_32F);
	return_hist = temp_result;
}

void Image::TransFormAllHistrogram(){
	for(int i = 0; i < colorhistrogram.size();i++){
		MatND temp;
		transformHistrogram(colorhistrogram,temp);
		MatND result;
		temp.convertTo(result,CV_32F);
		same_color_histrogram = result;
	}
}

void Image::MakeColorHistrogram(){
	int channel = image.channels();
	colorhistrogram.resize(channel);

	const int hist_size = 256;
	const int hdims[] = {hist_size};
	const float hranges[] = {0,256};
	const float* ranges[] = {hranges};

	for(int i = 0; i < channel; i++){
		calcHist(&image,1,&i,Mat(),colorhistrogram[i],1,hdims,ranges,true,false);
	}

	TransFormAllHistrogram();
}

void Image::GetExtractFeature(string algorithm,std::vector<ublas::vector<double> > &result){
	Mat mat = feature_descriptors[algorithm];
	mat2matrix(mat,result);
}