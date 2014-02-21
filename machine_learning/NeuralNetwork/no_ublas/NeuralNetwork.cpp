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
	input_data.resize(4);
	label_data.resize(4);
	for(int i = 0; i < input_data.size(); i++){
		input_data[i].resize(2);
	}
	for(int i = 0; i < label_data.size(); i++){
		label_data[i].resize(1);
	}

	input_data[0][0] = 0.0;
	input_data[0][1] = 0.0;
	input_data[1][0] = 0.0;
	input_data[1][1] = 1.0;
	input_data[2][0] = 1.0;
	input_data[2][1] = 0.0;
	input_data[3][0] = 1.0;
	input_data[3][1] = 1.0;

	label_data[0][0] = 0.0;
	label_data[1][0] = 1.0;
	label_data[2][0] = 1.0;
	label_data[3][0] = 0.0;
}

void generete_xor_many(std::vector<ublas::vector<double> >& input_data,std::vector<ublas::vector<double> >& label_data){
	int size = 6;
	input_data.resize(pow(2,size));
	label_data.resize(pow(2,size));
	for(int i = 0; i < input_data.size(); i++){
		input_data[i].resize(size);
	}
	for(int i = 0; i < label_data.size(); i++){
		label_data[i].resize(1);
	}
	int cnt = 0;
	for(int a = 0; a < 2; a++){
		for(int b = 0; b < 2; b++){
			for(int c = 0; c < 2; c++){
				for(int d = 0; d < 2; d++){
					for(int e = 0; e < 2; e++){
						for(int f = 0; f < 2; f++){
							input_data[cnt][0] = a;
							input_data[cnt][1] = b;
							input_data[cnt][2] = c;
							input_data[cnt][3] = d;
							input_data[cnt][4] = e;
							input_data[cnt][5] = f;
							if(a ^ b ^ c ^ d ^ e ^ f){
								label_data[cnt][0] = 1;
							}else{
								label_data[cnt][0] = 0;
							}
							cnt++;
						}
					}
				}
			}
		}
	}
}

void generete_and(std::vector<ublas::vector<double> >& input_data,std::vector<ublas::vector<double> >& label_data){
	input_data.resize(4);
	label_data.resize(4);
	for(int i = 0; i < input_data.size(); i++){
		input_data[i].resize(2);
	}
	for(int i = 0; i < label_data.size(); i++){
		label_data[i].resize(1);
	}

	input_data[0][0] = 0.0;
	input_data[0][1] = 0.0;
	input_data[1][0] = 0.0;
	input_data[1][1] = 1.0;
	input_data[2][0] = 1.0;
	input_data[2][1] = 0.0;
	input_data[3][0] = 1.0;
	input_data[3][1] = 1.0;

	label_data[0][0] = 0.0;
	label_data[1][0] = 0.0;
	label_data[2][0] = 0.0;
	label_data[3][0] = 1.0;
}

int main(int argc, char const *argv[])
{
	NeuralNetwork nn;
	nn.SetParameter(1,60,300000);
	std::vector<ublas::vector<double> > input_data;
	std::vector<ublas::vector<double> > label_data;
	generete_xor_many(input_data,label_data);

	nn.Training(input_data,label_data);
	nn.Check(input_data,label_data);
	return 0;
}