#include <iostream>
#include <vector>
//#include "./svm_model.hpp"
#include "./SVM.hpp"

using namespace std;

//andのテストデータを生成する。
void generete_xor(vector<vector<double> > &input_data,vector<double> &label_data){
	input_data.resize(4);
	label_data.resize(4);
	for(int i = 0; i < input_data.size(); i++){
		input_data[i].resize(2);
	}

	input_data[0][0] = 0.0;
	input_data[0][1] = 0.0;
	input_data[1][0] = 0.0;
	input_data[1][1] = 1.0;
	input_data[2][0] = 1.0;
	input_data[2][1] = 0.0;
	input_data[3][0] = 1.0;
	input_data[3][1] = 1.0;

	label_data[0] = -1.0;
	label_data[1] = 1.0;
	label_data[2] = 1.0;
	label_data[3] = -1.0;
}

int main(void){
	vector<vector<double> > input;
	vector<double> label;
	generete_xor(input,label);
	SVM svm;
	svm.SetParameter(1000.0,0,4.0,1.0,1000);
	svm.Training(input,label);

	//int a = svm.learning(input,label);
	cout << "aaaa" << endl;
	for(int i = 0; i < input.size(); i++){
		cout << svm.Test(input[i]) << endl;
		//cout << svm.discriminate(input[i]) << endl;
	}
	return 0;

}