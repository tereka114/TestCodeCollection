#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

//参考文献もとい引用（http://aidiary.hatenablog.com/entry/20091003/1254574041）
uchar decleaseColor(int value) {
    if (value < 64) {
        return 32;
    } else if (value < 128) {
        return 96;
    } else if (value < 196) {
        return 160;
    } else {
        return 224;
    }

    return 0;  // 未到達
}

int rgb2bin(int red,int green,int blue){
	int redpin = red / 64;
	int greenpin = green / 64;
	int bluepin = blue / 64;
	return 16 * redpin + 4 * greenpin + bluepin;
}

void color_histogram(Mat &image,IplImage *img){
	int histogram[64];
	for(int i=0; i < 64; i++){
		histogram[i] = 0;
	}

	for(int y = 0; y < image.rows; ++y){
		for(int x = 0; x < image.cols; ++x){
			// 画像のチャネル数分だけループ。白黒の場合は1回、カラーの場合は3回
			int red = 0;
			int green = 0;
			int blue = 0;
			for(int c = 0; c < image.channels(); ++c){
				int color = int(image.data[ y * image.step + x * image.elemSize() + c ]);
				if(c == 0){
					blue = color;
				}else if(c == 1){
					green = color;
				}else{
					red = color;
				}
			}
			int bin_no = rgb2bin(red,green,blue);
			histogram[bin_no] += 1;
		}
	}
	for(int i = 0; i < 64; i++){
		cout << histogram[i] << endl;
	}

	IplImage *outImage = cvCreateImage(cvGetSize(img),img->depth,3);
	for(int i=0; i < 64; i++){
		histogram[i] = 0;
	}

	for(int y = 0; y < img->height; y++){
		uchar *pin = (uchar *)(img->imageData + y * img->widthStep);
		uchar *pout = (uchar *)(outImage->imageData + y * outImage->widthStep);
		for(int x = 0; x < img->width; x++){
			int blue = pin[3*x+0];
			int green = pin[3*x+1];
			int red = pin[3*x+2];

			int binno = rgb2bin(red,green,blue);
			histogram[binno] += 1;
			// 64色に減色した画像を作成
			pout[3*x+0] = decleaseColor(blue);
			pout[3*x+1] = decleaseColor(green);
			pout[3*x+2] = decleaseColor(red);
		}
	}
	// ウィンドウを作成
	cvNamedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Declease Color Image", CV_WINDOW_AUTOSIZE);

	// 画像を描画
	cvShowImage("Original Image", img);
	cvShowImage("Declease Color Image", outImage);
	cvWaitKey(0);

	cvDestroyAllWindows();
	cvReleaseImage(&img);
	cvReleaseImage(&outImage);
	for(int i = 0; i < 64; i++){
		cout << histogram[i] << endl;
	}

	//openCv用
	cv::Mat hist;
	const int hdims[] = {256}; // 次元毎のヒストグラムサイズ
	const float hranges[] = {0,256};
	const float* ranges[] = {hranges}; // 次元毎のビンの下限上限
	double max_val = .0;

	calcHist(&image, 1, 0, Mat(), hist, 1, hdims, ranges);
}

void feature_extract(string filedirectory,string algorithm){
	Mat img = imread(filedirectory);
	Ptr<FeatureDetector> detector = FeatureDetector::create(algorithm);
	vector<KeyPoint> keypoint;
	detector->detect(img,keypoint);

	initModule_nonfree();

	Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create(algorithm);
	Mat descriptor;
	extractor->compute(img,keypoint,descriptor);
	for(int i = 0; i < descriptor.rows; i++){
		Mat d(descriptor, Rect(0,i,descriptor.cols,1));
		cout << i << ":" << d << endl;
	}
}

int main(int argc, char const *argv[]){
	IplImage *img = cvLoadImage("iruka.jpg",CV_LOAD_IMAGE_COLOR);
	Mat ims = imread("iruka.jpg");
	color_histogram(ims,img);

	//feature_extract("iruka.jpg","SURF");
}