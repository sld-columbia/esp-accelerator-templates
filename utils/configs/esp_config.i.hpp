// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Function

template <
    size_t _DMA_WIDTH_
    >
inline void esp_config::bind_with(esp_accelerator<_DMA_WIDTH_> &accelerator)
{
    clk(accelerator.clk);
    rst(accelerator.rst);
    conf_info(accelerator.conf_info);
    conf_done(accelerator.conf_done);
}

