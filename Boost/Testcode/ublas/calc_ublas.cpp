#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#define NDEBUG

using namespace boost::numeric::ublas;

int main(void){
	//初期化
	matrix<double> mat1(2,2);
	mat1(0,0) = 2.0; mat1(0,1) = 1.0;
	mat1(1,0) = 3.0; mat1(1,1) = 4.0;

	matrix<double> mat2(2,2);
	mat2(0,0) = 4.0; mat2(0,1) = 3.0;
	mat2(1,0) = 2.0; mat2(1,1) = 1.0;

	vector<double> vec1(2);
	vec1(0) = 2.0; vec1(1) = 3.0;

	vector<double> vec2(2);
	vec2(0) = 4.0; vec2(1) = 5.0;
	//一応
	std::cout << "行列の内容を出力" << std::endl;
	std::cout << mat1 << std::endl;
	std::cout << mat2 << std::endl;
	std::cout << vec1 << std::endl;
	std::cout << vec2 << std::endl;

	//和
	std::cout << "和" << std::endl;
	std::cout << mat1 + mat2 << std::endl;
	std::cout << vec1 + vec2 << std::endl;

	//差
	std::cout << "差" << std::endl;
	std::cout << mat1 - mat2 << std::endl;
	std::cout << vec1 - vec2 << std::endl;
	//積
	std::cout << "積" << std::endl;
	std::cout << prod(mat1,mat2) << std::endl;
	//定数倍
	std::cout << "定数倍" << std::endl;
	std::cout << 5 * mat1 << std::endl;

	std::cout << "定数で割る" << std::endl;
	std::cout << mat1 / 5 << std::endl;
}