// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Functions

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::reset_load_input()
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
inline void esp_accelerator_3P<_DMA_WIDTH_>::reset_compute_kernel()
{
#if defined(__MNTR_CONNECTIONS__)
    input_ready.reset_ack();
    output_ready.reset_req();
#else
    input_ready.ack.reset_ack();
    output_ready.req.reset_req();
#endif
}

template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::reset_store_output()
{
#if defined(__MNTR_CONNECTIONS__)
    output_ready.reset_ack();
#else
    output_ready.ack.reset_ack();
#endif
    this->reset_accelerator_done();
    this->reset_dma_write();
}

#pragma design modulario
template <
    size_t _DMA_WIDTH_
    >
inline void esp_accelerator_3P<_DMA_WIDTH_>::load_compute_handshake()
{
    {
        //HLS_DEFINE_PROTOCOL("load-compute-handshake");
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
    inline void esp_accelerator_3P<_DMA_WIDTH_>::compute_load_handshake()
    {
        {
            //HLS_DEFINE_PROTOCOL("compute-load-handshake");
#if defined(__MNTR_CONNECTIONS__)
            input_ready.ack();
#else
            input_ready.ack.ack();
#endif
        }
    }

#pragma design modulario
    template <
        size_t _DMA_WIDTH_
        >
    inline void esp_accelerator_3P<_DMA_WIDTH_>::compute_store_handshake()
    {
        {
            //HLS_DEFINE_PROTOCOL("compute-store-handshake");
#if defined(__MNTR_CONNECTIONS__)
            output_ready.req();
#else
            output_ready.req.req();
#endif
        }
    }

#pragma design modulario
    template <
        size_t _DMA_WIDTH_
        >
    inline void esp_accelerator_3P<_DMA_WIDTH_>::store_compute_handshake()
    {
        {
            //HLS_DEFINE_PROTOCOL("store-compute-handshake");
#if defined(__MNTR_CONNECTIONS__)
            output_ready.ack();
#else
            output_ready.ack.ack();
#endif
    }
}
