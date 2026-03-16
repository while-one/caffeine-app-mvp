<p align="center">
  <a href="https://whileone.me">
    <img src="https://whileone.me/images/caffeine-small.png" alt="Caffeine Logo" width="384" height="384">
  </a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C-11-blue.svg?style=flat-square&logo=c" alt="C11">
  <img src="https://img.shields.io/badge/CMake-%23008FBA.svg?style=flat-square&logo=cmake&logoColor=white" alt="CMake">
  <a href="https://github.com/while-one/caffeine-app-mvp/actions/workflows/ci.yml">
    <img src="https://img.shields.io/github/actions/workflow/status/while-one/caffeine-app-mvp/ci.yml?style=flat-square&branch=main" alt="CI Status">
  </a>
  <a href="https://github.com/while-one/caffeine-app-mvp/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/while-one/caffeine-app-mvp?style=flat-square&color=blue" alt="License: MIT">
  </a>
</p>

# Caffeine App MVP

The **Caffeine App MVP** is a reference implementation of a Minimum Viable Product (MVP) application built using the **Caffeine Framework**. It demonstrates a clean separation of concerns between hardware-agnostic application logic, a Board Support Package (BSP) layer, and the underlying Hardware Abstraction Layer (HAL).

## Features

- **Hardware Agnostic Logic:** The core application logic (`src/app`) remains completely unaware of physical pin mappings or vendor-specific registers.
- **Abstracted BSP:** A clean Board Support Package (`src/bsp`) abstracts the physical LED hardware, allowing the same app logic to run on different boards.
- **Unified Build Orchestrator:** Leverages the `caffeine-build` submodule for containerized, cross-platform builds.
- **Unit Testing:** Includes a GTest-based testing infrastructure for host-side verification.

## Directory Structure

```text
caffeine-app-mvp/
├── CMakeLists.txt           # Main project definition
├── CMakePresets.json        # Target definitions (STM32, Linux)
├── src/
│   ├── app/                 # Application Logic (Hardware Agnostic)
│   │   ├── app_main.c       # The main blink loop
│   │   └── app_main.h
│   ├── bsp/                 # Board Support Package (Hardware Specific)
│   │   ├── bsp_led.c        # Implementation mapping HAL to physical pins
│   │   └── bsp_led.h        # Abstract LED interface
│   └── main.c               # Entry point, initializes BSP, calls app_main
├── tests/                   # Unit test directory
└── caffeine-build/          # Submodule (Build System & Scripts)
```

## Quick Start

### 1. Build for STM32 (Containerized)
To build for a specific STM32 target (e.g., `blinky-board-ex1` using an F407):

```bash
./caffeine-build/scripts/build.sh blinky-board-ex1
```

### 2. Build and Run Unit Tests (Native)
To build and run the unit tests on your host machine:

```bash
./caffeine-build/scripts/build.sh tests-native
```

### 3. Build for Linux Native
To build the application as a native POSIX binary:

```bash
./caffeine-build/scripts/build.sh linux-native
```

## Hardware Parameter Overrides
This application demonstrates how to override default hardware parameters directly from `CMakePresets.json`.

Example configuration in a preset:
```json
"cacheVariables": {
  "CFN_APP_BOARD_ID": "BLINKY_STM32F407VG_HWV0001",
  "CFN_HAL_CLOCK_HSE_HZ": "8000000",
  "CFN_HAL_CLOCK_LSE_HZ": "32768",
  "CFN_HAL_POWER_VDD_MV": "3300"
}
```

The `CFN_APP_BOARD_ID` is a string injected as a C preprocessor definition (e.g., `#define BLINKY_STM32F407VG_HWV0001`), allowing the BSP to identify the board variant. Other `CFN_HAL_*` values are injected into the vendor HAL configuration, allowing for easy board-specific tuning.

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
