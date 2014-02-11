#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../../NeuralNetwork/NeuralNetwork.hpp"
#define NDEBUG

class BackPropAutoEncoder{
public:
	NeuralNetwork neural_network[100];
	int reduce_time;
	vector<vector<double> > result;

	BackPropAutoEncoder(){};
	void SetParameter(std::vector<int> dimension_list,std::vector<int> training_time_list);
	void Training(vector<vector<double> >& input_data);
	void Test(vector<vector<double> >&input_data);
	vector<vector<double> > BackPropAutoEncoder::GetResult();
};

void BackPropAutoEncoder::SetParameter(std::vector<int> dimension_list,std::vector<int> training_time_list){
	reduce_time = dimension_list.size();
	for(int i = 0; i < reduce_time; i++){
		neural_network[i].SetParameter(1,dimension_list[i],training_time_list[i]);
	}
}

void BackPropAutoEncoder::Training(vector<vector<double> >& input_data){
	//Training Start
	vector<vector<double> > input = input_data;

	for(int i = 0; i < reduce_time; i++){
		neural_network[i].Training(input,input);
		vector<vector<double > temp = neural_network[i].MiddleOut(input);
		for(int i = 0; i < temp.size(); i++){
			input.resize(temp[0].size());
		}
		input = temp;
	}
}

void BackPropAutoEncoder::Test(vector<vector<double> >&input_data){
	vector<vector<double> > input = input_data;

	for(int i = 0; i < reduce_time-1; i++){
		input = neural_network[i].MiddleOut(input);
	}
	result = neural_network[reduce_time-1].Test(input);
}

vector<vector<double> > BackPropAutoEncoder::GetResult(){
	return result;
}