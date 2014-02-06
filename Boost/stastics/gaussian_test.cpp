#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "gaussian.hpp"

using namespace boost::numeric::ublas;

int main(void){
	vector<double> data(2);
	vector<double> mean(2);
	matrix<double> cov(2,2);

	data(0) = 0.4; data(1) = 0.6;
	mean(0) = 0.7; mean(1) = 0.3;
	cov(0,0) = 0.4; cov(0,0) = 0.3;
	cov(1,0) = 0.3; cov(1,1) = 0.9;

	double a = gaussian(data,mean,cov);
}