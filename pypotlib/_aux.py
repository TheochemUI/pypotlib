__all__ = ["reshape_data"]

import numpy as np

def reshape_data(data):
    x = [d.x for d in data]
    y = [d.y for d in data]
    energy = [d.energy for d in data]
    nx = len(set(x))
    ny = len(set(y))
    x = np.array(x).reshape(ny, nx)
    y = np.array(y).reshape(ny, nx)
    energy = np.array(energy).reshape(ny, nx)
    return x, y, energy
