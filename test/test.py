"""
Unit tests.
"""
import sys
BUILD_DIR = sys.argv[-1]
sys.path.append(BUILD_DIR)

import os
import subprocess
import pytest
from numgrid import ffi, lib

#-------------------------------------------------------------------------------

@pytest.fixture(scope='function')
def context(request):
    """
    Add context to test functions.
    """
    ctx = lib.numgrid_new()
    def cleanup():
        """
        Clean up the context.
        """
        lib.numgrid_free(ctx)
    request.addfinalizer(cleanup)
    return ctx

#-------------------------------------------------------------------------------

def test_h2o_grid(context):
    """
    Test H2O grid generation.
    """
    radial_precision = 1.0e-12
    angular_min = 86
    angular_max = 302

    num_centers = 3
    center_xyz = [0.000000e+00,
                  0.000000e+00,
                  0.000000e+00,
                  5.516800e-01,
                  7.734000e-01,
                  0.000000e+00,
                  5.516800e-01,
                  -7.734000e-01,
                  0.000000e+00]
    center_element = [8, 1, 1]

    num_outer_centers = 0
    outer_center_xyz = []
    outer_center_element = []

    num_shells = 12
    shell_center = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3]
    l_quantum_num = [0, 0, 0, 1, 1, 2, 0, 0, 1, 0, 0, 1]
    shell_num_primitives = [9, 9, 1, 4, 1, 1, 4, 1, 1, 4, 1, 1]

    primitive_exp = [1.172000e+04,
                     1.759000e+03,
                     4.008000e+02,
                     1.137000e+02,
                     3.703000e+01,
                     1.327000e+01,
                     5.025000e+00,
                     1.013000e+00,
                     3.023000e-01,
                     1.172000e+04,
                     1.759000e+03,
                     4.008000e+02,
                     1.137000e+02,
                     3.703000e+01,
                     1.327000e+01,
                     5.025000e+00,
                     1.013000e+00,
                     3.023000e-01,
                     3.023000e-01,
                     1.770000e+01,
                     3.854000e+00,
                     1.046000e+00,
                     2.753000e-01,
                     2.753000e-01,
                     1.185000e+00,
                     1.301000e+01,
                     1.962000e+00,
                     4.446000e-01,
                     1.220000e-01,
                     1.220000e-01,
                     7.270000e-01,
                     1.301000e+01,
                     1.962000e+00,
                     4.446000e-01,
                     1.220000e-01,
                     1.220000e-01,
                     7.270000e-01]

    lib.numgrid_generate(
        context,
        radial_precision,
        angular_min,
        angular_max,
        num_centers,
        center_xyz,
        center_element,
        num_outer_centers,
        outer_center_xyz,
        outer_center_element,
        num_shells,
        shell_center,
        l_quantum_num,
        shell_num_primitives,
        primitive_exp
    )

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
        assert abs(grid[i] - reference_grid[i]) < 1.0e-6

#-------------------------------------------------------------------------------

def test_leaks():
    """
    Test memory leaks.
    """
    res = subprocess.Popen(
        [
            'valgrind',
            os.path.join(BUILD_DIR, 'bin', 'unit_tests')
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE).communicate()[1]
    if not 'All heap blocks were freed -- no leaks are possible' in res:
        assert 'definitely lost: 0 bytes in 0 blocks' in res
        assert 'indirectly lost: 0 bytes in 0 blocks' in res
        assert 'possibly lost: 0 bytes in 0 blocks' in res
