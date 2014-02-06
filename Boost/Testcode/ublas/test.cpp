#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

using namespace boost::numeric::ublas;

int main(void){
	matrix<double> mat1(3,2);
	mat1(0,0) = 2.0; mat1(0,1) = 1.0;
	mat1(1,0) = 3.0; mat1(1,1) = 4.0;
	mat1(2,0) = 5.0; mat1(2,1) = 6.0;

	vector<double> vect = column(mat1,0);
	std::cout << vect << std::endl;
	std::cout << mat1.size1() << std::endl;
	std::cout << mat1.size2() << std::endl;

	return 0;
}

