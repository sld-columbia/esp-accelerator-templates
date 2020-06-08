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

#include <connections/connections.h>

#endif // __ESP_SYSTEMC_HPP__
