#include <iostream>

using namespace std;
class Temp{
public:
	int tmp;
	Temp(int num){
		tmp = num;
	}
	void Boo();
	void Foo();
	void Bar();
};

void Temp::Boo(){
	for(int i = 0; i < tmp; i++){
		int temp_answer = i;
	}
}

void Temp::Foo(){
	int size = tmp;
	int temp_answer = 0;
	for(int i = 0; i < size; i++){
		temp_answer = i;
	}
}

void Temp::Bar(){
	int size = tmp;
	for(int i = 0; i < size; i++){
		int a = i;
	}
}
int main(int argc, char const *argv[]){
	Temp tmp(100000);
	void(Temp::*func[])() = {&Temp::Boo,&Temp::Foo,&Temp::Bar};
	for(int i = 0; i < 3; i++){

		clock_t start,end;
		start = clock();

		for(int j = 0; j < 100000; j++){
			(tmp.*func[i])();
		}

		end = clock();
		cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
	}
	return 0;
}