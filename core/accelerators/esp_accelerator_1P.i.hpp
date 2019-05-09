// Copyright (c) 2011-2019 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Functions

template <size_t _DMA_WIDTH_> inline
void esp_accelerator_1P<_DMA_WIDTH_>::reset_compute_kernel()
{
    this->reset_dma_read();
    this->reset_dma_write();

    this->reset_accelerator_done();
}

