// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_DMA_CONTROLLER_HPP__
#define __ESP_DMA_CONTROLLER_HPP__

#include "utils/esp_utils.hpp"

template <
    size_t _DMA_WIDTH_,
    size_t _MEM_SIZE_
    >
class esp_dma_controller : public sc_module
{

    public:

        // Input ports

        // Clock signal
        sc_in<bool> clk;

        // Reset signal
        sc_in<bool> rst;

        // DMA read control (non blocking)
        Connections::In<dma_info_t> dma_read_ctrl;
        // DMA write control (non blocking)
        Connections::In<dma_info_t> dma_write_ctrl;
        // DMA write channel (blocking)
        Connections::In<ac_int<_DMA_WIDTH_, false> > dma_write_chnl;

        // Accelerator done
        sc_in<bool> acc_done;

        // Output ports

        // DMA read channel
        Connections::Out<ac_int<_DMA_WIDTH_, false> > dma_read_chnl;

        // Accelerator reset
        sc_out<bool> acc_rst;

        // Constructor
        SC_HAS_PROCESS(esp_dma_controller);
        esp_dma_controller(sc_module_name name, ac_int<_DMA_WIDTH_, false> *ptr)
            : sc_module(name)
            , clk("clk")
            , rst("rst")
            , dma_read_ctrl("dma_read_ctrl")
            , dma_write_ctrl("dma_write_ctrl")
            , dma_write_chnl("dma_write_chnl")
            , acc_done("acc_done")
            , dma_read_chnl("dma_read_chnl")
            , acc_rst("acc_rst")
            , load_input_end(0, SC_NS)
            , store_output_end(0, SC_NS)
            , load_input_begin(0, SC_NS)
            , store_output_begin(0, SC_NS)
            , num_of_write_burst(0)
            , num_of_read_burst(0)
            , total_write_bytes(0)
            , total_read_bytes(0)
            , mem(ptr)
        {
#ifdef DMA_SINGLE_PROCESS
            SC_CTHREAD(controller, clk.pos());
            reset_signal_is(rst, false);
            set_stack_size(0x400000);
#else
            SC_CTHREAD(controller_read, clk.pos());
            reset_signal_is(rst, false);
            set_stack_size(0x400000);

            SC_CTHREAD(controller_write, clk.pos());
            reset_signal_is(rst, false);
            set_stack_size(0x400000);

            SC_CTHREAD(controller_done, clk.pos());
            reset_signal_is(rst, false);
            set_stack_size(0x400000);
#endif
            // Binding
            //dma_read_ctrl.clk_rst(clk, rst);
            //dma_read_chnl.clk_rst(clk, rst);
            //dma_write_ctrl.clk_rst(clk, rst);
            //dma_write_chnl.clk_rst(clk, rst);
        }

        // Process

        // Handle requests
#ifdef DMA_SINGLE_PROCESS
        void controller();
#else
        void controller_read();
        void controller_write();
        void controller_done();
#endif

        // Functions

        // Handle read requests
        inline void dma_read(uint32_t mem_base, uint32_t burst_size);

        // Handle write requests
        inline void dma_write(uint32_t mem_base, uint32_t burst_size);

        // Report information

        // End time of the first read burst
        sc_time load_input_end;

        // End time of the first write burst
        sc_time store_output_end;

        // Begin time of the first read burst
        sc_time load_input_begin;

        // Begin time of the first write burst
        sc_time store_output_begin;

        // Number of dma write bursts
        unsigned num_of_write_burst;

        // Number of dma read bursts
        unsigned num_of_read_burst;

        // Number of written bytes
        unsigned total_write_bytes;

        // Number of read bytes
        unsigned total_read_bytes;

        // Memory pointer
        ac_int<_DMA_WIDTH_, false> *mem;

};

// Implementation
#include "esp_dma_controller.i.hpp"

#endif // __ESP_DMA_CONTROLLER_HPP__

