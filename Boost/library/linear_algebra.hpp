#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/fwd.hpp>

#define NDEBUG
using namespace boost::numeric::ublas;

//行列の演算
matrix<double> linear_pow(matrix<double> &mat,int time){
	matrix<double> result(mat);

	for(int i = 0; i < time-1; i++){
		result = prod(result,mat);
	}
	return result;
}

//
matrix<double> inverse(matrix<double> &mat){
	matrix<double> lu(mat);
	permutation_matrix<> pm(mat.size1());
	matrix<double> B = identity_matrix<double>(mat.size1());

	lu_factorize(lu,pm);
	lu_substitute(lu,pm,B);

	return B;
}
//行列式
matrix<double> determinant(matrix<double> &mat){
	matrix<double> lu(mat);
	permutation_matrix<> pm(mat.size1());

	lu_factorize(lu,pm);
	std::cout << lu << std::endl;
	std::cout << pm << std::endl;

	double det(1);

	for (permutation_matrix<>::size_type i = 0; i < pm.size(); i++) {
		det *= (i == pm(i)) ? +lu(i, i) : -lu(i, i);
	}

	std::cout << det << std::endl;
}