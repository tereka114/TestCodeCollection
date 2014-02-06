#include <Python.h>

#include <numpy/arrayobject.h>
#include <boost/python.hpp>
#include <pyublas/numpy.hpp>

using namespace std;
using namespace boost;
using namespace boost::python;

pyublas::numpy_matrix<double> doubles(pyublas::numpy_matrix<double> x)
{
  return 2*x;
}

pyublas::numpy_vector<double> triples(pyublas::numpy_vector<double> x)
{
  return 3*x;
}

BOOST_PYTHON_MODULE(numpy_array){
  def("doubles",&doubles);
  def("triples",&triples);
}