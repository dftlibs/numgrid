from pytest import approx
import os
import numgrid


def test_h2o_grid():
    """
    Test H2O grid generation.
    """

    # read reference grid from file
    # we will compare results against this file
    _here = os.path.abspath(os.path.dirname(__file__))
    reference_grid_x_au = []
    reference_grid_y_au = []
    reference_grid_z_au = []
    reference_grid_w = []
    with open(os.path.join(_here, 'reference_grid.txt'), 'r') as f:
        for line in f.read().splitlines():
            (x, y, z, w) = line.split()
            reference_grid_x_au.append(float(x))
            reference_grid_y_au.append(float(y))
            reference_grid_z_au.append(float(z))
            reference_grid_w.append(float(w))
    reference_num_points = [16364, 14928, 14928]

    radial_precision = 1.0e-12
    min_num_angular_points = 86
    max_num_angular_points = 302

    num_centers = 3
    proton_charges = [8, 1, 1]

    x_coordinates_au = [0.0, 1.43, -1.43]
    y_coordinates_au = [0.0, 0.0, 0.0]
    z_coordinates_au = [0.0, 1.1, 1.1]

    # cc-pVDZ
    alpha_max = [11720.0, 13.01, 13.01]
    max_l_quantum_numbers = [2, 1, 1]
    alpha_min = [[0.3023, 0.2753, 1.185],  # oxygen
                 [0.122, 0.727],  # hydrogen
                 [0.122, 0.727]]  # hydrogen

    offset = 0
    for center_index in range(num_centers):
        context = numgrid.new_atom_grid(radial_precision,
                                        min_num_angular_points,
                                        max_num_angular_points,
                                        proton_charges[center_index],
                                        alpha_max[center_index],
                                        max_l_quantum_numbers[center_index],
                                        alpha_min[center_index])

        num_points = numgrid.get_num_grid_points(context)
        assert num_points == reference_num_points[center_index]

        x, y, z, w = numgrid.get_grid_points(context,
                                             num_centers,
                                             center_index,
                                             x_coordinates_au,
                                             y_coordinates_au,
                                             z_coordinates_au,
                                             proton_charges)

        assert x == approx(reference_grid_x_au[offset:offset + num_points], rel=1e-9)
        assert y == approx(reference_grid_y_au[offset:offset + num_points], rel=1e-9)
        assert z == approx(reference_grid_z_au[offset:offset + num_points], rel=1e-9)
        assert w == approx(reference_grid_w[offset:offset + num_points], rel=1e-9)

        offset += num_points

        numgrid.free_atom_grid(context)


def test_version():
    assert numgrid.get_version() == '1.0.0-alpha'
