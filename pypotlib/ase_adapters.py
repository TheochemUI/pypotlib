__all__ = ["PyPotLibCalc"]

import warnings
import numpy as np
from ase.calculators.calculator import Calculator, all_changes

class PyPotLibCalc(Calculator):
    implemented_properties = ['energy', 'forces']
    discard_results_on_any_change = True

    def __init__(self, cpot_instance):
        Calculator.__init__(self)
        self.cpot = cpot_instance

    def calculate(self, atoms=None,
                  properties=['energy'],
                  system_changes=all_changes):
        Calculator.calculate(self, atoms,
                             properties, system_changes)
        positions = atoms.get_positions()
        atmtypes = atoms.get_atomic_numbers()
        box = atoms.get_cell()
        if np.all(box == np.zeros(3)):
            warnings.warn("Box is zero, setting to rectangular 100, 100, 100")
            box = np.eye(3)*100
        energy, forces = self.cpot(positions, atmtypes, box)
        self.results['energy'] = energy
        self.results['forces'] = forces
