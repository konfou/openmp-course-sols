## OpenMP Course Set 2020 Solutions

Solving the 2d wave equation with constant coefficients,
given initial values (in function form) I(x,y)=u(x,y,0) and V(x,y)=u_t(x,y,0)
and boundary values del_u=S(x,y,t)=S(x,y). Taking into consideration
that dS/dt=0, the program is simplified as boundary values
can (and are) only be taken at the start, rather at every time step.
Nevertheless the code can be used in case another S is chosen where dS/dt!=0,
by simply changing the macro definition of `St` to `1`.