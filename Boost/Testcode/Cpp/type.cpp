#include <iostream>
#include <float.h>
using namespace std;

int main(void){
	int a = 100;
	long int b = 100;
	long long int c = 300;
	double d = 200.0;
	long double e = 200.0; 

	cout << FLT_MIN << endl;
	cout << FLT_MAX << endl;
	cout << DBL_MIN << endl;
	cout << DBL_MAX << endl;
	cout << LDBL_MIN << endl;
	cout << LDBL_MAX << endl;

	cout << a + b << endl;
	cout << d + e << endl;
}