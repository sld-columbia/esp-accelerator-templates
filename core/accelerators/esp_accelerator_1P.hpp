// Copyright (c) 2011-2025 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_ACCELERATOR_1P_HPP__
#define __ESP_ACCELERATOR_1P_HPP__

#include "esp_accelerator.hpp"

template <size_t _DMA_WIDTH_> class esp_accelerator_1P : public esp_accelerator<_DMA_WIDTH_> {
  public:
    // Constructor
    SC_HAS_PROCESS(esp_accelerator_1P);
    esp_accelerator_1P(const sc_module_name &name) : esp_accelerator<_DMA_WIDTH_>(name)
    {
        SC_CTHREAD(compute_kernel, this->clk.pos());
        this->reset_signal_is(this->rst, false);
        // set_stack_size(0x400000);
    }

    // Process

    // Realize the computation
    virtual void compute_kernel() = 0;

    // Function

    // Reset callable by compute_kernel
    inline void reset_compute_kernel();
};

// Implementation
#include "esp_accelerator_1P.i.hpp"

#endif // __ESP_ACCELERATOR_1P_HPP__
