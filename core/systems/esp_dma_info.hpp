// Copyright (c) 2011-2019 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_DMA_INFO_HPP__
#define __ESP_DMA_INFO_HPP__

#include <sstream>

#include "utils/esp_types.hpp"
#include "utils/esp_systemc.hpp"

class dma_info_t
{

    public:

        // Index
        uint32_t index;

        // Length
        uint32_t length;

        // Constructors

        dma_info_t()
            : index(0), length(0) { }

        dma_info_t(uint32_t i, uint32_t l)
            : index(i), length(l) { }

        dma_info_t(const dma_info_t &other)
            : index(other.index), length(other.length) { }

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
