/**
* Copyright (C) UT-Battelle, LLC. 2015. ALL RIGHTS RESERVED.
* Copyright (C) Mellanox Technologies Ltd. 2001-2015.  ALL RIGHTS RESERVED.
* See file LICENSE for terms.
*/

#ifndef UCT_MM_EP_H
#define UCT_MM_EP_H

#include "mm_iface.h"
#include <uct/tl/tl_log.h>
#include <ucs/datastruct/sglib.h>
#include <ucs/datastruct/sglib_wrapper.h>

struct uct_mm_ep {
    uct_base_ep_t       super;

    /* Remote peer */
    uct_mm_remote_seg_t  mapped_desc; /* pointer to the descriptor of the destination's shared_mem (FIFO) */
    uct_mm_fifo_ctl_t    *fifo_ctl;   /* pointer to the destination's ctl struct in the receive fifo */
    void                 *fifo;       /* fifo elements (destination's receive fifo) */

    /* mapped remote memory chunks to which remote descriptors belong to.
     * (after attaching to them) */
    uct_mm_remote_seg_t  *remote_segments_hash[UCT_MM_BASE_ADDRESS_HASH_SIZE];

    ucs_arbiter_group_t  arb_group;
};

UCS_CLASS_DECLARE_NEW_FUNC(uct_mm_ep_t, uct_ep_t, uct_iface_t*,
                           const struct sockaddr *);
UCS_CLASS_DECLARE_DELETE_FUNC(uct_mm_ep_t, uct_ep_t);


ucs_status_t uct_mm_ep_put_short(uct_ep_h tl_ep, const void *buffer,
                                 unsigned length, uint64_t remote_addr, 
                                 uct_rkey_t rkey);
ssize_t uct_mm_ep_put_bcopy(uct_ep_h ep, uct_pack_callback_t pack_cb,
                            void *arg, uint64_t remote_addr, uct_rkey_t rkey);
ucs_status_t uct_mm_ep_am_short(uct_ep_h tl_ep, uint8_t id, uint64_t header,
                                const void *payload, unsigned length);
ssize_t uct_mm_ep_am_bcopy(uct_ep_h tl_ep, uint8_t id, uct_pack_callback_t pack_cb,
                           void *arg);
ucs_status_t uct_mm_ep_atomic_add64(uct_ep_h tl_ep, uint64_t add,
                                    uint64_t remote_addr, uct_rkey_t rkey);
ucs_status_t uct_mm_ep_atomic_fadd64(uct_ep_h tl_ep, uint64_t add,
                                     uint64_t remote_addr, uct_rkey_t rkey,
                                     uint64_t *result, uct_completion_t *comp);
ucs_status_t uct_mm_ep_atomic_swap64(uct_ep_h tl_ep, uint64_t swap,
                                     uint64_t remote_addr, uct_rkey_t rkey,
                                     uint64_t *result, uct_completion_t *comp);
ucs_status_t uct_mm_ep_atomic_cswap64(uct_ep_h tl_ep, uint64_t compare,
                                      uint64_t swap, uint64_t remote_addr, 
                                      uct_rkey_t rkey, uint64_t *result,
                                      uct_completion_t *comp);
ucs_status_t uct_mm_ep_atomic_add32(uct_ep_h tl_ep, uint32_t add,
                                    uint64_t remote_addr, uct_rkey_t rkey);
ucs_status_t uct_mm_ep_atomic_fadd32(uct_ep_h tl_ep, uint32_t add,
                                     uint64_t remote_addr, uct_rkey_t rkey,
                                     uint32_t *result, uct_completion_t *comp);
ucs_status_t uct_mm_ep_atomic_swap32(uct_ep_h tl_ep, uint32_t swap,
                                     uint64_t remote_addr, uct_rkey_t rkey,
                                     uint32_t *result, uct_completion_t *comp);
ucs_status_t uct_mm_ep_atomic_cswap32(uct_ep_h tl_ep, uint32_t compare,
                                      uint32_t swap, uint64_t remote_addr, 
                                      uct_rkey_t rkey, uint32_t *result,
                                      uct_completion_t *comp);
ucs_status_t uct_mm_ep_get_bcopy(uct_ep_h ep, uct_unpack_callback_t unpack_cb,
                                 void *arg, size_t length,
                                 uint64_t remote_addr, uct_rkey_t rkey,
                                 uct_completion_t *comp);

ucs_status_t uct_mm_ep_pending_add(uct_ep_h tl_ep, uct_pending_req_t *n);

void uct_mm_ep_pending_purge(uct_ep_h ep, uct_pending_callback_t cb);

ucs_arbiter_cb_result_t uct_mm_ep_process_pending(ucs_arbiter_t *arbiter,
                                                  ucs_arbiter_elem_t *elem,
                                                  void *arg);

static inline uint64_t uct_mm_remote_seg_hash(uct_mm_remote_seg_t *seg)
{
    return seg->mmid % UCT_MM_BASE_ADDRESS_HASH_SIZE;
}

static inline int64_t uct_mm_remote_seg_compare(uct_mm_remote_seg_t *seg1, uct_mm_remote_seg_t *seg2)
{
    return  seg1->mmid - seg2->mmid;
}

SGLIB_DEFINE_LIST_PROTOTYPES(uct_mm_remote_seg_t, uct_mm_remote_seg_compare, next)
SGLIB_DEFINE_HASHED_CONTAINER_PROTOTYPES(uct_mm_remote_seg_t, UCT_MM_BASE_ADDRESS_HASH_SIZE, uct_mm_remote_seg_hash)

#endif
