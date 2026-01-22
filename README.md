# `pypotlib` [![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](https://github.com/TheochemUI/pypotlib/blob/main/code_of_conduct.md) ![Builds](https://github.com/TheochemUI/pypotlib/actions/workflows/build_test.yml/badge.svg) ![Wheels](https://github.com/TheochemUI/pypotlib/actions/workflows/build_wheels.yml/badge.svg)

Python bindings and ASE adapters for `potlib`.

## Details

The library consists of thin wrappers to `potlib` under `cpot` and a
`PyPotLibCalc` class which is an `ase` calculator under `ase_adapters`.

## Installation

This is [on PyPI](https://pypi.org/project/pypotlib), with wheels, so usage is simply:

```bash
pip install pypotlib
```

Users are advised to not try to build from source, since the underlying `potlib`
code includes `fortran` and `cpp` dependencies which can be slightly tricky to
work with.

### Local Development

The easiest way is to use the `pixi` environment.

```bash
pixi s
pdm install
# For tests
pixi s -e with-ase
pytest tests/test_cache.py
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

## Caching runs

`pypotlib` supports persistent caching via RocksDB. This allows energy and force
evaluations to be stored and retrieved, significantly speeding up repeated
calculations on identical configurations.

```python
import pypotlib.cpot as cpot
import numpy as np

# 1. Initialize the cache with a directory path
# This will create a RocksDB database at the specified location.
cache = cpot.PotentialCache("/tmp/my_pot_cache", create_if_missing=True)

# 2. Create the potential and link the cache
lj = cpot.LJPot()
lj.set_cache(cache)

# 3. Use as normal
pos = np.array([[0.0, 0.0, 0.0], [3.0, 0.0, 0.0]])
types = [1, 1]
box = np.eye(3) * 10.0

# First call: Computes and stores result in DB
e1, f1 = lj(pos, types, box)

# Second call (same inputs): Retrieves result from DB (Instant)
e2, f2 = lj(pos, types, box)
```

### ASE Caching

The ASE calculator provides more sophisticated caching, with the internal checks
for equivalent structures further reducing calls to the underlying compiled
code.

```python
from ase import Atoms
from pypotlib import cpot
from pypotlib.ase_adapters import PyPotLibCalc

# Setup Potential with Cache
cache = cpot.PotentialCache("ase_cache_db")
pot = cpot.CuH2Pot()
pot.set_cache(cache)

# Create Calculator
atoms = Atoms(symbols=["Cu", "H"], positions=[[0, 0, 0], [0.5, 0.5, 0.5]])
calc = PyPotLibCalc(pot)
atoms.set_calculator(calc)

print(atoms.get_potential_energy())
print(atoms.get_forces())
```

### NEB Example with Benchmarking

To really see the power of the cache, we can run an NEB optimization twice. The
first run performs the calculations and populates the RocksDB database. The
second run, performing the exact same optimization, hits the cache for every
step, reducing the computational cost to near zero.

```python
import time
import shutil
from ase import Atoms
from ase.mep import NEB
from ase.optimize import BFGS
from pypotlib import cpot
from pypotlib.ase_adapters import PyPotLibCalc

# Setup a persistent cache
cache_path = "/tmp/neb_demo_cache"
# Clear previous cache to ensure a "cold" start for demonstration
shutil.rmtree(cache_path, ignore_errors=True)
cache = cpot.PotentialCache(cache_path, create_if_missing=True)


def setup_neb_images():
    """Helper to create fresh images for the NEB."""
    atoms_initial = Atoms(symbols=["H", "H"], positions=[(0, 0, 0), (0, 0, 1)])
    atoms_final = Atoms(symbols=["H", "H"], positions=[(0, 0, 2), (0, 0, 3)])

    images = [atoms_initial]
    images += [atoms_initial.copy() for _ in range(3)]
    images += [atoms_final]

    # Attach calculators with the SHARED cache
    for image in images:
        pot = cpot.LJPot()
        pot.set_cache(cache)  # All images share the same DB
        image.calc = PyPotLibCalc(pot)

    return images


# --- Run 1: Cold Cache (Calculates & Writes) ---
print("Starting Run 1 (Cold Cache)...")
images_1 = setup_neb_images()
neb_1 = NEB(images_1)
neb_1.interpolate(method="idpp")
opt_1 = BFGS(neb_1)

start_1 = time.time()
opt_1.run(fmax=0.04)
duration_1 = time.time() - start_1
print(f"Run 1 finished in {duration_1:.4f} seconds.")

# --- Run 2: Warm Cache (Reads only) ---
print("\nStarting Run 2 (Warm Cache)...")
images_2 = setup_neb_images()  # Re-create identical initial state
neb_2 = NEB(images_2)
neb_2.interpolate(method="idpp")
opt_2 = BFGS(neb_2)

start_2 = time.time()
opt_2.run(fmax=0.04)
duration_2 = time.time() - start_2
print(f"Run 2 finished in {duration_2:.4f} seconds.")

# --- Results ---
speedup = duration_1 / duration_2 if duration_2 > 0 else 0
print(f"\nSpeedup factor: {speedup:.1f}x")
```

# Contributions

All contributions are welcome, this includes code and documentation
contributions but also questions or other clarifications. Note that we expect
all contributors to follow our [Code of
Conduct](https://github.com/TheochemUI/pypotlib/blob/main/CODE_OF_CONDUCT.md).

# License

[MIT](https://github.com/TheochemUI/pypotlib/blob/main/LICENSE).
