// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

template <
    size_t _DMA_WIDTH_,
    size_t _MEM_SIZE_
    >
inline int esp_system<_DMA_WIDTH_, _MEM_SIZE_>::clock_cycle(sc_core::sc_time _time)
{
    // Compute the latency and throughput and record them
    sc_clock *clk_p = dynamic_cast<sc_clock *>(clk.get_interface());
    sc_time clock_period = clk_p->period(); // get period
    return (int) (_time / clock_period);
}

