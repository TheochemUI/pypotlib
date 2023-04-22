#pragma once

// clang-format off
#include "../py_potential.hpp"
#include "../../subprojects/potlib/CppCore/src/CuH2/CuH2Pot.hpp"
// clang-format on

using namespace rgpot;

template <class CuH2PotBase = CuH2Pot>
class PyCuH2Pot : public PyPotential<CuH2PotBase> {
public:
    using PyPotential<CuH2PotBase>::PyPotential; // Inherit constructors
    // Override pure virtual with non-pure
    std::pair<double, AtomMatrix>
    operator()(const Eigen::Ref<const AtomMatrix> &positions,
               const Eigen::Ref<const VectorXi> &atmtypes,
               const Eigen::Ref<const Eigen::Matrix3d> &box) const override {
        // This is needed to prevent substitution errors
        using Return = std::pair<double, AtomMatrix>; // PURE doesn't throw if no override is found
        PYBIND11_OVERRIDE_NAME(Return,                // Return type
                               CuH2PotBase,           // Parent class
                               "__call__",            // Function name in Python
                               operator(),            // Function in C++
                               positions,             // Arg(s)
                               atmtypes,
                               box);
    }
};
