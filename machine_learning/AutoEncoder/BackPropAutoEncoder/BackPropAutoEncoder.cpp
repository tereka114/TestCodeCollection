#include <iostream>
#include <vector>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "./BackPropAutoEncoder.hpp"

using namespace std;

int main(void){
	BackPropAutoEncoder bpae;
	std::vector<double> reduce_dimensions(2);
	reduce_dimensions[0] = 2;
	reduce_dimensions[1] = 1;

 	std::vector<double> training_time(2);
 	training_time[0] = 1000000;
 	training_time[1] = 1000000;

	bpae.SetParameter(reduce_dimensions,training_time);
}