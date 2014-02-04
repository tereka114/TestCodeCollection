#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "OutputLayer.hpp"
#define NDEBUG

using namespace boost::numeric::ublas;

class NeuralNetwork{
public:
	int training_time;
	int middle_layer_num; //中間層の数
	int middle_neuron_num;//中間層のニューロンの数
	vector<vector<double> > input_data;
	vector<vector<double> > label_data;
	Layer middle_layer[300];
	OutputLayer output_layer;

	NeuralNetwork(int layer_num,int neuron_num,int training);
	void Training(vector<vector<double> >& input_data,vector<vector<double> >& label_data);
	void Test(vector<vector<double> >& input_data,vector<vector<double> >& label_data);
};

NeuralNetwork::NeuralNetwork(int layer_num,int neuron_num,int training){
	middle_layer_num = layer_num;
	middle_neuron_num = neuron_num;
	training_time = training;
}

void NeuralNetwork::Training(vector<vector<double> >& inputdata,vector<vector<double> >& labeldata){
	input_data = inputdata;
	label_data = labeldata;
	srand((unsigned)time(NULL)); 
	middle_layer[0].SetParameter(middle_neuron_num,input_data[0].size());

	for(int i = 1; i < middle_layer_num; i++){
		middle_layer[i].SetParameter(middle_neuron_num,middle_neuron_num);
	}

	output_layer.SetParameter(1,middle_neuron_num);
	vector<double> input_temp(input_data[0].size());
	std::cout << "training 前" << std::endl;
	std::cout << "training 開始" << std::endl;

	for(int n = 0; n < training_time; n++){
		for(int i = 0; i < input_data.size();i++){
			//学習
			middle_layer[0].OutPut(input_data[i]);
			input_temp = middle_layer[0].data_output;
			//各層の学習
			for(int j = 1; j < middle_layer_num; j++){
				middle_layer[j].OutPut(input_temp);
				input_temp = middle_layer[j].data_output;
			}
			//出力層
			output_layer.OutPut(input_temp);
			input_temp = output_layer.data_output;
			//出力層の逆伝搬
			output_layer.Update(label_data[i]);

			matrix<double> error_temp = output_layer.error;
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
	}
}

void NeuralNetwork::Test(vector<vector<double> >& input_data,vector<vector<double> >& label_data){
	vector<double> input_temp(input_data[0].size());
	std::cout << "test start!" << std::endl;
	for(int i = 0; i < input_data.size();i++){
		//学習
		middle_layer[0].OutPut(input_data[i]);
		input_temp = middle_layer[0].data_output;
		//各層の学習
		for(int j = 1; j < middle_layer_num; j++){
			middle_layer[j].OutPut(input_temp);
			input_temp = middle_layer[j].data_output;
		}
		//出力層
		output_layer.OutPut(input_temp);
		input_temp = output_layer.data_output;

		std::cout << i << "番目のデータ"<< std::endl;
		std::cout << input_data[i] << " " << label_data[i] << std::endl;
		std::cout << output_layer.data_output << std::endl;
	}
}