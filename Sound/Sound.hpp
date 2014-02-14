#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string.hpp>
#include <sndfile.hh>
#include "./signal.hpp"

using namespace boost::numeric;
using namespace boost::filesystem;
using namespace std;

class Sound{
public:
	double fs;
	int length;
	int channel;
	int sampling;
	int shift;
	ublas::vector<double> filedata;
	std::vector<ublas::vector<double> > mfcc_list;

	string wavefile;
	string mfccfile;

	Sound(){}
	//SoundRead();
	void FileReadMFCC();
	void FileReadWave(string path);
	void SetParameterStft(int num1,int num2);
	void FeatureExtract();
};

void Sound::FileReadMFCC(){
	std::ifstream ifs(mfccfile);
	string str;
	ublas::vector<double> vect(36);

	while(getline(ifs,str)){
		std::vector<string> v;

		boost::algorithm::split(v, str, boost::is_any_of(" "));
		for(int i = 0; i < v.size();i++){
			std::stringstream ss;
			double temp;
			ss << v[i];
			ss >> temp;
			vect[i] = temp;
		}
		mfcc_list.push_back(vect);
	}
}

void Sound::FileReadWave(string path){
	wavefile = path;
	mfccfile = boost::algorithm::replace_all_copy(path,".wav",".txt");
	SndfileHandle infile(path, SFM_READ);
	fs = infile.samplerate();
	channel = infile.channels();
	length = infile.frames();

	short *input = new short[infile.frames()];
	infile.readf(input, infile.frames());

	filedata.resize(infile.frames());

	// print input 
	for (int i = 0; i < infile.frames(); ++i) {
	  filedata[i] = input[i] / 32570.0;
	}
}

void Sound::SetParameterStft(int num1,int num2){
	shift = num1;
	sampling = num2;
}

void Sound::FeatureExtract(){
	int start = 0;
	int end = sampling;
	ublas::vector<double> index_cos;
	ublas::vector<double> index_sin;
	ublas::matrix<double> filterbank(20,sampling/2);
	fill(filterbank,0.0);

	dft_index(index_cos,index_sin,sampling);
	mel_filter_bank(fs,sampling,20,filterbank);

	dft_index(index_cos,index_sin,sampling);
	while(1){
		if(start + sampling > length) break;
		ublas::vector<double> data = cut(filedata,start,end);
		//cout << data << endl;
		ublas::vector<double> mfccs = mfcc(data,16000,20,filterbank,index_cos,index_sin);
		//cout << mfccs << endl;
		start += shift;
		end += shift;
		// cout << mfccs << endl;
	}
}