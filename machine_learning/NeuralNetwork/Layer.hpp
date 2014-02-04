#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#define NDEBUG

using namespace boost::numeric::ublas;

class Layer{
public:
	matrix<double> weight;
	vector<double> data_output;
	matrix<double> old_weight;
	matrix<double> error;
	vector<double> input;
	int number; //数
	int dimension; //次元数
	double coefficient; //学習係数

	double Sigmoid(double d);
	void SetParameter(int layer_number,int layer_dimension);
	void OutPut(vector<double>& input_data);
	void InputPrepare(vector<double>& input_data);
	void Update(matrix<double> &before_error,matrix<double> &before_old_weight,int layer_number,int before_dimension);
};

double Layer::Sigmoid(double d){
	return 1 / (1 + exp(-d));
}

void Layer::SetParameter(int layer_number,int layer_dimension){
	weight.resize(layer_number,layer_dimension+1);
	error.resize(layer_number,layer_dimension+1);
	for(int i = 0; i < layer_dimension+1; i++){
		for(int j = 0; j < layer_number; j++){
			weight(j,i) = (double)rand()/RAND_MAX;
		}
	}
	number = layer_number;
	dimension = layer_dimension+1;
	coefficient = 0.01;
}

void Layer::InputPrepare(vector<double>& input_data){
	vector<double> temp(input_data.size()+1);
	temp[0] = 1.0;
	for(int i = 0; i < input_data.size(); i++){
		temp[i+1] = input_data[i];
	}
	input_data.resize(temp.size());
	input_data = temp;
}

void Layer::OutPut(vector<double>& input_data){
	input = input_data;
	InputPrepare(input);
	data_output = prod(weight,input);

	for(int i = 0; i < data_output.size(); i++){
		data_output[i] = Sigmoid(data_output[i]);
	}
}

void Layer::Update(matrix<double> &before_error,matrix<double> &before_old_weight,int layer_number,int before_dimension){
	old_weight = weight;

	for(int i = 1; i < before_dimension; i++){
		double ek = 0.0;
		for(int j = 0; j < layer_number; j++){
			ek += before_error(j,i) * before_old_weight(j,i);
		}

		ek = ek * data_output[i-1] * (1 - data_output[i-1]);
		for(int j = 0; j < input.size(); j++){
			weight(i-1,j) += input[j] * ek * coefficient;
			error(i-1,j) = ek;
		}
	}
}