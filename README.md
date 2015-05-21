
[![Build Status](https://travis-ci.org/rbast/numgrid.svg?branch=master)](https://travis-ci.org/rbast/numgrid/builds) [![Coverage Status](https://coveralls.io/repos/rbast/numgrid/badge.png?branch=master)](https://coveralls.io/r/rbast/numgrid?branch=master)

![alt text](https://github.com/rbast/numgrid/raw/master/img/truegrid.jpg "numgrid")

- [Build and test history](https://travis-ci.org/rbast/numgrid/builds)
- [Code coverage](https://coveralls.io/r/rbast/numgrid)
- Licensed under [BSD-3](../master/LICENSE)


# About

Write me ...


# Building

Write me ...


# Testing

Write me ...


# Parallelization

Write me ...


# Distributed computation using "outer centers"

Write me ...


# API

Write me ...


# Python interface

Write me ...


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
