// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: MIT

// Process

inline void esp_config_proc::config_accelerator()
{
    HLS_DEFINE_PROTOCOL("config");

    done.write(false);
    wait();

    // ESP_REPORT_INFO("start configuration");

    // Wait for the configuration signal

    bool end = false;

    do {
        HLS_UNROLL_LOOP(OFF);
        wait();
        end = conf_done.read();

    } while (!end);

    // Configuration completed

    done.write(true);

    // ESP_REPORT_INFO("end configuration");

    while (true) {
        HLS_UNROLL_LOOP(OFF);

        wait();
    }
}

// Function

inline void esp_config_proc::wait_for_config()
{
    while (!done.read()) {
        HLS_UNROLL_LOOP(OFF);

        wait();
    }
}
