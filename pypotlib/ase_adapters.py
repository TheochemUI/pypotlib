__all__ = ["PyPotLibCalc"]

import warnings
import numpy as np
from ase.calculators.calculator import Calculator, all_changes


class PyPotLibCalc(Calculator):
    """
    ASE Calculator adapter for pypotlib potentials.

    This calculator delegates the heavy lifting to the C++ potential instance.
    It leverages both ASE's runtime caching (via `self.results` and `check_state`)
    and the optional persistent caching provided by the underlying C++ potential.
    """

    implemented_properties = ["energy", "forces"]

    # We set this to False because we want to rely on check_state to invalidate
    # only when necessary, and allow `calculate()` to handle the logic.
    # However, since potlib computes energy and forces simultaneously,
    # we update both.
    discard_results_on_any_change = True

    def __init__(self, cpot_instance, **kwargs):
        """
        Initialize the calculator.

        Parameters
        ----------
        cpot_instance : pypotlib.cpot.Potential
            An initialized instance of a pypotlib potential (e.g. LJPot, CuH2Pot).
        **kwargs
            Arguments passed to the base ASE Calculator.
        """
        super().__init__(**kwargs)
        self.cpot = cpot_instance

    def calculate(self, atoms=None, properties=["energy"], system_changes=all_changes):
        # Standard ASE housekeeping (sets self.atoms, handles system_changes)
        super().calculate(atoms, properties, system_changes)

        # Extract data for C++ call
        positions = self.atoms.get_positions()
        atmtypes = self.atoms.get_atomic_numbers().astype(np.int32)
        box = self.atoms.get_cell()

        # Sanity check for box (potlib requires a valid box)
        # NOTE(rg): Some potentials like LJ might work without a box if PBC is off,
        # but potlib assumes a box exists for historical eOn reasons.
        if np.all(np.abs(box) < 1e-6):
            # Warn once if the box is effectively zero
            warnings.warn(
                "Unit cell is empty or zero-volume. pypotlib potentials typically "
                "require a defined cell. Setting to default 100.0 * identity.",
                UserWarning,
            )
            box = np.eye(3) * 100.0

        # Call the C++ potential
        # This call handles the RocksDB caching internally if configured.
        energy, forces = self.cpot(positions, atmtypes, box)

        # Store results
        # Potlib returns both; we store both regardless of which 'property' was asked.
        self.results["energy"] = energy
        self.results["forces"] = forces
