__all__ = ["CuH2PotSlab", "FuncVal", "PltRange",
           "PlotPtPos", "contour_plot", "plt_data",
           "set_slab_dist"]

import warnings
from collections import namedtuple

import numpy as np
import matplotlib.pyplot as plt
from ase.calculators.calculator import Calculator, all_changes

from pypotlib import cpot
from pypotlib.aux import reshape_data

FuncVal = namedtuple("FuncVal", ["x", "y", "energy"])
PltRange = namedtuple("PltRange", ["low", "high"])
PlotPtPosData = namedtuple('PlotPtPosData', ['h2pos', 'pltpts'])
PlotPoints = namedtuple('PlotPoints', ['x_npt', 'y_npt'])

class CuH2PotSlab(Calculator):
    implemented_properties = ["energy", "forces"]
    discard_results_on_any_change = True

    def __init__(self,
                 e_zero=FuncVal(x=0.752542,
                                y=5.0,
                                energy=-697.311695)):
        Calculator.__init__(self)
        self.cpot = cpot.CuH2Pot()
        self._ezero = e_zero

    def calculate(
        self, atoms=None, properties=["energy"], system_changes=all_changes
    ):
        Calculator.calculate(self, atoms, properties, system_changes)
        positions = atoms.get_positions()
        atmtypes = atoms.get_atomic_numbers()
        box = atoms.get_cell()
        if np.all(box == np.zeros(3)):
            warnings.warn("Box is zero, setting to diagonal(100, 100, 100)")
            box = np.eye(3) * 100
        energy, forces = self.cpot(positions, atmtypes, box)
        self.results["energy"] = energy - self._ezero.energy
        self.results["forces"] = forces

def set_slab_dist(atoms, dist):
    h_ind = np.where(np.asarray(atoms.get_chemical_symbols()) == "H")[0]
    n_hatoms = len(h_ind)
    max_slab_z = max(atoms.positions[:-n_hatoms, 2])
    cpos = atoms.positions.copy()
    cpos[-n_hatoms:, 2] = np.repeat(max_slab_z + dist, n_hatoms)
    atoms.set_positions(cpos)

def plt_data(_atms, hh_range=PltRange(low=0.4, high=3),
                  h2slab_range = PltRange(low=-0.05, high=5),
                  n_points=PlotPoints(x_npt=60, y_npt=60)):
    h_dists = np.linspace(
        hh_range.low, hh_range.high, n_points.x_npt
    )
    slab_dists = np.linspace(
        h2slab_range.low, h2slab_range.high, n_points.y_npt
    )
    h_ind = np.where(np.asarray(_atms.get_chemical_symbols()) == "H")[0]
    plt_data = []  # x, y, energy
    plt_h2pos = []  # h2 positions
    for slab_dist in slab_dists:
        set_slab_dist(_atms, slab_dist)
        for h_dist in h_dists:
            _atms.set_distance(*h_ind, h_dist)
            plt_h2pos.append(_atms.positions[h_ind])
            plt_data.append(
                FuncVal(
                    x=h_dist,
                    y=slab_dist,
                    energy=_atms.get_potential_energy(),
                )
            )
    return PlotPtPosData(h2pos=plt_h2pos, pltpts=plt_data)

def contour_plot(data, _max_val = 5, _nlvls=500):
    x, y, energy = reshape_data(data)
    # Create a contour plot of the energy surface
    fig, ax = plt.subplots()
    # Always clip
    cs = ax.contourf(
        x,
        y,
        energy,
        levels=np.linspace(0, _max_val, _nlvls),
        extend="max",
    )
    # Add labels and title to the plot
    ax.set_xlabel("H-H distance")
    ax.set_ylabel("Slab distance")
    ax.set_title('Energy Surface Contour Plot\nShifted by the zero of the energy')
    # Add a colorbar to the plot
    cbar = fig.colorbar(cs)
    # Show the plot
    # plt.show()
    return plt
