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
#include "./Kmeans.hpp"

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
	Kmeans kmeans;
	std::vector<ublas::vector<double> > input;
	fileread(input);
	kmeans.SetParameter(2);
	kmeans.Training(input);
	//ublas::vector<int> result = kmeans.Test(input);
	//cout << result << endl;
}