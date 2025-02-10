// Copyright (c) 2011-2025 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_ACCELERATOR_3P_HPP__
#define __ESP_ACCELERATOR_3P_HPP__

#include "esp_accelerator.hpp"

#include "utils/esp_handshake.hpp"

template <size_t _DMA_WIDTH_> class esp_accelerator_3P : public esp_accelerator<_DMA_WIDTH_> {
  public:
    // Input <-> Computation
    handshake_t input_ready;

    // Computation <-> Output
    handshake_t output_ready;

    // Constructor
    SC_HAS_PROCESS(esp_accelerator_3P);
    esp_accelerator_3P(const sc_module_name &name) :
        esp_accelerator<_DMA_WIDTH_>(name), input_ready("input_ready"), output_ready("output_ready")
    {
        SC_CTHREAD(load_input, this->clk.pos());
        this->reset_signal_is(this->rst, false);
        // set_stack_size(0x400000);

        SC_CTHREAD(compute_kernel, this->clk.pos());
        this->reset_signal_is(this->rst, false);
        // set_stack_size(0x400000);

        SC_CTHREAD(store_output, this->clk.pos());
        this->reset_signal_is(this->rst, false);
        // set_stack_size(0x400000);

        input_ready.bind_with(*this);
        output_ready.bind_with(*this);
    }

    // Processes

    // Load the input data
    virtual void load_input() = 0;

    // Realize the computation
    virtual void compute_kernel() = 0;

    // Store the output data
    virtual void store_output() = 0;

    // Functions

    // Reset callable by load_input
    inline void reset_load_input();

    // Reset callable by compute_kernel
    inline void reset_compute_kernel();

    // Reset callable by store_output
    inline void reset_store_output();

    // Handshake callable by load_input
    inline void load_compute_handshake();

    // Handshake callable by compute_kernel
    inline void compute_load_handshake();

    // Handshake callable by compute_kernel
    inline void compute_store_handshake();

    // Handshake callable by store_output
    inline void store_compute_handshake();
};

// Implementation
#include "esp_accelerator_3P.i.hpp"

#endif // __ESP_ACCELERATOR_3P_HPP__
