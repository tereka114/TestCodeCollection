#include <boost/python.hpp>
#include <string>
#include "Image.hpp"

BOOST_PYTHON_MODULE(python_image){
	using namespace boost::python;

	class_<Image>("Image")
	.def("ImageRead",&Image::ImageRead)
	.def("FeatureExtract",&Image::FeatureExtract)
	.def("TransFormAllHistrogram",&Image::TransFormAllHistrogram)
	.def("MakeColorHistrogram",&Image::MakeColorHistrogram)
	.def("Color2Glay",&Image::Color2Glay)
	.def("Hog",&Image::Hog);
}