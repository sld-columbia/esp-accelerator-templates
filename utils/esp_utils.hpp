// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

#ifndef __ESP_UTILS_HPP__
#define __ESP_UTILS_HPP__

#include "esp_data.hpp"

#include "esp_systemc.hpp"

#define ESP_REPORT_INFO(...)                              \
    fprintf(stderr, "Info: %s: ", sc_object::basename()); \
    fprintf(stderr, __VA_ARGS__);                         \
    fprintf(stderr, "\n\n");

#define ESP_REPORT_ERROR(...)                              \
    fprintf(stderr, "Error: %s: ", sc_object::basename()); \
    fprintf(stderr, __VA_ARGS__);                          \
    fprintf(stderr, "\n\n");

#define ESP_REPORT_TIME(time, ...)                            \
    {                                                         \
        std::stringstream _ss;                                \
        _ss << time;                                          \
        std::string _s    = _ss.str();                        \
        const char *_time = _s.c_str();                       \
        fprintf(stderr, "Info: %s: ", sc_object::basename()); \
        fprintf(stderr, "@%s ", _time);                       \
        fprintf(stderr, __VA_ARGS__);                         \
        fprintf(stderr, "\n\n");                              \
    }

template <unsigned int n> struct slog_2 {
    enum { value = 1 + slog_2<n / 2>::value };
};

template <> struct slog_2<1> {
    enum { value = 0 };
};

// Helper macros

#define __str(s)  #s
#define __xstr(s) __str(s)

#define MYPPCAT_4(A, B, C, D) A##_DMA##B##_CHK##C##_PP##D

#define MYPPCAT_41(A, B, C, D) A##_DMA##B##_CHK##C##_PP##D##_t

#define MYPPCAT_42(A, B, C, D) A##_DMA##B##_CHK##C##_PP##D.hpp

// Macros for users

#define GENERATE_PLM_TYPE(mem, dma, chk, pp) MYPPCAT_41(mem, dma, chk, pp)

#define GENERATE_PLM_NAME(mem, dma, chk, pp) __xstr(MYPPCAT_4(mem, dma, chk, pp))

#define GENERATE_PLM_HDR(mem, dma, chk, pp) __xstr(MYPPCAT_42(mem, dma, chk, pp))

#ifdef ESP_DEBUG

    // Print info only if debug

    #define ESP_REPORT_DEBUG(...)                              \
        fprintf(stderr, "Debug: %s: ", sc_object::basename()); \
        fprintf(stderr, __VA_ARGS__);                          \
        fprintf(stderr, "\n\n");

#else

    #define ESP_REPORT_DEBUG(...)

#endif

#endif /* __ESP_UTILS_HPP__ */
