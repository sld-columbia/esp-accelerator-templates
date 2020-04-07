// Copyright (c) 2011-2020 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Processes

#ifdef DMA_SINGLE_PROCESS

template <
    size_t _DMA_WIDTH_,
    size_t _MEM_SIZE_
    >
void esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_>::controller()
{
#if defined(__MNTR_CONNECTIONS__)
    // Reset
    dma_read_ctrl.Reset();
    dma_read_chnl.Reset();
    dma_write_ctrl.Reset();
    dma_write_chnl.Reset();
#else
    // Reset
    dma_read_ctrl.reset_read();
    dma_read_chnl.reset_write();
    dma_write_ctrl.reset_read();
    dma_write_chnl.reset_read();
#endif

    acc_rst.write(false);
    wait();
    acc_rst.write(true);

    ESP_REPORT_TIME(VOFF, sc_time_stamp(), "reset done");

    while (true)
    {
        bool dma_read_ctrl_flag = false;
        bool dma_write_ctrl_flag = false;
        dma_info_t dma_read_ctrl_dma_info;
        dma_info_t dma_write_ctrl_dma_info;

        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request...");

        do {
            wait();
#if defined(__MNTR_CONNECTIONS__)
            dma_read_ctrl_flag = dma_read_ctrl.PopNB(dma_read_ctrl_dma_info);
            dma_write_ctrl_flag = dma_write_ctrl.PopNB(dma_write_ctrl_dma_info);
#else
            dma_read_ctrl_flag = dma_read_ctrl.nb_read(dma_read_ctrl_dma_info);
            dma_write_ctrl_flag = dma_write_ctrl.nb_read(dma_write_ctrl_dma_info);
#endif
            //ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: read = %d (index = %d, length = %d)", dma_read_ctrl_flag, ESP_TO_UINT64(dma_read_ctrl_dma_info.index), ESP_TO_UINT64(dma_read_ctrl_dma_info.length));
            //ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: write = %d (index = %d, length = %d)", dma_write_ctrl_flag, ESP_TO_UINT64(dma_write_ctrl_dma_info.index), ESP_TO_UINT64(dma_write_ctrl_dma_info.length));
        } while (!dma_read_ctrl_flag
                && !dma_write_ctrl_flag
                && !acc_done.read());

        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: done!");

        // Kernel is done

        if (acc_done.read())
        {
            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "accelerator done");

            ESP_REPORT_TIME(VOFF, load_input_begin, "BEGIN - first read");
            ESP_REPORT_TIME(VOFF, load_input_end, "END - first read");

            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "total read bursts #%d", num_of_read_burst);
            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "total read mbytes #%d", total_read_bytes / 1000);

            ESP_REPORT_TIME(VOFF, store_output_begin, "BEGIN - first write");
            ESP_REPORT_TIME(VOFF, store_output_end, "END - first write");

            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "total write bursts #%d", num_of_write_burst);
            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "total write mbytes #%d", total_write_bytes / 1000);

            acc_rst.write(false);
            wait();
            acc_rst.write(true);
        }

        // Read request
        if (dma_read_ctrl_flag)
        {
            uint32_t mem_base = dma_read_ctrl_dma_info.index;
            uint32_t burst_size = dma_read_ctrl_dma_info.length;
            ESP_REPORT_DEBUG("read request with index %d and length %d", mem_base, burst_size);

            num_of_read_burst++;

            total_read_bytes += dma_read_ctrl_dma_info.length * (_DMA_WIDTH_ / 8);

            dma_read(mem_base, burst_size);
        }

        // Write request
        if (dma_write_ctrl_flag)
        {
            uint32_t mem_base = dma_write_ctrl_dma_info.index;
            uint32_t burst_size = dma_write_ctrl_dma_info.length;
            ESP_REPORT_DEBUG("write request with index %d and length %d", mem_base, burst_size);

            num_of_write_burst++;

            total_write_bytes += dma_write_ctrl_dma_info.length * (_DMA_WIDTH_ / 8);

            dma_write(mem_base, burst_size);
        }
    }
}

#else

template <
    size_t _DMA_WIDTH_,
    size_t _MEM_SIZE_
    >
void esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_>::controller_read()
{
    // Reset
#if defined(__MNTR_CONNECTIONS__)
    dma_read_ctrl.Reset();
    dma_read_chnl.Reset();
#else
    dma_read_ctrl.reset_read();
    dma_read_chnl.reset_write();
#endif

    wait();

    ESP_REPORT_TIME(VOFF, sc_time_stamp(), "reset done");

    while (true)
    {
        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request...");

#if defined(__MNTR_CONNECTIONS__)
        dma_info_t dma_read_ctrl_dma_info = dma_read_ctrl.Pop();
#else
        dma_info_t dma_read_ctrl_dma_info = dma_read_ctrl.read();
#endif

        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: read (index = %llu, length = %llu)", ESP_TO_UINT64(dma_read_ctrl_dma_info.index), ESP_TO_UINT64(dma_read_ctrl_dma_info.length));
        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: done!");

        // Read request
        uint32_t mem_base = dma_read_ctrl_dma_info.index;
        uint32_t burst_size = dma_read_ctrl_dma_info.length;
        ESP_REPORT_DEBUG("read request with index %d and length %d", mem_base, burst_size);

        num_of_read_burst++;

        total_read_bytes += dma_read_ctrl_dma_info.length * (DMA_WIDTH / 8);

        dma_read(mem_base, burst_size);
    }
}

template <
    size_t _DMA_WIDTH_,
    size_t _MEM_SIZE_
    >
void esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_>::controller_write()
{
    // Reset
#if defined(__MNTR_CONNECTIONS__)
    dma_write_ctrl.Reset();
    dma_write_chnl.Reset();
#else
    dma_write_ctrl.reset_read();
    dma_write_chnl.reset_read();
#endif

    wait();

    ESP_REPORT_TIME(VOFF, sc_time_stamp(), "reset done");

    while (true)
    {
        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request...");

#if defined(__MNTR_CONNECTIONS__)
            dma_info_t dma_write_ctrl_dma_info = dma_write_ctrl.Pop();
#else
            dma_info_t dma_write_ctrl_dma_info = dma_write_ctrl.read();
#endif

        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: write (index = %llu, length = %llu)", ESP_TO_UINT64(dma_write_ctrl_dma_info.index), ESP_TO_UINT64(dma_write_ctrl_dma_info.length));
        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: done!");

        // Write request
        uint32_t mem_base = dma_write_ctrl_dma_info.index;
        uint32_t burst_size = dma_write_ctrl_dma_info.length;
        ESP_REPORT_DEBUG("write request with index %d and length %d", mem_base, burst_size);

        num_of_write_burst++;

        total_write_bytes += dma_write_ctrl_dma_info.length * (DMA_WIDTH / 8);

        dma_write(mem_base, burst_size);
    }
}

template <
    size_t _DMA_WIDTH_,
    size_t _MEM_SIZE_
    >
void esp_dma_controller<_DMA_WIDTH_, _MEM_SIZE_>::controller_done()
{
    // Reset
#if defined(__MNTR_CONNECTIONS__)
#else
#endif

    acc_rst.write(false);
    wait();
    acc_rst.write(true);

    ESP_REPORT_TIME(VOFF, sc_time_stamp(), "reset done");

    while (true)
    {
        wait();

        // Kernel is done
        if (acc_done.read())
        {
            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "accelerator done");

            ESP_REPORT_TIME(VOFF, load_input_begin, "BEGIN - first read");
            ESP_REPORT_TIME(VOFF, load_input_end, "END - first read");

            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "total read bursts #%d", num_of_read_burst);
            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "total read mbytes #%d", total_read_bytes / 1000);

            ESP_REPORT_TIME(VOFF, store_output_begin, "BEGIN - first write");
            ESP_REPORT_TIME(VOFF, store_output_end, "END - first write");

            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "total write bursts #%d", num_of_write_burst);
            ESP_REPORT_TIME(VOFF, sc_time_stamp(), "total write mbytes #%d", total_write_bytes / 1000);

            acc_rst.write(false);
            wait();
            acc_rst.write(true);
        }
    }
}

#endif

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
        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "(mem_base = %llu + i = %llu = %llu) <  (_MEM_SIZE_ = %lu)", ESP_TO_UINT64(mem_base), ESP_TO_UINT64(i), ESP_TO_UINT64(mem_base+i), _MEM_SIZE_);
        sc_assert(mem_base + i < _MEM_SIZE_);

        sc_dt::sc_bv<_DMA_WIDTH_> data = mem[mem_base + i];

#if defined(__MNTR_CONNECTIONS__)
        dma_read_chnl.Push(data);
#else
        dma_read_chnl.write(data);
#endif
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

#if defined(__MNTR_CONNECTIONS__)
        sc_dt::sc_bv<_DMA_WIDTH_> data = dma_write_chnl.Pop();
#else
        sc_dt::sc_bv<_DMA_WIDTH_> data = dma_write_chnl.read();
#endif
        mem[mem_base + i] = data;
    }

    if (first_dma_write)
    {
        store_output_end = sc_time_stamp();
        first_dma_write = false;
    }
}

