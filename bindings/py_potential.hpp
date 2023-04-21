#pragma once

// clang-format off
#include "py_wrapper.hpp"
#include "../subprojects/potlib/CppCore/src/Potential.hpp"
// clang-format on

using namespace rgpot;

template <class PotentialBase = Potential>
class PyPotential : public PotentialBase {
public:
    /* Constructors and inherited */
    using PotentialBase::PotentialBase;
    std::pair<double, AtomMatrix>
    operator()(const Eigen::Ref<const AtomMatrix> &positions,
               const Eigen::Ref<const VectorXi> &atmtypes,
               const Eigen::Ref<const Eigen::Matrix3d> &box) const override {
        // This is needed to prevent substitution errors
        using Return = std::pair<double, AtomMatrix>; // PURE doesn't throw if no override is found
        PYBIND11_OVERRIDE_PURE_NAME(Return,           // Return type
                                    PotentialBase,    // Parent class
                                    "__call__",       // Function name in Python
                                    operator(),       // Function in C++
                                    positions,        // Arg(s)
                                    atmtypes,
                                    box);
    }
};
