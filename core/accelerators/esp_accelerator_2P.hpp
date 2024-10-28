// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_ACCELERATOR_2P_HPP__
#define __ESP_ACCELERATOR_2P_HPP__

#include "esp_accelerator.hpp"

#include "utils/esp_handshake.hpp"

template <size_t _DMA_WIDTH_> class esp_accelerator_2P : public esp_accelerator<_DMA_WIDTH_> {
  public:
    // Input <-> Computation
    handshake_t input_ready;

    // Constructor
    SC_HAS_PROCESS(esp_accelerator_2P);
    esp_accelerator_2P(const sc_module_name &name) :
        esp_accelerator<_DMA_WIDTH_>(name), input_ready("input_ready")
    {
        SC_CTHREAD(load_input, this->clk.pos());
        this->reset_signal_is(this->rst, false);
        // set_stack_size(0x400000);

        SC_CTHREAD(compute_store, this->clk.pos());
        this->reset_signal_is(this->rst, false);
        // set_stack_size(0x400000);

        input_ready.bind_with(*this);
    }

    // Processes

    // Load the input data
    virtual void load_input() = 0;

    // Realize the computation and store the output data
    virtual void compute_store() = 0;

    // Functions

    // Reset callable by load_input
    inline void reset_load_input();

    // Reset callable by compute_store
    inline void reset_compute_store();

    // Handshake callable by load_input
    inline void load_compute_store_handshake();

    // Handshake callable by compute_kernel
    inline void compute_store_load_handshake();
};

// Implementation
#include "esp_accelerator_2P.i.hpp"

#endif // __ESP_ACCELERATOR_2P_HPP__
