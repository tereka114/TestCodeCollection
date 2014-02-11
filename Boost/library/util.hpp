#include <iostream>
#include <math.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric;
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
		std.push_back(sum / (row * col));
	}

	for(int i = 0; i < col; i++){
		for(int j = 0; j < row; j++){
			data[j][i] = (data[j][i] - mean[i]) / std[i];
		}
	}
}

void jointvector(std::vector<ublas::vector<double> > &data,std::vector<ublas::vector<double> > add_data){
	int row = add_data.size();

	for(int i = 0; i < row; i++){
		data.push_back(add_data[i]);
	}
}

template<typename T>
void normalize_vector(ublas::vector<T> &data){
	T total = sum(data);
	for(int i = 0; i < data.size(); i++){
		data[i] /= total;
	}
}

template<typename T>
void fill(ublas::vector<T> &data,int num){
	for(int i = 0; i < data.size(); i++){
		data[i] = num;
	}
}

template<typename T>
void fill(ublas::matrix<T> &data,int num){
	for(int i = 0; i < data.size1(); i++){
		for(int j = 0; j < data.size2(); j++){
			data(i,j) = num;
		}
	}
}
template<typename T>
ublas::vector<T> arange(T start,T end){
	int size = end - start;
	ublas::vector<T> result(size);

	for(int i = 0; i < size; i++){
		result[i] = i + start;
	}

	return result;
}

template<typename T>
ublas::vector<T> arange(T start,T end,T space){
	int size = (end - start) / space;
	ublas::vector<T> result(size);

	for(int i = 0; i < size; i++){
		result[i] = i * space + start;
	}

	return result;
}

template<typename T>
ublas::vector<T> cut(ublas::vector<T> &data,int start,int end){
	int size = end - start;
	ublas::vector<double> result(size);

	for(int i = 0; i < size; i++){
		result[i] = data[start + i];
	}

	return result;
}