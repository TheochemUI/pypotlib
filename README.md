# `pypotlib` [![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](code_of_conduct.md) ![Builds](https://github.com/TheochemUI/pypotlib/actions/workflows/build_test.yml/badge.svg) ![Wheels](https://github.com/TheochemUI/pypotlib/actions/workflows/build_wheels.yml/badge.svg)

Python bindings and ASE adapters for `potlib`.

## Details

The library consists of thin wrappers to `potlib` under `cpot` and a
`PyPotLibCalc` class which is an `ase` calculator under `ase_adapters`.

## Builds

### Local usage

The easiest way is to use the environment file, compatible with `conda`,
`mamba`, `micromamba` etc.

```bash
mamba env create -f environment.yml
mamba activate rgpotpy
pdm install
```

### Production

As such, due to the compiled extensions and what not, `cibuildwheel` is used to
generate `macos` and `linux` wheels. Locally this may be emulated (on `linux`)
by:

```bash
cibuildwheel --output-dir wheelhouse --platform linux
```

## Usage examples

The simplest usage is just:

```python
import pypotlib as ppl
import numpy as np
ljpot = ppl.cpot.LJPot()
pos = np.array([1, 2, 3, 1.2, 2.3, 3.6]).reshape(-1, 3)
atm_types = [0, 0]
cell_dim = np.eye(3)*50
print(ljpot(pos, atm_types, cell_dim))
```

For using the ASE calculator we need an instantiated class.

```python
from ase import Atoms
from pypotlib import cpot
from pypotlib.ase_adapters import PyPotLibCalc
atoms = Atoms(symbols=['Cu', 'H'], positions=[[0, 0, 0], [0.5, 0.5, 0.5]])
calc = PyPotLibCalc(cpot.CuH2Pot())
atoms.set_calculator(calc)
print(atoms.get_potential_energy())
print(atoms.get_forces())
```

To run an NEB with this, consider the following toy example:

```python
from ase import Atoms
from ase.neb import NEB
from ase.optimize import BFGS

from pypotlib import cpot
from pypotlib.ase_adapters import PyPotLibCalc

atoms_initial = Atoms(symbols=['H', 'H'], positions=[(0, 0, 0), (0, 0, 1)])
atoms_final = Atoms(symbols=['H', 'H'], positions=[(0, 0, 2), (0, 0, 3)])

images = [atoms_initial]
images += [atoms_initial.copy() for idx in range(3)]
images += [atoms_final]

for image in images:
    image.calc = PyPotLibCalc(cpot.LJPot())

neb = NEB(images)
neb.interpolate(method = 'idpp')
optimizer = BFGS(neb)
optimizer.run(fmax=0.04)
```


# Contributions

All contributions are welcome, this includes code and documentation
contributions but also questions or other clarifications. Note that we expect
all contributors to follow our [Code of
Conduct](https://github.com/TheochemUI/pypotlib/blob/main/CODE_OF_CONDUCT.md).

# License
[MIT](https://github.com/TheochemUI/pypotlib/blob/main/LICENSE).
