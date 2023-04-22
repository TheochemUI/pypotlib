#include "py_wrapper.hpp"

PYBIND11_MODULE(cpot, m) {
    // Potentials
    py_pottypes(m);
    py_potential(m);
    py_ljpot(m);
    py_cuh2pot(m);
}
