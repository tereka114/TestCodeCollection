#include <iostream>
#include <math.h>

#define N 1000000000

using namespace std;

int arr[1000000000];

void t_access(){
	for(int i = 0; i < N; i++){
		arr[i];
	}
}

void t_exp(){
	for(int i = 0; i < N; i++){
		exp(100);
	}
}

void t_sin(){
	for(int i = 0; i < N; i++){
		sin(M_PI);
	}
}

void t_sqrt(){
	for(int i = 0; i < N; i++){
		sqrt(100);
	}
}

void t_pow(){
	for(int i = 0; i < N; i++){
		pow(100,8.0);
	}
}

void t_log(){
	for(int i = 0; i < N; i++){
		log(100);
	}
}

void t_acosh(){
	for(int i = 0; i < N; i++){
		acosh(100);
	}
}

int main(void){
	for(int i = 0; i < N; i++){
		arr[i] = 100;
	}

	void (*po[])() = {t_access,t_exp,t_sin,t_sqrt,t_pow,t_log,t_acosh};
	for(int i = 0; i < 7; i++){
		clock_t start,end;
		start = clock();
		(*po[i])();
		end = clock();
		cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
	}
	return 0;
}