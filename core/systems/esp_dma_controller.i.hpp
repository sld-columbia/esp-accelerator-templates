// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Processes

template <
    size_t _DMA_WIDTH_,
    size_t _MEM_SIZE_
    >
void esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_>::controller()
{
    #if 0
    dma_read_ctrl.reset_get();
    dma_read_chnl.reset_put();
    dma_write_ctrl.reset_get();
    dma_write_chnl.reset_get();
    #else
    dma_read_ctrl.reset();
    dma_read_chnl.reset();
    dma_write_ctrl.reset();
    dma_write_chnl.reset();
    #endif

    acc_rst.write(false);
    wait();
    acc_rst.write(true);

    ESP_REPORT_INFO("reset done");

    while (true)
    {

        do { wait(); }
        while ( !dma_read_ctrl.nb_can_get()
                && !dma_write_ctrl.nb_can_get()
                && !acc_done.read());

        // Kernel is done

        if (acc_done.read())
        {
            ESP_REPORT_INFO("accelerator done");

            ESP_REPORT_TIME(load_input_begin, "BEGIN - first read");
            ESP_REPORT_TIME(load_input_end, "END - first read");

            ESP_REPORT_INFO("total read bursts #%d", num_of_read_burst);
            ESP_REPORT_INFO("total read mbytes #%d", total_read_bytes / 1000);

            ESP_REPORT_TIME(store_output_begin, "BEGIN - first write");
            ESP_REPORT_TIME(store_output_end, "END - first write");

            ESP_REPORT_INFO("total write bursts #%d", num_of_write_burst);
            ESP_REPORT_INFO("total write mbytes #%d", total_write_bytes / 1000);

            acc_rst.write(false);
            wait();
            acc_rst.write(true);
        }

        // Read request

        if (dma_read_ctrl.nb_can_get())
        {
            dma_info_t dma_info;
            bool flag = dma_read_ctrl.nb_get(dma_info);
            sc_assert(flag);
            uint32_t mem_base = dma_info.index;
            uint32_t burst_size = dma_info.length;

            ESP_REPORT_DEBUG("read request with index %d and length %d",
                             mem_base, burst_size);

            num_of_read_burst++;
            total_read_bytes += dma_info.length * (_DMA_WIDTH_ / 8);

            dma_read(mem_base, burst_size);
        }

        // Write request

        if (dma_write_ctrl.nb_can_get())
        {
            dma_info_t dma_info;
            bool flag = dma_write_ctrl.nb_get(dma_info);
            sc_assert(flag);
            uint32_t mem_base = dma_info.index;
            uint32_t burst_size = dma_info.length;

            ESP_REPORT_DEBUG("write request with index %d and length %d",
                             mem_base, burst_size);

            num_of_write_burst++;
            total_write_bytes += dma_info.length * (_DMA_WIDTH_ / 8);

            dma_write(mem_base, burst_size);
        }
    }
}

// Functions

template <size_t _DMA_WIDTH_, size_t _MEM_SIZE_> inline
void esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_>::dma_read(
    uint32_t mem_base, uint32_t burst_size)
{
    static bool first_dma_read = true;

    if (first_dma_read)
    {
        load_input_begin = sc_time_stamp();
    }

    sc_assert(mem != NULL);
    for (uint32_t i = 0; i < burst_size; ++i)
    {
        sc_assert(mem_base + i < _MEM_SIZE_);

        sc_dt::sc_bv<_DMA_WIDTH_> data = mem[mem_base + i];
        dma_read_chnl.put(data);
    }

    if (first_dma_read)
    {
        load_input_end = sc_time_stamp();
        first_dma_read = false;
    }
}

template <size_t _DMA_WIDTH_, size_t _MEM_SIZE_> inline
void esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_>::dma_write(
    uint32_t mem_base, uint32_t burst_size)
{
    static bool first_dma_write = true;

    if (first_dma_write)
    {
        store_output_begin = sc_time_stamp();
    }

    sc_assert(mem != NULL);
    for (uint32_t i = 0; i < burst_size; ++i)
    {
        sc_assert(mem_base + i < _MEM_SIZE_);

        sc_dt::sc_bv<_DMA_WIDTH_> data = dma_write_chnl.get();
        mem[mem_base + i] = data;
    }

    if (first_dma_write)
    {
        store_output_end = sc_time_stamp();
        first_dma_write = false;
    }
}

