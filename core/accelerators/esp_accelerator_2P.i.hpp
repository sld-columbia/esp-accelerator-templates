// Copyright (c) 2011-2019 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Functions

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_2P<_DMA_WIDTH_>::reset_load_input()
{
#if defined(__MNTR_CONNECTIONS__)
    input_ready.reset_req();
#else
    input_ready.req.reset_req();
#endif
    this->reset_dma_read();
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_2P<_DMA_WIDTH_>::reset_compute_store()
{
#if defined(__MNTR_CONNECTIONS__)
    input_ready.reset_ack();
#else
    input_ready.ack.reset_ack();
#endif
    this->reset_accelerator_done();
    this->reset_dma_write();
}

#pragma design modulario
template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_2P<_DMA_WIDTH_>::load_compute_store_handshake()
{
    {
        //HLS_DEFINE_PROTOCOL("load-compute-store-handshake");
#if defined(__MNTR_CONNECTIONS__)
        input_ready.req();
#else
        input_ready.req.req();
#endif
    }
}

#pragma design modulario
template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_2P<_DMA_WIDTH_>::compute_store_load_handshake()
{
    {
        //HLS_DEFINE_PROTOCOL("compute-store-load-handshake");
#if defined(__MNTR_CONNECTIONS__)
        input_ready.ack();
#else
        input_ready.ack.ack();
#endif
    }
}

