#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "Layer.hpp"
#define NDEBUG

using namespace boost::numeric::ublas;

class OutputLayer:public Layer{
public:
	double Update(ublas::vector<double>& label);
};

double OutputLayer::Update(ublas::vector<double>& label){
	double total_error = 0.0;
	old_weight = weight;
	
	for(int i = 0; i < data_output.size(); i++){
		double p = (label[i] - data_output[i]) * (1.0 - data_output[i]) * data_output[i];
		total_error += pow(label[i] - data_output[i],2);
		for(int j = 0; j < input.size();j++){
			error(i,j) = p * input[j];
			weight(i,j) +=  coefficient * error(i,j);
		}
	}
	return total_error;
}