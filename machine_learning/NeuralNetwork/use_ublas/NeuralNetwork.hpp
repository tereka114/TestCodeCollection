#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "OutputLayer.hpp"
#define NDEBUG

using namespace boost::numeric;

class NeuralNetwork{
public:
	int training_time;
	int middle_layer_num; //中間層の数
	int middle_neuron_num;//中間層のニューロンの数
	std::vector<ublas::vector<double> > input_data;
	std::vector<ublas::vector<double> > label_data;
	Layer middle_layer[300];
	OutputLayer output_layer;

	NeuralNetwork(){};
	void SetParameter(int layer_num,int neuron_num,int training);
	void FeedForward(ublas::vector<double> &one_of_input_data);
	void Training(std::vector<ublas::vector<double> >& input_data,std::vector<ublas::vector<double> >& label_data);
	void Check(std::vector<ublas::vector<double> >& input_data,std::vector<ublas::vector<double> >& label_data);
	ublas::vector<double> Predict(ublas::vector<double> &test_data);
	std::vector<ublas::vector<double> > MiddleOut(std::vector<ublas::vector<double> >& input);
};

void NeuralNetwork::SetParameter(int layer_num,int neuron_num,int training){
	middle_layer_num = layer_num;
	middle_neuron_num = neuron_num;
	training_time = training;
}

//前半の部分。
void NeuralNetwork::FeedForward(ublas::vector<double> &one_of_input_data){
	ublas::vector<double> input_temp(input_data[0].size());
	middle_layer[0].OutPut(one_of_input_data);
	input_temp = middle_layer[0].data_output;

	for(int i = 1; i < middle_layer_num; i++){
		middle_layer[i].OutPut(input_temp);
		input_temp = middle_layer[i].data_output;
	}
	output_layer.OutPut(input_temp);
}

void NeuralNetwork::Training(std::vector<ublas::vector<double> >& inputdata,std::vector<ublas::vector<double> >& labeldata){
	input_data = inputdata;
	label_data = labeldata;
	srand((unsigned)time(NULL)); 
	middle_layer[0].SetParameter(middle_neuron_num,input_data[0].size());

	for(int i = 1; i < middle_layer_num; i++){
		middle_layer[i].SetParameter(middle_neuron_num,middle_neuron_num);
	}

	output_layer.SetParameter(labeldata[0].size(),middle_neuron_num);
	std::cout << "training 前" << std::endl;
	std::cout << "training 開始" << std::endl;

	//double before_error = 10000000000.0;
	for(int n = 0; n < training_time; n++){
		double error = 0.0;
		for(int i = 0; i < input_data.size();i++){
			//前半
			FeedForward(input_data[i]);
			//std::cout << output_layer.data_output << std::endl;
			//出力層の逆伝搬
			error += output_layer.Update(label_data[i]);

			ublas::vector<double> error_temp = output_layer.error;
			matrix<double> old_weight_temp = output_layer.old_weight;
			int dimension_temp = output_layer.dimension;
			int layer_number_temp = output_layer.number;

			//最期まで逆伝搬
			for(int j = middle_layer_num-1; j >= 0; j--){
				//std::cout << "check update" << j << std::endl;
				middle_layer[j].Update(error_temp,old_weight_temp,layer_number_temp,dimension_temp);
				error_temp = middle_layer[j].error;
				old_weight_temp = middle_layer[j].old_weight;
				dimension_temp = middle_layer[j].dimension;
				layer_number_temp = middle_layer[j].number;
			}
		}
		if((n % 50000) == 0){
			std::cout << n << std::endl;
			Check(inputdata,labeldata);
		}
	}
}

//出力を返す。
ublas::vector<double> NeuralNetwork::Predict(ublas::vector<double> &test_data){
	FeedForward(test_data);
	input_temp = output_layer.data_output;
	return input_temp;
}

//XORチェック、および出力確認用
void NeuralNetwork::Check(std::vector<ublas::vector<double> >& test_data,std::vector<ublas::vector<double> >& label_data){
	ublas::vector<double> input_temp(test_data[0].size());
	std::cout << "test start!" << std::endl;
	bool flag = true;
	int cnt = 0;
	for(int i = 0; i < test_data.size();i++){
		FeedForward(test_data[i]);
		input_temp = output_layer.data_output;

		std::cout << i << "番目のデータ"<< std::endl;
		std::cout << test_data[i] << " " << label_data[i] << std::endl;
		std::cout << output_layer.data_output << std::endl;
		if(output_layer.data_output[0] > 0.5){
			if(label_data[i][0] == 1){
				std::cout << "True" << std::endl;
				cnt++;
			}else{
				std::cout << "False" << std::endl;
				flag = false;
			}
		}else{
			if(label_data[i][0] == 0){
				std::cout << "True" << std::endl;
				cnt++;
			}else{
				std::cout << "False" << std::endl;
				flag = false;
			}
		}
	}
	if(flag){
		std::cout << "True" << " " << cnt << std::endl;
	}else{
		std::cout << "False" << " " << cnt << std::endl;
	}
}

//中間層の結果を出力する。(BackPropAutoEncoder用)
std::vector<ublas::vector<double> > NeuralNetwork::MiddleOut(std::vector<ublas::vector<double> > &test_data){
	std::vector<ublas::vector<double> > result;

	result.resize(test_data.size());
	for(int i = 0; i < test_data.size(); i++){
		result[i].resize(middle_layer_num);
	}

	for(int i = 0; i < test_data.size(); i++){
		ublas::vector<double> input_temp(test_data[0].size());
		middle_layer[0].OutPut(test_data[i]);
		input_temp = middle_layer[0].data_output;
		result[i] = input_temp;
	}
	return result;
}