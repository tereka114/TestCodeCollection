#include <iostream>
#include <vector>

using namespace std;

int N = 1000000;

int main(void){
	int size = 100000;
	vector<int> array1(size);
	vector<int> array2(size);
	for(int i = 0; i < size; i++){
		array1[i] = i;
	}
	clock_t start,end;

	//size関数を使う
	start = clock();
	for(int i = 0; i < N; i++){
		for(int j = 0; j < array1.size(); j++){
			int a = array1[j] + array2[j];
		}
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;

	start = clock();
	//sizeはあらかじめ指定
	for(int i = 0; i < N; i++){
		for(int j = 0; j < size; j++){
			int a = array1[j] + array2[j];
		}
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}