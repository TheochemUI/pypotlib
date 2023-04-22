#include "py_ljpot.hpp"

using namespace rgpot;
void py_ljpot(py::module &m) {
    py::class_<LJPot, Potential, PyLJPot<>>(m, "LJPot")
        .def(py::init<>())
        .def("__call__", &LJPot::operator());
};
