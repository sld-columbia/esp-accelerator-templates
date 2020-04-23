// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_HANDSHAKE_HPP__
#define __ESP_HANDSHAKE_HPP__

// Using initiators (default)

#include "esp_systemc.hpp"

// Forward declaration

class handshake_t;

// Handshake request
#if defined(__MATCHLIB_CONNECTIONS__)
#else
class handshake_req_t
{
public:
    handshake_req_t(sc_module_name name) {} //: __req(std::string(name).append("_req")) { }

    sc_signal<bool> __req;
};

// Handshake acknowledge

class handshake_ack_t
{
public:
    handshake_ack_t(sc_module_name name) {} //: __ack(std::string(name).append("_ack")) { }

    sc_signal<bool> __ack;
};
#endif

// Interface

class handshake_t
{
public:

    // Constructor
    handshake_t(sc_module_name name) :
#if defined(__MATCHLIB_CONNECTIONS__)
#else
        __req(std::string(name).append("_req").c_str()),
        __ack(std::string(name).append("_ack").c_str()),
#endif
        __channel(name) {
#if defined(__MATCHLIB_CONNECTIONS__)
#else
//#warning "Catapult HLS does not suppor the binding of Legacy P2P channels in the same SC_MODULE."
//            __req.__req(__channel);
//            __ack.__ack(__channel);
#endif
    }

#if defined(__MATCHLIB_CONNECTIONS__)
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
    void reset_req() {
        __req.__req.write(0);
    }

    void reset_ack() {
        __ack.__ack.write(0);
    }

#pragma design modulario<sync>
    void req() {
        do { wait(); }
        while (!__ack.__ack.read());
        __req.__req.write(1);
        wait();
        __req.__req.write(0);
    }

#pragma design modulario<sync>
    void ack() {
        wait();
        __ack.__ack.write(1);
        do{ wait(); }
        while (!__req.__req.read());
        __ack.__ack.write(0);
    }

    // Req and ack
    handshake_req_t __req;
    handshake_ack_t __ack;
#endif

    // Channel
#if defined(__MATCHLIB_CONNECTIONS__)
    Connections::Combinational<bool> __channel;
#else
    p2p_sync<>::chan<> __channel;
#endif
};


#endif // __ESP_HANDSHAKE_HPP__
