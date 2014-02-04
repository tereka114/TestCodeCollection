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
	void Update(vector<double>& label);
};

void OutputLayer::Update(vector<double>& label){
	old_weight = weight;
	for(int i = 0; i < data_output.size(); i++){
		double p = (label[i] - data_output[i]) * (1 - data_output[i]) * data_output[i];
		for(int j = 0; j < input.size();j++){
			error(i,j) = p * input[j];
			weight(i,j) +=  coefficient * error(i,j);
		}
	}
}