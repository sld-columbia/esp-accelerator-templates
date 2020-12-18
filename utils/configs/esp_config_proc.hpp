// Copyright (c) 2011-2021 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_CONFIG_PROC_HPP__
#define __ESP_CONFIG_PROC_HPP__

#include "esp_config.hpp"

class esp_config_proc : public esp_config
{
    // Inline module
    HLS_INLINE_MODULE;

    public:

        // Internal signal

        // Process synchronization
        sc_signal<bool> done;

        // Constructor
        SC_HAS_PROCESS(esp_config_proc);
        esp_config_proc(const sc_module_name& name)
          : esp_config(name) { }

        // Process

        // Configure the accelerator
        void config_accelerator();

        // Function

        // Call to wait for configuration
        inline void wait_for_config();
};

// Implementation
#include "esp_config_proc.i.hpp"

#endif // __ESP_CONFIG_PROC_HPP__

