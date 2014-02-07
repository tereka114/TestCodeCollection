#include <iostream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "../../Boost/stastics/gaussian.hpp"

#define NDEBUG

using namespace boost::numeric;
using namespace std;

class GaussianDistribution{
public:
	ublas::vector<double> mean;
	ublas::matrix<double> cov;
};

class GaussianMixtureModel{
public:
	GaussianDistribution distribution[100]; //Another
	std::vector<ublas::vector<double> > input_data;
	int N; //データの数

	double pi[100];
	int distribution_number; //正規分布の数
	int dimension;
	double gamma[1000][100];

	void SetParameter(int number);
	double Likelihood();
	void Training(std::vector<ublas::vector<double> > &input);
	void Scale();
	void OutPutParam();
	void Test(std::vector<ublas::vector<double> > &test_data);
	~GaussianMixtureModel(){};
};

void GaussianMixtureModel::Scale(){
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

void GaussianMixtureModel::SetParameter(int number){
	distribution_number = number;
	double per = 1.0 / distribution_number;
	//混合係数の初期化
	// for(int i = 0; i < distribution_number; i++){
	// 	pi[i] = per;
	// }
	pi[0] =0.4;
	pi[1] = 0.6;

	//平均の初期化
	srand((unsigned)time(NULL)); 
	for(int i = 0; i < distribution_number; i++){
		ublas::vector<double> vect(distribution_number);

		for(int j = 0; j < distribution_number; j++){
			vect[j] = (double)rand()/RAND_MAX;
			vect[j] = 0.5;
		}
		distribution[i].mean = vect;
	}
	//共分散行列を初期化
	for(int i = 0; i < distribution_number; i++){
		ublas::matrix<double> mat(distribution_number,distribution_number);
		for(int j = 0; j < mat.size1(); j++){
			for(int k = 0; k < mat.size2(); k++){
				mat(j,k) = (j == k) ? 1.0 : 0;
			}
		}
		distribution[i].cov = mat;
	}
}

double GaussianMixtureModel::Likelihood(){
	//対数尤度関数
	double sum = 0.0;

	for(int i = 0; i < N; i++){
		double temp = 0.0;
		for(int j = 0; j < distribution_number; j++){
			temp += pi[j] * gaussian(input_data[i],distribution[j].mean,distribution[j].cov);
		}
		sum += log(temp);
	}

	return sum;
}
void GaussianMixtureModel::OutPutParam(){
	for(int i = 0; i < distribution_number; i++){
		cout << pi[i] << endl;
		cout << distribution[i].mean << endl;
		cout << distribution[i].cov << endl;
	}
}
void GaussianMixtureModel::Training(std::vector<ublas::vector<double> > &input){
	input_data = input;
	N = input_data.size();
	dimension = input_data[0].size();

	Scale();
	double like = Likelihood();
	ublas::vector<double> vect_temp(input_data[0].size());
	//OutPutParam();
	int cnt = 0;
	while(1){
		cout << cnt << " " << like << endl;
		//Estep
		for(int i = 0; i < N;i++){
			double denominator = 0.0;
			for(int j = 0; j < distribution_number; j++){
				denominator += pi[j] * gaussian(input_data[i],distribution[j].mean,distribution[j].cov);
			}
			for(int j = 0; j < distribution_number; j++){
				gamma[i][j] = pi[j] * gaussian(input_data[i],distribution[j].mean,distribution[j].cov) / denominator;
			}
		}

		//MStep
		for(int k = 0; k < distribution_number; k++){
			double Nk = 0.0;
			for(int i = 0; i < N; i++){
				Nk += gamma[i][k];
			}

			for(int i = 0; i < distribution_number; i++){
				distribution[k].mean[i] = 0.0;
			}

			for(int i = 0; i < N; i++){
				distribution[k].mean += gamma[i][k] * input_data[i];
			}
			distribution[k].mean /= Nk;


			for(int i = 0; i < N; i++){
				vect_temp = input_data[i] - distribution[k].mean;

				ublas::matrix<double> mat(vect_temp.size(),1);
				for(int j = 0; j < vect_temp.size(); j++){
					mat(j,0) = vect_temp[j];
				}

				distribution[k].cov += gamma[i][k] * prod(mat,trans(mat));
			}
			distribution[k].cov = distribution[k].cov * (1/Nk);

			pi[k] = Nk / N;
		}
		//OutPutParam();
		cnt++;
		double new_like = Likelihood();
		double diff = new_like - like;
		like = new_like;
	}
}

void GaussianMixtureModel::Test(std::vector<ublas::vector<double> > &test_data){
	ublas::vector<double> vect_temp;

	for(int i = 0; i < test_data.size(); i++){
		double max = 0.0;
		int now_distribution = 0;
		for(int j = 0; j < distribution_number; j++){
			double per = pi[j] * gaussian(test_data[i],distribution[j].mean,distribution[j].cov);
			if(per > max){max = per; now_distribution = j;}
		}
	}
}