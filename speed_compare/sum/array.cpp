#include <vector>
#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#define NDEBUG

using namespace std;
using namespace boost::numeric;

int N = 100000000;
int arr1[100000000];
int arr2[100000000];

void sum_vector(){
	clock_t start,end;
	start = clock();

	vector<int> vect1;
	vector<int> vect2;
	vect1.resize(N);
	vect2.resize(N);
	for(int i = 0; i < N; i++){
		vect1[i] = i;
		vect2[i] = i;
	}
	start = clock();

	for(int i = 0; i < N; i++){
		vect1[i] + vect2[i];
	}

	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}

void ublas_vector(){
	clock_t start,end;
	start = clock();

	ublas::vector<int> vect1(N);
	ublas::vector<int> vect2(N);

	for(int i = 0; i < N; i++){
		vect1[i] = i;
		vect2[i] = i;
	}
	start = clock();

	vect1 + vect2;

	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}

void array_sum(){
	clock_t start,end;
	start = clock();

	for(int i = 0; i < N; i++){
		arr1[i] = i;
		arr2[i] = i;
	}
	start = clock();
	for(int i = 0; i < N; i++){
		arr1[i] + arr2[i];
	}

	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}

int main(void){
	sum_vector();
	ublas_vector();
	array_sum();
}