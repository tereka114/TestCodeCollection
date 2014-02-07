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

using namespace boost::numeric;

void fileread(std::vector<ublas::vector<double> > &input){
	std::ifstream ifs("faithful.txt");
	std::string str;
	ublas::vector<double> vect(2);
	while(getline(ifs,str)){
		std::vector<std::string> v;

		boost::algorithm::split(v, str, boost::is_any_of(" "));
		for(int i = 0; i < v.size();i++){
			std::stringstream ss;
			double temp;
			ss << v[i];
			ss >> temp;
			vect[i] = temp;
		}
		input.push_back(vect);
	}
}

int main(void){
	GaussianMixtureModel gmm;
	gmm.SetParameter(2);
	std::vector<ublas::vector<double> > input;
	fileread(input);
	gmm.Training(input);
}