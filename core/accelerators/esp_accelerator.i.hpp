// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Reset functions

template <
  size_t _DMA_WIDTH_
>
inline void esp_accelerator<_DMA_WIDTH_>::reset_dma_read()
{
#if defined(__MNTR_CONNECTIONS__)
    // Reset
    dma_read_ctrl.Reset();
    dma_read_chnl.Reset();
#else
    // Reset
    dma_read_ctrl.reset_write();
    dma_read_chnl.reset_read();
#endif
}

template <
  size_t _DMA_WIDTH_
>
inline void esp_accelerator<_DMA_WIDTH_>::reset_dma_write()
{
#if defined(__MNTR_CONNECTIONS__)
    // Reset
    dma_write_ctrl.Reset();
    dma_write_chnl.Reset();
#else
    // Reset
    dma_write_ctrl.reset_write();
    dma_write_chnl.reset_write();
#endif
}

template <
  size_t _DMA_WIDTH_
>
inline void esp_accelerator<_DMA_WIDTH_>::reset_accelerator_done()
{
    acc_done.write(false);
}

// Utility functions

#pragma design modulario
template <
    size_t _DMA_WIDTH_
>
inline void esp_accelerator<_DMA_WIDTH_>::process_done()
{
    //HLS_DEFINE_PROTOCOL("process-done");

#pragma hls_unroll no
    do
    {

        wait();

    } while (true);
}

#pragma design modulario
template <
    size_t _DMA_WIDTH_
>
inline void esp_accelerator<_DMA_WIDTH_>::accelerator_done()
{
    //HLS_DEFINE_PROTOCOL("accelerator-done");

    acc_done.write(true); wait();

    acc_done.write(false);
}
