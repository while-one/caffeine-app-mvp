<p align="center">
  <a href="https://whileone.me">
    <img src="https://raw.githubusercontent.com/while-one/caffeine-build/main/assets/logo.png" alt="Caffeine Logo" width="50%">
  </a>
<h1 align="center">The Caffeine Framework</h1>
</p>

# Caffeine-App-MVP

<p align="center">
  <img src="https://img.shields.io/badge/C-11-blue.svg?style=flat-square&logo=c" alt="C11">
  <img src="https://img.shields.io/badge/CMake-%23008FBA.svg?style=flat-square&logo=cmake&logoColor=white" alt="CMake">
  <a href="https://github.com/while-one/caffeine-app-mvp/tags">
    <img src="https://img.shields.io/github/v/tag/while-one/caffeine-app-mvp?style=flat-square&label=Release" alt="Latest Release">
  </a>
  <a href="https://github.com/while-one/caffeine-app-mvp/actions/workflows/ci.yml">
    <img src="https://img.shields.io/github/actions/workflow/status/while-one/caffeine-app-mvp/ci.yml?style=flat-square&branch=main" alt="CI Status">
  </a>
  <a href="https://github.com/while-one/caffeine-app-mvp/commits/main">
    <img src="https://img.shields.io/github/last-commit/while-one/caffeine-app-mvp.svg?style=flat-square" alt="Last Commit">
  </a>
  <a href="https://github.com/while-one/caffeine-app-mvp/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/while-one/caffeine-app-mvp?style=flat-square&color=blue" alt="License: MIT">
  </a>
</p>

---

# Caffeine App MVP

The **Caffeine App MVP** is a reference implementation of a Minimum Viable Product (MVP) application built using the **Caffeine Framework**. It demonstrates a clean separation of concerns between hardware-agnostic application logic, a Board Support Package (BSP) layer, and the underlying Hardware Abstraction Layer (HAL).

---

## Features

- **Hardware Agnostic Logic:** The core application logic (`src/app`) remains completely unaware of physical pin mappings or vendor-specific registers.
- **Abstracted BSP:** A clean Board Support Package (`src/bsp`) abstracts the physical LED hardware, allowing the same app logic to run on different boards.
- **Unified Build Orchestrator:** Leverages the `caffeine-build` submodule for containerized, cross-platform builds.
- **Unit Testing:** Includes a GTest-based testing infrastructure. Automatically links against `caffeine::hal-mock` for native host testing without hardware dependencies.
---

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
---
## Quick Start

### 1. Build for STM32 (Containerized)
To build for a specific STM32 target (e.g., `blinky-board-ex1` using an F407):

```bash
./caffeine-build/scripts/build.sh blinky-board-ex1
```

### 2. Build and Run Unit Tests (Native)
To build and run the unit tests on your host machine (automatically uses mocks):

```bash
./caffeine-build/scripts/build.sh unit-tests-gtest
```

---

## Development & Analysis

The project includes built-in targets for maintaining code quality:

*   **Format Code:** `cmake --build build/unit-tests-gtest --target caffeine-app-mvp-format`
*   **Run Static Analysis:** `cmake --build build/unit-tests-gtest --target caffeine-app-mvp-analyze`
*   **Run Unit Tests:** `cmake --build build/unit-tests-gtest --target caffeine-app-mvp-test` (Requires `-D CFN_BUILD_TESTS=ON`)

---

## Hardware Configuration
This application demonstrates the **Hardware Contract** model. Applications specify their hardware requirements in their `CMakePresets.json`:

```json
"cacheVariables": {
  "CFN_HAL_PORT_VENDOR": "stm32",
  "CFN_HAL_PORT_FAMILY": "stm32f4",
  "CFN_HAL_PORT_TARGET": "stm32f417vgtx",
  "CFN_APP_BOARD_ID": "BLINKY_STM32F417VG_HWV0001"
}
```

The framework automatically resolves the correct compiler flags, core architecture, and linker script based on the `CFN_HAL_PORT_TARGET`. The `CFN_APP_BOARD_ID` is injected as a C preprocessor definition, allowing the BSP to identify the board variant.

---
## Support the Gallery

While this library is no Mondrian, it deals with a different form of **abstraction art**. Hardware abstraction is a craft of its own—one that keeps your application code portable and your debugging sessions short.

Whether **Caffeine** is fueling an elegant embedded project or just helping you wake up your hardware, you can contribute in the following ways:

* **Star & Share:** If you find this project useful, give it a ⭐ on GitHub and share it with your fellow firmware engineers. It helps others find the library and grows the Caffeine community.
* **Show & Tell:** If you are using Caffeine in a project (personal or professional), **let me know!** Hearing how it's being used is a huge motivator.
* **Propose Features:** If the library is missing a specific "brushstroke," let's design the interface together.
* **Port New Targets:** Help us expand the collection by porting the HAL to new silicon or peripheral sets.
* **Expand the HIL Lab:** Contributions go primarily toward acquiring new development boards. These serve as dedicated **Hardware-in-the-Loop** test targets, ensuring every commit remains rock-solid across our entire fleet of supported hardware.

**If my projects helped you, feel free to buy me a brew. Or if it caused you an extra debugging session, open a PR!**

<a href="https://www.buymeacoffee.com/whileone" target="_blank">
  <img src="https://img.shields.io/badge/Caffeine%20me--0077ff?style=for-the-badge&logo=buy-me-a-coffee&logoColor=white" 
       height="40" 
       style="border-radius: 5px;">
</a>&nbsp;&nbsp;&nbsp;&nbsp;
<a href="https://github.com/sponsors/while-one" target="_blank">
<img src="https://img.shields.io/badge/Sponsor--ea4aaa?style=for-the-badge&logo=github-sponsors" height="40" style="border-radius: 5px;"> </a>&nbsp;&nbsp;&nbsp;
<a href="https://github.com/while-one/caffeine-app-mvp/compare" target="_blank">
<img src="https://img.shields.io/badge/Open%20a%20PR--orange?style=for-the-badge&logo=github&logoColor=white" height="40" style="border-radius: 5px;">
</a>

---

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
