#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "../../Boost/stastics/gaussian.hpp"

#define NDEBUG

using namespace boost::numeric::ublas;

class GaussianDistribution{
public:
	vector<double> mean;
	matrix<double> cov;
};

class GaussianMixtureModel{
public:
	GaussianDistribution distribution[100]; //Another
	matrix<double> input_data;
	int N; //データの数

	double pi[100];
	int distribution_number; //正規分布の数
	double gamma[1000][100];

	void SetParameter(int number);
	double Likelihood();
	void Training(matrix<double> &input);
	void Scale();
	void OutPutParam();
	~GaussianMixtureModel(){};
};

void GaussianMixtureModel::Scale(){
	vector<double> mean(input_data.size2());
	for(int i = 0; i < input_data.size2(); i++){
		vector<double> vect_temp = column(input_data,i);
		//std::cout << vect_temp << std::endl;
		mean[i] = sum(vect_temp) / input_data.size1();
	}

	vector<double> std(input_data.size2());

	for(int i = 0; i < input_data.size2(); i++){
		double sum = 0.0;
		for(int j = 0; j < input_data.size1(); j++){
			sum += pow(input_data(j,i) - mean[i],2);
		}
		std[i] = sqrt(sum / input_data.size1());
	}

	for(int i = 0; i < input_data.size2(); i++){
		for(int j = 0; j < input_data.size1(); j++){
			input_data(j,i) = (input_data(j,i) - mean[i]) / std[i];
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
		vector<double> vect(distribution_number);

		for(int j = 0; j < distribution_number; j++){
			vect[j] = (double)rand()/RAND_MAX;
			vect[j] = 0.5;
		}
		distribution[i].mean = vect;
	}
	//共分散行列を初期化
	for(int i = 0; i < distribution_number; i++){
		matrix<double> mat(distribution_number,distribution_number);
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

	for(int i = 0; i < input_data.size1(); i++){
		double temp = 0.0;
		for(int j = 0; j < distribution_number; j++){
			vector<double> vect_temp = row(input_data,i);
			temp += pi[j] * gaussian(vect_temp,distribution[j].mean,distribution[j].cov);
		}
		sum += log(temp);
		// std::cout << log(temp) << std::endl;
		// std::cout << "result" << std::endl;
		// std::cout << sum << std::endl;
	}

	return sum;
}
void GaussianMixtureModel::OutPutParam(){
	for(int i = 0; i < distribution_number; i++){
		std::cout << pi[i] << std::endl;
		std::cout << distribution[i].mean << std::endl;
		std::cout << distribution[i].cov << std::endl;
	}
}
void GaussianMixtureModel::Training(matrix<double> &input){
	input_data = input;
	Scale();
	N = input_data.size1();
	double like = Likelihood();
	//OutPutParam();
	matrix<double> cov(input_data.size2(),input_data.size2());
	vector<double> vect_temp;
	while(1){
		std::cout << like << std::endl;
		//Estep
		for(int i = 0; i < N;i++){
			double denominator = 0.0;
			for(int j = 0; j < distribution_number; j++){
				vect_temp = row(input_data,i);
				denominator += pi[j] * gaussian(vect_temp,distribution[j].mean,distribution[j].cov);
			}
			for(int j = 0; j < distribution_number; j++){
				vect_temp = row(input_data,i);
				gamma[i][j] = pi[j] * gaussian(vect_temp,distribution[j].mean,distribution[j].cov) / denominator;
				//std::cout << gamma[i][j] << std::endl;
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
			//std::cout << "amen" << " " << Nk << std::endl;

			for(int i = 0; i < N; i++){
				distribution[k].mean += gamma[i][k] * row(input_data,i);
			}
			distribution[k].mean /= Nk;
			//std::cout << "amen" << " " << distribution[k].mean << std::endl;


			for(int i = 0; i < N; i++){
				vect_temp = row(input_data,i) - distribution[k].mean;

				matrix<double> mat(vect_temp.size(),1);
				for(int j = 0; j < vect_temp.size(); j++){
					mat(j,0) = vect_temp[j];
				}

				distribution[k].cov += gamma[i][k] * prod(mat,trans(mat));
			}
			distribution[k].cov = distribution[k].cov * (1/Nk);

			pi[k] = Nk / N;
		}
		OutPutParam();

		double new_like = Likelihood();
		double diff = new_like - like;
		like = new_like;
	}
}