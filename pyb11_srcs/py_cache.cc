#include "py_wrapper.hpp"
#include "../subprojects/potlib/CppCore/rgpot/PotentialCache.hpp"

namespace py = pybind11;
using rgpot::cache::PotentialCache;

void py_cache(py::module_ &m) {
    py::class_<PotentialCache>(m, "PotentialCache")
        .def(py::init<std::string, bool>(), "db_path"_a, "create_if_missing"_a = true)
        // We don't strictly need to expose set_db for Python usage
        // as the constructor covers the "Managed" case.
        .def("__repr__", [](const PotentialCache &) {
            return "<pypotlib.cpot.PotentialCache>";
        });
}
