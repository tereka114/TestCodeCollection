#include <iostream>
#include <vector>

using namespace std;
//参考：http://msirocoder.blog35.fc2.com/blog-entry-35.html
//更新方法はSMOを利用
class SVM{
	vector<vector<double> > input_data;
	vector<double> label_data;
	vector<double> a; //変数a

	int input_size;
	int karnel_function;
	double eps;
	double karnel_p; //カーネル関数のパラメータ
	double karnel_c; //カーネル関数のパラメータ
	double delta;
	double svm_c;
	double E_num1,E_num2;

	void Training();
	double Karnel()
};

SVM::SVM(){

}

void SVM::Training(vector<vector<double> > &input,vector<double> &label){
	//メンバ変数の初期化
	input_data = input;
	label_data = label;
	input_size = input_data.size();
	a.resize(input_size);
	fill(a.begin(),a.end(),0.0);

	//更新開始
	int loop = 0;
	while(1){
		if(loop > training_time){
			break;
		}
		for(int i = 0; i < input_size; i++){
			if(a[i] > eps && a[i] < (C - eps)){
				CheckUpdate(i);
			}
		}
		loop++;
	}
}

//number番目のデータを更新。KKT条件のチェック
void SVM::CheckUpdate(int number){
	if(a[number] > eps && a[number] < (svm_c - eps)){
	}else{

	}
	label_data[number] * Function(number);
	if((a[number] < ))
}

//Updateする対象の変数を選択
void SVM::Update(){
	int max_num2 = -1;
	//ランダムでindexを選択する為の変数
	int offset = (int)((double)rand()/(double)RAND_MAX * (double)(input_size - 1));
	//1番目の更新条件
	for(int i = 0; i < input_size; i++){
		int index = (i + offset) % input_size;

	}

	offset = (int)((double)rand()/(double)RAND_MAX * (double)(input_size - 1));
	//2番目の更新条件
	for(int i = 0; i < input_size; i++){
		int index = (i + offset) % input_size;
		if(a[index] > eps && a[index] < (svm_c - eps)){

		}	
	}

	offset = (int)((double)rand()/(double)RAND_MAX * (double)(input_size - 1));
	//3番目の更新条件
	for(int i = 0; i < input_size; i++){
		int index = (i + offset) % input_size;
		if(!(a[index] > eps && a[index] < (svm_c - eps))){

		}
	}
}

void SVM::SMO(int number1 int number2){
	if(number1 == number2){
		break;
	}
	double err = Function()
	double a1_old = a[number1];
	double a2_old = a[number2];
	double t1 = label_data[number1];
	double t2 = label_data[number2];
	double low;
	double high;

	//閾値を求める。
	if(label_data[number1] == label_data[number2]){
		low = max(0.0,a1_old + a2_old - svm_c);
		high = max(svm_c,a1_old + a2_old);
	}else{
		low = max(0.0,a1_old - a2_old);
		high = min(svm_c,svm_c - a2_old + a1_old);
	}

	double karnel_result11 = Karnel(a1_old,a1_old);
	double karnel_result12 = Karnel(a1_old,a2_old);
	double karnel_result22 = Karnel(a2_old,a2_old);
	double multi_karnel_result = karnel_result11 - (2 * karnel_result12) + karnel_result22;

	Ej = Function(number2) - label_data[number2];

	double unclipped_a2 = a2_old;
	double a1_new;
	double a2_new;

	//Karnel が正常な数字を出さなかった場合。
	if(multi_karnel_result <= 0.0){
		a1_new = low;
		a2_new = a2_old + t1 * t2 * (a1_old - a1_new);
		double v1 = Function(number2) + b - t2 * a2_old * karnel_result22 - t1 * a1_old * karnel_result12;
		double v2 = Function(number1) + b - t2 * a2_old * karnel_result12 - t1 * a1_old * karnel_result11;

		double Lobj = a2_new + a1_new - karnel_result22 * a2_new * a2_new / 2.0 - karnel_result11  * a1_new * a1_new / 2.0
			- t2 * t1 * karnel_result12 * a2_new * a1_new - t2 * a2_new * v1 - t1 * a1_new * v2;

		a1_new = high
		a2_new = a2_old + t1 * t2 * (a1_old - a1_new);
		double v1 = Function(number2) + b - t2 * a2_old * karnel_result22 - t1 * a1_old * karnel_result12;
		double v2 = Function(number1) + b - t2 * a2_old * karnel_result12 - t1 * a1_old * karnel_result11;

		double Hobj = a2_new + a1_new - karnel_result22 * a2_new * a2_new / 2.0 - karnel_result11  * a1_new * a1_new / 2.0
			- t2 * t1 * karnel_result12 * a2_new * a1_new - t2 * a2_new * v1 - t1 * a1_new * v2;
		if(Lobj > Hobj + eps){
			a1_new = low;
		}else if(Lobj < Hobj - eps){
			a1_new = high;
		}else{
			a1_new = a1_old;
		}
		a[number1] = a1_old;
		a[number2] = a2_old;
	}else{
		//clipping
		if(high <= unclipped_a2){
			a2_new = high;
		}else if(unclipped_a2 <= low){
			a2_new = low;
		}else{
			a2_new = unclipped_a2;
		}
	}
	a1_new = a1_old + t1 * t2 * (a2_old - a2_new);

	if(a[number1] > eps && a[number2] < (svm_c - eps))
}

//input_dataのi番目の場合の関数fの値を求める。
double SVM::Function(int number){
	double ans = 0.0;
	for(int j = 0; j < input_size; j++){
		if(a[j] == 0){
			continue;
		}
		ans += a[j] * label_data[j] * Karnel(input_data[j],input_data[number]);
	}
	ans -= b;
	return ans;
}

//カーネル関数の計算
double SVM::Karnel(const vector<double> vect1,const vector<double> vect2){
	int vect_size = vect1.size();
	double ans;
	switch(karnel_function){
		//多項式カーネル
		case 0:
			ans = karnel_c;
			for(int i = 0; i < vect_size; i++){
				ans += vect1[i] * vect2[i];
			}
			pow(ans,karnel_p);
			return ans;
		//gaussian カーネル
		case 1:
			ans = 0.0;
			for(int i = 0; i < vect_size; i++){
				ans += pow(vect1[i] - vect2[i],2);
			}
			ans = -ans / (2 * delta * delta);
			ans = exp(ans);
			return ans;
	}
}