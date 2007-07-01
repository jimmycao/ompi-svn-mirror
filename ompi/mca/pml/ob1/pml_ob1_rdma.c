/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2006 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "ompi_config.h"
#include "ompi/constants.h"
#include "orte/util/proc_info.h"
#include "ompi/mca/pml/pml.h"
#include "ompi/mca/btl/btl.h"
#include "ompi/mca/bml/bml.h"
#include "orte/mca/ns/ns_types.h"
#include "ompi/mca/mpool/mpool.h" 
#include "pml_ob1.h"
#include "pml_ob1_rdma.h"

/* Use this registration if no registration needed for a BTL instead of NULL.
 * This will help other code to distinguish case when memory is not registered
 * from case when registration is not needed */
static mca_mpool_base_registration_t pml_ob1_dummy_reg;

/* Calculate what percentage of a message to send through each BTL according to
 * relative weight */
static inline void calc_weighted_length(mca_pml_ob1_rdma_btl_t *rdma_btls,
        int num_btls, size_t size, double weight_total)
{
    int i;
    size_t length_left = size;

    /* shortcut for common case for only one BTL */
    if(num_btls == 1) {
        rdma_btls[0].length = size;
        return;
    }

    for(i = 0; i < num_btls; i++) {
        mca_bml_base_btl_t* bml_btl = rdma_btls[i].bml_btl;
        size_t length = 0;
        if(length_left != 0) {
            length = (length_left > bml_btl->btl_eager_limit)?
                ((size_t)(size * (bml_btl->btl_weight / weight_total))) :
                length_left;

            if(length > length_left)
                length = length_left;
            length_left -= length;
        }
        rdma_btls[i].length = length;
    }

    /* account for rounding errors */
    rdma_btls[0].length += length_left;
}

/*
 * Check to see if memory is registered or can be registered. Build a 
 * set of registrations on the request.
 */

size_t mca_pml_ob1_rdma_btls(
    mca_bml_base_endpoint_t* bml_endpoint,
    unsigned char* base,
    size_t size,
    mca_pml_ob1_rdma_btl_t* rdma_btls)
{
    int num_btls = mca_bml_base_btl_array_get_size(&bml_endpoint->btl_rdma);
    double weight_total = 0;
    int num_btls_used = 0, n;

    /* shortcut when there are no rdma capable btls */
    if(num_btls == 0) {
        return 0;
    }

    /* check to see if memory is registered */        
    for(n = 0; n < num_btls && num_btls_used < mca_pml_ob1.max_rdma_per_request;
            n++) {
        mca_bml_base_btl_t* bml_btl =
            mca_bml_base_btl_array_get_index(&bml_endpoint->btl_rdma,
                    (bml_endpoint->btl_rdma_index + n) % num_btls); 
        mca_mpool_base_registration_t* reg = NULL;
        mca_mpool_base_module_t *btl_mpool = bml_btl->btl_mpool;

        if(NULL != btl_mpool) {
            if(!mca_pml_ob1.leave_pinned) {
                /* look through existing registrations */
                btl_mpool->mpool_find(btl_mpool, base, size, &reg);
            } else {
                /* register the memory */
                btl_mpool->mpool_register(btl_mpool, base, size, 0, &reg);
            }

            if(NULL == reg)
                bml_btl = NULL; /* skip it */
        } else {
            /* if registration is not required use dummy registration */
            reg = &pml_ob1_dummy_reg;
        }

        if(bml_btl != NULL) {
            rdma_btls[num_btls_used].bml_btl = bml_btl;
            rdma_btls[num_btls_used].btl_reg = reg;
            weight_total += bml_btl->btl_weight;
            num_btls_used++;
        }
    }

    /* if we don't use leave_pinned and all BTLs that already have this memory
     * registered amount to less then half of available bandwidth - fall back to
     * pipeline protocol */
    if(0 == num_btls_used || (!mca_pml_ob1.leave_pinned && weight_total < 0.5))
        return 0;

    calc_weighted_length(rdma_btls, num_btls_used, size, weight_total);

    bml_endpoint->btl_rdma_index = (bml_endpoint->btl_rdma_index + 1) % num_btls;
    return num_btls_used;
}

size_t mca_pml_ob1_rdma_pipeline_btls(
    mca_bml_base_endpoint_t* bml_endpoint,
    size_t size,
    mca_pml_ob1_rdma_btl_t* rdma_btls)
{
    int i, num_btls = mca_bml_base_btl_array_get_size(&bml_endpoint->btl_rdma);
    double weight_total = 0;

    for(i = 0; i < num_btls && i < mca_pml_ob1.max_rdma_per_request; i++) {
        rdma_btls[i].bml_btl =
            mca_bml_base_btl_array_get_next(&bml_endpoint->btl_rdma);
        if(rdma_btls[i].bml_btl->btl_mpool != NULL)
            rdma_btls[i].btl_reg = NULL;
        else
            rdma_btls[i].btl_reg = &pml_ob1_dummy_reg;

        weight_total += rdma_btls[i].bml_btl->btl_weight;
    }

    calc_weighted_length(rdma_btls, i, size, weight_total);

    return i;
}
