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
	int middle_layer_num; //中間層の数
	int middle_neuron_num;//中間層のニューロンの数
	Layer middle_layer[10]; //適当
	OutputLayer output_layer; //出力層

	NeuralNetwork(){};
	void SetParameter(int layer_num,int neuron_num);
	void Training(const std::vector<ublas::vector<double> >& input_data,const std::vector<ublas::vector<double> >& label_data,int training);
	void Check(const std::vector<ublas::vector<double> > &input_data,const std::vector<ublas::vector<double> >& label_data);
	ublas::vector<double> Predict(ublas::vector<double> &test_data);
private:
	void FeedForward(const ublas::vector<double> &one_of_input_data);
	double Backward(const ublas::vector<double> &one_of_label_data);
};

//中間層の層数、ニューロンの指定
void NeuralNetwork::SetParameter(int layer_num,int neuron_num){
	middle_layer_num = layer_num;
	middle_neuron_num = neuron_num;
}

//前半の部分。guess
void NeuralNetwork::FeedForward(const ublas::vector<double> &one_of_input_data){
	middle_layer[0].OutPut(one_of_input_data);

	output_layer.OutPut(middle_layer[0].data_output);
	//cout << one_of_input_data << " " << output_layer.data_output << endl;
}

//後半部分 learning
double NeuralNetwork::Backward(const ublas::vector<double> &one_of_label_data){
	double tmp_error = output_layer.Update(one_of_label_data);

	middle_layer[0].Update(output_layer.error,output_layer.old_weight);
	
	return tmp_error;
	//最期まで逆伝搬
		//std::cout << "check update" << j << std::endl;
	// 	middle_layer[j].Update(error_temp,old_weight_temp,layer_number_temp,dimension_temp);
	// 	error_temp = middle_layer[j].error;
	// 	old_weight_temp = middle_layer[j].old_weight;
	// 	dimension_temp = middle_layer[j].dimension;
	// 	layer_number_temp = middle_layer[j].number;
	// }
}

void NeuralNetwork::Training(const std::vector<ublas::vector<double> >& input_data,const std::vector<ublas::vector<double> >& label_data,int training){
	srand((unsigned)time(NULL)); 
	middle_layer[0].SetParameter(middle_neuron_num,input_data[0].size());

	// for(int i = 1; i < middle_layer_num; i++){
	// 	middle_layer[i].SetParameter(middle_neuron_num,middle_neuron_num);
	// }

	output_layer.SetParameter(label_data[0].size(),middle_neuron_num);
	std::cerr << "training 前" << std::endl;
	std::cerr << "training 開始" << std::endl;

	//double before_error = 10000000000.0;
	for(int n = 0; n < training; n++){
		double error = 0.0;
		for(int i = 0; i < input_data.size();i++){
			FeedForward(input_data[i]);
			error += Backward(label_data[i]);
		}
		//5万回に一度、チェックする。
		if((n % 50000) == 0){
			std::cout << n << std::endl;
			Check(input_data,label_data);
		}
		std::cout << n+1 << " " << error << std::endl;
		if(error < 0.01){
			std::cout << n+1 << endl;
			break;
		}
	}
}

//出力を返す。
ublas::vector<double> NeuralNetwork::Predict(ublas::vector<double> &test_data){
	FeedForward(test_data);
	return output_layer.data_output;
}

//XORチェック、および出力確認用
void NeuralNetwork::Check(const std::vector<ublas::vector<double> >& test_data,const std::vector<ublas::vector<double> >& label_data){
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