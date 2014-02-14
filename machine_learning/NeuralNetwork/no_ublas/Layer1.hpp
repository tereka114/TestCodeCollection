#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric;
using namespace std;

class Layer{
public:
	ublas::matrix<double> weight;
	ublas::matrix<double> old_weight;
	ublas::vector<double> input_data;
	ublas::vector<double> data_output;
	ublas::vector<double> error;

	int number;
	int dimension;
	double coefficient;

	double Sigmoid(double d);
	void SetParameter(int layer_number,int layer_dimension);
	void OutPut(ublas::vector<double> &inputdata);
	void Update(ublas::vector<double> &before_error,ublas::matrix<double> &before_weight);
};

double Layer::Sigmoid(double d){
	return 1.0 / (1.0 + exp(-d));
}

void Layer::SetParameter(int layer_number,int layer_dimension){
	weight.resize(layer_number,layer_dimension);
	error.resize(layer_number);

	for(int i = 0; i < layer_number; i++){
		for(int j = 0; j < layer_dimension; j++){
			weight(i,j) = (double)rand()/RAND_MAX - 0.5;
		}
	}
	number = layer_number;
	dimension = layer_dimension;
	coefficient = 0.01;
}

void Layer::OutPut(ublas::vector<double> &inputdata){
	input_data = inputdata;
	// cout << input_data << endl;
	// cout << data_output.size() << endl;
	// cout << weight << endl;
	// cout << number << endl;
	// cout << dimension << endl;

	data_output.resize(number);

	for(int i = 0; i < number; i++){
		data_output[i] = 1.0;
		for(int j = 0; j < dimension; j++){
			data_output[i] += input_data[j] * weight(i,j);
		}
		data_output[i] = Sigmoid(data_output[i]);
	}
}

void Layer::Update(ublas::vector<double> &before_error,ublas::matrix<double> &before_weight){
	old_weight = weight;

	for(int i = 0; i < number; i++){
		double temp_error = 0.0;
		for(int j = 0; j < before_error.size(); j++){
			temp_error += before_error[j] * before_weight(j,i);
		}
		error[i] = data_output[i] * (1.0 - data_output[i]) * temp_error;
	}
	for(int i = 0; i < number; i++){
		for(int j = 0; j < dimension; j++){
			weight(i,j) += coefficient * error[i] * input_data[j];
		}
	}
}