// Copyright (c) 2011-2025 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

inline dma_info_t &dma_info_t::operator=(const dma_info_t &other)
{
    index  = other.index;
    length = other.length;
    size   = other.size;
    user   = other.user;
    return *this;
}

inline bool dma_info_t::operator==(const dma_info_t &rhs) const
{
    return ((rhs.index == index) && (rhs.length == length) && (rhs.size == size) &&
            (rhs.user == user));
}

inline ostream &operator<<(ostream &os, dma_info_t const &dma_info)
{
    os << "{" << dma_info.index << "," << dma_info.length << "," << dma_info.size << ","
       << dma_info.user << "}";
    return os;
}

inline void sc_trace(sc_trace_file *tf, const dma_info_t &v, const std::string &name)
{
    std::stringstream sstm_c;
    sstm_c << name << ".index";
    sc_trace(tf, v.index, sstm_c.str());
    sstm_c << name << ".length";
    sc_trace(tf, v.length, sstm_c.str());
    sstm_c << name << ".size";
    sc_trace(tf, v.size, sstm_c.str());
    sstm_c << name << ".user";
    sc_trace(tf, v.user, sstm_c.str());
}
