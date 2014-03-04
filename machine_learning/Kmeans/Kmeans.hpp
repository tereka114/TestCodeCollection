#pragma once
#include <iostream>
#include <float.h>
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

	ublas::vector<double> mean;
	ublas::vector<double> std;
	int cluster;
	int dimension;
	int N;
	void SetParameter(int cluster_num);
	void Setting();
	void Scale();
	std::vector<ublas::vector<double> > TestScale(std::vector<ublas::vector<double> > &test_data);
	void PrintOutParameter();
	void Training(std::vector<ublas::vector<double> > &input);
	ublas::vector<int> Predict(std::vector<ublas::vector<double> > &test);
	void Gnuplot();
};

void Kmeans::SetParameter(int cluster_num){
	cluster = cluster_num;
}

void Kmeans::Setting(){
	srand((unsigned)time(NULL));
	for(int i = 0; i < cluster; i++){
		ublas::vector<double> vect_temp(dimension);
		for(int j = 0; j < dimension; j++){
			vect_temp[j] = ((double)rand()/RAND_MAX) - 0.5;
		}
		centroid.push_back(vect_temp);
	}
}
void Kmeans::Scale(){
	mean.resize(N);

	for(int i = 0; i < dimension; i++){
		double sum = 0.0;
		for(int j = 0; j < N; j++){
			sum += input_data[j][i];
		}
		//cout << vect_temp << endl;
		mean[i] = sum / N;
	}

	std.resize(dimension);

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

std::vector<ublas::vector<double> > Kmeans::TestScale(std::vector<ublas::vector<double> > &test_data){
	for(int i = 0; i < test_data[0].size(); i++){
		for(int j = 0; j < test_data.size(); j++){
			test_data[j][i] = (test_data[j][i] - mean[i]) / std[i];
		}
	}
	return test_data;
}

void Kmeans::PrintOutParameter(){
	for(int i = 0; i < cluster; i++){
		cout << centroid[i] << endl;
	}
}

void Kmeans::Training(std::vector<ublas::vector<double> > &input){
	cout << input.size() << endl;
	input_data = input;
	dimension = input[0].size();
	N = input_data.size();
	attribute.resize(N);

	Setting();
	Scale();
	//PrintOutParameter();

	double like; //指標
	double before_like = DBL_MAX; //新しい指標（更新先）
	int turn = 0;

	while(1){
		//E-step
		ublas::vector<double> min_distance(N);
		fill( min_distance.begin(), min_distance.end(),DBL_MAX );

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
	//Gnuplot();
}

ublas::vector<int> Kmeans::Predict(std::vector<ublas::vector<double> > &test){
	//cout << test[0] << endl;
	ublas::vector<int> result(test.size());
	std::vector<ublas::vector<double> > test_data = TestScale(test);

	for(int i = 0; i < test.size(); i++){
		double min_distance = DBL_MAX;
		int now_attribute = 0;
		for(int j = 0; j < cluster; j++){
			double temp_distance = euclidean_distance(test_data[i],centroid[j]);
			//cout << temp_distance << endl;
			if(temp_distance < min_distance){
				min_distance = temp_distance;
				now_attribute = j;
			}
		}
		result[i] = now_attribute;
	}
	return result;
}

void Kmeans::Gnuplot(){
	for(int i = 0; i < cluster; i++){
		for(int j = 0; j < N; j++){
			if(i == attribute[j]){
				cout << input_data[j][0] << " " << input_data[j][1] << endl;
			}
		}
		cout << endl;
	}
	for(int i = 0; i < N; i++){
		cout << attribute[i] << endl;
	}
}