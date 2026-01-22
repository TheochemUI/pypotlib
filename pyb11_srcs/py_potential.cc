#include "py_potential.hpp"
#include "../subprojects/potlib/CppCore/rgpot/PotentialCache.hpp"

using namespace rgpot;
void py_potential(py::module &m) {
    // FIX: PyPotentialBase is a class, not a template.
    py::class_<PotentialBase, PyPotentialBase>(m, "Potential")
        .def(py::init<PotType>())
        .def("__call__", &PotentialBase::operator())
        .def("set_cache", &PotentialBase::set_cache)
        .def_property_readonly("type", &PotentialBase::get_type);
};
