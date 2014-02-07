#include <iostream>
#include <math.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;
using namespace std;

void allscale(std::vector<ublas::vector<double> > &data){
	double sum = 0.0;
	int row = data.size(); int col = data[0].size();

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			sum += data[i][j];
		}
	}
	double mean =  sum / (row * col);

	sum = 0.0;

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			sum += pow(data[i][j] - mean,2.0);
		}
	}
	double std = sqrt(sum / (row * col));

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			data[i][j] = (data[i][j] - mean) / std;
		}
	}
}

void attributescale(std::vector<ublas::vector<double> > &data){
	std::vector<double> mean;
	std::vector<double> std;

	int row = data.size(); int col = data[0].size();
	for(int i = 0; i < col; i++){
		double sum = 0.0;
		for(int j = 0; j < row; j++){
			sum += data[j][i];
		}
		mean.push_back(sum / (row * col));
	}

	for(int i = 0; i < col; i++){
		double sum = 0.0;
		for(int j = 0; j < row; j++){
			sum += pow(data[j][i] - mean[i],2.0);
		}
		std.push_back(sum / (row * col))
	}

	for(int i = 0; i < col; i++){
		for(int j = 0; j < row; j++){
			data[j][i] = (data[j][i] - mean[i]) / std[i];
		}
	}
}

void jointvector(std::vector<ubas::vector<double> > &data,std::vector<ublas::vector<double> > add_data){
	if(add_data.empty()){
		cout << "no data(jointvector)" << endl;
	}
	if(data[0].size() != add_data[0].size()){
		cout << "difference dimensions" << endl;
	}
	int row = add_data.size();

	for(int i = 0; i < row; i++){
		data.push_back(add_data[i]);
	}
}

void normalizevector(ublas::vector<double> &data){
	double total = sum(data);
	for(int i = 0; i < data.size(); i++){
		data[i] /= total;
	}
}