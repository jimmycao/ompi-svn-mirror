/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_ATTR_GET = mpi_attr_get_f
#pragma weak pmpi_attr_get = mpi_attr_get_f
#pragma weak pmpi_attr_get_ = mpi_attr_get_f
#pragma weak pmpi_attr_get__ = mpi_attr_get_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_ATTR_GET,
                           pmpi_attr_get,
                           pmpi_attr_get_,
                           pmpi_attr_get__,
                           pmpi_attr_get_f,
                           (MPI_Fint *comm, MPI_Fint *keyval, char *attribute_val, MPI_Fint *flag, MPI_Fint *ierr),
                           (comm, keyval, attribute_val, flag, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_ATTR_GET = mpi_attr_get_f
#pragma weak mpi_attr_get = mpi_attr_get_f
#pragma weak mpi_attr_get_ = mpi_attr_get_f
#pragma weak mpi_attr_get__ = mpi_attr_get_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_ATTR_GET,
                           mpi_attr_get,
                           mpi_attr_get_,
                           mpi_attr_get__,
                           mpi_attr_get_f,
                           (MPI_Fint *comm, MPI_Fint *keyval, char *attribute_val, MPI_Fint *flag, MPI_Fint *ierr),
                           (comm, keyval, attribute_val, flag, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/f77/profile/defines.h"
#endif

void mpi_attr_get_f(MPI_Fint *comm, MPI_Fint *keyval,
		    char *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
    MPI_Comm c_comm;
    int size;
    OMPI_ARRAY_NAME_DECL(flag);

    size = 1;
    c_comm = MPI_Comm_f2c(*comm);
    
    OMPI_ARRAY_FINT_2_INT_ALLOC(flag, size);

    *ierr = OMPI_INT_2_FINT(MPI_Attr_get(c_comm, 
					 OMPI_FINT_2_INT(*keyval),
					 attribute_val, 
					 OMPI_ARRAY_NAME_CONVERT(flag)));
    OMPI_ARRAY_INT_2_FINT(flag, size);
}
