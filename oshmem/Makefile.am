#
# Copyright (c) 2013      Mellanox Technologies, Inc.
#                         All rights reserved.
# Copyright (c) 2013 Cisco Systems, Inc.  All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

# Do we need to build OSHMEM?
if OSHMEM_SUPPORT

# Do we have profiling?
if OSHMEM_PROFILING
c_pshmem_lib = shmem/c/profile/libshmem_c_pshmem.la
else
c_pshmem_lib = 
endif

# Do we have the Fortran bindings?
if OSHMEM_WANT_FORTRAN_BINDINGS
fortran_shmem_lib = shmem/fortran/libshmem_fortran.la
else
fortran_shmem_lib =
endif

SUBDIRS = \
	include \
	shmem/c \
	shmem/fortran \
	$(EXT_oshmem_FRAMEWORKS_SUBDIRS) \
	$(EXT_oshmem_FRAMEWORK_COMPONENT_STATIC_SUBDIRS) \
	$(MCA_oshmem_FRAMEWORKS_SUBDIRS) \
	$(MCA_oshmem_FRAMEWORK_COMPONENT_STATIC_SUBDIRS) \
	. \
	$(MCA_oshmem_FRAMEWORK_COMPONENT_DSO_SUBDIRS) 

DIST_SUBDIRS = \
	include \
	shmem/c \
	shmem/fortran \
	$(EXT_oshmem_FRAMEWORKS_SUBDIRS) \
	$(EXT_oshmem_FRAMEWORK_COMPONENT_ALL_SUBDIRS) \
	$(MCA_oshmem_FRAMEWORKS_SUBDIRS) \
	$(MCA_oshmem_FRAMEWORK_COMPONENT_ALL_SUBDIRS) 

#Build The main SHMEM library 
lib_LTLIBRARIES = libshmem.la
libshmem_la_SOURCES =
libshmem_la_LIBADD = \
	shmem/c/libshmem_c.la \
	$(c_pshmem_lib) \
	$(fortran_shmem_lib) \
	$(MCA_oshmem_FRAMEWORK_LIBS) \
	$(top_ompi_builddir)/ompi/libmpi.la
libshmem_la_DEPENDENCIES = $(libshmem_la_LIBADD)
libshmem_la_LDFLAGS = \
        -version-info $(libshmem_so_version) \
        $(OSHMEM_LIBSHMEM_EXTRA_LDFLAGS)

# included subdirectory Makefile.am's and appended-to variables
headers =
noinst_LTLIBRARIES =
include_HEADERS =
nobase_oshmem_HEADERS = 
dist_pkgdata_DATA = 
libshmem_la_SOURCES += $(headers)
nodist_man_MANS =

# Conditionally install the header files

if WANT_INSTALL_HEADERS
oshmemdir = $(includedir)/oshmem/oshmem
nobase_oshmem_HEADERS += $(headers)
else
oshmemdir = $(includedir)
endif

include op/Makefile.am
include proc/Makefile.am
include request/Makefile.am
include runtime/Makefile.am
include shmem/Makefile.am
include tools/Makefile.am

# Ensure that the man page directory exists before we try to make man
# page files (because oshmem/shmem/man/man3 has no config.status-generated
# Makefile)
dir_stamp = $(top_builddir)/$(subdir)/shmem/man/man3/.dir-stamp

# Also ensure that the man pages are rebuilt if the opal_config.h file
# changes (e.g., configure was run again, meaning that the release
# date or version may have changed)
$(nodist_man_MANS): $(dir_stamp) $(top_builddir)/opal/include/opal_config.h

$(dir_stamp):
	$(mkdir_p) `dirname $@`
	touch "$@"

# Remove the generated man pages
distclean-local:
	rm -f $(nodist_man_MANS) $(dir_stamp)

endif # OSHMEM_SUPPORT
