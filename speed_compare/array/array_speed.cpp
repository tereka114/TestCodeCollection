#include <vector>
#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#define NDEBUG

using namespace std;
using namespace boost::numeric;

int N = 10000;
int arr[10000][10000];

//全てpush_backでの追加
void vector1(){
	clock_t start,end;
	vector<vector<int> > list;
	start = clock();

	for(int i = 0; i < N; i++){
		vector<int> list2;
		for(int j = 0; j < N; j++){
			list2.push_back(j);
		}
		list.push_back(list2);
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}

//resizeしてから要素にアクセスして代入
void vector2(){
	clock_t start,end;
	vector<vector<int> > list;
	start = clock();
	list.resize(N);
	for(int i = 0; i < N; i++){
		list[i].resize(N);
	}

	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			list[i][j] = i;
		}
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}

//ublasの行列 matrix型
void ublas_matrix(){
	clock_t start,end;
	start = clock();
	ublas::matrix<int> mat(N,N);
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			mat(i,j) = i;
		}
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}

//普通の配列
void array(){
	clock_t start,end;
	start = clock();
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			arr[i][j] = j;
		}
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}

int main(void){
	vector1();
	vector2();
	ublas_matrix();
	array();
}