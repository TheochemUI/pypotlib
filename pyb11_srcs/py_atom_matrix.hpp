// pyb11_srcs/py_atom_matrix.hpp
#pragma once
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "../subprojects/potlib/CppCore/rgpot/types/AtomMatrix.hpp"

namespace pybind11::detail {

template <>
struct type_caster<rgpot::types::AtomMatrix> {
public:
    PYBIND11_TYPE_CASTER(rgpot::types::AtomMatrix, _("numpy.ndarray[numpy.float64]"));

    bool load(handle src, bool convert) {
        if (!convert && !array_t<double>::check_(src))
            return false;

        auto buf = array_t<double, array::c_style | array::forcecast>::ensure(src);
        if (!buf)
            return false;

        if (buf.ndim() != 2)
            return false;

        size_t rows = buf.shape(0);
        size_t cols = buf.shape(1);

        value = rgpot::types::AtomMatrix(rows, cols);
        std::memcpy(value.data(), buf.data(), rows * cols * sizeof(double));

        return true;
    }

    static handle
    cast(const rgpot::types::AtomMatrix &src, return_value_policy /*policy*/, handle /*parent*/) {
        array_t<double> ret({src.rows(), src.cols()});
        std::memcpy(ret.mutable_data(), src.data(), src.rows() * src.cols() * sizeof(double));
        return ret.release();
    }
};

} // namespace pybind11::detail
