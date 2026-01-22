#pragma once
// MIT License
// Copyright 2026--present Rohit Goswami <HaoZeke>

#include <pybind11/pybind11.h>

namespace py = pybind11;

/**
 * Helper to enable copy, deepcopy, and basic pickling for Potentials.
 * NOTE(rg): We return `new T(self)` (raw pointer) instead of `T(self)` (value).
 * Returning by value creates a temporary stack object which registers itself
 * in the PotHelpers registry. When that temporary dies (after being moved to Python),
 * the registry holds a dangling pointer.
 * By returning a pointer allocated with `new`, pybind11 takes ownership,
 * the object stays at that memory address, and the registry remains valid.
 */
template <typename T, typename PyClass>
void enable_copy_pickle(PyClass &cls) {
    // Copy Support
    cls.def("__copy__", [](const T &self) { return new T(self); });

    // Deepcopy Support (Same as copy for these potentials)
    cls.def("__deepcopy__", [](const T &self, py::dict /* memo */) { return new T(self); });

    // Pickle Support
    // Assumes the potential can be reconstructed from default constructor.
    // If potentials have state (parameters), this needs to be specialized.
    cls.def(py::pickle(
        [](const T &self) { // __getstate__
            // Return empty tuple if no state needs saving.
            // (Cache pointer is NOT saved/restored on purpose)
            return py::make_tuple();
        },
        [](py::tuple t) { // __setstate__
            // Reconstruct
            return new T();
        }));
}
