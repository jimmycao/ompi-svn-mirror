!
! Copyright (c) 2013      Mellanox Technologies, Inc.
!                         All rights reserved.
! $COPYRIGHT$
!
! Additional copyrights may follow
!
! $HEADER$
!
program hello_oshmem
    implicit none
    include 'shmem.fh'

    integer proc, nproc

    call START_PES(0)
    proc = MY_PE()
    nproc = NUM_PES()

    write(*, '("Hello, world, I am ", i2, " of ", i2)') proc, nproc
end program hello_oshmem

