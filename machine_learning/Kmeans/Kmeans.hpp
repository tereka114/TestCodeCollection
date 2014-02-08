#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "../../Boost/Library/linear_algebra.hpp"
#include "../../Boost/Library/distance.hpp"

#define NDEBUG

using namespace boost::numeric;
using namespace std;

class Kmeans{
public:
	std::vector<ublas::vector<double> > input_data;
	std::vector<ublas::vector<double> > centroid;
	std::vector<double> attribute;
	int cluster;
	int dimension;
	int N;
	void SetParameter(int cluster_num);
	void Setting();
	void Scale();
	void PrintOutParameter();
	void Training(std::vector<ublas::vector<double> > &input);
	ublas::vector<int> Test(std::vector<ublas::vector<double> > &test);
};

void Kmeans::SetParameter(int cluster_num){
	cluster = cluster_num;
}

void Kmeans::Setting(){
	srand((unsigned)time(NULL));
	for(int i = 0; i < cluster; i++){
		ublas::vector<double> vect_temp(dimension);
		for(int j = 0; j < dimension; j++){
			vect_temp[j] = (double)rand()/RAND_MAX;
		}
		centroid.push_back(vect_temp);
	}
}
void Kmeans::Scale(){
	ublas::vector<double> mean(N);

	for(int i = 0; i < dimension; i++){
		double sum = 0.0;
		for(int j = 0; j < N; j++){
			sum += input_data[j][i];
		}
		//cout << vect_temp << endl;
		mean[i] = sum / N;
	}

	ublas::vector<double> std(dimension);

	for(int i = 0; i < dimension; i++){
		double sum = 0.0;
		for(int j = 0; j < N; j++){
			sum += pow(input_data[j][i] - mean[i],2);
		}
		std[i] = sqrt(sum / N);
	}

	for(int i = 0; i < dimension; i++){
		for(int j = 0; j < N; j++){
			input_data[j][i] = (input_data[j][i] - mean[i]) / std[i];
		}
	}
}

void Kmeans::PrintOutParameter(){
	for(int i = 0; i < cluster; i++){
		cout << centroid[i] << endl;
	}
}
void Kmeans::Training(std::vector<ublas::vector<double> > &input){
	input_data = input;
	dimension = input[0].size();
	N = input_data.size();
	attribute.resize(N);

	Setting();
	Scale();
	PrintOutParameter();

	double like; //指標
	double before_like = 10000000; //新しい指標（更新先）
	int turn = 0;
	while(1){
		//E-step
		ublas::vector<double> min_distance(N);
		fill( min_distance.begin(), min_distance.end(), 100000000 );

		for(int i = 0; i < cluster; i++){
			for(int j = 0; j < N; j++){
				double temp_distance = euclidean_distance(input_data[j],centroid[i]);
				if(temp_distance < min_distance[j]){
					min_distance[j] = temp_distance;
					attribute[j] = i;
				}
			}
		}

		like = sum(min_distance);
		// for(int i = 0; i < N; i++){
		// 	cout << attribute[i] << endl;
		// }

		std::vector<ublas::vector<double> > total_distance(cluster);
		for(int i = 0; i < cluster; i++){
			ublas::vector<double> vect_temp(dimension);
			for(int i = 0; i < vect_temp.size(); i++){
				vect_temp[i] = 0.0;
			}
			total_distance[i] = vect_temp;
		}
		ublas::vector<float> total_attribute(cluster);
		for(int i = 0; i < total_attribute.size(); i++){
			total_attribute[i] = 0.0;
		}

		//M-step
		for(int i = 0; i < N; i++){
			//cout << total_distance[attribute[i]] << " " << input_data[i] << endl;
			total_distance[attribute[i]] = total_distance[attribute[i]] + input_data[i];
			total_attribute[attribute[i]] += 1;
		}
		// for(int i = 0; i < cluster; i++){
		// 	cout << total_distance[i] << endl;
		// }

		for(int i = 0; i< cluster; i++){
			centroid[i] = total_distance[i] * (1.0 / (float)total_attribute[i]);
		}
		//PrintOutParameter();
		cout << like << endl;
		if(before_like - like < 0.01){
			break;
		}
		before_like = like;
	}
}

ublas::vector<int> Kmeans::Test(std::vector<ublas::vector<double> > &test){
	ublas::vector<int> result(test.size());

	for(int i = 0; i < test.size(); i++){
		double min_distance = 10000000;
		int now_attribute = 0;
		for(int j = 0; j < cluster; j++){
			double temp_distance = euclidean_distance(test[i],centroid[j]);
			cout << temp_distance << endl;
			if(temp_distance < min_distance){
				min_distance = temp_distance;
				now_attribute = j;
			}
		}
		result[i] = now_attribute;
	}
	return result;
}