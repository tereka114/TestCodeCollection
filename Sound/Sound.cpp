#include <iostream>
#include "./Sound.hpp"

int main(void){
	Sound sound;
	sound.FileReadWave("./temp.wav");
	sound.SetParameterStft(128,512);
	sound.FeatureExtract();
}