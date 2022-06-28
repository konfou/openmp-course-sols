program main
    use omp_lib
    implicit none
    integer, dimension (:), allocatable :: u, v
    integer :: uv, i, n
    real(8) :: t0, t1
    character(32) :: arg

    if(command_argument_count() .eq. 1) then
        call get_command_argument(1, arg)
        read (arg, *) n
    else
        call get_command_argument(0, arg)
        call usage(arg)
        stop 1
    endif

    allocate(u(n))
    allocate(v(n))
    do i=1,n
        u(i) = i
        v(i) = i
    end do

    t0 = omp_get_wtime()
    uv = dot_prod(u, v)
    t1 = omp_get_wtime()

    print *, "dot(u,v) = ", uv
    print *, "Total time taken: ", t1-t0

    deallocate(u)
    deallocate(v)

contains
    integer function dot_prod(u, v) result(uv)
        integer, dimension(:), intent (in) :: u, v
        integer :: i, n
        n = size(u)
        uv = 0
        !$omp parallel do &
        !$omp       default(none) shared(n,u,v) private(i) reduction(+:uv)
        do i=1,n
            uv = uv + u(i) + v(i)
        end do
        !$omp end parallel do
    end function

end program

subroutine usage(arg)
    character(32) :: arg
    print *, "Usage: ", arg(1:len_trim(arg)), " <length>"
end subroutine
