// Copyright (c) 2011-2025 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_HANDSHAKE_HPP__
#define __ESP_HANDSHAKE_HPP__

#if defined(USE_CYNW_P2P)

    // Using cynw_p2p library

    #include "cynw_p2p.h"

// Forward declaration

class handshake_t;

// Handshake request

class handshake_req_t {

  public:
    // Friend zone
    friend class handshake_t;

    // Constructor
    handshake_req_t(sc_module_name name) : __req(name) {}

    // Clock and reset binding
    template <typename CLK, typename RST> void clk_rst(CLK &clk, RST &rst)
    {
        __req.clk_rst(clk, rst);
    }

    // Reset method
    void reset_req() { __req.reset(); }

    // Req method
    void req() { __req.get(); }

  private:
    // Request channel
    cynw_p2p<bool>::in __req;
};

// Handshake acknowledge

class handshake_ack_t {
  public:
    // Friend zone
    friend class handshake_t;

    // Constructor
    handshake_ack_t(sc_module_name name) : __ack(name) {}

    // Clock and reset binding
    template <typename CLK, typename RST> void clk_rst(CLK &clk, RST &rst)
    {
        __ack.clk_rst(clk, rst);
    }

    // Reset method
    void reset_ack() { __ack.reset(); }

    // Ack method
    void ack() { __ack.put(true); }

  private:
    // Ack channel
    cynw_p2p<bool>::out __ack;
};

// Handshake interface

class handshake_t {
  public:
    // Constructor
    handshake_t(sc_module_name name) :
        req(std::string(name).append("_req").c_str()),
        ack(std::string(name).append("_ack").c_str()), __channel(name)
    {
        req.__req.bind(__channel);
        ack.__ack.bind(__channel);
    }

    template <size_t _DMA_WIDTH_> inline void bind_with(esp_accelerator<_DMA_WIDTH_> &accelerator)
    {
        req.clk_rst(accelerator.clk, accelerator.rst);
        ack.clk_rst(accelerator.clk, accelerator.rst);
    }

    // Req and ack
    handshake_req_t req;
    handshake_ack_t ack;

  private:
    // Channel
    cynw_p2p<bool> __channel;
};

#else

    // Using initiators (default)

    #include "esp_systemc.hpp"

// Forward declaration

class handshake_t;

// Handshake request

class handshake_req_t {
  public:
    // Friend zone
    friend class handshake_t;

    // Constructor
    handshake_req_t(sc_module_name name) : __req(name) {}

    // Clock and reset binding
    template <typename CLK, typename RST> void clk_rst(CLK &clk, RST &rst)
    {
        __req.clk_rst(clk, rst);
    }

    // Reset method
    void reset_req() { __req.reset_get(); }

    // Req method
    void req() { __req.get(); }

  private:
    // Request channel
    b_get_initiator<bool> __req;
};

// Handshake acknowledge

class handshake_ack_t {

  public:
    // Friend zone
    friend class handshake_t;

    // Constructor
    handshake_ack_t(sc_module_name name) : __ack(name) {}

    // Clock and reset binding
    template <typename CLK, typename RST> void clk_rst(CLK &clk, RST &rst)
    {
        __ack.clk_rst(clk, rst);
    }

    // Reset method
    void reset_ack() { __ack.reset_put(); }

    // Ack method
    void ack() { __ack.put(true); }

  private:
    // Ack channel
    b_put_initiator<bool> __ack;
};

// Interface

class handshake_t {
  public:
    // Constructor
    handshake_t(sc_module_name name) :
        req(std::string(name).append("_req").c_str()),
        ack(std::string(name).append("_ack").c_str()), __channel(name)
    {
        req.__req.bind(__channel);
        ack.__ack.bind(__channel);
    }

    template <size_t _DMA_WIDTH_> inline void bind_with(esp_accelerator<_DMA_WIDTH_> &accelerator)
    {
        req.clk_rst(accelerator.clk, accelerator.rst);
        ack.clk_rst(accelerator.clk, accelerator.rst);
    }

    // Req and ack
    handshake_req_t req;
    handshake_ack_t ack;

  private:
    // Channel
    put_get_channel<bool> __channel;
};

#endif

#endif // __ESP_HANDSHAKE_HPP__
