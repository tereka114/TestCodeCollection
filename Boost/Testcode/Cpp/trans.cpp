#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/triangular.hpp>

using namespace boost::numeric;
using namespace std;

int main(int argc, char const *argv[])
{
	ublas::matrix<double> mat1(2,2);
	mat1(0,0) = 1; mat1(0,1) = 2;
	mat1(1,0) = 4; mat1(1,1) = 3;

	cout << mat1 << endl;
	//転置
	cout << trans(mat1) << endl;

	//1行目を抽出
	cout << row(mat1,1) << endl;
	//1列目を抽出
	cout << column(mat1,1) << endl;
	ublas::vector<double> vect =  row(mat1,1);
	cout << vect << endl;
	//りさいず
	mat1.resize(3,3);
	cout << mat1 << endl;

	//クリア
	mat1.clear();
	cout << mat1 << endl;
	return 0;
}