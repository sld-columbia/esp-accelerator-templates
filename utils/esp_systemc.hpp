// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_SYSTEMC_HPP__
#define __ESP_SYSTEMC_HPP__

// Fixed point

#if defined(SC_FIXED_POINT) || defined(SC_FIXED_POINT_FAST)

// Using SystemC fixed point

#define SC_INCLUDE_FX
#include <systemc.h>

#else

// Using Algorithmic C Datatype fixed point (default)

#include <systemc.h>
#include <ac_int.h>
#include <ac_fixed.h>

#endif

// Channels

#if defined(__CARGO__)

#error "CARGO does not support Catapult HLS primitives yet."

#elif defined(__MNTR_CONNECTIONS__)

#include <connections/connections.h>

#else

// Using legacy P2P library (default)

#include "ccs_p2p.h"

#endif

#endif // __ESP_SYSTEMC_HPP__
