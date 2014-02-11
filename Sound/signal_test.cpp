#include <iostream>
#include "./signal.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

int main(void){
	ublas::matrix<double> filter_bank(20,256);
	mel_filter_bank(16000,512,20,filter_bank);
}