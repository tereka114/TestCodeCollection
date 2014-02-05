#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "AutoEncoderLayer.hpp"
#define NDEBUG

class AutoEncoder{
public:
	int training_time; //
	int nueron_number; //
	AutoEncoderLayer middle_layer;
	AutoEncoderOutputLayer output_layer;
};

AutoEncoder::AutoEncoder(int neuron_num,int training){
	middle_neuron_num = neuron_num;
	training_time = training;
};

AutoEncoder::training(vector<vector<double> >& inputdata,vector<vector<double> >& labeldata){
	input_data = inputdata;
	label_data = labeldata;
	srand((unsigned)time(NULL));

	middle_layer.SetParameter(middle_neuron_num,input_data[0].size());
	output_layer.SetParameter(1,middle_neuron_num);

	//一時的に保存する場所
	vector<double> input_temp(inputdata[0].size());
	
	//トレーニング開始
	for(int n = 0; n < training_time; n++){
		for(int i = 0; i < input_data.size(); i++){
			middle_layer[0].OutPut(input_data[i]);
			input_temp = middle_layer[0].data_output;

			output_layer.OutPut(input_temp);
		}
	}
}