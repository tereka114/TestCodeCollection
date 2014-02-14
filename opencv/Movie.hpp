#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include "Image.hpp"

using namespace cv;
using namespace std;

class Movie{
public:
	VideoCapture video;
	string filepath;

	//カラーヒストグラム関係
	vector<Image> frame_list;
	vector<double> compare_color_histrogram; //比較した値 サイズ-1の値となる。
	vector<Image> compress_video; //圧縮したびでお

	Movie(string filename){
		VideoCapture cap(filename);
		video = cap;
		filepath = filename;
	}

	void DataRead();
	void CenterExtract();
	void MakeColorHistrogram();
	void CompareHistrogram();
	void CompressFeatureExtract();
};

//動画の読み出し
void Movie::DataRead(){
	while(1){
		Mat frame;
		video >> frame;

		if(frame.empty() || waitKey(30) >= 0 || video.get(CV_CAP_PROP_POS_AVI_RATIO) == 1){
			break;
		}
		Image img;
		img.MatRead(frame);
		frame_list.push_back(img);
	}
}

void Movie::CenterExtract(){
	int size = frame_list.size();
	int center = size / 2;
	string write_path = boost::algorithm::replace_all_copy(filepath,".mp4",".jpg");

	imwrite(write_path,frame_list[center].image);
}
//カラーヒストグラムの生成
void Movie::MakeColorHistrogram(){
	for(int i = 0; i < frame_list.size(); i++){
		frame_list[i].MakeColorHistrogram();
	}
}

//全フレームに対してヒストグラムを比べる
void Movie::CompareHistrogram(){
	Mat now = frame_list[0].image;
	compress_video.push_back(frame_list[0]);

	for(int i = 0; i < frame_list.size()-1; i++){
		double result = compareHist(frame_list[i].same_color_histrogram,frame_list[i+1].same_color_histrogram,CV_COMP_CORREL);
		if(result < 0.9){
			cout << result << endl;
			cout << norm(now) << endl;
			now = frame_list[i+1].image;
			compress_video.push_back(frame_list[i+1]);
		}
	}
}

//圧縮した内容に対し、特徴量を求める。
void Movie::CompressFeatureExtract(){
	for(int i = 0; i < compress_video.size(); i++){
		cout<< norm(compress_video[i].image) << endl;
		compress_video[i].FeatureExtract("SURF");
	}
}