project('pypotlib', 'cpp',
       version: '0.0.4',
  default_options : ['warning_level=2', 'cpp_std=c++17'])
# IMPORTANT!! warning_level=3 passes -fimplicit-none
# Many of the older Fortran codes need implicit typing

host_system = host_machine.system()

# Add C++ compiler options
cpp_args = ['-Wall', '-Wextra', '-Wpedantic']
_args = [cpp_args] # Extra arguments
_deps = [] # Dependencies
_linkto = [] # All the sub-libraries (potentials)

# Languages
add_languages('c', required: true)
add_languages('fortran', required: true)
cc = meson.get_compiler('c')
cppc = meson.get_compiler('cpp')

# ---------------------- Library Dependencies
potlib_deps = subproject('potlib', version: '0.1').get_variable('_deps')
potlib = subproject('potlib', version: '0.1').get_variable('_linkto')
_deps += potlib_deps
_linkto += potlib

# ---------------------- Bindings
py_mod = import('python')
py = py_mod.find_installation(pure: false)


py = import('python').find_installation()
python_dep = py.dependency()
pyb11f_deps = [ python_dep ]


_deps += [dependency('python3'),
          dependency('pybind11'),
          declare_dependency(link_args: '-lstdc++') ]

py.extension_module(
  'pypotlib',
  sources : [
    'bindings/py_wrapper.cc',
    'bindings/py_pottypes.cc',
    'bindings/py_potential.cc',
    'bindings/LennardJones/py_ljpot.cc',
    'bindings/CuH2/py_cuh2pot.cc',
  ],
  dependencies: _deps,
  link_with: _linkto,
  cpp_args : _args,
  install: true
)
