#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "Layer1.hpp"

using namespace boost::numeric;

class OutPutLayer:public Layer{
public:
	void Update(ublas::vector<double> &label);
	double total_error;
};

void OutPutLayer::Update(ublas::vector<double> &label){
	// cout << dimension << endl;
	// cout << weight << endl;
	// cout << data_output << endl;
	// cout << label << endl;
	old_weight = weight;
	for(int i = 0; i < number; i++){
		error[i] = (label[i] - data_output[i]) * data_output[i] * (1.0 - data_output[i]);
	}
	for(int i = 0; i < number; i++){
		for(int j = 0; j < input_data.size(); j++){
			weight(i,j) += coefficient * error[i] * input_data[j];
		}
	}
	total_error = sum(error);
}