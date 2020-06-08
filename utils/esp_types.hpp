// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_TYPES_HPP__
#define __ESP_TYPES_HPP__

#include <stdint.h>

#include <systemc.h>

// Unsigned integers

#define uint8_t sc_uint<8>

#define uint16_t sc_uint<16>

#define uint32_t sc_uint<32>

#define uint64_t sc_uint<64>

// Signed integers

#define int8_t sc_int<8>

#define int16_t sc_int<16>

#define int32_t sc_int<32>

#define int64_t sc_int<64>

#endif // __ESP_TYPES_HPP__
