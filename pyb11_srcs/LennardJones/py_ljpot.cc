#include "py_ljpot.hpp"

#include "../py_copy_pickle.hpp"

using namespace rgpot;
void py_ljpot(py::module &m) {
    auto cls = py::class_<LJPot, PotentialBase, PyLJPot<>>(m, "LJPot")
                   .def(py::init<>())
                   .def("__call__", &LJPot::operator())
                   .def("set_cache", &LJPot::set_cache);
    enable_copy_pickle<LJPot>(cls);
};
