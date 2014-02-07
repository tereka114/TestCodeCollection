#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include "Image.hpp"

using namespace cv;
using namespace std;

int main(){

	Image img;
	img.ImageRead("Lenna.jpg");
	img.Color2Glay();
	img.Hog();
}