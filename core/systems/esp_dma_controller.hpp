// Copyright (c) 2011-2024 Columbia University, System Level Design Group
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

        #if 1

        // DMA read control (non blocking)
        nb_get_initiator<dma_info_t> dma_read_ctrl;

        // DMA write control (non blocking)
        nb_get_initiator<dma_info_t> dma_write_ctrl;

        // DMA write channel (blocking)
        b_get_initiator<sc_dt::sc_bv<_DMA_WIDTH_> > dma_write_chnl;

        #else

        // DMA read control (non blocking)
        cynw_p2p<dma_info_t>::in dma_read_ctrl;

        // DMA write control (non blocking)
        cynw_p2p<dma_info_t>::in dma_write_ctrl;

        // DMA write channel (blocking)
        cynw_p2p<sc_dt::sc_bv<32> >::in dma_write_chnl;

        #endif

        // Accelerator done
        sc_in<bool> acc_done;

        // Output ports

        #if 1

        // DMA read channel (blocking)
        b_put_initiator<sc_dt::sc_bv<_DMA_WIDTH_> > dma_read_chnl;

        #else

        // DMA read channel (blocking)
        cynw_p2p<sc_dt::sc_bv<32> >::out dma_read_chnl;

        #endif

        // Accelerator reset
        sc_out<bool> acc_rst;

        // Constructor
        SC_HAS_PROCESS(esp_dma_controller);
        esp_dma_controller(sc_module_name name, sc_dt::sc_bv<_DMA_WIDTH_> *ptr)
            : sc_module(name)
            , clk("clk")
            , rst("rst")
            , dma_read_ctrl("dma_read_ctrl")
            , dma_write_ctrl("dma_write_ctrl")
            , dma_write_chnl("dma_write_chnl")
            , dma_read_chnl("dma_read_chnl")
            , acc_done("acc_done")
            , acc_rst("acc_rst")
            , num_of_write_burst(0)
            , num_of_read_burst(0)
            , total_write_bytes(0)
            , total_read_bytes(0)
            , mem(ptr)
        {
            SC_CTHREAD(controller, clk.pos());
            reset_signal_is(rst, false);
            set_stack_size(0x400000);

            // Binding
            dma_read_ctrl.clk_rst(clk, rst);
            dma_read_chnl.clk_rst(clk, rst);
            dma_write_ctrl.clk_rst(clk, rst);
            dma_write_chnl.clk_rst(clk, rst);
        }

        // Process

        // Handle requests
        void controller();

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
        sc_dt::sc_bv<_DMA_WIDTH_> *mem;

};

// Implementation
#include "esp_dma_controller.i.hpp"

#endif // __ESP_DMA_CONTROLLER_HPP__

