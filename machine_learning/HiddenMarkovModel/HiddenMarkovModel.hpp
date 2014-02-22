#include <iostream>
#include <vector>

using namespace std;

class MalkovStatus{
	vector<double> translation_prob;
	vector<double> prob;
};

void MalkovStatus::RandomVector(int number){
	vector<double> temp_vect(number);
	double total = 0.0;

	for(int i = 0; i < status_num; i++){
		temp_vect[i] = (double)rand()/RAND_MAX - 0.5;
		total += temp_vect[i];
	}

	for(int i = 0; i < status_num; i++){
		temp_vect[i] /= total;
	}
	return temp_vect;
}

void MalkovStatus::SetParameter(int status_num,int prob_num){
	translation_prob = RandomVector(status_num);
	prob = RandomVector(prob_num);
}

//入力として与えられたとき
void MalkovStatus::SetParameter(vector<double>,vector<double>){
}
class HiddenMalkovModel{
	vector<double> first_status_prob;
	MalkovStatus status[100];
};

void HiddenMalkovModel::SetParameter(){

}