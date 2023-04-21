#include "py_wrapper.hpp"

PYBIND11_MODULE(pypotlib, m) {
    // Potentials
    py_pottypes(m);
    py_potential(m);
    py_ljpot(m);
}
