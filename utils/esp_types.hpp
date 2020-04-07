// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_TYPES_HPP__
#define __ESP_TYPES_HPP__

#include <stdint.h>

#if defined(__MNTR_CONNECTIONS__)
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

#else

// Unsigned integers

#define uint8_t unsigned char

#define uint16_t unsigned short

#define uint32_t unsigned int

#define uint64_t unsigned long long

// Signed integers

#define int8_t char

#define int16_t short

#define int32_t int

#define int64_t long long

#endif

#endif // __ESP_TYPES_HPP__
