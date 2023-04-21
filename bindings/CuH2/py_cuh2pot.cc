#include "py_cuh2pot.hpp"

using namespace rgpot;
void py_cuh2pot(py::module &m) {
    py::class_<CuH2Pot, Potential, PyCuH2Pot<>>(m, "CuH2Pot")
        .def(py::init<>())
        .def("__call__", &CuH2Pot::operator());
};
