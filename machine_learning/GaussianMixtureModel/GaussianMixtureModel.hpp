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

#define NDEBUG

using namespace boost::numeric;
using namespace std;

template<class X>
class GaussianDistribution{
public:
	ublas::vector<X> mean;
	ublas::matrix<X> cov;
	ublas::matrix<X> inv_cov;
	X det;
};

class GaussianMixtureModel{
public:
	GaussianDistribution<double> distribution[100]; //Another
	std::vector<ublas::vector<double> > input_data;
	int N; //データの数
	
	double pi[100];
	int distribution_number; //正規分布の数
	int dimension;
	std::vector<std::vector<double> > gamma;

	//計算用データ保存位置
	double gaussian_temp1;

	void SetParameter(int number);
	double Likelihood();
	void Training(std::vector<ublas::vector<double> > &input);
	void Scale();
	void OutPutParam();
	ublas::vector<int> Test(std::vector<ublas::vector<double> > &test_data);
	void Setting();
	void ReCalcParam();
	double Gaussian(ublas::vector<double> &data,ublas::vector<double> &mean,ublas::matrix<double> &cov,matrix<double> &inv_cov,double det);
	void Check(double x);
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
//多変量正規分布の式、高速化を目指してメモを中心に
double GaussianMixtureModel::Gaussian(ublas::vector<double> &data,ublas::vector<double> &mean,ublas::matrix<double> &cov,matrix<double>& inv_cov,double det){
	ublas::vector<double> minus_data_mean = data - mean;
	double temp1 = 1.0 / (pow((2.0 * M_PI),(dimension / 2.0)));
	double temp2 = 1.0 / pow(det,0.5);
	double temp3 = - 0.5 * inner_prod(prod(minus_data_mean,inv_cov),minus_data_mean);
	return temp1 * temp2 * exp(temp3);
}

void GaussianMixtureModel::SetParameter(int number){
	distribution_number = number;
}

void GaussianMixtureModel::Setting(){
	srand((unsigned)time(NULL)); 
	//混合係数の初期化
	double sum = 0.0;
	for(int i = 0; i < distribution_number; i++){
		 pi[i] = (double)rand()/RAND_MAX;
		 sum += pi[i];
	}

	for(int i = 0; i < distribution_number; i++){
		pi[i] /= sum;
		cout << pi[i] << endl;
	}

	//平均の初期化
	for(int i = 0; i < distribution_number; i++){
		ublas::vector<double> vect(dimension);

		for(int j = 0; j < dimension; j++){
			vect[j] = (double)rand()/RAND_MAX;
			vect[j] = 0.5;
		}
		distribution[i].mean = vect;
	}
	//共分散行列を初期化
	for(int i = 0; i < distribution_number; i++){
		ublas::matrix<double> mat(dimension,dimension);
		for(int j = 0; j < mat.size1(); j++){
			for(int k = 0; k < mat.size2(); k++){
				mat(j,k) = (j == k) ? 1.0 : 0.0;
			}
		}
		distribution[i].cov = mat;
	}
	gaussian_temp1 = 1.0 / (pow((2.0 * M_PI),(dimension / 2.0)));
}

void GaussianMixtureModel::ReCalcParam(){
	for(int i = 0; i < distribution_number;i++){
		distribution[i].det = determinant(distribution[i].cov);
		distribution[i].inv_cov = invert(distribution[i].cov);
		// cout << distribution[i].det << endl;
		// cout << distribution[i].inv_cov << endl;
		//Check(distribution[i].det);
	}
}

double GaussianMixtureModel::Likelihood(){
	//対数尤度関数
	double sum = 0.0;

	for(int i = 0; i < N; i++){
		double temp = 0.0;
		for(int j = 0; j < distribution_number; j++){
			// cout << input_data[i] << endl;
			// cout << distribution[j].mean << endl;
			// cout << distribution[j].cov << endl;
			temp += pi[j] * Gaussian(input_data[i],distribution[j].mean,distribution[j].cov,distribution[j].inv_cov,distribution[j].det);
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

void GaussianMixtureModel::Check(double x){
	if(isnan(x)){
		cout << "NaNがでまっせー" << endl;
		OutPutParam();
	}
}
void GaussianMixtureModel::Training(std::vector<ublas::vector<double> > &input){
	input_data = input;
	N = input_data.size();
	dimension = input_data[0].size();
	gamma.resize(N);
	for(int i = 0; i < N; i++){
		gamma[i].resize(dimension);
	}

	Setting();
	Scale();
	cout << input_data[0] << endl;
	ReCalcParam();
	double like = Likelihood();
	ublas::vector<double> vect_temp(input_data[0].size()); //
	ublas::vector<double> temp_gaussian(distribution_number); //各データの確率の計算結果を一時的に保存
	//OutPutParam();
	int cnt = 0;
	while(1){
		cout << cnt << " " << like << endl;
		//Estep
		ublas::vector<double> temp_vect(distribution_number); //計算結果を一時的に保存
		for(int i = 0; i < N;i++){
			double denominator = 0.0;
			for(int j = 0; j < distribution_number; j++){
				temp_gaussian[j] = Gaussian(input_data[i],distribution[j].mean,distribution[j].cov,distribution[j].inv_cov,distribution[j].det);
				denominator += pi[j] * temp_gaussian[j];
			}
			for(int j = 0; j < distribution_number; j++){
				gamma[i][j] = pi[j] * temp_gaussian[j] / denominator;
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

				ublas::matrix<double> mat(dimension,1);
				for(int j = 0; j < dimension; j++){
					mat(j,0) = vect_temp[j];
				}

				distribution[k].cov += gamma[i][k] * prod(mat,trans(mat));
			}
			distribution[k].cov = distribution[k].cov * (1/Nk);

			pi[k] = Nk / N;
		}
		//OutPutParam();
		cnt++;
		ReCalcParam();
		double new_like = Likelihood();
		double diff = new_like - like;
		if(cnt == 10){
			break;
		}
		like = new_like;
	}
}

ublas::vector<int> GaussianMixtureModel::Predict(std::vector<ublas::vector<double> > &test_data){
	ublas::vector<double> vect_temp;
	ublas::vector<int> result(test_data.size());
	ReCalcParam();

	for(int i = 0; i < test_data.size(); i++){
		double max = 0.0;
		int now_distribution = 0;
		for(int j = 0; j < distribution_number; j++){
			double per = pi[j] * Gaussian(test_data[i],distribution[j].mean,distribution[j].cov,distribution[j].inv_cov,distribution[j].det);
			if(per > max){max = per; now_distribution = j;}
		}
		result[i] = now_distribution;
	}
	return result;
}