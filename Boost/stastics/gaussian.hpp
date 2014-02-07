#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../library/linear_algebra.hpp"
#define NDEBUG

using namespace boost::numeric;

double gaussian(ublas::vector<double> &data,ublas::vector<double> &mean,ublas::matrix<double> &cov){
	double temp1 = 1.0 / (pow((2 * M_PI),(data.size() / 2.0)));
	double temp2 = 1 / pow(determinant(cov),0.5);
	double temp3 = - 0.5 * inner_prod(prod(data - mean,invert(cov)),data-mean);
	return temp1 * temp2 * exp(temp3);
}

double gaussian(ublas::vector<double> &data,ublas::vector<double> &mean,ublas::matrix<double> &cov,matrix<double> inv_cov,double det){
	double temp1 = 1.0 / (pow((2 * M_PI),(data.size() / 2.0)));
	double temp2 = 1 / pow(det,0.5);
	double temp3 = - 0.5 * inner_prod(prod(data - mean,inv_cov),data-mean);
	return temp1 * temp2 * exp(temp3);
}