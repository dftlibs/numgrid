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
    with open(os.path.join(_here, "reference", "cc-pVDZ.txt"), "r") as f:
        for line in f.read().splitlines():
            (x, y, z, w) = line.split()
            reference_grid_x_bohr.append(float(x))
            reference_grid_y_bohr.append(float(y))
            reference_grid_z_bohr.append(float(z))
            reference_grid_w.append(float(w))
    reference_num_points = [16364, 14928, 14928]

    radial_precision = 1.0e-12
    min_num_angular_points = 86
    max_num_angular_points = 302

    num_centers = 3
    proton_charges = [8, 1, 1]

    center_coordinates_bohr = [(0.0, 0.0, 0.0), (1.43, 0.0, 1.1), (-1.43, 0.0, 1.1)]

    # cc-pVDZ basis
    alpha_max = [11720.0, 13.01, 13.01]  # O, H, H
    max_l_quantum_numbers = [2, 1, 1]  # O, H, H
    alpha_min = [[0.3023, 0.2753, 1.185], [0.122, 0.727], [0.122, 0.727]]  # O  # H  # H

    hardness = 3

    offset = 0
    for center_index in range(num_centers):
        if use_bse:
            xyz, w = numgrid.atom_grid_bse(
                "cc-pVDZ",
                radial_precision,
                min_num_angular_points,
                max_num_angular_points,
                num_centers,
                proton_charges,
                center_index,
                center_coordinates_bohr,
                hardness,
            )
        else:
            xyz, w = numgrid.atom_grid(
                radial_precision,
                alpha_min[center_index],
                alpha_max[center_index],
                max_l_quantum_numbers[center_index],
                min_num_angular_points,
                max_num_angular_points,
                num_centers,
                proton_charges,
                center_index,
                center_coordinates_bohr,
                hardness,
            )

        num_points = len(w)
        assert num_points == reference_num_points[center_index]

        rel_error = 1.0e-9
        for i in range(num_points):
            assert xyz[i][0] == approx(reference_grid_x_bohr[offset + i], rel=rel_error)
            assert xyz[i][1] == approx(reference_grid_y_bohr[offset + i], rel=rel_error)
            assert xyz[i][2] == approx(reference_grid_z_bohr[offset + i], rel=rel_error)
        assert w == approx(
            reference_grid_w[offset : offset + num_points], rel=rel_error
        )

        # we only check that call works but for the moment do not verify results
        r, w = numgrid.radial_grid(
            radial_precision,
            alpha_min[center_index],
            alpha_max[center_index],
            max_l_quantum_numbers[center_index],
            proton_charges[center_index],
        )

        # we only check that call works but for the moment do not verify results
        xyz, w = numgrid.angular_grid(14)

        offset += num_points


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
