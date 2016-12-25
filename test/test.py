import pytest


@pytest.fixture(scope='function')
def context(request):
    """
    Add context to test functions.
    """
    from numgrid import lib
    ctx = lib.numgrid_new()

    def cleanup():
        """
        Clean up the context.
        """
        lib.numgrid_free(ctx)

    request.addfinalizer(cleanup)
    return ctx


def test_h2o_grid(context):
    """
    Test H2O grid generation.
    """
    from numgrid import lib

    radial_precision = 1.0e-12
    min_num_angular_points = 86
    max_num_angular_points = 302

    num_centers = 3
    center_coordinates = [
        0.0000e+00,
        0.0000e+00,
        0.0000e+00,
        5.5168e-01,
        7.7340e-01,
        0.0000e+00,
        5.5168e-01,
        -7.734e-01,
        0.0000e+00,
    ]
    center_elements = [8, 1, 1]

    num_outer_centers = 0
    outer_center_coordinates = []
    outer_center_elements = []

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

    ierr = lib.numgrid_generate(context,
                                radial_precision,
                                min_num_angular_points,
                                max_num_angular_points,
                                num_centers,
                                center_coordinates,
                                center_elements,
                                num_outer_centers,
                                outer_center_coordinates,
                                outer_center_elements,
                                num_shells,
                                shell_centers,
                                shell_l_quantum_numbers,
                                shell_num_primitives,
                                primitive_exponents)

    num_points = lib.numgrid_get_num_points(context)
    assert num_points == 46220

    here = os.path.abspath(os.path.dirname(__file__))
    reference_grid = []
    with open(os.path.join(here, 'referece_grid.txt'), 'r') as f:
        for line in f.readlines():
            for x in line.split():
                reference_grid.append(float(x))
    grid = lib.numgrid_get_grid(context)
    for i in range(num_points):
        error = grid[i] - reference_grid[i]
        if abs(reference_grid[i]) > 1.0e-20:
            error /= reference_grid[i]
        assert abs(error) < 1.0e-6
