#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../NeuralNetwork/Layer.hpp"
#include "../NeuralNetwork/OutputLayer.hpp"

class AutoEncoderLayer{
public:
	void Update();
};

void AutoEncoderLayer::Update(){

}

class AutoEncoderOutputLayer{
public:
	void Update();
	void SetParameter()
};

void AutoEncoderOutputLayer::Update(){
public:
	void Update(matirx<double>& middle_weight);
}

void AutoEncoderOutputLayer::Update(matirx<double>& middle_weight){
	weight = trans(middle_weight)
}