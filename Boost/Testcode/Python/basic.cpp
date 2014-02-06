#include <boost/python.hpp>

int diff(int a, int b)
{
    return a - b;
}

BOOST_PYTHON_MODULE(basic)
{
    using namespace boost::python;
    def("diff", &diff);
}