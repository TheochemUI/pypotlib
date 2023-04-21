#pragma once

// clang-format off
#include "../py_potential.hpp"
#include "../../subprojects/potlib/CppCore/src/LennardJones/LJPot.hpp"
// clang-format on

using namespace rgpot;

template <class LJPotBase = LJPot>
class PyLJPot : public PyPotential<LJPotBase> {
public:
    using PyPotential<LJPotBase>::PyPotential; // Inherit constructors
    // Override pure virtual with non-pure
    std::pair<double, AtomMatrix>
    operator()(const Eigen::Ref<const AtomMatrix> &positions,
               const Eigen::Ref<const VectorXi> &atmtypes,
               const Eigen::Ref<const Eigen::Matrix3d> &box) const override {
        // This is needed to prevent substitution errors
        using Return = std::pair<double, AtomMatrix>; // PURE doesn't throw if no override is found
        PYBIND11_OVERRIDE_NAME(Return,                // Return type
                               LJPotBase,             // Parent class
                               "__call__",            // Function name in Python
                               operator(),            // Function in C++
                               positions,             // Arg(s)
                               atmtypes,
                               box);
    }
};
