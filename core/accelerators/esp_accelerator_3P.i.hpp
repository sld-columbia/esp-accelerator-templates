// Copyright (c) 2011-2019 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Functions

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::reset_load_input()
{
    input_ready.req.reset_req();
    this->reset_dma_read();
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::reset_compute_kernel()
{
    input_ready.ack.reset_ack();
    output_ready.req.reset_req();
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::reset_store_output()
{
    output_ready.ack.reset_ack();
    this->reset_accelerator_done();
    this->reset_dma_write();
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::load_compute_handshake()
{
    {
        HLS_DEFINE_PROTOCOL("load-compute-handshake");

        input_ready.req.req();
    }
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::compute_load_handshake()
{
    {
        HLS_DEFINE_PROTOCOL("compute-load-handshake");

        input_ready.ack.ack();
    }
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::compute_store_handshake()
{
    {
        HLS_DEFINE_PROTOCOL("compute-store-handshake");

        output_ready.req.req();
    }
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::store_compute_handshake()
{
    {
        HLS_DEFINE_PROTOCOL("store-compute-handshake");

        output_ready.ack.ack();
    }
}
