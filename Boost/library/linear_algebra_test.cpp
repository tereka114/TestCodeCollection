#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include "linear_algebra.hpp"
#define NDEBUG
using namespace boost::numeric::ublas;

int main(void){
	matrix<double> mat1(2,2);
	mat1(0,0) = 0; mat1(0,1) = 2;
	mat1(1,0) = 1; mat1(1,1) = 0;
	matrix<double> result = linear_pow(mat1,2);

	std::cout << result << std::endl;

	matrix<double> mat2(2,2);
	mat2(0,0) = 2; mat2(0,1) = 1;
	mat2(1,0) = -1; mat2(1,1) = 3;

	determinant(mat2);

	matrix<double> mat3(2,2);
	mat3(0,0) = 1; mat3(0,1) = 2;
	mat3(1,0) = 2; mat3(1,1) = 5;

	inverse(mat3);
}