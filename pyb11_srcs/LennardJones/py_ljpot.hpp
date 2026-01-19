#pragma once

// clang-format off
#include "../py_potential.hpp"
#include "../../subprojects/potlib/CppCore/rgpot/LennardJones/LJPot.hpp"
// clang-format on

using namespace rgpot;

template <class LJPotBase = LJPot>
class PyLJPot : public PyPotentialConcrete<LJPotBase> {
public:
    using PyPotentialConcrete<LJPotBase>::PyPotentialConcrete;
    // Override pure virtual with non-pure
    std::pair<double, AtomMatrix>
    operator()(const AtomMatrix &positions,
               const std::vector<int> &atmtypes,
               const std::array<std::array<double, 3>, 3> &box) override {
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
