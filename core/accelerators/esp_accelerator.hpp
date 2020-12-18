// Copyright (c) 2011-2021 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_ACCELERATOR_HPP__
#define __ESP_ACCELERATOR_HPP__

#include "utils/esp_utils.hpp"

#include "core/systems/esp_dma_info.hpp"

template <
    size_t _DMA_WIDTH_
    >
class esp_accelerator : public sc_module
{
    public:

        // Input ports

        // Clock signal
        sc_in<bool> clk;

        // Reset signal
        sc_in<bool> rst;

        #if 1

        // DMA read channel
        get_initiator<sc_dt::sc_bv<_DMA_WIDTH_> > dma_read_chnl;

        #else

        // DMA read channel
        cynw_p2p<sc_dt::sc_bv<32> >::in dma_read_chnl;

        #endif

        // Accelerator configuration
        sc_in<conf_info_t> conf_info;

        // Accelerator start signal
        sc_in<bool> conf_done;

        // Output ports

        // Computation complete
        sc_out<bool> acc_done;

        // Debug port
        sc_out<debug_info_t> debug;

        #if 1

        // DMA read control
        b_put_initiator<dma_info_t> dma_read_ctrl;

        // DMA write control
        b_put_initiator<dma_info_t> dma_write_ctrl;

        // DMA write channel
        put_initiator<sc_dt::sc_bv<_DMA_WIDTH_> > dma_write_chnl;

        #else

        // DMA read control
        cynw_p2p<dma_info_t>::out dma_read_ctrl;

        // DMA write control
        cynw_p2p<dma_info_t>::out dma_write_ctrl;

        // DMA write channel
        cynw_p2p<sc_dt::sc_bv<32> >::out dma_write_chnl;

        #endif

        // Constructor
        SC_HAS_PROCESS(esp_accelerator);
        esp_accelerator(const sc_module_name &name)
            : sc_module(name)
            , clk("clk")
            , rst("rst")
            , acc_done("acc_done")
            , dma_read_chnl("dma_read_chnl")
            , dma_read_ctrl("dma_read_ctrl")
            , dma_write_ctrl("dma_write_ctrl")
            , dma_write_chnl("dma_write_chnl")
        {
            // Clock and reset binding
            dma_read_ctrl.clk_rst(clk, rst);
            dma_read_chnl.clk_rst(clk, rst);
            dma_write_ctrl.clk_rst(clk, rst);
            dma_write_chnl.clk_rst(clk, rst);
        }

        // Reset functions

        // Reset DMA read channels
        inline void reset_dma_read();

        // Reset DMA write channels
        inline void reset_dma_write();

        // Reset the accelerator status
        inline void reset_accelerator_done();

        // Utility functions

        // The process is done
        inline void process_done();

        // The accelerator is done
        inline void accelerator_done();

};

// Implementation
#include "esp_accelerator.i.hpp"

#endif // __ESP_ACCELERATOR_HPP__
