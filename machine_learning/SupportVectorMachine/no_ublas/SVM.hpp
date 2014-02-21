#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
//参考：http://msirocoder.blog35.fc2.com/blog-entry-35.html
//更新方法はSMOを利用
class SVM{
private:
	int CheckUpdate(int number);
	int Update(int number);
	int SMO(int number1,int number2);
	double Function(int number);
	double Karnel(const vector<double> vect1,const vector<double> vect2);
public:
	vector<vector<double> > input_data;
	vector<double> label_data;
	vector<double> weight;//重み
	vector<double> a; //変数a
	vector<double> err_cache;
	vector<int> sv_index;

	int input_size;
	int training_time;//学習回数
	double eps; //猶予
	double delta;//delta
	double svm_c;
	double b;
	double tolerance;
	double E_num1,E_num2;

	//カーネル関数のパラメータ
	int karnel_function;
	double karnel_p; //カーネル関数のパラメータ
	double karnel_c; //カーネル関数のパラメータ

	void SetParameter(double param_svm_c,int function,double p,double param_karnel_c,int training);
	void Training(vector<vector<double> > &input,vector<double> &label);
	double Test(vector<double> test);
};

void SVM::SetParameter(double param_svm_c = 1000.0,int function = 0,double p = 4,double param_karnel_c = 1.0,int training = 1000000){
	svm_c = param_svm_c;
	karnel_function = function;
	karnel_p = p;
	karnel_c = param_karnel_c;
	eps = 0.0001;
	training_time = training;
	b = 0.0;
	tolerance = 0.001;
}

void SVM::Training(vector<vector<double> > &input,vector<double> &label){
	//メンバ変数の初期化
	input_data = input;
	label_data = label;
	input_size = input_data.size();
	a.resize(input_size);
	err_cache.resize(input_size);
	weight.resize(input_size);

	fill(a.begin(),a.end(),0.0);
	fill(err_cache.begin(),err_cache.end(),0.0);

	//更新開始
	int changed;
	int loop = 0;
	bool flag = true;
	while(1){
		changed = 0; //変化の回数。
		if(loop > training_time){
			break;
		}
		for(int i = 0; i < input_size; i++){
			//最初もしくは
			if(flag || (a[i] > eps && a[i] < (svm_c - eps)) ){
				changed += CheckUpdate(i);
			}
		}
		if(flag){
			flag = false;
			if(changed == 0){
				break;
			}
		}else{
			if(changed == 0){
				flag = true;
			}
		}
		loop++;
		cout << loop << endl;
	}

	for( int i = 0; i < input_size; i++ ){
		if( a[i] != 0.0 ){
			sv_index.push_back(i);
		}
	}
	// 重みw 計算
	vector<int>::iterator it,it0;
	for( it = sv_index.begin(); it != sv_index.end(); it++ ){
		weight[*it] = label_data[*it] * a[*it];
	}
}

//number番目のデータを更新。KKT条件のチェック
int SVM::CheckUpdate(int number){
	if(a[number] > eps && a[number] < (svm_c - eps)){
		E_num1 = err_cache[number];
	}else{
		E_num1 = Function(number) - label_data[number];
	}
	double yFunciton = E_num1 * label_data[number];

	if((a[number] < (svm_c - eps) && yFunciton < -tolerance) || (a[number] > eps && yFunciton > tolerance)){
		return Update(number);
	}
	return 0;
}

//Updateする対象の変数を選択
int SVM::Update(int number){
	int max_num2 = -1;
	int max_num2_index = -1;
	//ランダムでindexを選択する為の変数
	int offset = (int)((double)rand()/(double)RAND_MAX * (double)(input_size - 1));
	//1番目の更新条件
	for(int i = 0; i < input_size; i++){
		int index = (i + offset) % input_size;
		if(a[index] > eps && a[index] < (svm_c - eps)){
			E_num2 = err_cache[index];
			if(fabs(E_num2 - E_num1) > max_num2){
				max_num2 = fabs(E_num2 - E_num1);
				max_num2_index = index;
			}
		}
	}
	if(max_num2_index >= 0){
		if(SMO(number,max_num2_index) == 1){
			return 1;
		}
	}

	offset = (int)((double)rand()/(double)RAND_MAX * (double)(input_size - 1));
	//2番目の更新条件
	for(int i = 0; i < input_size; i++){
		int index = (i + offset) % input_size;
		if(a[index] > eps && a[index] < (svm_c - eps)){
			if(SMO(number,index) == 1){
				return 1;
			}
		}	
	}

	offset = (int)((double)rand()/(double)RAND_MAX * (double)(input_size - 1));
	//3番目の更新条件
	for(int i = 0; i < input_size; i++){
		int index = (i + offset) % input_size;
		if(!(a[index] > eps && a[index] < (svm_c - eps))){
			if(SMO(number,index)){
				return 1;
			}
		}
	}
	return 0;
}

int SVM::SMO(int number1,int number2){
	if(number1 == number2){
		return 0;
	}
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
		high = min(svm_c,svm_c + a1_old - a2_old);
	}

	double karnel_result11 = Karnel(input_data[number1],input_data[number1]);
	double karnel_result12 = Karnel(input_data[number1],input_data[number2]);
	double karnel_result22 = Karnel(input_data[number2],input_data[number2]);
	double multi_karnel_result = karnel_result11 + karnel_result22 - (2.0 * karnel_result12);
	//cout << karnel_result11 << " " << karnel_result22 << " " << karnel_result12 << endl;

	double a1_new;
	double a2_new;

	if(a[number2] > eps && a[number2] <(svm_c - eps)){
		E_num2 = err_cache[number2];
	}else{
		E_num2 = Function(number2) - t2;
	}

	bool bClip = false;

	//Karnel が正常な数字を出さなかった場合。
	if(multi_karnel_result <= 0.0){
		a1_new = low;
		a2_new = a2_old + t1 * t2 * (a1_old - a1_new);
		double v1 = Function(number2) + b - t2 * a2_old * karnel_result22 - t1 * a1_old * karnel_result12;
		double v2 = Function(number1) + b - t2 * a2_old * karnel_result12 - t1 * a1_old * karnel_result11;

		double Lobj = a2_new + a1_new - karnel_result22 * a2_new * a2_new / 2.0 - karnel_result11  * a1_new * a1_new / 2.0
			- t2 * t1 * karnel_result12 * a2_new * a1_new - t2 * a2_new * v1 - t1 * a1_new * v2;

		a1_new = high;
		a2_new = a2_old + t1 * t2 * (a1_old - a1_new);
		v1 = Function(number2) + b - t2 * a2_old * karnel_result22 - t1 * a1_old * karnel_result12;
		v2 = Function(number1) + b - t2 * a2_old * karnel_result12 - t1 * a1_old * karnel_result11;

		double Hobj = a2_new + a1_new - karnel_result22 * a2_new * a2_new / 2.0 - karnel_result11  * a1_new * a1_new / 2.0
			- t2 * t1 * karnel_result12 * a2_new * a1_new - t2 * a2_new * v1 - t1 * a1_new * v2;
		if(Lobj > Hobj + eps){
			bClip = true;
			a1_new = low;
		}else if(Lobj < Hobj - eps){
			bClip = true;
			a1_new = high;
		}else{
			bClip = true;
			a1_new = a1_old;
		}
		a[number1] = a1_old;
		a[number2] = a2_old;
	}else{
		a1_new = a1_old + (t1 * (E_num2-E_num1) / multi_karnel_result);
		//cout << a1_new << " " << a1_old << " " << t1 << " " << E_num2 << " " << E_num1 << " " << multi_karnel_result << endl;
		//clipping
		if(high < a1_new){
			bClip = true;
			a1_new = high;
		}else if(a1_new < low){
			bClip = true;
			a1_new = low;
		}
	}

	if(fabs(a1_new - a1_old) < eps * (a1_new + a1_old +eps)){
		return 0;
	}

	a2_new = a2_old + t1 * t2 * (a1_old - a1_new);

	double old_b = b;
	if(a[number1] > eps && a[number1] < (svm_c - eps)){
		b+= E_num1 + (a1_new - a1_old) * t1 * karnel_result11 
				   + (a2_new - a2_old) * t2 * karnel_result12;
	}else if(a[number2] > eps && a[number2] < (svm_c - eps)){
		b+= E_num2 + (a1_new - a1_old) * t1 * karnel_result12 
				   + (a2_new - a2_old) * t2 * karnel_result22;
	}else{
		b+= ((E_num1 + (a1_new - a1_old) * t1 * karnel_result11 
				+ (a2_new - a2_old) * t2 * karnel_result12) +
			(E_num2 + (a2_new - a2_old) * t1 * karnel_result12 
				+ (a2_new - a2_old) * t2 * karnel_result22)) / 2.0;
	}

	for(int i = 0; i < input_size; i++){
		if(i ==  number1 || i == number2){
			continue;
		}else if(a[i] > eps && a[i] < (svm_c - eps) ){
			err_cache[i] = err_cache[i] + t1 * (a1_new - a1_old) * Karnel(input_data[number1],input_data[i])
										+ t2 * (a2_new - a2_old) * Karnel(input_data[number2],input_data[i])
										+ old_b - b;
		}
	}
	a[number1] = a1_new;
	a[number2] = a2_new;
	//cout << a[number1] << " " << a[number2] << endl;
	if(bClip){
		if(a1_new > eps && a1_new < (svm_c - eps)){
			err_cache[number1] = Function(number1) - t1;
		}
	}else{
		err_cache[number1] = 0.0;
	}
	err_cache[number2] = Function(number2) - t2;

	return 1;
}

double SVM::Test(vector<double> test){
	double eval = 0.0;
	vector<int>::iterator it;
	for(it = sv_index.begin(); it != sv_index.end(); it++){
		eval += weight[*it] * Karnel(input_data[*it],test);
	}
	eval -= b;

	return eval;
}

//input_dataのi番目の場合の関数fの値を求める。
double SVM::Function(int number){
	double ans = 0.0;
	for(int i = 0; i < input_size; i++){
		if(a[i] == 0.0){
			continue;
		}
		ans += a[i] * label_data[i] * Karnel(input_data[i],input_data[number]);
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
			ans = pow(ans,karnel_p);
			return ans;
		//gaussian カーネル
		case 1:
			ans = 1.0;
			for(int i = 0; i < vect_size; i++){
				ans += pow(vect1[i] - vect2[i],2);
			}
			ans = -ans / (2 * delta * delta);
			ans = exp(ans);
			return ans;
	}
	return 0;
}