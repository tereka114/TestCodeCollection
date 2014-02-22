#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#define NDEBUG

using namespace boost::numeric;
using namespace std;

class Layer{
public:
	ublas::matrix<double> weight; //重み
	ublas::vector<double> data_output; //output
	ublas::matrix<double> old_weight; //古い重み
	ublas::vector<double> error; //エラー値
	ublas::vector<double> input; //入力
	ublas::vector<double> bias;

	int number; //数
	int dimension; //次元数
	double coefficient; //学習係数

	double Sigmoid(double d);
	void SetParameter(int layer_number,int layer_dimension);
	void OutPut(ublas::vector<double>& input_data);
	void Update(ublas::vector<double> &before_error,ublas::matrix<double> &before_old_weight,int layer_number,int before_dimension);
};

double Layer::Sigmoid(double d){
	return 1.0 / (1.0 + exp(-d));
}

void Layer::SetParameter(int layer_number,int layer_dimension){
	number = layer_number;
	dimension = layer_dimension;
	coefficient = 0.8;

	//リサイズ
	weight.resize(layer_number,dimension);
	error.resize(layer_number);
	bias.resize(layer_number);

	//bias
	for(int i = 0; i < layer_number; i++){
		bias[i] = (double)rand()/RAND_MAX - 0.5;
	}

	//重みの初期化
	for(int i = 0; i < dimension; i++){
		for(int j = 0; j < layer_number; j++){
			weight(j,i) = (double)rand()/RAND_MAX - 0.5;
		}
	}
}

//出力
void Layer::OutPut(ublas::vector<double>& input_data){
	input = input_data;
	data_output = prod(weight,input);

	//biasの修正
	for(int i = 0; i < data_output.size(); i++){
		data_output[i] += bias[i];
		data_output[i] = Sigmoid(data_output[i]);
	}
}

void Layer::Update(ublas::vector<double> &before_error,ublas::matrix<double> &before_old_weight,int layer_number,int before_dimension){
	old_weight = weight;

	for(int i = 0; i < before_dimension; i++){
		double ek = 0.0;
		for(int j = 0; j < layer_number; j++){
			ek += before_error[j] * before_old_weight(j,i);
		}
		error[i] = ek;
	}
	
	for(int i = 0; i < layer_number; i++){
		double ek = error[i] * data_output[i] * (1.0 - data_output[i]);
		bias[i] += ek * coefficient;

		for(int j = 0; j < dimension; j++){
			weight(i,j) += coefficient * input[j] * ek;
		}
	}
}