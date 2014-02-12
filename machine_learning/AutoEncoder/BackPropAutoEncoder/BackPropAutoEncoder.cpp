#include <iostream>
#include <vector>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "./BackPropAutoEncoder.hpp"

using namespace std;

int main(void){
	BackPropAutoEncoder bpae;
	std::vector<int> reduce_dimensions(2);
	reduce_dimensions[0] = 2;
	reduce_dimensions[1] = 1;

 	std::vector<int> training_time(2);
 	training_time[0] = 1000000;
 	training_time[1] = 1000000;

	bpae.SetParameter(reduce_dimensions,training_time);

	std::vector<ublas::vector<double> > test_data;
	ublas::vector<double> test(4);

	test[0] = 0.3;
	test[1] = 0.6;
	test[2] = 0.8;
	test[3] = 0.5;

	test_data.push_back(test);

	bpae.Training(test_data);
}