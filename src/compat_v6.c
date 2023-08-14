/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All rights reserved.
 */

#include "config.h"

#include "nccl_ofi.h"

/*
 * TODO: The plugin is missing an API for cleanup.
 *
 * 1. This results in having various memory leaks because nccl_net_ofi_init()
 * allocates resources that are never freed. Such as never calling device->scheduler->fini()
 * to free the blocks allocated by scheduler's internal freelist.
 *
 * 2. The getProperties() API is expected to fill struct ncclNetProperties_v6_t. This struct
 * defines `pciPath` member as pointer to string that is allocated by plugin. However, this
 * memory is never freed as it is not well defined for NCCL that it can free it with free()
 * and there is no plugin API to free previously allocated properties resources.
 * Probably the easiest way to address this is to introduce a v7 version of the structure
 * that changes `name` and `pciPath` fields to char arrays with limited length rather than pointers.
 */
const ncclNet_v6_t ncclNetPlugin_v6 = {
        .name = "AWS Libfabric",
        .init = nccl_net_ofi_init,
        .devices = nccl_net_ofi_devices,
        .getProperties = nccl_net_ofi_getProperties,
        .listen = nccl_net_ofi_listen,
        .connect = nccl_net_ofi_connect,
        .accept = nccl_net_ofi_accept,
        .regMr = nccl_net_ofi_regMr,
        .regMrDmaBuf = nccl_net_ofi_regMrDmaBuf,
        .deregMr = nccl_net_ofi_deregMr,
        .isend = nccl_net_ofi_isend,
        .irecv = nccl_net_ofi_irecv,
        .iflush = nccl_net_ofi_iflush,
        .test = nccl_net_ofi_test,
        .closeSend = nccl_net_ofi_closeSend,
        .closeRecv = nccl_net_ofi_closeRecv,
        .closeListen = nccl_net_ofi_closeListen,
};
