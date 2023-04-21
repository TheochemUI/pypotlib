#include "py_potential.hpp"

using namespace rgpot;
void py_potential(py::module &m) {
    py::class_<Potential, PyPotential<>>(m, "Potential")
        .def(py::init<PotType>())
        .def("__call__", &Potential::operator());
};
