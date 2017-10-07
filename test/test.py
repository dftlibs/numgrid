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

    radial_precision = 1.0e-12
    min_num_angular_points = 86
    max_num_angular_points = 302

    num_centers = 3
    proton_charges = [8, 1, 1]
    num_shells = 12
    shell_centers = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3]
    shell_l_quantum_numbers = [0, 0, 0, 1, 1, 2, 0, 0, 1, 0, 0, 1]
    shell_num_primitives = [9, 9, 1, 4, 1, 1, 4, 1, 1, 4, 1, 1]
    primitive_exponents = [
        1.172e+04,
        1.759e+03,
        4.008e+02,
        1.137e+02,
        3.703e+01,
        1.327e+01,
        5.025e+00,
        1.013e+00,
        3.023e-01,
        1.172e+04,
        1.759e+03,
        4.008e+02,
        1.137e+02,
        3.703e+01,
        1.327e+01,
        5.025e+00,
        1.013e+00,
        3.023e-01,
        3.023e-01,
        1.770e+01,
        3.854e+00,
        1.046e+00,
        2.753e-01,
        2.753e-01,
        1.185e+00,
        1.301e+01,
        1.962e+00,
        4.446e-01,
        1.220e-01,
        1.220e-01,
        7.270e-01,
        1.301e+01,
        1.962e+00,
        4.446e-01,
        1.220e-01,
        1.220e-01,
        7.270e-01,
    ]

    reference_num_points = [16364, 14928, 14928]

    x_coordinates_au = [0.0, 1.43, -1.43]
    y_coordinates_au = [0.0, 0.0, 0.0]
    z_coordinates_au = [0.0, 1.1, 1.1]

    offset = 0
    for i in range(num_centers):
        _num_shells = 0
        _shell_l_quantum_numbers = []
        _shell_num_primitives = []
        _primitive_exponents = []
        k = 0
        for j, center in enumerate(shell_centers):
            if i == center - 1:
                _num_shells += 1
                _shell_l_quantum_numbers.append(shell_l_quantum_numbers[j])
                _shell_num_primitives.append(shell_num_primitives[j])
                for ip in range(shell_num_primitives[j]):
                    _primitive_exponents.append(primitive_exponents[k])
                    k += 1
            else:
                k += shell_num_primitives[j]
        context = numgrid.new_atom_grid(radial_precision,
                                        min_num_angular_points,
                                        max_num_angular_points,
                                        proton_charges[i],
                                        _num_shells,
                                        _shell_l_quantum_numbers,
                                        _shell_num_primitives,
                                        _primitive_exponents)

        num_points = numgrid.get_num_grid_points(context)
        assert num_points == reference_num_points[i]

        x, y, z, w = numgrid.get_grid_points(context,
                                             num_centers,
                                             i,
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
    from numgrid import get_version
    assert get_version() == '1.0.0-alpha'
