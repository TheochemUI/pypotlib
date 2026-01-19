#include "py_cuh2pot.hpp"

using namespace rgpot;
void py_cuh2pot(py::module &m) {
    py::class_<CuH2Pot, PotentialBase, PyCuH2Pot<>>(m, "CuH2Pot")
        .def(py::init<>())
        .def("__call__", &CuH2Pot::operator())
        .def("set_cache", &CuH2Pot::set_cache)
        .def("__copy__", [](const CuH2Pot &self){
            return CuH2Pot(self);
        })
        // XXX: These don't really do anything, currently just compatibility stuff
        .def("__deepcopy__", [](const CuH2Pot &self, py::dict){
            return CuH2Pot(self);
        }, "memo"_a)
        .def(py::pickle([](const CuH2Pot &p){
                return py::make_tuple(p);
        }, [](py::tuple t){
                CuH2Pot p(t[0].cast<CuH2Pot>());
                return p;
        }));
};
