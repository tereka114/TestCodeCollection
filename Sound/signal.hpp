#pragma once
#include <iostream>
#include <math.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../Boost/library/util.hpp"

using namespace std;
using namespace boost::numeric;

template<typename T>
ublas::vector<T> hamming(ublas::vector<T> &signal_data){
	int size = signal_data.size();
	ublas::vector<T> window(size);
	ublas::vector<T> result(size);

	for(int i = 0; i < size; i++){
		window[i] = 0.54 - 0.46 * cos((2 * M_PI * i) / (size - 1));
		result[i] = signal_data[i] * window[i];
 	}
 	return result;
}

//要高速化
template<typename T>
void dft(ublas::vector<T> &signal_data,ublas::vector<T> &Rk,ublas::vector<T> &Ik){
	int size = signal_data.size();

	Rk.resize(size);
	Ik.resize(size);
	fill(Rk,0.0);
	fill(Ik,0.0);

	std::vector<double> index_cos;
	std::vector<double> index_sin;

	index_cos.resize(size*size);
	index_sin.resize(size*size);

	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			index_cos[i * j] = cos(2 * M_PI * i * j / size);
			index_sin[i * j] = -sin(2 * M_PI * i * j / size);
		}
	}

	for(int i = 0; i < size; i++){
		for(int j = 0; j < size;j++){
			Rk[i] += signal_data[j] * index_cos[i * j];
			Ik[i] += signal_data[j] * index_sin[i * j];
		}
	}
}

template<typename T>
void dft(ublas::vector<T> &signal_data,ublas::vector<T> &Rk,ublas::vector<T> &Ik,ublas::vector<T> &index_cos,ublas::vector<T> &index_sin){
	int size = signal_data.size();

	Rk.resize(size);
	Ik.resize(size);
	fill(Rk,0.0);
	fill(Ik,0.0);

	for(int i = 0; i < size; i++){
		for(int j = 0; j < size;j++){
			Rk[i] += signal_data[j] * index_cos[i * j];
			Ik[i] += signal_data[j] * index_sin[i * j];
		}
	}
}

template<typename T>
void dft_index(ublas::vector<T> &index_cos,ublas::vector<T> &index_sin,int size){
	index_cos.resize(size*size);
	index_sin.resize(size*size);

	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			index_cos[i * j] = cos(2 * M_PI * i * j / size);
			index_sin[i * j] = -sin(2 * M_PI * i * j / size);
		}
	}
}

template<typename T>
ublas::vector<T> power_spectrum(ublas::vector<T> &fft_result){
	int size = fft_result.size();
	ublas::vector<T> p_signal_data(size);
	for(int i = 0; i < size; i++){
		p_signal_data[i] = pow(fft_result[i],2.0);
	}
	return p_signal_data;
}

template<typename T>
ublas::vector<T> amplitude_spectrum(ublas::vector<T> &Rk,ublas::vector<T> &Ik){
	int size = Rk.size();
	ublas::vector<T> result(size);

	for(int i = 0; i < size; i++){
		result[i] = sqrt(pow(Rk[i],2) + pow(Ik[i],2));
	}

	return result;
}

ublas::vector<int> fft_scale(int fs,int sampling){
	ublas::vector<int> scale(sampling);
	for(int i = 0; i < sampling; i++){
		scale[i] = fs / sampling;
	}
	return scale;
}

template<typename T>
long double spectrum_centroid(ublas::vector<T> &fft_result){
	int size = fft_result.size();
	long double total1;
	long double total2;
	for(int i = 0; i < size; i++){
		total1 += i * fft_result[i];
		total2 += fft_result;
	}
	return total1 / total2;
}

template<typename T>
ublas::vector<T> dct(ublas::vector<T> &signal_data){
	int size = signal_data.size();

	ublas::vector<long double> cos_result(size);
	fill(cos_result,0.0);

	for(int i = 0; i < size; i++){
		cos_result[0] += signal_data[i];
 	}

 	cos_result[0] = cos_result[0] / sqrt((double)size);
 	for(int i = 1; i < size; i++){
 		for(int j = 0; j < size; j++){
 			cos_result[i] += signal_data[j] * cos(M_PI * (2.0 * j + 1.0) * i / (2.0 * size));
 			//cout << signal_data[j] * cos(M_PI * (2.0 * j + 1.0) * i / (2.0 * size)) << endl;
 		}

 		cos_result[i] = cos_result[i] * sqrt(2.0 / size);
 	}

	return cos_result;
}

template<typename T>
ublas::vector<T> fir_filter(ublas::vector<T> &signal_data,ublas::vector<T> filter_coefficient){
	ublas::vector<T> result(signal_data.size());
	fill(result,0);

	for(int i = 0; i < signal_data.size(); i++){
		for(int j = 0; j < filter_coefficient.size(); j++){
			if(i - j >= 0){
				result[i] += filter_coefficient[j] * signal_data[i - j];
			}
		}
	}
	return result;
}

template<typename T>
double hz2mel(T frequency){
	return 1127.01048 * log(frequency / 700.0 + 1.0);
}

template<typename T>
double mel2hz(T mel){
	return 700 * (exp(mel / 1127.01048)-1.0);
}

template<typename T>
void mel_filter_bank(int fs,int sampling,int numChannels,ublas::matrix<T> &filterbank){
	double freqmax = fs / 2.0;
	double melmax = hz2mel(freqmax);
	double nmax = sampling / 2.0;
	double df = fs / sampling;

	double dmel = melmax / (numChannels+1);

	ublas::vector<T> melcenters = arange(1,numChannels+1) * dmel;
	ublas::vector<int> indexcenter(melcenters.size());

	for(int i = 0; i < melcenters.size(); i++){
		indexcenter[i] = round(mel2hz(melcenters[i]) / df);
	}
	ublas::vector<int> indexstart(melcenters.size());
	ublas::vector<int> indexstop(melcenters.size());

	for(int i = 0; i < melcenters.size(); i++){
		if(i == 0){
			indexstart[i] = 0;
			indexstop[i] = indexcenter[i + 1];
		}else if(i == melcenters.size() - 1){
			indexstart[i] = indexcenter[i - 1];
			indexstop[i] = nmax;
		}else{
			indexstart[i] = indexcenter[i - 1];
			indexstop[i] = indexcenter[i +1]; 
		}
	}

	for(int i = 0; i < numChannels; i++){
		double increment = 1.0 / (indexcenter[i] - indexstart[i]);
		for(int j = indexstart[i]; j < indexcenter[i]; j++){
			filterbank(i,j) = (j - indexstart[i]) * increment;
		}
		double decrement = 1.0 / (indexstop[i] - indexcenter[i]);
		for(int j = indexcenter[i]; j < indexstop[i]; j++){
			filterbank(i,j) = 1.0 - ((j - indexcenter[i]) * decrement);
		}
	}
}

template<typename T>
ublas::vector<T> pre_prepare_mfcc(ublas::vector<T> &signal_data){
	int sampling = signal_data.size() / 2;

	ublas::vector<T> filter_coefficient(2);
	filter_coefficient[0] = 1.0;
	filter_coefficient[1] = -0.97;

	ublas::vector<T> pre_signal = fir_filter(signal_data,filter_coefficient);

	pre_signal = hamming(pre_signal);

	ublas::vector<T> Rk;
	ublas::vector<T> Ik;
	dft(pre_signal,Rk,Ik);
	ublas::vector<T> amlitude_spectrum = amplitude_spectrum(Rk,Ik);

	amlitude_spectrum = cut(amlitude_spectrum,0,sampling);
	return amlitude_spectrum;
}

template<typename T>
ublas::vector<T> pre_prepare_mfcc(ublas::vector<T> &signal_data,ublas::vector<T> &index_cos,ublas::vector<T> &index_sin){
	int sampling = signal_data.size() / 2;

	ublas::vector<T> filter_coefficient(2);
	filter_coefficient[0] = 1.0;
	filter_coefficient[1] = -0.97;

	ublas::vector<T> pre_signal = fir_filter(signal_data,filter_coefficient);

	pre_signal = hamming(pre_signal);

	ublas::vector<T> Rk;
	ublas::vector<T> Ik;
	dft(pre_signal,Rk,Ik,index_cos,index_sin);
	ublas::vector<T> amlitude_spectrum = amplitude_spectrum(Rk,Ik);

	amlitude_spectrum = cut(amlitude_spectrum,0,sampling);
	return amlitude_spectrum;
}

template<typename T>
ublas::vector<T> exec_mfcc(ublas::vector<T> &p_spectrum,ublas::matrix<T> &filterbank){
	ublas::vector<T> mel_signal = prod(p_spectrum,trans(filterbank));
	ublas::vector<T> log_m_signal(mel_signal.size());

	for(int i = 0; i < mel_signal.size(); i++){
		log_m_signal[i] = log10(mel_signal[i]);
	}
	ublas::vector<T> result = dct(log_m_signal);

	return result;
}

//filterbankも計算
template<typename T>
ublas::vector<T> mfcc(ublas::vector<T> &signal_data,int fs,int numChannels){
	int sampling = signal_data.size();

	ublas::vector<T> amlitude_spectrum = pre_prepare_mfcc(signal_data);

	ublas::matrix<T> filterbank(numChannels,sampling/2);
	fill(filterbank,0.0);
	mel_filter_bank(fs,sampling,numChannels,filterbank);

	ublas::vector<T> result = exec_mfcc(amlitude_spectrum,filterbank);

	return result;
}

//filterbankは引数で！
template<typename T>
ublas::vector<T> mfcc(ublas::vector<T> &signal_data,int fs,int numChannels,ublas::matrix<T> filterbank){
	ublas::vector<T> p_spectrum = pre_prepare_mfcc(signal_data);
	ublas::vector<T> result = exec_mfcc(p_spectrum,filterbank);

	return result;
}

//dftのサインコサインも引数に
template<typename T>
ublas::vector<T> mfcc(ublas::vector<T> &signal_data,int fs,int numChannels,ublas::matrix<T> filterbank,ublas::vector<T> &index_cos,ublas::vector<T> &index_sin){
	int sampling = signal_data.size() / 2;

	ublas::vector<T> p_spectrum = pre_prepare_mfcc(signal_data,index_cos,index_sin);
	ublas::vector<T> result = exec_mfcc(p_spectrum,filterbank);

	return result;
}

//ケプストラム平均正規化
template<typename T>
void cmn(std::vector<ublas::vector<T> > &mfcc){
	int size = mfcc.size();
	ublas::vector<T> average(size);
	fill(average,0.0);

	for(int i = 0; i < size; i++){
		average += mfcc[i];
	}
	average = average * (1 / size);

	for(int i = 0; i < size; i++){
		mfcc[i] = mfcc[i] - average;
	}
}

//delta parameter
void delta(std::vector<ublas::vector<double> > &mfcc,int param){
	int size = mfcc.size();
	int dimensions = mfcc[0].size();

	for(int i = 0; i < size; i++){
		double x_ave = 0.0;
		ublas::vector<double> y_ave(dimensions);
		fill(y_ave,0.0);
		int cnt = 0;

		//平均を算出する。
		for(int j = i - param; j < i + param + 1; j++){
			if(j < 0 || j >= size){
				continue;
			}else{
				cnt++;
				y_ave += mfcc[j];
				x_ave += j;
			}
		}
		x_ave /= cnt;
		y_ave /= cnt;
	}
}