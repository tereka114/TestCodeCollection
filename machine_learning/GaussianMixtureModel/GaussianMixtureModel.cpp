#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/algorithm/string.hpp>
#include "./GaussianMixtureModel.hpp"

using namespace boost::numeric::ublas;

matrix<double> fileread(){
	std::ifstream ifs("faithful.txt");
	std::string str;
	std::vector<std::vector<double> > temp_matrix;
	while(getline(ifs,str)){
		std::vector<double> vect;
		std::vector<std::string> v;

		boost::algorithm::split(v, str, boost::is_any_of(" "));
		for(int i = 0; i < v.size();i++){
			std::stringstream ss;
			double temp;
			ss << v[i];
			ss >> temp;
			vect.push_back(temp);
		}
		temp_matrix.push_back(vect);
	}

	matrix<double> result(temp_matrix.size(),temp_matrix[0].size());

	for(int i = 0; i < temp_matrix.size(); i++){
		for(int j = 0; j < temp_matrix[i].size(); j++){
			result(i,j) = temp_matrix[i][j];
		}
	}
	return result;
}

int main(void){
	GaussianMixtureModel gmm;
	gmm.SetParameter(2);
	matrix<double> input = fileread();
	gmm.Training(input);
}