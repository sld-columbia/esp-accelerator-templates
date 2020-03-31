// Copyright (c) 2011-2019 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Process
#pragma design modulario
inline void esp_config_proc::config_accelerator()
{
    // HLS_DEFINE_PROTOCOL("config");

    done.write(false); wait();

    ESP_REPORT_INFO("start configuration");

    // Wait for the configuration signal

    bool end = false;

#pragma hls_unroll no
    do
    {
      wait();
      end = conf_done.read();

    } while (!end);

    // Configuration completed

    done.write(true);

    ESP_REPORT_INFO("end configuration");

#pragma hls_unroll no
    while (true)
    {
        wait();
    }
}

// Function

inline void esp_config_proc::wait_for_config()
{
#pragma hls_unroll no
    while (!done.read())
    {
        wait();
    }
}

