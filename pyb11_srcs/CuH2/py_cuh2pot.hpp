#pragma once

// clang-format off
#include "../py_potential.hpp"
#include "../../subprojects/potlib/CppCore/rgpot/CuH2/CuH2Pot.hpp"
// clang-format on

using namespace rgpot;

template <class CuH2PotBase = CuH2Pot>
class PyCuH2Pot : public PyPotentialConcrete<CuH2PotBase> {
public:
    using PyPotentialConcrete<CuH2PotBase>::PyPotentialConcrete;
    // Override pure virtual with non-pure
    std::pair<double, AtomMatrix>
    operator()(const AtomMatrix &positions,
               const std::vector<int> &atmtypes,
               const std::array<std::array<double, 3>, 3> &box) override {
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
