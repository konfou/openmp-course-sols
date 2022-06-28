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
        integer, dimension(:), allocatable :: sums
        integer :: i, j, n, nt
        n = size(u)
        nt =  omp_get_max_threads()
        allocate(sums(nt))
        sums = 0
        !$omp parallel default(none) shared(sums,n,u,v) private(i,j)
            ! omp() is index 0
            j = 1 + omp_get_thread_num()
            !$omp do
            do i=1,n
                sums(j) = sums(j) + u(i) + v(i)
            end do
            !$omp end do
        !$omp end parallel
        uv = 0
        do j=1,nt
            uv = uv + sums(j)
        end do
        deallocate(sums)
    end function

end program

subroutine usage(arg)
    character(32) :: arg
    print *, "Usage: ", arg(1:len_trim(arg)), " <length>"
end subroutine
