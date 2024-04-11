project('pypotlib', 'cpp',
       version: '0.0.13',
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
potlib_dep = declare_dependency(link_with: subproject('potlib', version: '0.1').get_variable('_linkto'),
                               dependencies: subproject('potlib', version: '0.1').get_variable('_deps'),
                               )
_deps += potlib_dep

# ---------------------- Bindings
py_mod = import('python')
py = py_mod.find_installation(pure: false)
pyb11_dep = [
  py.dependency(),
  dependency('pybind11')
]
_deps += [pyb11_dep]

# cpot, for the bindings to potlib
py.extension_module(
  'cpot',
  sources : [
    'pyb11_srcs/py_wrapper.cc',
    'pyb11_srcs/py_pottypes.cc',
    'pyb11_srcs/py_potential.cc',
    'pyb11_srcs/LennardJones/py_ljpot.cc',
    'pyb11_srcs/CuH2/py_cuh2pot.cc',
  ],
  dependencies: _deps,
  link_with: _linkto,
  cpp_args : _args,
  install: true,
  subdir: 'pypotlib/'
)

# pypotlib, main package
py.install_sources([
    'pypotlib/__init__.py',
  ],
  pure: false, # install next to compiled extension
  subdir: 'pypotlib'
)

# Adapters
py.install_sources([
    'pypotlib/ase_adapters.py',
    'pypotlib/_aux.py',
  ],
  pure: false,
  subdir: 'pypotlib'
)

# Systems
py.install_sources([
    'pypotlib/systems/cu_slab_h2.py',
  ],
  pure: false,
  subdir: 'pypotlib/systems'
)
