
#ifndef PRESSIODEMOAPPS_TYPES_HPP_
#define PRESSIODEMOAPPS_TYPES_HPP_

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace pressiodemoappspy
{

using scalar_t	= double;
using py_cstyle_arr_sc = pybind11::array_t<scalar_t, pybind11::array::c_style>;
using py_fstyle_arr_sc = pybind11::array_t<scalar_t, pybind11::array::f_style>;

using ordinal_t   = int32_t;
using py_cstyle_arr_int = pybind11::array_t<ordinal_t, pybind11::array::c_style>;
using py_fstyle_arr_int = pybind11::array_t<ordinal_t, pybind11::array::f_style>;

}
#endif
