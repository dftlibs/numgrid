import numgrid

radial_precision = 1.0e-12
min_num_angular_points = 86
max_num_angular_points = 302

proton_charges = [8, 1, 1]

center_coordinates_bohr = [(0.0, 0.0, 0.0), (1.43, 0.0, 1.1), (-1.43, 0.0, 1.1)]

# cc-pVDZ basis
alpha_max = [11720.0, 13.01, 13.01]  # O, H, H
max_l_quantum_numbers = [2, 1, 1]  # O, H, H
alpha_min = [[0.3023, 0.2753, 1.185], [0.122, 0.727], [0.122, 0.727]]  # O  # H  # H

hardness = 3

offset = 0
for center_index in range(len(center_coordinates_bohr)):
    # atom grid using explicit basis set parameters
    coordinates, weights = numgrid.atom_grid(
        alpha_min[center_index],
        alpha_max[center_index],
        max_l_quantum_numbers[center_index],
        radial_precision,
        min_num_angular_points,
        max_num_angular_points,
        proton_charges,
        center_index,
        center_coordinates_bohr,
        hardness,
    )

    # atom grid using basis set name
    coordinates, weights = numgrid.atom_grid_bse(
        "cc-pVDZ",
        radial_precision,
        min_num_angular_points,
        max_num_angular_points,
        proton_charges,
        center_index,
        center_coordinates_bohr,
        hardness,
    )

    # radial grid using explicit basis set parameters
    radii, weights = numgrid.radial_grid(
        alpha_min[center_index],
        alpha_max[center_index],
        max_l_quantum_numbers[center_index],
        radial_precision,
        proton_charges[center_index],
    )

    # angular grid with 14 points
    coordinates, weights = numgrid.angular_grid(14)
