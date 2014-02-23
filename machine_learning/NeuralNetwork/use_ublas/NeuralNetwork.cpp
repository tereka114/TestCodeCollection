#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <vector>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "NeuralNetwork.hpp"
#define NDEBUG

using namespace boost::numeric::ublas;

//andのテストデータを生成する。
void generete_xor(std::vector<ublas::vector<double> >& input_data,std::vector<ublas::vector<double> >& label_data){
	int bit = 3;
	int cnt = 1 << bit;

	for(int i = 0; i < cnt; i++){
		ublas::vector<double> temp_data(bit);
		ublas::vector<double> temp_label(1);
		for(int j = 0; j < bit; j++){
			temp_data[j] = ((i & (1 << j)) != 0) ? 1 : 0;
		}

		int flag = temp_data[0];
		for(int j = 1; j < bit; j++){
			flag = flag ^ (int)temp_data[j];
		}
		temp_label[0] = (double)flag;

		input_data.push_back(temp_data);
		label_data.push_back(temp_label);
	}
}

void generete_and(std::vector<ublas::vector<double> >& input_data,std::vector<ublas::vector<double> >& label_data){
	int bit = 3;
	int cnt = 1 << bit;

	for(int i = 0; i < cnt; i++){
		ublas::vector<double> temp_data(bit);
		ublas::vector<double> temp_label(1);
		for(int j = 0; j < bit; j++){
			temp_data[j] = ((i & (1 << j)) != 0) ? 1 : 0;
		}

		int flag = temp_data[0];
		for(int j = 1; j < bit; j++){
			flag = flag & (int)temp_data[j];
		}
		temp_label[0] = (double)flag;

		input_data.push_back(temp_data);
		label_data.push_back(temp_label);
	}
}

int main(int argc, char const *argv[])
{
	NeuralNetwork nn;
	nn.SetParameter(2,100,300000);
	std::vector<ublas::vector<double> > input_data;
	std::vector<ublas::vector<double> > label_data;
	generete_and(input_data,label_data);

	nn.Training(input_data,label_data);
	nn.Check(input_data,label_data);
	return 0;
}