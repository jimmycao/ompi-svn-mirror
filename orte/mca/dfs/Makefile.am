#
# Copyright (c) 2012      Los Alamos National Security, LLC.  All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(includedir)/openmpi -I$(includedir)/openmpi/opal/mca/event/libevent2021/libevent -I$(includedir)/openmpi/opal/mca/hwloc/hwloc152/hwloc/include


# main library setup
noinst_LTLIBRARIES = libmca_dfs.la
libmca_dfs_la_SOURCES =

# local files
headers = dfs.h dfs_types.h
libmca_dfs_la_SOURCES += $(headers)

# Conditionally install the header files
if WANT_INSTALL_HEADERS
ortedir = $(includedir)/openmpi/$(subdir)
nobase_orte_HEADERS = $(headers)
endif

include base/Makefile.am

distclean-local:
	rm -f base/static-components.h
