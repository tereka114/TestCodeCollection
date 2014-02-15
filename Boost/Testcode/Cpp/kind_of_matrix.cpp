#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

using namespace boost::numeric;
using namespace std;

int main(int argc, char const *argv[])
{
	ublas::matrix<double> mat0(3,3);
	ublas::zero_matrix<double> mat1(3,3);
	ublas::identity_matrix<double> mat2(3,3);
	ublas::symmetric_matrix<double> mat3(3,3);
	ublas::banded_matrix<double> mat4(4,5);
	ublas::diagonal_matrix<double> mat5(3,3);

	//対称行列の初期化
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			mat3(i,j) = j;
		}
	}
	//密行列の初期化
	mat4(0,0) = 100; mat4(1,1) = 200; mat4(2,2) = 150; mat4(3,3) = 200;

	//対角行列の初期化
	mat5(0,0) = 100; mat5(1,1) = 200; mat5(2,2) = 300;mat5(0,1) =100;

	//全ての行列を出力
	cout << mat0 << endl;
	cout << mat1 << endl;
	cout << mat2 << endl;
	cout << mat3 << endl;
	cout << mat4 << endl;
	cout << mat5 << endl;

	return 0;
}