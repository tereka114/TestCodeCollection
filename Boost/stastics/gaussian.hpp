#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../linear_algebra.hpp"

double gaussian(vector<double> &data,vector<double> &mean,vector<matrix> &cov){
	double temp1 = 1 / pow((1 / math.pi),(data.size() / 2.0);
	double temp2 = 1 / pow(determinant(cov),0.5);
	double temp3 = - 0.5 * prod(prod(data - mean,inverse(cov)),data-mean);
	return temp1 * temp2 * exp(temp3);
}