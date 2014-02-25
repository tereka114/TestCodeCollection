#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "Layer.hpp"
#define NDEBUG

using namespace boost::numeric::ublas;

class OutputLayer:public Layer{
public:
	double Update(const ublas::vector<double>& label);
};

double OutputLayer::Update(const ublas::vector<double>& label){
	double total_error = 0.0;
	old_weight = weight;
	
	//label[i] - data_output[i] を変数
	//sigmoidと微分を組に、delta_weightかえる。
	for(int i = 0; i < weight.size1(); i++){
		error[i] = (label[i] - data_output[i]) * (1.0 - data_output[i]) * data_output[i];
		total_error += 0.5 * pow(label[i] - data_output[i],2);

		row(weight,i) += input * (error[i] * coefficient);
		bias[i] += coefficient * error[i];
	}
	return total_error;
}