// Copyright (c) 2011-2022 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_SYSTEMC_HPP__
#define __ESP_SYSTEMC_HPP__

// Fixed point

#if defined(SC_FIXED_POINT) || defined(SC_FIXED_POINT_FAST)

// Using SystemC fixed point

#define SC_INCLUDE_FX
#include <systemc.h>

#else

// Using cynw fixed point (default)

#include <systemc.h>
#include <cynw_fixed.h>

#endif

// Channels

#ifdef __CARGO__

// Using CARGO flex channels

#include <flex_channels.hpp>

#else

// Using cynw flex channels (default)

#include <cynw_flex_channels.h>

#endif

#endif // __ESP_SYSTEMC_HPP__
