#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>

// clang-format off
#include "py_wrapper.hpp"
#include "../subprojects/potlib/CppCore/rgpot/Potential.hpp"
// clang-format on

#include <pybind11/stl.h>

using namespace rgpot;

// Trampoline for Abstract Base (PotentialBase)
class PyPotentialBase : public PotentialBase {
public:
    using PotentialBase::PotentialBase;

    std::pair<double, AtomMatrix>
    operator()(const AtomMatrix &positions,
               const std::vector<int> &atmtypes,
               const std::array<std::array<double, 3>, 3> &box) override {
        // Fix: Use an alias to avoid comma in macro argument
        using Return = std::pair<double, AtomMatrix>;

        PYBIND11_OVERRIDE_PURE_NAME(Return,        // Return type
                                    PotentialBase, // Parent class
                                    "__call__",    // Name in Python
                                    operator(),    // Name in C++
                                    positions,     // Arguments
                                    atmtypes,
                                    box);
    }

    void set_cache(rgpot::cache::PotentialCache *c) override {
        PYBIND11_OVERRIDE(void, PotentialBase, set_cache, c);
    }
};

// Template Trampoline for Concrete Classes (LJPot, CuH2Pot)
template <class Base>
class PyPotentialConcrete : public Base {
public:
    using Base::Base;

    std::pair<double, AtomMatrix>
    operator()(const AtomMatrix &positions,
               const std::vector<int> &atmtypes,
               const std::array<std::array<double, 3>, 3> &box) override {

        using Return = std::pair<double, AtomMatrix>;

        PYBIND11_OVERRIDE_NAME(Return,     // Return type
                               Base,       // Parent class
                               "__call__", // Name in Python
                               operator(), // Name in C++
                               positions,  // Arguments
                               atmtypes,
                               box);
    }

    void set_cache(rgpot::cache::PotentialCache *c) override {
        PYBIND11_OVERRIDE(void, Base, set_cache, c);
    }
};
