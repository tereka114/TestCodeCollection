#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
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
	std::vector<double> filedata;
	std::vector<ublas::vector<double> > mfcc;

	Sound(){}
	//SoundRead();
	void FileReadMFCC(string path);
	void FileReadWave(string path);
	void FeatureExtract();
};

void Sound::FileReadMFCC(string path){
	std::ifstream ifs(path);
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
		mfcc.push_back(vect);
	}
}

void Sound::FileReadWave(string path){
	SndfileHandle infile(path, SFM_READ);
	fs = infile.samplerate();
	channel = infile.channels();
	length = infile.frames();

	short int* input = new short int [infile.frames()];
	infile.readf(input, infile.frames());

	// print input 
	for (int i = 0; i < infile.frames(); ++i) {
	  filedata.push_back(input[i]);
	}
}

void Sound::FeatureExtract(){

}