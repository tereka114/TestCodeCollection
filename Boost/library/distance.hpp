#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric;

template <typename T>
double euclidean_distance(ublas::vector<T> &data1,ublas::vector<T> &data2){
	double distance = 0.0;
	for(int i = 0; i < data1.size(); i++){
		distance += pow(data1[i] - data2[i],2);
	}
	return distance;
}

template <typename T>
double histogram_intersection(ublas::vector<T> &data1,ublas::vector<T> &data2){
	double total1,total2 = 0.0;
	std::cout << data1 << std::endl;
	std::cout << data2 << std::endl;
	for(int i = 0; i < data1.size(); i++){
		double temp = (data1[i] < data2[i]) ? data1[i] : data2[i];
		total1 += temp;
		total2 += data1[i];
	}
	//return total1;
	return total1 / total2;
}

template <typename T>
double histogram_chisqr(ublas::vector<T> &data1,ublas::vector<T> &data2){
	double total = 0.0;
	for(int i = 0; i < data1.size(); i++){
		if(data1[i] == 0){
			total += 0.0;
		}else{
			total += pow(data1[i] - data2[i],2.0) / (data1[i]);
		}
	}
	return total;
}

template<typename T>
double histogram_correl(ublas::vector<T> &data1,ublas::vector<T> &data2){
	double data1_mean = 0.0;
	double data2_mean = 0.0;
	for(int i = 0; i < data1.size(); i++){
		data1_mean += data1[i];
		data2_mean += data2[i];
	}
	data1_mean /= data1.size();
	data2_mean /= data2.size();

	double result;
	double data1_total = 0.0;
	double data2_total = 0.0;

	for(int i =0; i < data1.size(); i++){
		double data1_temp = data1[i] - data1_mean;
		double data2_temp = data2[i] - data2_mean;
		result += data1_temp * data2_temp;
		data1_total += pow(data1_temp,2.0);
		data2_total += pow(data2_temp,2.0);
	}
	return result / sqrt(data1_total * data2_total);
}

//怪しい
template<typename T>
double histogram_bhattacharyya(ublas::vector<T> &data1,ublas::vector<T> &data2){
	double data1_mean = 0.0;
	double data2_mean = 0.0;
	double temp = 0.0;
	for(int i = 0; i < data1.size(); i++){
		temp += data1[i] * data2[i];
		data1_mean += data1[i];
		data2_mean += data2[i];
	}
	data1_mean /= data1.size();
	data1_mean /= data2.size();

	return 1.0 - (1 / (data1_mean * data2_mean * pow(data1.size(),2.0)) * temp);
}

template <typename T>
double histogram_distance(ublas::vector<T> &data1,ublas::vector<T> &data2,int num){
	double ans = 0.0;
	switch(num){
		case 0:
			ans = histogram_intersection(data1,data2);
			break;
		case 1:
			ans = histogram_chisqr(data1,data2);
			break;
		case 2:
			ans = histogram_correl(data1,data2);
			break;
		case 3:
			ans = histogram_bhattacharyya(data1,data2);
			break;
	}
	return ans;
}