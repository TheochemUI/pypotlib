project(
    'pypotlib',
    'cpp',
    version: '0.0.14',
    default_options: ['warning_level=2', 'cpp_std=c++20'],
)

# Host system check
host_system = host_machine.system()

# Compiler Setup
cpp_args = ['-Wall', '-Wextra', '-Wpedantic']
_args = [cpp_args]
_deps = []
_linkto = []

# Languages
add_languages('c', required: true)
add_languages('fortran', required: true)
cc = meson.get_compiler('c')
cppc = meson.get_compiler('cpp')

# ---------------------- Library Dependencies
pot_subproject = subproject('potlib', version: '0.1.0')

potlib_dep = declare_dependency(
    include_directories: pot_subproject.get_variable('_incdirs'),  # Added this!
    link_with: pot_subproject.get_variable('_linkto'),
    dependencies: pot_subproject.get_variable('_deps'),
)
_deps += potlib_dep

# ---------------------- Bindings
eigen_dep = dependency('eigen3')
_deps += eigen_dep
py_mod = import('python')
py = py_mod.find_installation(pure: false)
pyb11_dep = [py.dependency(), dependency('pybind11')]
_deps += [pyb11_dep]

# cpot extension module
py.extension_module(
    'cpot',
    sources: [
        'pyb11_srcs/py_wrapper.cc',
        'pyb11_srcs/py_pottypes.cc',
        'pyb11_srcs/py_potential.cc',
        'pyb11_srcs/LennardJones/py_ljpot.cc',
        'pyb11_srcs/CuH2/py_cuh2pot.cc',
        'pyb11_srcs/py_cache.cc',
    ],
    dependencies: _deps,
    link_with: _linkto,
    cpp_args: _args,
    install: true,
    subdir: 'pypotlib/',
)

# pypotlib python package
py.install_sources(
    [
        'pypotlib/__init__.py',
    ],
    pure: false,
    subdir: 'pypotlib',
)

# Adapters
py.install_sources(
    [
        'pypotlib/ase_adapters.py',
        'pypotlib/_aux.py',
    ],
    pure: false,
    subdir: 'pypotlib',
)

# Systems
py.install_sources(
    [
        'pypotlib/systems/cu_slab_h2.py',
    ],
    pure: false,
    subdir: 'pypotlib/systems',
)
