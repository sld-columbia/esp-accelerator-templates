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
    // Reset
    dma_read_ctrl.Reset();
    dma_read_chnl.Reset();
    dma_write_ctrl.Reset();
    dma_write_chnl.Reset();

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
            dma_read_ctrl_flag = dma_read_ctrl.PopNB(dma_read_ctrl_dma_info);
            dma_write_ctrl_flag = dma_write_ctrl.PopNB(dma_write_ctrl_dma_info);
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
    dma_read_ctrl.Reset();
    dma_read_chnl.Reset();

    wait();

    ESP_REPORT_TIME(VOFF, sc_time_stamp(), "reset done");

    while (true)
    {
        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request...");

        dma_info_t dma_read_ctrl_dma_info = dma_read_ctrl.Pop();

        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: read (index = %u, length = %u)", ESP_TO_UINT32(dma_read_ctrl_dma_info.index), ESP_TO_UINT32(dma_read_ctrl_dma_info.length));
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
    dma_write_ctrl.Reset();
    dma_write_chnl.Reset();

    wait();

    ESP_REPORT_TIME(VOFF, sc_time_stamp(), "reset done");

    while (true)
    {
        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request...");

        dma_info_t dma_write_ctrl_dma_info = dma_write_ctrl.Pop();

        ESP_REPORT_TIME(VOFF, sc_time_stamp(), "Waiting for a DMA request: write (index = %u, length = %u)", ESP_TO_UINT32(dma_write_ctrl_dma_info.index), ESP_TO_UINT32(dma_write_ctrl_dma_info.length));
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
        //ESP_REPORT_TIME(VOFF, sc_time_stamp(), "(mem_base = %u + i = %u = %u) <  (_MEM_SIZE_ = %lu)", ESP_TO_UINT32(mem_base), ESP_TO_UINT32(i), ESP_TO_UINT32(mem_base+i), _MEM_SIZE_);
        sc_assert(mem_base + i < _MEM_SIZE_);

        ac_int<_DMA_WIDTH_, false> data = mem[mem_base + i];

        dma_read_chnl.Push(data);
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

        ac_int<_DMA_WIDTH_, false> data = dma_write_chnl.Pop();

        mem[mem_base + i] = data;
    }

    if (first_dma_write)
    {
        store_output_end = sc_time_stamp();
        first_dma_write = false;
    }
}

