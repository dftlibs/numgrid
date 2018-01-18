module numgrid

   use, intrinsic :: iso_c_binding, only: c_ptr, c_double, c_int

   implicit none

   public numgrid_new_atom_grid
   public numgrid_free_atom_grid
   public numgrid_get_num_grid_points
   public numgrid_get_num_radial_grid_points
   public numgrid_get_grid
   public numgrid_get_radial_grid
   public numgrid_get_angular_grid

   private

   interface numgrid_new_atom_grid
      function numgrid_new_atom_grid(radial_precision,       &
                                     min_num_angular_points, &
                                     max_num_angular_points, &
                                     proton_charge,          &
                                     alpha_max,              &
                                     max_l_quantum_number,   &
                                     alpha_min) result(context) bind (c)
         import :: c_ptr, c_double, c_int
         type(c_ptr) :: context
         real(c_double), intent(in), value :: radial_precision
         integer(c_int), intent(in), value :: min_num_angular_points
         integer(c_int), intent(in), value :: max_num_angular_points
         integer(c_int), intent(in), value :: proton_charge
         real(c_double), intent(in), value :: alpha_max
         integer(c_int), intent(in), value :: max_l_quantum_number
         real(c_double), intent(in)        :: alpha_min(*)
      end function
   end interface

   interface numgrid_free_atom_grid
      subroutine numgrid_free_atom_grid(context) bind (c)
         import :: c_ptr
         type(c_ptr), value :: context
      end subroutine
   end interface

   interface numgrid_get_num_grid_points
      function numgrid_get_num_grid_points(context) result(num_grid_points) bind (c)
         import :: c_ptr, c_int
         type(c_ptr), value :: context
         integer(c_int)     :: num_grid_points
      end function
   end interface

   interface numgrid_get_num_radial_grid_points
      function numgrid_get_num_radial_grid_points(context) result(num_radial_grid_points) bind (c)
         import :: c_ptr, c_int
         type(c_ptr), value :: context
         integer(c_int)     :: num_radial_grid_points
      end function
   end interface

   interface numgrid_get_grid
      subroutine numgrid_get_grid(context,          &
                                  num_centers,      &
                                  center_index,     &
                                  x_coordinates_bohr, &
                                  y_coordinates_bohr, &
                                  z_coordinates_bohr, &
                                  proton_charges,   &
                                  grid_x_bohr,        &
                                  grid_y_bohr,        &
                                  grid_z_bohr,        &
                                  grid_w) bind (c)
         import :: c_ptr, c_double, c_int
         type(c_ptr), value                :: context
         integer(c_int), intent(in), value :: num_centers
         integer(c_int), intent(in), value :: center_index
         real(c_double), intent(in)        :: x_coordinates_bohr(*)
         real(c_double), intent(in)        :: y_coordinates_bohr(*)
         real(c_double), intent(in)        :: z_coordinates_bohr(*)
         integer(c_int), intent(in)        :: proton_charges(*)
         real(c_double), intent(inout)     :: grid_x_bohr(*)
         real(c_double), intent(inout)     :: grid_y_bohr(*)
         real(c_double), intent(inout)     :: grid_z_bohr(*)
         real(c_double), intent(inout)     :: grid_w(*)
      end subroutine
   end interface

   interface numgrid_get_radial_grid
      subroutine numgrid_get_radial_grid(context,  &
                                         radial_grid_r_bohr,&
                                         radial_grid_w) bind (c)
         import :: c_ptr, c_double, c_int
         type(c_ptr), value            :: context
         real(c_double), intent(inout) :: radial_grid_r_bohr(*)
         real(c_double), intent(inout) :: radial_grid_w(*)
      end subroutine
   end interface

   interface numgrid_get_angular_grid
      subroutine numgrid_get_angular_grid(num_angular_grid_points, &
                                          angular_grid_x_bohr,       &
                                          angular_grid_y_bohr,       &
                                          angular_grid_z_bohr,       &
                                          angular_grid_w) bind (c)
         import :: c_double, c_int
         integer(c_int), intent(in), value :: num_angular_grid_points
         real(c_double), intent(inout)     :: angular_grid_x_bohr(*)
         real(c_double), intent(inout)     :: angular_grid_y_bohr(*)
         real(c_double), intent(inout)     :: angular_grid_z_bohr(*)
         real(c_double), intent(inout)     :: angular_grid_w(*)
      end subroutine
   end interface

end module
