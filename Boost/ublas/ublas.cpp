#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;

int main(void){
	matrix<double> mat(3,4); //3行4列
	vector<double> vect(4); //4次元ベクトル

	std::cout << mat << std::endl;
	std::cout << vect << std::endl;
	mat(1,2) = 100; //1行目の2列目
	mat(2,3) = 200; //2行目の3列目
	vect(1) = 30;
	vect(0) = 20;
	std::cout << mat << std::endl;
	std::cout << vect << std::endl;
}