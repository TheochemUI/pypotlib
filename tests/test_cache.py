import shutil
import time
import numpy as np
import pytest
from ase import Atoms
from pypotlib import cpot
from pypotlib.ase_adapters import PyPotLibCalc


# Helper to create a random system
def create_system(n_atoms=10):
    positions = np.random.rand(n_atoms, 3) * 10.0
    types = np.zeros(n_atoms, dtype=np.int32)
    box = np.eye(3) * 20.0
    return positions, types, box


@pytest.fixture
def clean_cache_path(tmp_path):
    """Fixture to provide a clean path for the RocksDB cache."""
    db_path = tmp_path / "pot_cache"
    return str(db_path)


def test_direct_cache_hit(clean_cache_path):
    """Test that the cache returns the same result for identical inputs."""
    cache = cpot.PotentialCache(clean_cache_path, True)
    pot = cpot.LJPot()
    pot.set_cache(cache)

    pos, types, box = create_system()

    e1, f1 = pot(pos, types, box)
    e2, f2 = pot(pos, types, box)

    assert e1 == pytest.approx(e2)
    np.testing.assert_array_equal(f1, f2)


def test_cache_persistence(clean_cache_path):
    """Test that the cache persists across different Potential/Cache instances."""
    pos, types, box = create_system()
    expected_e = None

    # Phase 1: Write to cache
    def phase_one():
        cache = cpot.PotentialCache(clean_cache_path, True)
        pot = cpot.LJPot()
        pot.set_cache(cache)
        e, _ = pot(pos, types, box)
        return e

    expected_e = phase_one()

    # Force garbage collection / object destruction logic by leaving function scope
    # (Though in CPython refcounting handles this immediately)

    # Phase 2: Read from cache (New instances)
    def phase_two():
        cache_new = cpot.PotentialCache(clean_cache_path, False)  # Open existing
        pot_new = cpot.LJPot()
        pot_new.set_cache(cache_new)

        e_new, _ = pot_new(pos, types, box)
        assert e_new == pytest.approx(expected_e)

    phase_two()


def test_ase_adapter_lifecycle(clean_cache_path):
    """Test the ASE adapter correctly handles cache and internal state."""
    cache = cpot.PotentialCache(clean_cache_path, True)
    pot = cpot.LJPot()
    pot.set_cache(cache)

    atoms = Atoms(
        "Ar2", positions=[[0, 0, 0], [0, 0, 2.5]], cell=[10, 10, 10], pbc=True
    )

    calc = PyPotLibCalc(pot)
    atoms.calc = calc

    # 1. Get Potential Energy (Triggers C++ calculation)
    e1 = atoms.get_potential_energy()

    # 2. Get Forces (Should hit ASE memory cache)
    f1 = atoms.get_forces()
    assert len(f1) == 2

    # 3. Move atoms (Invalidates ASE memory cache)
    atoms.positions[1, 2] = 3.0

    # 4. Get New Energy (Triggers C++ calc, Miss C++ cache due to new pos)
    e2 = atoms.get_potential_energy()
    assert e2 != e1

    # 5. Move atoms BACK (Invalidates ASE memory, but HITS C++ cache)
    atoms.positions[1, 2] = 2.5
    e3 = atoms.get_potential_energy()

    assert e3 == pytest.approx(e1)


def test_cache_sharing_between_calculators(clean_cache_path):
    """Test multiple ASE calculators sharing the same underlying RocksDB."""
    cache = cpot.PotentialCache(clean_cache_path, True)

    pot1 = cpot.LJPot()
    pot1.set_cache(cache)
    pot2 = cpot.LJPot()
    pot2.set_cache(cache)

    atoms1 = Atoms("Ar", positions=[[0, 0, 0]], cell=[10, 10, 10])
    atoms2 = Atoms("Ar", positions=[[0, 0, 0]], cell=[10, 10, 10])  # Identical state

    atoms1.calc = PyPotLibCalc(pot1)
    atoms2.calc = PyPotLibCalc(pot2)

    # Calc 1 (Writes to DB)
    e1 = atoms1.get_potential_energy()

    # Calc 2 (Reads from DB)
    start = time.time()
    e2 = atoms2.get_potential_energy()
    duration = time.time() - start

    assert e1 == pytest.approx(e2)
    # Sanity check for speed (cache hit should be instant)
    assert duration < 1.0


def test_ase_stress_warning():
    """Verify that unsupported properties don't crash but raise appropriate errors."""
    pot = cpot.LJPot()
    calc = PyPotLibCalc(pot)
    atoms = Atoms("Ar", positions=[[0, 0, 0]], cell=[10, 10, 10])
    atoms.calc = calc

    # ASE default behavior for unimplemented properties is raising PropertyNotImplementedError
    # We check for generic Exception to cover that.
    with pytest.raises(Exception):
        atoms.get_stress()
