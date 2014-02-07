#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include "Image.hpp"

using namespace cv;
using namespace std;

class Movie{
public:
	VideoCapture video;
	//カラーヒストグラム関係
	vector<Image> frame_list;
	vector<double> compare_color_histrogram; //比較した値 サイズ-1の値となる。
	vector<Image> compress_video; //圧縮したびでお

	Movie(string filename){
		VideoCapture cap(filename);
		video = cap;
	}

	void DataRead();
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
		frame_list.push_back(img.MatRead(frame));
	}
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

int main(int argc, char const *argv[])
{
	Movie movie("uma.mp4");
	movie.DataRead();
	movie.MakeColorHistrogram();
	movie.CompareHistrogram();
	movie.CompressFeatureExtract();
	cout << movie.video.get(CV_CAP_PROP_FRAME_COUNT) << endl;
	return 0;
}