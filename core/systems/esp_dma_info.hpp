// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_DMA_INFO_HPP__
#define __ESP_DMA_INFO_HPP__

#include <sstream>

#include "utils/esp_types.hpp"
#include "utils/esp_systemc.hpp"

#define SIZE_BYTE   0
#define SIZE_HWORD  1
#define SIZE_WORD   2
#define SIZE_DWORD  3
#define SIZE_4WORD  4
#define SIZE_8WORD  5
#define SIZE_16WORD 6
#define SIZE_32WORD 7

class dma_info_t
{

    public:

        // Index
        uint32_t index;

        // Length
        uint32_t length;

        // Length
        ac_int<3, false> size;

        static const unsigned int width = 32 + 32 + 3;
        template <unsigned int Size> void Marshall(Marshaller<Size> &m) {
            m &index;
            m &length;
            m &size;
        }

        // Constructors

        dma_info_t()
            : index(0), length(0), size(SIZE_WORD) { }

        dma_info_t(uint32_t i, uint32_t l, ac_int<3, false> s)
            : index(i), length(l), size(s) { }

        dma_info_t(const dma_info_t &other)
            : index(other.index), length(other.length), size(other.size) { }

        // Operators

        // Assign operator
        inline dma_info_t &operator=(const dma_info_t &other);

        // Equals operator
        inline bool operator==(const dma_info_t &rhs) const;

        // Friend zone

        // Dump operator
        friend inline ostream &operator<<(ostream &os, dma_info_t const &dma_info);

        // Makes this type traceable by SystemC
        friend inline void sc_trace(sc_trace_file *tf, const dma_info_t &v,
                                    const std::string &name);

};

// Implementation
#include "esp_dma_info.i.hpp"

#endif // __ESP_DMA_INFO_HPP__
