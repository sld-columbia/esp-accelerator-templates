// Copyright (c) 2011-2019 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_CONFIG_HPP__
#define __ESP_CONFIG_HPP__

#include "core/accelerators/esp_accelerator.hpp"

class esp_config : public sc_module
{

    public:

        // Input ports

        // Clock signal
        sc_in<bool> clk;

        // Reset signal
        sc_in<bool> rst;

        // Accelerator configuration
        sc_in<conf_info_t> conf_info;

        // Accelerator start signal
        sc_in<bool> conf_done;

        // Constructor
        SC_HAS_PROCESS(esp_config);
        esp_config(const sc_module_name &name)
            : sc_module(name)
        {
            SC_CTHREAD(config_accelerator, clk.pos());
            reset_signal_is(rst, false);
            // set_stack_size(0x400000);
        }

        // Process

        // Configure the accelerator
        virtual void config_accelerator() = 0;

        // Function

        // Binding the accelerator
        template <size_t _DMA_WIDTH_> inline void
        bind_with(esp_accelerator<_DMA_WIDTH_> &accelerator);

};

// Implementation
#include "esp_config.i.hpp"

#endif // __ESP_CONFIG_HPP__

