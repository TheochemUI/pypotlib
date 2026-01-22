#include "py_cuh2pot.hpp"

#include "../py_copy_pickle.hpp"

using namespace rgpot;
void py_cuh2pot(py::module &m) {
    auto cls = py::class_<CuH2Pot, PotentialBase, PyCuH2Pot<>>(m, "CuH2Pot")
                   .def(py::init<>())
                   .def("__call__", &CuH2Pot::operator())
                   .def("set_cache", &CuH2Pot::set_cache)
                   .def("__copy__", [](const CuH2Pot &self) { return CuH2Pot(self); });
    enable_copy_pickle<CuH2Pot>(cls);
};
