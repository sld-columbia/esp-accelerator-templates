// Copyright (c) 2011-2025 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_SYSTEM_HPP__
#define __ESP_SYSTEM_HPP__

#include "utils/esp_systemc.hpp"
#include "utils/esp_types.hpp"

#include "esp_dma_controller.hpp"

template <size_t _DMA_WIDTH_, size_t _MEM_SIZE_> class esp_system : public sc_module {

  public:
    // Input ports

    // Clock signal
    sc_in<bool> clk;

    // Reset signal
    sc_in<bool> rst;

#if 1

    // DMA read control
    put_get_channel<dma_info_t> dma_read_ctrl;

    // DMA write control
    put_get_channel<dma_info_t> dma_write_ctrl;

    // DMA read channel
    put_get_channel<sc_dt::sc_bv<_DMA_WIDTH_>> dma_read_chnl;

    // DMA write channel
    put_get_channel<sc_dt::sc_bv<_DMA_WIDTH_>> dma_write_chnl;

#else

    // DMA read control
    cynw_p2p<dma_info_t> dma_read_ctrl;

    // DMA write control
    cynw_p2p<dma_info_t> dma_write_ctrl;

    // DMA read channel
    cynw_p2p<sc_dt::sc_bv<32>> dma_read_chnl;

    // DMA write channel
    cynw_p2p<sc_dt::sc_bv<32>> dma_write_chnl;

#endif

    // Internal signals

    // Configuration information
    sc_signal<conf_info_t> conf_info;

    // Accelerator starts
    sc_signal<bool> conf_done;

    // Accelerator reset
    sc_signal<bool> acc_rst;

    // Accelerator is done
    sc_signal<bool> acc_done;

    // Debug port
    sc_signal<debug_info_t> debug;

    // Shared memory buffer model
    sc_dt::sc_bv<_DMA_WIDTH_> *mem;

    // DMA controller instace
    esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_> *dmac;

    // Constructor

    SC_HAS_PROCESS(esp_system);

    esp_system(sc_module_name name) :
        sc_module(name), clk("clk"), rst("rst"), dma_read_ctrl("dma_read_ctrl"),
        dma_write_ctrl("dma_write_ctrl"), dma_read_chnl("dma_read_chnl"),
        dma_write_chnl("dma_write_chnl"), conf_info("conf_info"), conf_done("conf_done"),
        acc_rst("acc_rst"), acc_done("acc_done"), debug("debug"),
        mem(new sc_dt::sc_bv<_DMA_WIDTH_>[_MEM_SIZE_]),
        dmac(new esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_>("dma-controller", mem))
    {
        SC_CTHREAD(config_proc, clk.pos());
        reset_signal_is(rst, false);
        // set_stack_size(0x400000);

        // Binding
        dmac->clk(clk);
        dmac->rst(rst);
        dmac->dma_read_ctrl(dma_read_ctrl);
        dmac->dma_read_chnl(dma_read_chnl);
        dmac->dma_write_ctrl(dma_write_ctrl);
        dmac->dma_write_chnl(dma_write_chnl);
        dmac->acc_done(acc_done);
        dmac->acc_rst(acc_rst);
    }

    // Processes

    // Configure accelerator
    virtual void config_proc() = 0;

    // Load internal memory
    virtual void load_memory() = 0;

    // Dump internal memory
    virtual void dump_memory() = 0;

    // Validate results
    virtual int validate() = 0;

    // Function

    // Profiling
    inline int clock_cycle(sc_time _time);
};

// Implementation
#include "esp_system.i.hpp"

#endif // __ESP_SYSTEM_HPP__
