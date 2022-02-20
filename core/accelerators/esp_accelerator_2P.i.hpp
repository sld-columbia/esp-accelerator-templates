// Copyright (c) 2011-2022 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Functions

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_2P<_DMA_WIDTH_>::reset_load_input()
{
    input_ready.req.reset_req();
    this->reset_dma_read();
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_2P<_DMA_WIDTH_>::reset_compute_store()
{
    input_ready.ack.reset_ack();
    this->reset_accelerator_done();
    this->reset_dma_write();
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_2P<_DMA_WIDTH_>::load_compute_store_handshake()
{
    {
        HLS_DEFINE_PROTOCOL("load-compute-store-handshake");

        input_ready.req.req();
    }
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_2P<_DMA_WIDTH_>::compute_store_load_handshake()
{
    {
        HLS_DEFINE_PROTOCOL("compute-store-load-handshake");

        input_ready.ack.ack();
    }
}

