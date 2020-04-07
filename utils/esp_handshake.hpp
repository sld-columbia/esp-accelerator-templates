// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_HANDSHAKE_HPP__
#define __ESP_HANDSHAKE_HPP__

// Using initiators (default)

#include "esp_systemc.hpp"

// Forward declaration

class handshake_t;

// Handshake request
#if defined(__MNTR_CONNECTIONS__)
#else
class handshake_req_t
{
    public:

        // Friend zone
        friend class handshake_t;

        // Constructor
        handshake_req_t(sc_module_name name)
          : __req(name) { }

        // Clock and reset binding
        template<typename CLK, typename RST>
        void clk_rst(CLK& clk, RST& rst)
          {
              //__req.clk_rst(clk,rst);
          }

        // Reset method
        void reset_req()
          {
              __req.reset_read();
          }

        // Req method
        void req()
          {
              __req.read();
          }

    private:

        // Request channel
        p2p<>::in<bool> __req;

};

// Handshake acknowledge

class handshake_ack_t
{

    public:

        // Friend zone
        friend class handshake_t;

        // Constructor
        handshake_ack_t(sc_module_name name)
          : __ack(name) { }

        // Clock and reset binding
        template<typename CLK, typename RST>
        void clk_rst(CLK& clk, RST& rst)
          {
            //__ack.clk_rst(clk,rst);
          }

        // Reset method
        void reset_ack()
          {
              __ack.reset_write();
          }

        // Ack method
        void ack()
          {
              __ack.write(true);
          }

    private:

        // Ack channel
        p2p<>::out<bool> __ack;
};

#endif

// Interface

class handshake_t
{
    public:

        // Constructor
        handshake_t(sc_module_name name)
          :
#if defined(__MNTR_CONNECTIONS__)
#else
            req(std::string(name).append("_req").c_str()),
            ack(std::string(name).append("_ack").c_str()),
#endif
            __channel(name)
        {
#if defined(__MNTR_CONNECTIONS__)
//            req.__req(__channel);
//            ack.__ack(__channel);
#else
            req.__req.bind(__channel);
            ack.__ack.bind(__channel);
#endif
        }

//        template <
//            size_t _DMA_WIDTH_
//        >
//        inline void bind_with(esp_accelerator<_DMA_WIDTH_> &accelerator)
//        {
//#if defined(__MNTR_CONNECTIONS__)
//
//#else
//            req.clk_rst(accelerator.clk, accelerator.rst);
//            ack.clk_rst(accelerator.clk, accelerator.rst);
//#endif
//        }

#if defined(__MNTR_CONNECTIONS__)

        inline void reset_req() {
            __channel.ResetRead();
        }

        inline void reset_ack() {
            __channel.ResetWrite();
        }

        inline void req() {
            __channel.Pop();
        }

        inline void ack() {
            __channel.Push(true);
        }
#else
        // Req and ack
        handshake_req_t req;
        handshake_ack_t ack;
#endif

    private:

        // Channel
#if defined(__MNTR_CONNECTIONS__)
        Connections::Combinational<bool> __channel;
#else
        p2p<>::chan<bool> __channel;
#endif
};


#endif // __ESP_HANDSHAKE_HPP__
