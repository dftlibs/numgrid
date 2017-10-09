program test

   ! in this test we compute the grid for O
   ! in the presence of two H in the H2O molecule

   use numgrid
   use, intrinsic :: iso_c_binding, only: c_ptr

   implicit none

   real(8)              :: radial_precision
   integer              :: min_num_angular_points
   integer              :: max_num_angular_points
   integer              :: num_centers
   integer              :: proton_charges(3)
   real(8)              :: x_coordinates_au(3)
   real(8)              :: y_coordinates_au(3)
   real(8)              :: z_coordinates_au(3)
   real(8)              :: alpha_max
   integer              :: max_l_quantum_number
   real(8)              :: alpha_min(3)
   integer              :: num_points
   integer              :: num_radial_points
   integer              :: num_angular_points
   real(8), allocatable :: grid_x_au(:)
   real(8), allocatable :: grid_y_au(:)
   real(8), allocatable :: grid_z_au(:)
   real(8), allocatable :: grid_w(:)
   real(8), allocatable :: angular_grid_x_au(:)
   real(8), allocatable :: angular_grid_y_au(:)
   real(8), allocatable :: angular_grid_z_au(:)
   real(8), allocatable :: angular_grid_w(:)
   real(8), allocatable :: radial_grid_r_au(:)
   real(8), allocatable :: radial_grid_w(:)
   integer              :: center_index
   integer, parameter   :: io_unit = 13
   real(8)              :: ref(4), own(4)
   integer              :: ipoint
   integer              :: j
   real(8)              :: error
   type(c_ptr)          :: context

   radial_precision = 1.0d-12
   min_num_angular_points = 86
   max_num_angular_points = 302

   num_centers = 3

   proton_charges(1) = 8
   proton_charges(2) = 1
   proton_charges(3) = 1

   x_coordinates_au(1) = 0.0d0
   x_coordinates_au(2) = 1.43d0
   x_coordinates_au(3) = -1.43d0

   y_coordinates_au(1) = 0.0d0
   y_coordinates_au(2) = 0.0d0
   y_coordinates_au(3) = 0.0d0

   z_coordinates_au(1) = 0.0d0
   z_coordinates_au(2) = 1.1d0
   z_coordinates_au(3) = 1.1d0

   ! oxygen
   alpha_max = 11720.0d0
   max_l_quantum_number = 2
   alpha_min(1) = 0.3023d0
   alpha_min(2) = 0.2753d0
   alpha_min(3) = 1.185d0

   open(unit=io_unit, file='../test/reference_grid.txt', access='sequential', action='read')

   context = numgrid_new_atom_grid(radial_precision,       &
                                   min_num_angular_points, &
                                   max_num_angular_points, &
                                   proton_charges(1),      &
                                   alpha_max,              &
                                   max_l_quantum_number,   &
                                   alpha_min)

   num_points = numgrid_get_num_grid_points(context)
   if (num_points /= 16364) stop 1

   allocate(grid_x_au(num_points))
   allocate(grid_y_au(num_points))
   allocate(grid_z_au(num_points))
   allocate(grid_w(num_points))

   center_index = 0

   call numgrid_get_grid(context,          &
                         num_centers,      &
                         center_index,     &
                         x_coordinates_au, &
                         y_coordinates_au, &
                         z_coordinates_au, &
                         proton_charges,   &
                         grid_x_au,        &
                         grid_y_au,        &
                         grid_z_au,        &
                         grid_w)

   do ipoint = 1, num_points
      read(io_unit, *) ref(1), ref(2), ref(3), ref(4)
      own(1) = grid_x_au(ipoint)
      own(2) = grid_y_au(ipoint)
      own(3) = grid_z_au(ipoint)
      own(4) = grid_w(ipoint)
      do j = 1, 4
         error = own(j) - ref(j)
         if (dabs(ref(j)) > 1.0e-15) error = error/ref(j)
         if (dabs(error) > 1.0e-5) stop 1
      end do
   end do
   close(io_unit)

   deallocate(grid_x_au)
   deallocate(grid_y_au)
   deallocate(grid_z_au)
   deallocate(grid_w)

   num_radial_points = numgrid_get_num_radial_grid_points(context)
   if (num_radial_points /= 106) stop 1

   allocate(radial_grid_r_au(num_radial_points))
   allocate(radial_grid_w(num_radial_points))

   call numgrid_get_radial_grid(context,          &
                                radial_grid_r_au, &
                                radial_grid_w)

   deallocate(radial_grid_r_au)
   deallocate(radial_grid_w)

   num_angular_points = 14

   allocate(angular_grid_x_au(num_angular_points))
   allocate(angular_grid_y_au(num_angular_points))
   allocate(angular_grid_z_au(num_angular_points))
   allocate(angular_grid_w(num_angular_points))

   call numgrid_get_angular_grid(num_angular_points, &
                                 angular_grid_x_au,  &
                                 angular_grid_y_au,  &
                                 angular_grid_z_au,  &
                                 angular_grid_w)

   deallocate(angular_grid_x_au)
   deallocate(angular_grid_y_au)
   deallocate(angular_grid_z_au)
   deallocate(angular_grid_w)

   call numgrid_free_atom_grid(context)

end program
