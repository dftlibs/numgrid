from pytest import approx
import os
import numgrid


def _helper(use_bse):

    # read reference grid from file
    # we will compare results against this file
    reference_grid_x_bohr = []
    reference_grid_y_bohr = []
    reference_grid_z_bohr = []
    reference_grid_w = []
    _here = os.path.abspath(os.path.dirname(__file__))
    with open(os.path.join(_here, 'cc-pVDZ.txt'), 'r') as f:
        for line in f.read().splitlines():
            (x, y, z, w) = line.split()
            reference_grid_x_bohr.append(float(x))
            reference_grid_y_bohr.append(float(y))
            reference_grid_z_bohr.append(float(z))
            reference_grid_w.append(float(w))
    reference_num_points = [16364, 14928, 14928]
    reference_num_radial_points = [106, 78, 78]

    radial_precision = 1.0e-12
    min_num_angular_points = 86
    max_num_angular_points = 302

    num_centers = 3
    proton_charges = [8, 1, 1]

    x_coordinates_bohr = [0.0, 1.43, -1.43]
    y_coordinates_bohr = [0.0, 0.0, 0.0]
    z_coordinates_bohr = [0.0, 1.1, 1.1]

    # cc-pVDZ basis
    alpha_max = [11720.0, 13.01, 13.01]  # O, H, H
    max_l_quantum_numbers = [2, 1, 1]  # O, H, H
    alpha_min = [[0.3023, 0.2753, 1.185],  # O
                 [0.122, 0.727],  # H
                 [0.122, 0.727]]  # H

    offset = 0
    for center_index in range(num_centers):
        if use_bse:
            context = numgrid.new_atom_grid_bse(radial_precision,
                                                min_num_angular_points,
                                                max_num_angular_points,
                                                proton_charges[center_index],
                                                'cc-pVDZ')
        else:
            context = numgrid.new_atom_grid(radial_precision,
                                            min_num_angular_points,
                                            max_num_angular_points,
                                            proton_charges[center_index],
                                            alpha_max[center_index],
                                            max_l_quantum_numbers[center_index],
                                            alpha_min[center_index])

        num_points = numgrid.get_num_grid_points(context)
        assert num_points == reference_num_points[center_index]

        num_radial_points = numgrid.get_num_radial_grid_points(context)
        assert num_radial_points == reference_num_radial_points[center_index]

        x, y, z, w = numgrid.get_grid(context,
                                      num_centers,
                                      center_index,
                                      x_coordinates_bohr,
                                      y_coordinates_bohr,
                                      z_coordinates_bohr,
                                      proton_charges)

        rel_error = 1.0e-9
        assert x == approx(reference_grid_x_bohr[offset:offset + num_points], rel=rel_error)
        assert y == approx(reference_grid_y_bohr[offset:offset + num_points], rel=rel_error)
        assert z == approx(reference_grid_z_bohr[offset:offset + num_points], rel=rel_error)
        assert w == approx(reference_grid_w[offset:offset + num_points], rel=rel_error)

        # we only check that call works but for the moment do not verify results
        r, w = numgrid.get_radial_grid(context)

        # we only check that call works but for the moment do not verify results
        x, y, z, w = numgrid.get_angular_grid(num_angular_grid_points=14)

        offset += num_points

        numgrid.free_atom_grid(context)


def test_h2o_grid_explicit():
    """
    Test H2O grid generation using explicit exponents.
    """
    _helper(use_bse=False)


def test_h2o_grid_bse():
    """
    Test H2O grid generation using a basis set fetched via BSE web API.
    """
    _helper(use_bse=True)


def test_version():
    assert numgrid.get_version() == '1.1.0'
