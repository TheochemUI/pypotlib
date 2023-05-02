---
jupytext:
  formats: ipynb,md:myst
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
    jupytext_version: 1.14.5
kernelspec:
  display_name: rgpotpy
  language: python
  name: rgpotpy
---

```{code-cell} ipython3
from pypotlib.systems import cu_slab_h2 as cuh2slab
```

```{code-cell} ipython3
import numpy as np
import ase.io
```

```{code-cell} ipython3
cuh2init = ase.io.read("data/cuh2/init_shift.con")
cuh2init.set_calculator(cuh2slab.CuH2PotSlab())
cuh2init.get_potential_energy()
```

```{code-cell} ipython3
# min1 = ase.io.read("data/cuh2/min1_0Cu.con")
# min2 = ase.io.read("data/cuh2/min2_0Cu.con")
# init = ase.io.read("data/cuh2/init.con")

# min1.positions[:, 2:] = min1.positions[:, 2:] - np.max(min1.positions[:-2, 2:])
# min2.positions[:, 2:] = min2.positions[:, 2:] - np.max(min2.positions[:-2, 2:])
# init.positions[:, 2:] = init.positions[:, 2:] - np.max(init.positions[:-2, 2:])

# ase.io.write("data/cuh2/min1_0Cu_shift.con", min1)
# ase.io.write("data/cuh2/min2_0Cu_shift.con", min2)
# ase.io.write("data/cuh2/init_shift.con", init)
```

```{code-cell} ipython3
pltdat = cuh2slab.plt_data(cuh2init)
```

```{code-cell} ipython3
plt = cuh2slab.contour_plot(pltdat.pltpts)
```
