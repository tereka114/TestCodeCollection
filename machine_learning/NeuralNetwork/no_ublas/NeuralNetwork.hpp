#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "OutPutLayer.hpp"

using namespace boost::numeric;
using namespace std;

class NeuralNetwork{
public:
	int training_time;
	int middle_layer_num;
	int middle_neuron_num;//中間層のニューロンの数
	std::vector<ublas::vector<double> > input_data;
	std::vector<ublas::vector<double> > label_data;

	Layer middle_layer[300];
	OutPutLayer output_layer;

	NeuralNetwork(){};
	void SetParameter(int layer_num,int neuron_num,int training_time);
	void Training(std::vector<ublas::vector<double> >& inputdata,std::vector<ublas::vector<double> >& labeldata);
	void Check(std::vector<ublas::vector<double> >& test_data,std::vector<ublas::vector<double> >& label_data);
};

void NeuralNetwork::SetParameter(int layer_num,int neuron_num,int training){
	middle_layer_num = layer_num;
	middle_neuron_num = neuron_num;
	training_time = training;
}

void NeuralNetwork::Training(std::vector<ublas::vector<double> >& inputdata,std::vector<ublas::vector<double> >& labeldata){
	input_data = inputdata;
	label_data = labeldata;
	srand((unsigned)time(NULL)); 

	middle_layer[0].SetParameter(middle_neuron_num,input_data[0].size());
	output_layer.SetParameter(labeldata[0].size(),middle_neuron_num);


	ublas::vector<double> input_temp;
	ublas::vector<double> error_temp;

	for(int n = 0; n < training_time; n++){
		double error = 0.0;
		for(int i = 0; i < input_data.size(); i++){
			middle_layer[0].OutPut(input_data[i]);
			input_temp = middle_layer[0].data_output;

			output_layer.OutPut(input_temp);

			output_layer.Update(label_data[i]);
			error_temp = output_layer.error;
			middle_layer[0].Update(error_temp,output_layer.old_weight);

			error += output_layer.total_error;
		}
		cout << error << endl;
	}
}

void NeuralNetwork::Check(std::vector<ublas::vector<double> >& test_data,std::vector<ublas::vector<double> >& label_data){
	ublas::vector<double> input_temp(test_data[0].size());
	std::cout << "test start!" << std::endl;
	bool flag = true;
	int cnt = 0;
	for(int i = 0; i < test_data.size();i++){
		//学習
		middle_layer[0].OutPut(test_data[i]);
		input_temp = middle_layer[0].data_output;
		//出力層
		output_layer.OutPut(input_temp);
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