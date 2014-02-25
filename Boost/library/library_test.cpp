#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "./distance.hpp"

using namespace std;
using namespace boost::numeric;

int main(void){
	int size = 3;
	ublas::vector<double> vect1_data1(size);
	ublas::vector<double> vect1_data2(size);

	ublas::vector<double> vect2_data1(size);
	ublas::vector<double> vect2_data2(size);

	for(int i = 0; i < size; i++){
		vect1_data1[i] = i;
		vect1_data2[i] = i;
	}
	for(int i = 0; i < 4; i++){
		cout << histogram_distance(vect1_data1,vect1_data2,i) << endl;
	}
}