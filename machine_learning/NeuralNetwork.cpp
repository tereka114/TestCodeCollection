#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "NeuralNetwork.hpp"
#define NDEBUG

using namespace boost::numeric::ublas;

//andのテストデータを生成する。
void generete_xor(vector<vector<double> >& input_data,vector<vector<double> >& label_data){
	input_data.resize(4);
	label_data.resize(4);
	for(int i = 0; i < input_data.size(); i++){
		input_data[i].resize(2);
	}
	for(int i = 0; i < label_data.size(); i++){
		label_data[i].resize(1);
	}

	input_data[0][0] = 0.0;
	input_data[0][1] = 0.0;
	input_data[1][0] = 0.0;
	input_data[1][1] = 1.0;
	input_data[2][0] = 1.0;
	input_data[2][1] = 0.0;
	input_data[3][0] = 1.0;
	input_data[3][1] = 1.0;

	label_data[0][0] = 0.0;
	label_data[1][0] = 1.0;
	label_data[2][0] = 1.0;
	label_data[3][0] = 0.0;
}

void generete_and(vector<vector<double> >& input_data,vector<vector<double> >& label_data){
	input_data.resize(4);
	label_data.resize(4);
	for(int i = 0; i < input_data.size(); i++){
		input_data[i].resize(2);
	}
	for(int i = 0; i < label_data.size(); i++){
		label_data[i].resize(1);
	}

	input_data[0][0] = 0.0;
	input_data[0][1] = 0.0;
	input_data[1][0] = 0.0;
	input_data[1][1] = 1.0;
	input_data[2][0] = 1.0;
	input_data[2][1] = 0.0;
	input_data[3][0] = 1.0;
	input_data[3][1] = 1.0;

	label_data[0][0] = 0.0;
	label_data[1][0] = 0.0;
	label_data[2][0] = 0.0;
	label_data[3][0] = 1.0;
}

int main(int argc, char const *argv[])
{
	NeuralNetwork nn;
	vector<vector<double> > input_data;
	vector<vector<double> > label_data;
	generete_xor(input_data,label_data);

	nn.Training(input_data,label_data);
	nn.Test(input_data,label_data);
	return 0;
}