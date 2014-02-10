#pragma once
#include <iostream>
#include <math.h>

using namespace std;

template<typename T>
vector<T> hamming(vector<T> &signal_data){
	int size = signal_data.size();
	vector<T> window(size);
	vector<T> result(size);

	for(int i = 0; i < size; i++){
		window[i] = 0.54 - 0.46 * cos((2 * M_PI * i) / (size - 1));
		result[i] = signal_data[i] * window[i];
 	}
 	return result;
}

template<typename T1,typename T2>
vector<T2> dft(vector<T1> &signal_data){
	int size = signal_data.size();
	vector<T1> re(size);
	vector<T1> im(size);
	vector<T2> Rk(size);
	vector<T2> Ik(size);

	for(int i = 0; i < size; i++){
		re[i] = signal_data[i];
		im[i] = 0.0;
	}

	for(int i = 0; i < size; i++){
		Rk[i] = 0.0;
		Ik[i] = 0.0;
		for(int j = 0; j < size;j++){
			Rk[i] += re[j] * ( cos((2 * M_PI / size) * i * j) ) + re[j] * ( sin((2 * M_PI / size) * i * j));
			Ik[i] += im[j] * (-sin((2 * M_PI / size) * i * j)) + im[j] * ( cos((2 * M_PI / size) * i * j));
		}
		Rk[i] /= size;
		Ik[i] /= size;
	}
	return Rk;
}

template<typename T>
vector<T> power_spectrum(vector<T> &fft_result){
	int size = fft_result.size();
	vector<T> p_signal_data(size);
	for(int i = 0; i < size; i++){
		p_signal_data[i] = pow(fft_result[i],2.0);
	}
	return p_signal_data;
}