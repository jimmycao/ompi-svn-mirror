/*
 * Copyright (c) 2008 Cisco Systems, Inc.  All rights reserved.
 *
 * Copyright (c) 2009-2012 Oak Ridge National Laboratory.  All rights reserved.
 * Copyright (c) 2009-2012 Mellanox Technologies.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "ompi_config.h"

#include "base.h"
#include "connect.h"
#include "ompi/constants.h"

static void empty_component_register(void);
static int empty_component_init(void);
static int empty_component_query(ompi_common_ofacm_base_dev_desc_t *dev, 
                                 ompi_common_ofacm_base_module_t **cpc);

ompi_common_ofacm_base_component_t ompi_common_ofacm_empty = {
    "empty",
    empty_component_register,
    empty_component_init,
    empty_component_query,
    NULL
};

static void empty_component_register(void)
{
    /* Nothing to do */
}

static int empty_component_init(void)
{
    /* Never let this CPC run */
    return OMPI_ERR_NOT_SUPPORTED;
}

static int empty_component_query(ompi_common_ofacm_base_dev_desc_t *dev, 
                                 ompi_common_ofacm_base_module_t **cpc)
{
    /* Never let this CPC run */
    return OMPI_ERR_NOT_SUPPORTED;
}
