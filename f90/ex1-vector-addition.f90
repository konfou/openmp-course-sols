program main
    use omp_lib
    implicit none

    interface
        subroutine vec_add(u, v, w)
            integer, dimension(:) :: u, v, w
        end subroutine
    end interface

    integer, dimension (:), allocatable :: u, v, w
    integer :: i, n
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
    allocate(w(n))
    do i=1,n
        u(i) = i
        v(i) = i
    end do

    t0 = omp_get_wtime()
    call vec_add(u, v, w)
    t1 = omp_get_wtime()

    print *, "Total time taken: ", t1-t0

    deallocate(u)
    deallocate(v)
    deallocate(w)
end program

subroutine vec_add(u, v, w)
    integer, dimension(:) :: u, v, w
    integer :: i, n
    n = size(w)
    do i=1,n
        w(i) = u(i) + v(i)
    end do
end subroutine

subroutine usage(arg)
    character(32) :: arg
    print *, "Usage: ", arg(1:len_trim(arg)), " <length>"
end subroutine
