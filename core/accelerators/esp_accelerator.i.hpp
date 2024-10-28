// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Reset functions

template <size_t _DMA_WIDTH_> inline void esp_accelerator<_DMA_WIDTH_>::reset_dma_read()
{
#if 0
    dma_read_ctrl.reset_put();
    dma_read_chnl.reset_get();
#else
    dma_read_ctrl.reset();
    dma_read_chnl.reset();
#endif
}

template <size_t _DMA_WIDTH_> inline void esp_accelerator<_DMA_WIDTH_>::reset_dma_write()
{
#if 0 
    dma_write_ctrl.reset_put();
    dma_write_chnl.reset_put();
#else
    dma_write_ctrl.reset();
    dma_write_chnl.reset();
#endif
}

template <size_t _DMA_WIDTH_> inline void esp_accelerator<_DMA_WIDTH_>::reset_accelerator_done()
{
    acc_done.write(false);
}

// Utility functions

template <size_t _DMA_WIDTH_> inline void esp_accelerator<_DMA_WIDTH_>::process_done()
{
    HLS_DEFINE_PROTOCOL("process-done");

    do {
        HLS_UNROLL_LOOP(OFF);

        wait();

    } while (true);
}

template <size_t _DMA_WIDTH_> inline void esp_accelerator<_DMA_WIDTH_>::accelerator_done()
{
    HLS_DEFINE_PROTOCOL("accelerator-done");

    acc_done.write(true);
    wait();

    acc_done.write(false);
}
