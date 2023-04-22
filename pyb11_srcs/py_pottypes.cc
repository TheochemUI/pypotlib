// clang-format off
#include "py_wrapper.hpp"
// clang-format on

using namespace rgpot;

void py_pottypes(py::module_ &m) {
    py::enum_<PotType>(m, "PotType")
        .value("UNKNOWN", PotType::UNKNOWN)
        .value("LJ", PotType::LJ)
        .value("CUH2", PotType::CuH2);
    // m.def("getPotentialType", &helper_functions::getPotentialType);
    // m.def("getPotentialName", &helper_functions::getPotentialName);
}
