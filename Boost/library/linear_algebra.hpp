#pragma once
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

#undef BOOST_UBLAS_TYPE_CHECK
#define BOOST_UBLAS_TYPE_CHECK 0

template <typename T>
matrix<T> invert(const matrix<T>& m)
throw (std::invalid_argument) {
    typedef matrix<T> m_t;
    m_t a(m);
    m_t b(identity_matrix<T>(m.size1()));
    permutation_matrix<std::size_t> pm(m.size1());
 
    if (lu_factorize(a,pm) != 0)
        throw std::invalid_argument(std::string("No inverse exists."));
 
    lu_substitute(a,pm,b);
 
    return b;
}

//行列式
double determinant(matrix<double> &mat){
	matrix<double> lu(mat);
	permutation_matrix<> pm(mat.size1());

	lu_factorize(lu,pm);

	double det(1);

	for (permutation_matrix<>::size_type i = 0; i < pm.size(); i++) {
		det *= (i == pm(i)) ? +lu(i, i) : -lu(i, i);
	}

	return det;
}