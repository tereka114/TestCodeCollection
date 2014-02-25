#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
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

	double coefficient; //学習係数

	void SetParameter(int n_neuron,int n_input);
	void OutPut(ublas::vector<double>& input_data);
	void Update(ublas::vector<double> &before_error,ublas::matrix<double> &before_old_weight);
private:
	double Sigmoid(double d);
};

double Layer::Sigmoid(double d){
	return 1.0 / (1.0 + exp(-d));
}

void Layer::SetParameter(int n_neuron,int n_input){
	coefficient = 1; // 学習係数

	//リサイズ
	weight.resize(n_neuron,n_input);
	error.resize(n_neuron);
	bias.resize(n_neuron);

	//bias random関数を抜き出した方がいい。
	for(int i = 0; i < bias.size(); i++){
		bias[i] = (double)rand()/RAND_MAX - 0.5;
	}

	//重みの初期化
	for(int i = 0; i <weight.size1(); i++){
		for(int j = 0; j < weight.size2(); j++){
			weight(i,j) = (double)rand()/RAND_MAX - 0.5;
		}
	}
}

//出力
void Layer::OutPut(ublas::vector<double>& input_data){
	input = input_data;
	//しぐもいど関数の入力と出力を分割する。
	data_output = prod(weight,input);

	//biasの修正
	for(int i = 0; i < data_output.size(); i++){
		data_output[i] += bias[i];
		data_output[i] = Sigmoid(data_output[i]);
	}
}

void Layer::Update(ublas::vector<double> &before_error,ublas::matrix<double> &before_old_weight){
	old_weight = weight;
	// cerr << __func__ << endl; 
	// cerr << before_error << endl;
	// cerr << before_old_weight << endl;
	// cerr << number << endl;
	// cerr << before_dimension << endl;
	// cerr << dimension << endl;

	for(int i = 0; i < error.size(); i++){
		double ek = 0.0;
		for(int j = 0; j < before_old_weight.size1(); j++){
			ek += before_error[j] * before_old_weight(j,i);
		}
		error[i] = ek;
	}
	
	for(int i = 0; i < weight.size1(); i++){
		double ek = error[i] * data_output[i] * (1.0 - data_output[i]);
		row(weight,i) += input * (ek * coefficient);

		bias[i] += ek * coefficient;
	}
}