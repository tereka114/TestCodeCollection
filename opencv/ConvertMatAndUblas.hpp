#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace cv;
using namespace boost::numeric;
using namespace std;

void mat2matrix(Mat& oldmat,vector<ublas::vector<double> > &result){
	Mat mat;
	oldmat.convertTo( mat, CV_64F );

	int row = mat.rows;
	int col = mat.cols;
	vector<double> v1;
	mat = mat.reshape(0,1);
	mat.copyTo(v1);
	ublas::vector<double> vect(col);

	for(int i = 0; i < row; i++){
		int index = 128 * i;
		for(int j = 0; j < col; j++){
			vect[j] = v1[index + j];
		}
		result.push_back(vect);
	}
}

