# ESP32 Cache Simulator

## Project Overview

The ESP32 Cache Simulator is an embedded systems project designed to provide a hands-on simulation of cache memory behavior and its interaction with main memory. The system demonstrates both Direct-Mapped and Set-Associative cache architectures and allows the user to explore replacement policies including FIFO and LRU. By integrating real-time visualization through LEDs and a web-based dashboard, the project provides an intuitive understanding of cache hits, misses, and memory access patterns. Additionally, the project incorporates CACTI-based analysis to simulate cache performance, including access time, power consumption, and area estimates.

## Features

- **Cache Architectures**: Supports both Direct-Mapped and Set-Associative caches with configurable associativity and block sizes.
- **Replacement Policies**: Implements FIFO and LRU policies to manage cache line replacement.
- **Real-Time Visualization**: LEDs indicate cache hits (green) and misses (red) for immediate feedback during simulation.
- **Web Dashboard**: Displays cache contents, hit/miss ratios, average memory access time, and graphical trends of performance metrics. Provides live updates from the ESP32 over Wi-Fi.
- **CACTI Integration**: Facilitates cache performance evaluation using CACTI, providing access time, power, and area metrics.
- **Configurable System**: Cache, memory, and hardware parameters can be configured through JSON files to support a wide range of experimental setups.
- **Logging**: Maintains detailed logs of memory accesses, performance metrics, and error reports for analysis.

## System Architecture

The simulator consists of three major components: the ESP32 firmware, the dashboard, and CACTI integration. The firmware handles cache operations, memory access requests, replacement policies, and real-time LED signaling. The dashboard, hosted on the ESP32, visualizes the cache state and metrics over a network connection. CACTI is used to model cache hardware characteristics, allowing evaluation of latency, power, and area for different cache configurations. The interaction between CPU requests, cache, main memory, LED output, and the dashboard provides a comprehensive understanding of caching behavior and performance.

## Cache and Memory Configuration

Users can define cache size, block size, associativity, and replacement policy in `cache_config.json`. Main memory parameters, including memory size and block configuration, are defined in `memory_config.json`. LED pin assignments and blink timing are configurable through `led_config.json`. This modular configuration allows the simulator to be adapted for educational purposes, experiments, or comparative analysis.

## Logging and Metrics

The simulator maintains detailed access logs (`access_log.txt`) recording memory addresses, hit or miss status, and timestamps. Performance metrics, including hit ratio, miss ratio, and average memory access time (AMAT), are computed in real time and recorded in `metrics_log.csv`. An error log (`error_log.txt`) captures any failed accesses or unexpected events. These logs support both live analysis through the dashboard and offline evaluation of cache performance.

## CACTI Integration

The project integrates with CACTI to provide hardware-level simulation of cache configurations. Users can specify cache parameters in `cacti/cache.cfg` and generate reports for access time, dynamic and leakage power, and area. CACTI results are stored in `cacti/results/` and can be cross-referenced with real-time metrics from the simulator for validation and analysis.

## Usage Instructions

1. Connect the ESP32 to LEDs according to the provided wiring diagram.
2. Configure cache, memory, and LED parameters using the JSON files in the `config` directory.
3. Upload the firmware from the `src` directory to the ESP32 using the Arduino IDE.
4. Access the dashboard via the ESP32-hosted web server to monitor cache performance and visualize metrics.
5. Optionally, run CACTI simulations to evaluate hardware-level characteristics of the configured cache.

## Educational and Research Applications

This project provides a tangible and interactive environment for understanding cache memory behavior, replacement policies, and memory access patterns. It serves as a valuable educational tool for computer architecture courses and as a research platform for evaluating cache performance under different configurations and access patterns.

## License

This project is distributed under the MIT License. See `LICENSE` for details.
