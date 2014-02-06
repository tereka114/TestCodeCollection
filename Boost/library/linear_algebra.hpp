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

template <class M>
matrix<double> invert(const M& m)
{
    namespace ublas = boost::numeric::ublas;

    BOOST_UBLAS_CHECK(m.size1() == m.size2(), external_logic());

    matrix<double>       lhs(m);
    matrix<double>       rhs(identity_matrix<double>(m.size1()));
    permutation_matrix<> pm(m.size1());

    lu_factorize(lhs, pm);
    lu_substitute(lhs, pm, rhs);

    BOOST_UBLAS_CHECK(rhs.size1() == m.size1(), internal_logic());
    BOOST_UBLAS_CHECK(rhs.size2() == m.size2(), internal_logic());

#if BOOST_UBLAS_TYPE_CHECK
    BOOST_UBLAS_CHECK(
        detail::expression_type_check(
            prod(m, rhs),
            identity_matrix<typename M::value_type>(m.size1())
        ),
        internal_logic()
    );
#endif
    matrix<double> mi;
    mi.resize(rhs.size1(), rhs.size2(), false);
    mi.assign(rhs);
    // mi.assign_temporary(rhs);
    return mi;
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