#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../library/linear_algebra.hpp"
#define NDEBUG

using namespace boost::numeric::ublas;

double gaussian(vector<double> &data,vector<double> &mean,matrix<double> &cov){
	double temp1 = 1.0 / (pow((2 * M_PI),(data.size() / 2.0)));
	//std::cout << temp1 << std::endl;
	double temp2 = 1 / pow(determinant(cov),0.5);
	//std::cout << temp2 << std::endl;
	//std::cout << prod(data - mean,invert(cov)) << std::endl;
	double temp3 = - 0.5 * inner_prod(prod(data - mean,invert(cov)),data-mean);
	//std::cout << temp1 << " " << temp2 << " " << temp3 << std::endl;
	//std::cout << temp1 * temp2 * exp(temp3) << std::endl;
	return temp1 * temp2 * exp(temp3);
}