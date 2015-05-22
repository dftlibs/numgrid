
[![Build Status](https://travis-ci.org/rbast/numgrid.svg?branch=master)](https://travis-ci.org/rbast/numgrid/builds) [![Coverage Status](https://coveralls.io/repos/rbast/numgrid/badge.png?branch=master)](https://coveralls.io/r/rbast/numgrid?branch=master)

![alt text](https://github.com/rbast/numgrid/raw/master/img/truegrid.jpg "numgrid")

- [Build and test history](https://travis-ci.org/rbast/numgrid/builds)
- [Code coverage](https://coveralls.io/r/rbast/numgrid)
- Licensed under [BSD-3](../master/LICENSE) (except John Burkardt's Lebedev code which is redistributed under LGPLv3)


# About

Numgrid is a library that produces numerical integration grid for molecules
based on atom coordinates, atom types, and basis set information.


# Contributing

Yes please!


# Requirements

- CMake
- C compiler
- C++ compiler
- [CFFI](https://cffi.readthedocs.org) (Python interface)
- [py.test](http://pytest.org) (to test the Python interface)


# Building and testing

```
git clone --recursive https://github.com/rbast/numgrid.git
cd numgrid
python setup.py --cc=gcc --cxx=g++
cd build
make
make test
```


# Parallelization

The design decision was to not parallelize the library but rather parallelize
over the generated points by the caller. This simplifies modularity and code
reuse.  See also the section about "Distributed computation using 'outer
centers'".


# Distributed computation using 'outer centers'

For very large molecules the caller may not want to generate the entire grid at
once but perhaps only generate center by center or fragment by fragment
(however taking Becke partitioning into account) and possibly keep the centers
or fragments on different parallel tasks or threads. This can be achieved by
using 'outer centers'. The 'outer centers' with influence the active centers in
the generation of the Becke partitioning weights but will not carry any grid
points themselves. This can be used for multi-scale computations.


# Why is the grid basis set dependent?

The basis set (more precisely the Gaussian primitives/exponents) are used to
generate the atomic radial grid range. This means that a more diffuse basis set
generates a more diffuse radial grid.

If you need a grid but you do not have a basis set or choose not to use a
specific one, then you can feed the library with a fantasy basis set consisting
of just two primitives. You can then adjust the range by making the exponents
more steep or more diffuse.


# API

Write me ...


# Python interface

The Python interface is generated using [CFFI](https://cffi.readthedocs.org).

As an example let us generate a grid for the water molecule:

```python
from numgrid import lib

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
                 ...  # we skip many numbers here for brevity
                 7.270000e-01]

context = lib.numgrid_new()

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

grid = lib.numgrid_get_grid(context)

lib.numgrid_free(context)
```


# Testing the Python interface

```
python test/test.py <build_dir>
```

# Integration grid

The molecular integration grid is generated from atom-centered
grids by scaling the grid weights according
to the Becke partitioning scheme,
[JCP 88, 2547 (1988)](http://dx.doi.org/10.1063/1.454033).
The default Becke hardness is 3.

Each atomic grid has radial shells with corresponding radial weights.  Each of
the radial shells carries an angular grid with a certain number of angular
points. The generating schemes for the radial and angular grids are outlined
below.


## Radial grid

The radial grid is generated according to Lindh, Malmqvist, and Gagliardi,
[TCA 106, 178 (2001)](http://dx.doi.org/10.1007/s002140100263).

The motivation for this choice is the nice feature of the above scheme that the
range of the radial grid is basis set dependent. The precision can be tuned
with one single radial precision parameter.
The smaller the radial precision, the better.


## Angular grid

The angular grid is generated according to
Lebedev and Laikov
[A quadrature formula for the sphere of the 131st
algebraic order of accuracy,
Russian Academy of Sciences Doklady Mathematics,
Volume 59, Number 3, 1999, pages 477-481].

The angular grid is pruned.
The pruning is a primitive linear interpolation between the minimum number and
the maximum number of angular points per radial shell.
The maximum number is reached at 0.2 times the Bragg radius of the center.

The higher the values for minimum and maximum number of angular points, the better.

For the minimum and maximum number of angular points the code will use the following
table and select the closest number with at least the desired precision:

```
  {6,   14,   26,   38,   50,   74,   86,  110,  146,  170,
 194,  230,  266,  302,  350,  434,  590,  770,  974, 1202,
1454, 1730, 2030, 2354, 2702, 3074, 3470, 3890, 4334, 4802,
4934, 5294, 5810}
```

Taking the same number for the minimum and maximum number of angular points
switches off pruning.
